
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include "app_daemon.h"

void signal_handler(int sig)
{
#ifdef OPEN_SYSLOG
    syslog(LOG_USER | LOG_DEBUG, "%d Caught signal %d\n", getpid(), sig);
#else
    fprintf(stdout, "%d Caught signal %d\n", getpid(), sig);
#endif
}

int pipe_send(int fd)
{

    if (write(fd, "*", 1) <= 0)
	return -1;

    return 0;
}

int pipe_recv(int fd)
{
    int ch;
    if (read(fd, &ch, 1) <= 0)
	return -1;

    return 0;
}

void pipe_loop(int fdr, int fdw, int fd1[2], int fd2[2])
{

    if (pipe_send(fdw)) {

	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	app_daemon_process();
    }

    sleep(1);

    if (pipe_recv(fdr)) {

	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	app_daemon_process();
    }
}

void father_process(int fd1[2], int fd2[2])
{

    if (fork() > 0) exit(1);

    int fdr = fd1[0];
    int fdw = fd2[1];
#ifdef OPEN_SYSLOG
    syslog(LOG_USER | LOG_DEBUG, "father_process::[%d][%d]%d\n", fdr, fdw, getpid());
#else
    fprintf(stdout, "father_process::[%d][%d]%d\n", fdr, fdw, getpid());
#endif
    for (;;) pipe_loop(fdr, fdw, fd1, fd2);
}

void child_process(int fd1[2], int fd2[2])
{
    if (fork() > 0) exit(1);

    int fdr = fd2[0];
    int fdw = fd1[1];
#ifdef OPEN_SYSLOG
    syslog(LOG_USER | LOG_DEBUG, "child_process::[%d][%d]%d\n", fdr, fdw, getpid());
#else
    fprintf(stdout, "child_process::[%d][%d]%d\n", fdr, fdw, getpid());
#endif
    for (;;) pipe_loop(fdr, fdw, fd1, fd2);
}

void app_daemon_process()
{

    if (fork() > 0) exit(1);

#ifdef OPEN_SYSLOG
    openlog("app_daemon_process", LOG_PID | LOG_USER, LOG_DEBUG);
    syslog(LOG_USER | LOG_DEBUG, "app_daemon_process::%d\n", getpid());
#else
    fprintf(stdout, "app_daemon_process::%d\n", getpid());
#endif
    sigset_t new_mask, old_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGHUP);
    sigaddset(&new_mask, SIGINT);
    sigaddset(&new_mask, SIGQUIT);
    sigaddset(&new_mask, SIGILL);
    sigaddset(&new_mask, SIGTRAP);
    sigaddset(&new_mask, SIGABRT);
    sigaddset(&new_mask, SIGFPE);
    sigaddset(&new_mask, SIGKILL);
    sigaddset(&new_mask, SIGUSR1);
    sigaddset(&new_mask, SIGSEGV);
    sigaddset(&new_mask, SIGUSR2);
    sigaddset(&new_mask, SIGPIPE);
    sigaddset(&new_mask, SIGALRM);
    sigaddset(&new_mask, SIGTERM);
    sigaddset(&new_mask, SIGCHLD);
    sigaddset(&new_mask, SIGCONT);
    sigaddset(&new_mask, SIGSTOP);
    sigaddset(&new_mask, SIGTSTP);
    sigaddset(&new_mask, SIGTTIN);
    sigaddset(&new_mask, SIGTTOU);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);

    signal(SIGHUP, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGILL, signal_handler);
    signal(SIGTRAP, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGKILL, signal_handler);
    signal(SIGUSR1, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGUSR2, signal_handler);
    signal(SIGPIPE, signal_handler);
    signal(SIGALRM, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGCHLD, signal_handler);
    signal(SIGCONT, signal_handler);
    signal(SIGSTOP, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGTTIN, signal_handler);
    signal(SIGTTOU, signal_handler);

    int fd1[2], fd2[2];
    pipe2(fd1, O_NONBLOCK);
    pipe2(fd2, O_NONBLOCK);

    pid_t pid = fork();
    if (pid < 0) app_daemon_process();
    else if (pid > 0) father_process(fd1, fd2);
    else child_process(fd1, fd2);
}

void app_daemon_thrown(JNIEnv *env, char *buffer)
{

    jclass newExcCls = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
    if (newExcCls == NULL)
	return;

    (*env)->ThrowNew(env, newExcCls, buffer);
}

JNIEXPORT void JNICALL Java_app_1daemon_run(JNIEnv *env, jclass obj)
{

    jclass cls = (*env)->FindClass(env, "app_daemon");
    if (cls == NULL) 
	return app_daemon_thrown(env, "FindClass::err");

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "call", "()V");
    if (method == NULL) 
	return app_daemon_thrown(env, "GetStaticMethodID::err");

    (*env)->CallStaticVoidMethod(env, cls, method);
}

int main(int argc, char **argv)
{

    app_daemon_process();
    return 0;
}



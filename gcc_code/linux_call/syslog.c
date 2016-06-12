#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>

void signal_handler(int sig);

int main(int argc, char **argv)
{

    //void openlog(const char *ident, int option, int facility);
    openlog(argv[0], LOG_PID | LOG_USER, LOG_DEBUG);

    //fprintf(stdout, "pid = %d\n", getpid());
    //void syslog(int priority, const char *format, ...);
    syslog(LOG_USER | LOG_DEBUG, "pid = %d\n", getpid());

    //void closelog(void);

    //typedef void (*sighandler_t) (int);
    //sighandler_t signal(int signum, sighandler_t handler);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    //signal(SIGKILL, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGPIPE, signal_handler);

    for (;;)
    {
	fprintf(stdout, ".");
	fflush(stdout);
	sleep(1);
    }

    return 0;
}

void signal_handler(int sig)
{
    syslog(LOG_USER | LOG_DEBUG, "Caught signal %d\n", sig);
}

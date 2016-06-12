#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#include<signal.h>

struct timeval time;

void signal_callback() 
{
    time.tv_usec++; 
}
static int _gettimeofday(struct timeval* tt) {

    tt->tv_sec = time.tv_sec;
    tt->tv_usec = time.tv_usec;
}
int main() {

    struct itimerval timer;
    gettimeofday(&time, NULL);
    signal(SIGALRM, signal_callback);
    
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    struct timeval my;
    while(1){

	_gettimeofday(&my);
	sleep(200);
	printf("sec usec \t %ld:%ld\n", my.tv_sec, my.tv_usec);
    }
    return 1;
}

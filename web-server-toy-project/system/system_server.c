#include <stdio.h>
#include <sys/prctl.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#include <system_server.h>
#include <gui.h>
#include <input.h>
#include <web_server.h>

static int toy_timer = 0;

static void timer_expire_signal_handler() {
    printf("timer_expire_signal_handler: %d\n", toy_timer);
    toy_timer++;
}

void set_periodic_timer(long sec_delay, long usec_delay) {
    struct itimerval itimer_val = {
		 .it_interval = { .tv_sec = sec_delay, .tv_usec = usec_delay },
		 .it_value = { .tv_sec = sec_delay, .tv_usec = usec_delay }
    };

    setitimer(ITIMER_REAL, &itimer_val, (struct itimerval*)0);
}

int posix_sleep_ms(unsigned int timeout_ms) {
    struct timespec sleep_time;

    sleep_time.tv_sec = timeout_ms / MILLISEC_PER_SECOND;
    sleep_time.tv_nsec = (timeout_ms % MILLISEC_PER_SECOND) * (NANOSEC_PER_USEC * USEC_PER_MILLISEC);

    return nanosleep(&sleep_time, NULL);
}

int system_server()
{
    struct itimerspec ts;
    struct sigaction sa;
    struct sigevent sev;
    timer_t *tidlist;

    printf("나 system_server 프로세스!\n");

    /* 5sec timer creation */
    signal(SIGALRM, timer_expire_signal_handler);

    /* 5sec timer registeration */
    set_periodic_timer(5, 0);

    while (1) {
        posix_sleep_ms(5000);
    }
    

    return 0;
}

int create_system_server()
{
    pid_t systemPid;
    const char *name = "system_server";

    printf("여기서 시스템 프로세스를 생성합니다.\n");

    /* fork */
   switch (systemPid = fork()) {
    case -1:
        printf("fork failed\n");
    case 0:
        /* change process name */
        if (prctl(PR_SET_NAME, (unsigned long) name) < 0)
            perror("prctl()");
        system_server();
        break;
    default:
        break;
    }
    
    
    return 0;
}

#include <stdio.h>
#include <sys/prctl.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

#include <system_server.h>
#include <gui.h>
#include <input.h>
#include <web_server.h>

pthread_mutex_t system_loop_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  system_loop_cond  = PTHREAD_COND_INITIALIZER;
bool            system_loop_exit = false;    ///< true if main loop should exit

static int toy_timer = 0;

void signal_exit(void);

static void timer_expire_signal_handler() {
    toy_timer++;
    signal_exit();
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

void *watchdog_thread(void* arg) {
    char *s = arg;

    printf("%s", s);

    while (1)
    {
        posix_sleep_ms(5000);
    }

    return 0; 
}

void *disk_service_thread(void* arg) {
    char *s = arg;

    printf("%s", s);

    while (1)
    {
        posix_sleep_ms(5000);
    }

    return 0;    
}

void *monitor_thread(void* arg) {
    char *s = arg;

    printf("%s", s);

    while (1)
    {
        posix_sleep_ms(5000);
    }

    return 0; 
}

void *camera_service_thread(void* arg) {
    char *s = arg;

    printf("%s", s);

    toy_camera_open();
    toy_camera_take_picture();

    while (1)
    {
        posix_sleep_ms(5000);
    }

    return 0; 
}

void signal_exit(void)
{
    pthread_mutex_lock(&system_loop_mutex);
    system_loop_exit = true;
    pthread_cond_broadcast(&system_loop_cond);
    pthread_mutex_unlock(&system_loop_mutex);
}

int system_server()
{
    struct itimerspec ts;
    struct sigaction sa;
    struct sigevent sev;
    timer_t *tidlist;

    int retcode;
    pthread_t watchdog_thread_tid, disk_service_thread_tid, monitor_thread_tid, camera_service_thread_tid;

    printf("나 system_server 프로세스!\n");

    /* 5sec timer creation */
    signal(SIGALRM, timer_expire_signal_handler);

    /* 5sec timer registeration */
    set_periodic_timer(5, 0);


    /* Thread creation */
    retcode = pthread_create(&watchdog_thread_tid, NULL, watchdog_thread, "watchdog_thread\n");
    assert(retcode == 0);
    retcode = pthread_create(&disk_service_thread_tid, NULL, disk_service_thread, "watchdog_thread\n");
    assert(retcode == 0);
    retcode = pthread_create(&monitor_thread_tid, NULL, monitor_thread, "watchdog_thread\n");
    assert(retcode == 0);
    retcode = pthread_create(&camera_service_thread_tid, NULL, camera_service_thread, "watchdog_thread\n");
    assert(retcode == 0);

    printf("system init done. waiting...");

    /* cond wait로 대기, 10초 후 알람이 울리면 <== system 출력 */
    pthread_mutex_lock(&system_loop_mutex);
    /* wake-up every 1sec */
    while (system_loop_exit == false) {
        sleep(1);
    }
    pthread_mutex_unlock(&system_loop_mutex);

    printf("<== system\n");

    /* wake-up every 1sec */
    while (1) {
        sleep(1);
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

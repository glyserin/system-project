#include <stdio.h>

#include <system_server.h>
#include <gui.h>
#include <input.h>
#include <web_server.h>

int create_gui()
{
    pid_t systemPid;

    printf("여기서 GUI 프로세스를 생성합니다.\n");

    sleep(3);
    
    /* fork + exec */
    /* exec chromium-browser */
    switch (systemPid = fork()) {
    case -1:
        printf("fork failed\n");
    case 0:
        if (execl("/usr/bin/chromium-browser", "chromium-browser", "http://localhost:8282", NULL)) {
            printf("execfailed\n");
        }
        break;
    default:
        break;
    }
    
    return 0;
}

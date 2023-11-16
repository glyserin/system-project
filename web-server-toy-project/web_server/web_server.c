#include <stdio.h>

#include <system_server.h>
#include <gui.h>
#include <input.h>
#include <web_server.h>

int create_web_server()
{
    pid_t systemPid;

    printf("여기서 Web Server 프로세스를 생성합니다.\n");

    /* fork + exec 를 이용하세요 */
    /* exec으로 filebrowser을 실행 하세요. */
    /* execl("/usr/local/bin/filebrowser", "filebrowser", "-p", "8282", (char *) NULL)) */
    if ((systemPid = fork()) < 0)
    {
        perror("create_web_server error\n");
        exit(-1);
    }
    else if (systemPid == 0)
    {
        execl("/usr/local/bin/filebrowser", "filebrowser", "-p", "8282", (char *) NULL);
    }

    return 0;
}

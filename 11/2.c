#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int
main(void) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        pid_t pid2;
        if ((pid2 = fork()) == 0) {
            printf("3 ");
            fflush(stdout);
            _exit(0);
        } else {
            waitpid(pid2, NULL, 0);
            printf("2 ");
            fflush(stdout);
        }
        _exit(0);
    } else {
        waitpid(pid, NULL, 0);
        printf("1\n");
        fflush(stdout);
    }

    return 0;
}
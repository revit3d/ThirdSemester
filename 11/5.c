#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int
main(void) {
    pid_t pid;
    pid_t ppid = getpid();
    int return_value;
    int n;
    while (scanf("%d", &n) != EOF) {
        pid = fork();
        if (pid == -1) {
            _exit(23);
        }
        if (pid != 0) {
            wait(&return_value);
            if (WIFEXITED(return_value) && WEXITSTATUS(return_value) == 23) {
                if (getpid() == ppid) {
                    while (wait(NULL) != -1) { }
                    printf("-1\n");
                    exit(0);
                } else {
                    _exit(23);
                }
            } else {
                printf("%d\n", n);
                fflush(stdout);
                _exit(0);
            }
        }
    }

    return 0;
}
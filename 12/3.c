#include <linux/limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/wait.h>

int
main (int argc, char *argv[]) {
    int running = 0;
    int status = 0;
    int okcnt = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == 'p') {
            if (!fork()) {
                execl(argv[i] + 1, argv[i] + sizeof(char), NULL);
                _exit(1);
            } else {
                running++;
            }
        } else if (argv[i][0] == 's') {
            for (int ch = 0; ch < running; ch++) {
                wait(&status);
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    okcnt++;
                }
            }
            running = 0;
            if (!fork()) {
                execl(argv[i] + 1, argv[i] + sizeof(char), NULL);
                _exit(1);
            } else {
                wait(&status);
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    okcnt++;
                }
            }
        }
    }
    while (wait(&status) != -1) {
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            okcnt++;
        }
    }
    printf("%d\n", okcnt);

    return 0;
}
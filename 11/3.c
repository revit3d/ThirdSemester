#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int
main(void) {
    pid_t pid;
    int n;
    char str[9];
    for (int i = 1; i <= 3; i++) {
        if ((pid = fork()) == 0) {
            read(0, str, 8);
            n = atoi(str);
            printf("%d %d\n", i, n * n);
            fflush(stdout);
            _exit(0);
        }
    }
    while (wait(NULL) != -1) { }
    exit(0);
}
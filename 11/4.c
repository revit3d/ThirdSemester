#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    int n;
    int cur = 0;
    int sync = 1;
    scanf("%d", &n);
    pid_t pid;
    pid_t ppid = getpid();
    do {
        cur++;
        if (cur == n) {
            printf("%d\n", cur);
            fflush(stdout);
            _exit(0);
        } else {
            printf("%d ", cur);
            fflush(stdout);
        }
        if ((pid = fork()) == 0) {
            sync++;
        }
    } while (cur < sync && cur < n);
    wait(NULL);
    if (getpid() != ppid) {
        _exit(0);
    } else {
        exit(0);
    }
}
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

volatile sig_atomic_t flag = 0;

void int_handler() {
    flag = 0;
}

void quit_handler() {
    flag = 1;
}

int main(void) {
    int i = 0;
    int res = 0;
    signal (SIGFPE, SIG_IGN);
    signal(SIGINT, int_handler);
    signal(SIGQUIT, quit_handler);
    printf("%d\n", getpid());
    fflush(stdout);
    while (scanf("%d", &i) > 0) {
        if (flag & 1) {
            res *= (long long)i;
        } else {
            res += (long long)i;
        }
        printf("%d\n", res);
        fflush(stdout);
    }

    return 0;
}
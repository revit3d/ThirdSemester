#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t i = 0;

void handler() {
    signal(SIGHUP, handler);
    if (i < 5) {
        printf("%d\n", i);
        fflush(stdout);
    }
    i++;
}
 
int main(void) {
    signal(SIGHUP, handler);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
        if (i == 6) {
            break;
        }
    }

    return 0;
}
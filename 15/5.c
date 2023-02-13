#include <sys/signalfd.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

sig_atomic_t term_flag;

typedef long long longint[2];

void handler(int sig) {
    if (sig == SIGTERM) {
        term_flag = 0;
    } else {

    }
}

int main (int argc, char *argv[]) {
    enum {
        ERROR = -1,
        MAXFIFO = 20,
    };
    sigset_t sigset;
    int sigfd;
    char **fifo = argv + 1;
    size_t fifo_count = argc - 1;
    longint data[MAXFIFO];
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGTERM);
    for (int i = 0; i < fifo_count; i++) {
        sigaddset(&sigset, SIGRTMIN+i);
    }
    if (sigprocmask(SIG_BLOCK, &sigset, NULL) == ERROR) {
        perror("sigprocmask");
        _exit(EXIT_FAILURE);
    }
    if ((sigfd = signalfd(-1, &sigset, 0)) == ERROR) {
        perror("signalfd");
        _exit(EXIT_FAILURE);
    }
    printf("%d\n", getpid());
    while (1) {
        
    }

    exit(EXIT_SUCCESS);
}
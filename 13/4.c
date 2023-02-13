#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

void handle_error(pid_t *chpids, int chcnt) {
    for (int i = 1; i < chcnt; i++) {
        kill(chpids[i], SIGKILL);
    }
    while (wait(NULL) > 0) {}
    exit(1);
}

int main(int argc, char *argv[])
{
    enum {
        ERROR = -1,
        PIPE_FDCNT = 2,
        MAX_PIDCNT = 10000,
    };
    int fd[PIPE_FDCNT] = { STDIN_FILENO, STDOUT_FILENO };
    pid_t chpids[MAX_PIDCNT];
    for (int i = 1; i < argc; i++) {
        int tmpfd[PIPE_FDCNT] = { STDIN_FILENO, STDOUT_FILENO };
        if (i != argc - 1) {
            if (pipe2(tmpfd, O_CLOEXEC) == ERROR) {
                handle_error(chpids, i);
            }
        }
        chpids[i] = fork();
        if (chpids[i] == -1) {
            handle_error(chpids, i);
        }
        if (chpids[i] == 0) {
            if (dup2(fd[0], STDIN_FILENO) == ERROR
            || dup2(tmpfd[1], STDOUT_FILENO) == ERROR
            || execlp(argv[i], argv[i], NULL) == ERROR) {
                _exit(1);
            }
        }
        if (tmpfd[1] != STDOUT_FILENO) {
            if (close(tmpfd[1]) == ERROR) {
                handle_error(chpids, i + 1);
            }
        }
        if (fd[0] != STDIN_FILENO) {
            if (close(fd[0]) == ERROR) {
                handle_error(chpids, i + 1);
            }
        }
        fd[0] = tmpfd[0];
    }
    while(wait(NULL) > 0) {};
    exit(0);
}

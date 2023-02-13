#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void) {
    pid_t pid;
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        pid = getpid();
        write(fd[1], &pid, sizeof(pid_t));
        kill(getpid(), SIGSTOP);
        read(fd[0], &pid, sizeof(pid_t));
        printf("son1: %d\n", (int)pid);
        fflush(stdout);
        close(fd[0]);
        close(fd[1]);
        _exit(0);
    }
    if (fork() == 0) {
        read(fd[0], &pid, sizeof(pid_t));
        printf("son2: %d\n", (int)pid);
        fflush(stdout);
        kill(pid, SIGCONT);
        pid = getpid();
        write(fd[1], &pid, sizeof(pid_t));
        close(fd[0]);
        close(fd[1]);
        _exit(0);
    }
    close(fd[0]);
    close(fd[1]);

    return 0;
}

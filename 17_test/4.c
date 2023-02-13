#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <wait.h>

int main (int argc, char *argv[]) {
    int res;
    int exitstat = 0;
    if (!fork()) {
        int fd[2];
        pipe(fd);
        if (!fork()) {
            int fd_rd = open(argv[2], O_RDONLY);
            if (fd_rd == -1) {
                _exit(1);
            }
            dup2(fd_rd, 0);
            dup2(fd[1], 1);
            close(fd_rd);
            close(fd[1]);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }
        close(fd[1]);
        if (!fork()) {
            dup2(fd[0], 0);
            close(fd[0]);
            execlp(argv[3], argv[3], NULL);
            _exit(1);
        }
        close(fd[0]);
        while (wait(&res) > 0) {
            if (WIFEXITED(res)) {
                exitstat |= WEXITSTATUS(res);
            } else {
                exitstat |= 1;
            }
        }
        return exitstat;
    }
    wait(&res);
    if (!res) {
        if (!fork()) {
            execlp(argv[4], argv[4], NULL);
            _exit(1);
        }
    }
    wait(NULL);

    return 0;
}
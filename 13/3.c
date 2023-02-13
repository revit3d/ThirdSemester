#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <wait.h>

int main (int argc, char *argv[]) {
    int fd[2];
    int res;
    pipe(fd);
    if (!fork()) {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        if (!fork()) {
            int fd_rd = open(argv[4], O_RDONLY);
            dup2(fd_rd, 0);
            close(fd_rd);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }
        wait(&res);
        if (!WIFEXITED(res) || WEXITSTATUS(res) != 0) {
            _exit(1);
        }
        if (!fork()) {
            execlp(argv[2], argv[2], NULL);
            _exit(1);
        }
        wait(&res);
        if (WIFEXITED(res)) {
            return WEXITSTATUS(res);
        }
        _exit(1);
    }
    close(fd[1]);
    if (!fork()) {
        int fd_wr = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, 0777);
        dup2(fd[0], 0);
        dup2(fd_wr, 1);
        close(fd[0]);
        close(fd_wr);
        execlp(argv[3], argv[3], NULL);
        _exit(1);
    }
    close(fd[0]);
    while (wait(NULL) > 0) {}
    _exit(0);

    return 0;
}
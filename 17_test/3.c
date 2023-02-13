#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
    setvbuf(stdin, NULL, _IONBF, 0);
    pid_t ppid = getpid();
    int num = 0;
    int fd[2];
    scanf("%d", &num);
    printf("%d %d\n", ppid, num);
    fflush(stdout);
    pipe(fd);
    while (num != -1) {
        while (num > 0) {
            if (!fork()) {
                close(fd[0]);
                num--;
                if (num == 0) {
                    if (scanf("%d", &num) == EOF) {
                        num = -1;
                        write(fd[1], &num, sizeof(num));
                        close(fd[1]);
                        _exit(0);
                    }
                    printf("%d %d\n", getpid(), num);
                    fflush(stdout);
                    write(fd[1], &num, sizeof(num));
                    close(fd[1]);
                    _exit(0);
                }
            } else {
                while (wait(NULL) > 0) { }
                if (ppid != getpid()) {
                    close(fd[0]);
                    close(fd[1]);
                    _exit(0);
                }
                num = -1;
            }
        }
        read(fd[0], &num, sizeof(num));
    }
    close(fd[1]);
    close(fd[0]);

    return 0;
}
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main (void) {
    struct tm time_pr;
    time_t cur_time;
    int fd[2];
    pipe(fd);
    if (!fork()) {
        if (!fork()) {
            if (!fork()) {
                close(fd[0]);
                cur_time = time(NULL);
                for (int i = 0; i < 3; i++) {
                    write(fd[1], &cur_time, sizeof(time_t));
                }
                close(fd[1]);
                _exit(0);
            }
            close(fd[1]);
            wait(NULL);
            read(fd[0], &cur_time, sizeof(time_t));
            localtime_r(&cur_time, &time_pr);
            printf("D:%02d\n", time_pr.tm_mday);
            fflush(stdout);
            close(fd[0]);
            _exit(0);
        }
        close(fd[1]);
        wait(NULL);
        read(fd[0], &cur_time, sizeof(time_t));
        localtime_r(&cur_time, &time_pr);
        printf("M:%02d\n", time_pr.tm_mon + 1);
        fflush(stdout);
        close(fd[0]);
        _exit(0);
    }
    close(fd[1]);
    wait(NULL);
    read(fd[0], &cur_time, sizeof(time_t));
    localtime_r(&cur_time, &time_pr);
    printf("Y:%04d\n", time_pr.tm_year + 1900);
    fflush(stdout);
    close(fd[0]);

    return 0;
}
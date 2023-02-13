#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <wait.h>

int main(void) {
    int fd[2];
    long long buf;
    pipe(fd);
    if (!fork()) {
        if(!fork()) {
            long long ans = 0;
            close(fd[1]);
            while (read(fd[0], &buf, sizeof(long long))) {
                if (buf == LLONG_MAX) {
                    printf("%lld\n", ans);
                    fflush(stdout);
                    _exit(0);
                } else {
                    ans += buf;
                }
            }
            _exit(1);
        }
        wait(NULL);
        close(fd[0]);
        close(fd[1]);
        _exit(0);
    }
    close(fd[0]);
    while (scanf("%lld", &buf) == 1) {
        write(fd[1], &buf, sizeof(long long));
    }
    buf = LLONG_MAX;
    write(fd[1], &buf, sizeof(long long));
    close(fd[1]);
    wait(NULL);

    return 0;
}
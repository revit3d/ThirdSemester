#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void work(int rfd, int wfd, int chd_pos, int maxn)
{
    enum {
        MIN_READ_PIPE = 1,
        MIN_WRITE_STD = 4,
        MIN_WRITE_PIPE = 2,
    };
    int curnum;
    FILE *istream = fdopen(rfd, "r");
    FILE *ostream = fdopen(wfd, "a");
    while (1) {
        if (fscanf(istream, "%d", &curnum) < MIN_READ_PIPE) {
            _exit(1);
        }
        if (curnum == maxn) {
            if (close(wfd) != 0 || close(rfd) != 0) {
                _exit(1);
            }
            break;
        }
        if (printf("%d %d\n", chd_pos, curnum) < MIN_WRITE_STD
            || fflush(stdout) != 0) {
            _exit(1);
        }
        curnum++;
        if (fprintf(ostream, "%d\n", curnum) < MIN_WRITE_PIPE
            || fflush(ostream) != 0) {
            _exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    enum {
        PIPE_FDCNT = 2,
        FIRST_CHD = 1,
        SECOND_CHD = 2,
        MIN_WRITE_PIPE = 2,
        MIN_WRITE_STD = 5,
    };
    int maxn;
    int fd12[PIPE_FDCNT];
    int fd21[PIPE_FDCNT];
    pipe(fd12);
    pipe(fd21);
    maxn = atoi(argv[1]);

    if (!fork()) {
        close(fd12[0]); 
        close(fd21[1]);
        work(fd21[0], fd12[1], FIRST_CHD, maxn);
        _exit(0);
    }
    if (!fork()) {
        close(fd12[1]); 
        close(fd21[0]);
        work(fd12[0], fd21[1], SECOND_CHD, maxn);
        _exit(0);
    }
    FILE *istream = fdopen(fd21[1], "w");
    if (fprintf(istream, "%d\n", 1) != MIN_WRITE_PIPE
        || fflush(istream) != 0) {
        return 1;
    }

    if (close(fd12[0])
        || close(fd12[1])
        || close(fd21[0])
        || close(fd21[1])) {
        _exit(1);
    }

    while (wait(NULL) > 0) {}
    if (printf("Done\n") != MIN_WRITE_STD) {
        _exit(1);
    }

    return 0;
}
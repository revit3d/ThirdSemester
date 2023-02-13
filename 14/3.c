#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

int start(char *argv) {
    FILE *fstream;
    if ((fstream = fopen(argv, "r")) == NULL) {
        return 0;
    }
    char s[PATH_MAX] = {};
    fgets(s, PATH_MAX, fstream);
    s[strlen(s) - 1] = '\0';
    if (!fork()) {
        execlp(s, s, NULL);
        _exit(1);
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);
    int running = 0;
    int ans = 0;
    int status;
    for (int i = 2; i < argc; i++) {
        if (running < N) {
            running += start(argv[i]);
        } else {
            while (wait(&status) > 0) {
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    ans++;
                }
            }
            start(argv[i]);
        }
    }
    while (wait(&status) > 0) {
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            ans++;
        }
    }
    printf("%d\n", ans);

    return 0;
}
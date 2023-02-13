#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <stdio.h>

extern ssize_t getcwd2(int fd, char *buf, size_t size);

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }
    char buf[100000];
    char buf_pwd[100000];
    DIR *dirp;
    if ((dirp = opendir(argv[1])) == NULL) {
        fprintf(stderr, "Cannot open directory\n");
        return 1;
    }
    int fd = dirfd(dirp);
    if (fd == -1) {
        fprintf(stderr, "Cannot get descriptor\n");
        return 1;
    }
    ssize_t ans = getcwd2(fd, buf, atoi(argv[2]));
    if (ans == -1) {
        printf("ERROR\n");
        return 0;
    }
    if (getcwd(buf_pwd, 100000) != NULL) {
        if (!strncmp(buf_pwd, buf, atoi(argv[2]))) {
            printf("OK\n%s\n%ld\n", buf, ans);
        } else {
            printf("WA\n%s\n%s\nlen: %ld, correct: %ld\n",
            buf, buf_pwd, strlen(buf), strlen(buf_pwd));
        }
    } else {
        fprintf(stderr, "Cannot distinguish current directory\n");
        return 1;
    }
    closedir(dirp);

    return 0;
}
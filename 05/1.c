#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int 
main (int argc, char *argv[]) {
    long long ans = 0;
    struct stat res;
    for (int i = 1; i < argc; i++) {
        if (access(argv[i], F_OK) == -1) {
            continue;
        }
        lstat(argv[i], &res);
        if (S_ISREG(res.st_mode) && !S_ISLNK(res.st_mode)
            && res.st_size % 1024 == 0 && res.st_nlink == 1) {
            ans += res.st_size;
        }
    }
    printf("%lld\n", ans);

    return 0;
}
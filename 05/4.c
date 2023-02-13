#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int
comp (const char** left, const char** right) {
    return strcmp(*left, *right);
}

int
main (int argc, char *argv[]) {
    enum { 
        ERROR = -1,
        MAXN = 10000,
        HEAD = 1
    };
    if (argc == 1) {
        return 0;
    }
    struct stat file_info;
    ino_t files_ino[MAXN];
    qsort(
        argv + HEAD, 
        argc - HEAD,
        sizeof(char**), 
        (int(*) (const void *, const void *))comp
    );
    for (int curfile = argc - 1; curfile >= HEAD; curfile--) {
        if (stat(argv[curfile], &file_info) == ERROR) {
            files_ino[curfile] = ERROR;
            continue;
        }
        files_ino[curfile] = file_info.st_ino;
        for (int prev_file = argc - 1; prev_file > curfile; prev_file--) {
            if (files_ino[prev_file] == files_ino[curfile]) {
                files_ino[curfile] = ERROR;
            }
        }
    }
    for (int curfile = HEAD; curfile < argc; curfile++) {
        if (files_ino[curfile] != ERROR) {
            printf("%s\n", argv[curfile]);
        }
    }

    return 0;
}
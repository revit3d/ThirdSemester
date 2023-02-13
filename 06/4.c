#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void 
read_dir(char *path, int max_size, int root_len, int depth) {
    enum {
        MAX_DEPTH = 4,
        STEP = 1,
        ERROR = -1,
        CUR_PATH = 2,
        PREV_PATH = 3
    };
    if (depth > MAX_DEPTH) {
        return;
    }
    DIR *dp = opendir(path);
    if (dp == NULL) {
        return;
    }
    struct dirent *rec;
    struct stat finf;
    char name[PATH_MAX];
    int pos, pos2;
    size_t path_len = strlen(path);
    while ((rec = readdir(dp)) != NULL) {
        size_t dlen = strlen(rec->d_name);
        if (rec->d_name[0] == '.'
            && (dlen <= CUR_PATH || (rec->d_name[1] == '.' && dlen <= PREV_PATH))) {
            continue;
        }
        pos = 0;
        for (pos = 0; pos < path_len; pos++) {
            name[pos] = path[pos];
        }
        if (pos > 0 && name[pos - 1] != '/') {
            name[pos] = '/';
            pos++;
        }
        for (pos2 = 0; pos2 < dlen; pos2++) {
            name[pos + pos2] = rec->d_name[pos2];
        }
        name[pos + pos2] = '\0';
        if (lstat(name, &finf) != ERROR) {
            if (S_ISDIR(finf.st_mode)) {
                read_dir(name, max_size, root_len, depth + STEP);
            } else if (S_ISREG(finf.st_mode) 
                && access(name, R_OK) == 0
                && finf.st_size <= max_size) {
                printf("%s\n", name + root_len);
            }
        }
    }
    closedir(dp);
}

int
main (int argc, char *argv[]) {
    enum {
        ARGS_COUNT = 3,
        DIR_PATH = 1,
        MAX_SIZE = 2,
    };
    if (argc < ARGS_COUNT) {
        return 1;
    }
    read_dir(argv[DIR_PATH], atoi(argv[MAX_SIZE]), strlen(argv[DIR_PATH]) + 1, 1);

    return 0;
}
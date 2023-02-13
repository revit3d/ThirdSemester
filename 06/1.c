#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int check_name(char *name) {
    int len = strlen(name);
    return name[len - 4] == '.' && name[len - 3] == 'e'
        && name[len - 2] == 'x' && name[len - 1] == 'e';
}

int
main (int argc, char *argv[]) {
    int ans = 0;
    if (argc < 2) {
        return 0;
    }
    DIR *dp = opendir(argv[1]);
    struct dirent *rec;
    struct stat finf;
    char *name;
    int pos;
    while ((rec = readdir(dp)) != NULL) {
        name = calloc(strlen(rec->d_name) + strlen(argv[1]) + 1 + 1, sizeof(char));
        pos = 0;
        for (pos = 0; pos < strlen(argv[1]); pos++) {
            name[pos] = argv[1][pos];
        }
        if (name[pos] != '/') {
            name[pos] = '/';
            pos++;
        }
        for (int i = 0; i < strlen(rec->d_name); i++) {
            name[pos + i] = rec->d_name[i];
        }
        if (stat(name, &finf) != -1
            && S_ISREG(finf.st_mode)
            && access(name, X_OK) == 0
            && check_name(rec->d_name)) {
            ans++;
        }
        free(name);
    }
    printf("%d\n", ans);
    closedir(dp);

    return 0;
}
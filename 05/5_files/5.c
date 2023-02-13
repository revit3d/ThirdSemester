#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *
concat(const char *str1, const char *str2) {
    enum { 
        NULL_TERM = 1,
        SLASH = 1,
    };
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    char *res = calloc(len1 + len2 + NULL_TERM + SLASH, sizeof(char));
    if (res == NULL) {
        exit(1);
    }
    int pos;
    for (pos = 0; pos < len1; pos++) {
        res[pos] = str1[pos];
    }
    if (res[pos - 1] != '/') {
        res[pos] = '/';
        pos++;
    }
    for (int i = 0; i < len2; i++) {
        res[pos + i] = str2[i];
    }
    return res;
}

unsigned long long dfs(char *dirname, int usr) {
    DIR *dirp = opendir(dirname);
    struct dirent *fdir;
    struct stat finf;
    char *path;
    unsigned long long res = 0;
    int dircnt = 0;
    char *dirnames[100000];
    while ((fdir = readdir(dirp)) != NULL) {
        if (!strcmp(fdir->d_name, ".") || !strcmp(fdir->d_name, "..")) {
            continue;
        }
        path = concat(dirname, fdir->d_name);
        lstat(path, &finf);
        if (S_ISREG(finf.st_mode) && finf.st_uid == usr) {
            res += finf.st_size;
        } else if (S_ISDIR(finf.st_mode)) {
            int len = strlen(path);
            dirnames[dircnt] = calloc(len + 2, sizeof(char));
            for (int i = 0; i < len; i++) {
                dirnames[dircnt][i] = path[i];
            }
            dirnames[dircnt][len] = '/';
            dirnames[dircnt][len + 1] = '\0';
            dircnt++;
        }
        free(path);
    }
    closedir(dirp);
    for (int i = 0; i < dircnt; i++) {
        res += dfs(dirnames[i], usr);
    }
    return res;
}

int
main (int argc, char *argv[]) {
    unsigned long long res;
    res = dfs(argv[1], atoi(argv[2]));
    printf("%llu\n", res);

    return 0;
}
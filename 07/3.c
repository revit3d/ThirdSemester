#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

void
concat(const char *str1, const char *str2, char *res) {
    enum { 
        NULL_TERM = 1,
        SLASH = 1,
    };
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int pos;
    for (pos = 0; pos < len1; pos++) {
        res[pos] = str1[pos];
    }
    if (res[pos - 1] != '/') {
        res[pos] = '/';
        pos++;
    }
    for (int i = 0; i <= len2; i++) {
        res[pos + i] = str2[i];
    }
}

int
main (int argc, char *argv[]) {
    enum {
        FIRST_DIR = 1,
        SECOND_DIR = 2,
        ERROR = -1,
        MAXPATH = 100000,
    };
    DIR *first_dir;
    DIR *second_dir;
    struct dirent *fdir;
    struct dirent *sdir;
    struct stat file1_info;
    struct stat file2_info;
    long long sum = 0;
    long begin;
    char path1[MAXPATH];
    char path2[MAXPATH];
    if (argc < 3 || (first_dir = opendir(argv[FIRST_DIR])) == NULL 
        || (second_dir = opendir(argv[SECOND_DIR])) == NULL
        || (begin = telldir(second_dir)) == ERROR) {
        return 1;
    }
    while ((fdir = readdir(first_dir)) != NULL) {
        seekdir(second_dir, begin);
        if (strcmp(fdir->d_name, ".") != 0 && strcmp(fdir->d_name, "..") != 0) {
            concat(argv[FIRST_DIR], fdir->d_name, path1);
            if (lstat(path1, &file1_info) == ERROR) {
                return 1;
            }
            if (access(path1, W_OK) != ERROR && S_ISREG(file1_info.st_mode)) {
                while ((sdir = readdir(second_dir)) != NULL) {
                    if (strcmp(sdir->d_name, ".") != 0
                        && strcmp(sdir->d_name, "..") != 0
                        && strcmp(fdir->d_name, sdir->d_name) == 0) {
                        concat(argv[SECOND_DIR], sdir->d_name, path2);
                        if (stat(path2, &file2_info) == ERROR) {
                            return 1;
                        }
                        if (file1_info.st_dev == file2_info.st_dev
                            && file1_info.st_ino == file2_info.st_ino) {
                            sum += file1_info.st_size;
                        }
                    }
                }
            }
        }
    }
    closedir(first_dir);
    closedir(second_dir);
    printf("%lld\n", sum);
}
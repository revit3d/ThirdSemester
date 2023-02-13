#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

char **files;

enum {
    FILENUM = 0,    //file position in argv
    ID = 1,         //file inode
    FEATURES = 2    //number of file features
};

int
sgn(long long num) {
    printf("num: %lld\n", num);
    int res = num / llabs(num);
    printf("res: %d\n", res);
    return num / llabs(num);
}

int 
comp_files_info(const long long **left, const long long **right) {
    return sgn((*left)[ID] - (*right)[ID]);
}

int
comp_files_path(const int *left, const int *right) {
    return strcmp(files[*left], files[*right]);
}

int
main (int argc, char *argv[]) {
    if (argc == 1) {
        return 0;
    }
    files = argv;
    enum { 
        ERROR = -1,
        MAXN = 10000,   //max files
        HEAD = 1        //first file index in files_info
    };
    long long *files_info[MAXN];
    int unique_files[MAXN];
    int ans_size = 0;
    struct stat file_info;
    //getting inode of the file and binding it to file position in argv
    for (int curfile = 1; curfile < argc; curfile++) {
        files_info[curfile] = calloc(FEATURES, sizeof(long long));
        files_info[curfile][FILENUM] = curfile;
        if (stat(argv[curfile], &file_info) == ERROR) {
            files_info[curfile][ID] = LLONG_MAX;
        } else {
            files_info[curfile][ID] = file_info.st_ino;
        }
    }
    //sort file_info by ID, so the same file will be
    //a subset of consistent elements of resulting array
    qsort(
        files_info + HEAD, 
        argc - HEAD, 
        sizeof(long long*),
        (int(*) (const void *, const void *))comp_files_info
    );
    int file1_idx;
    int file2_idx;
    int grep_cur = HEAD;
    int comp;
    //filtering unique files
    for (int curfile = HEAD + 1; curfile < argc; curfile++) {
        if (files_info[curfile - 1][ID] == files_info[curfile][ID]) {
            file1_idx = files_info[grep_cur][FILENUM];
            file2_idx = files_info[curfile][FILENUM];
            //find max lexicographical file length with the same ID
            comp = strcmp(argv[file1_idx], argv[file2_idx]);
            if (comp < 0) {
                //file1 < file2
                grep_cur = curfile;
            }
        } else {
            //if a subset of the files with the same ID ended
            //add to the answer max lexicographical path to this file
            unique_files[ans_size] = files_info[grep_cur][FILENUM];
            ans_size++;
            //and make a new 'path of choice' first path to the file
            //with new ID
            grep_cur = curfile;
        }
    }
    if (files_info[grep_cur][ID] != LLONG_MAX) {
        unique_files[ans_size] = files_info[grep_cur][FILENUM];
        ans_size++;
    }
    //sort unique files indices by lexicographical order of 
    //corresponding paths in argv
    qsort(
        unique_files,
        ans_size,
        sizeof(int),
        (int(*) (const void *, const void *))comp_files_path
    );
    int path_idx;
    for (int curidx = 0; curidx < ans_size; curidx++) {
        path_idx = unique_files[curidx];
        printf("%s\n", argv[path_idx]);
    }

    return 0;
}
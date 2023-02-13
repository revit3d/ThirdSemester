#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <dlfcn.h>

int main (int argc, char *argv[]) {
    enum {
        ERROR = -1,
        ACCESS_MASK = 0700,
        FILE_NAME_MAX = 30,
        REJECT = -1,
        SUMMON = 0,
        DISQUALIFY = 1,
        SPECIFIER_LEN = 2,
    };

    char c_file_name[FILE_NAME_MAX] = {};
    char c_file_path[PATH_MAX] = {};
    char inter_file_path[PATH_MAX] = {};
    char shared_file_path[PATH_MAX] = {};
    char *c_file_text = 
    "int categorize(int x){\n\tenum {\n\t\treject = -1,\n\t\tsummon = 0,\n\t\tdisqualify = 1\n};\n\n\treturn %s;\n}\n";
    char *dir_path;
    FILE *fstream;
    int fd;
    unsigned int seed;
    void *handle;
    int (*categorize) (int);
    int x;
    int res;

    //recognize the path for temporary files
    if ((dir_path = getenv("XDG_RUNTIME_DIR")) == NULL) {
        if ((dir_path = getenv("TMPDIR")) == NULL) {
            dir_path = "/tmp";
        }
    }

    //change seed
    if ((fd = open("/dev/urandom", O_RDONLY)) == ERROR
        || read(fd, &seed, sizeof(unsigned int)) == ERROR
        || close(fd) == ERROR) {
        fprintf(stderr, "cannot access /dev/urandom\n");
        exit(1);
    }
    srand(seed);

    //create py file
    sprintf(c_file_name, "%d%d.c", rand(), rand());
    sprintf(c_file_path, "%s/%s", dir_path, c_file_name);
    sprintf(shared_file_path, "%s/%s.so", dir_path, c_file_name);

    //paste code
    if ((fd = open(c_file_path, O_WRONLY | O_CREAT, ACCESS_MASK)) == ERROR
        || (fstream = fdopen(fd, "w")) == NULL) {
        fprintf(stderr, "cannot create c file\n");
        exit(1);
    }
    if (fprintf(fstream, c_file_text, argv[1]) != strlen(c_file_text) + strlen(argv[1]) - SPECIFIER_LEN
        || fflush(fstream) != 0) {
        fprintf(stderr, "failed to write code to temporary file\n");
        exit(1);
    }
    fflush(fstream);
    if (close(fd) == ERROR) {
        fprintf(stderr, "cannot close c file\n");
        exit(1);
    }

    //create interpretator file
    sprintf(inter_file_path, "%s/%d%d", dir_path, rand(), rand());
    
    //paste code
    if ((fd = open(inter_file_path, O_WRONLY | O_CREAT, ACCESS_MASK)) == ERROR
        || (fstream = fdopen(fd, "w")) == NULL) {
        fprintf(stderr, "cannot create interpretator file\n");
        exit(1);
    }
    if (fprintf(
        fstream, 
        "#! /bin/sh\ngcc -m32 -O2 -Wall -Werror -std=gnu11 -fPIC -DPIC -shared -o %s %s\n", 
        shared_file_path,
        c_file_path
        ) == 0
        || fflush(fstream) != 0) {
        fprintf(stderr, "failed to write code to temporary file\n");
        exit(1);
    }
    fflush(fstream);
    if (close(fd) == ERROR) {
        fprintf(stderr, "cannot close c file\n");
        exit(1);
    }

    //compile instruction as dynamic library
    if (!fork()) {
        execl(inter_file_path, inter_file_path, NULL);
        fprintf(stderr, "failed starting gcc\n");
    }
    wait(NULL);

    //retrieve symbol from dynamic library
    if ((handle = dlopen(shared_file_path, RTLD_LAZY)) == NULL
        || (categorize = dlsym(handle, "categorize")) == NULL) {
        fprintf(stderr, "failed retrieving symbol from dynamic library\n");
        exit(1);
    }

    while (scanf("%d", &x) == 1) {
        res = categorize(x);
        if (res == REJECT) {
            printf("reject\n");
        } else if (res == SUMMON) {
            printf("summon\n");
        }
        else if (res == DISQUALIFY) {
            printf("disqualify\n");
        }
    }

    if (remove(c_file_path) != 0
        || remove(shared_file_path) != 0
        || remove(inter_file_path) != 0) {
        fprintf(stderr, "failed deleting temporary files\n");
        exit(1);
    }

    exit(0);
}
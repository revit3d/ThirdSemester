#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int main (int argc, char *argv[]) {
    enum {
        ERROR = -1,
        ACCESS_MASK = 0711,
        FILE_NAME_MAX = 30,
    };

    char inter_file_path[PATH_MAX] = {};
    char py_file_path[PATH_MAX] = {};
    char py_file_name[FILE_NAME_MAX] = {};
    char inter_file_name[FILE_NAME_MAX] = {};
    FILE *fstream;
    char *dir_path;
    int fd;
    unsigned int seed;

    //recognize the path for temporary files
    if ((dir_path = getenv("XDG_RUNTIME_DIR")) == NULL) {
        if ((dir_path = getenv("TMPDIR")) == NULL) {
            dir_path = "/tmp";
        }
    }

    //change seed
    if ((fd = open("/dev/urandom", O_RDONLY)) == ERROR
        || read(fd, &seed, sizeof(unsigned int)) == ERROR
        || close(fd)) {
        fprintf(stderr, "cannot access /dev/urandom\n");
        exit(1);
    }
    srand(seed);

    //create py file
    sprintf(py_file_name, "%d%d.py", rand(), rand());
    sprintf(py_file_path, "%s/%s", dir_path, py_file_name);

    //paste code
    if ((fd = open(py_file_path, O_WRONLY | O_CREAT, ACCESS_MASK)) == ERROR
        || (fstream = fdopen(fd, "w")) == NULL) {
        fprintf(stderr, "cannot create py file\n");
        exit(1);
    }
    fprintf(fstream, "import sys\nsys.set_int_max_str_digits(100000)\n");
    fprintf(fstream, "print(\n\t%s", argv[1]);
    for (int i = 2; i < argc; i++) {
        fprintf(fstream, "\n\t*%s", argv[i]);
    }
    fprintf(fstream, "\n)\n");
    fflush(fstream);
    if (close(fd) == ERROR) {
        fprintf(stderr, "cannot close py file\n");
        exit(1);
    }

    //create interpretator file
    sprintf(inter_file_name, "%d%d", rand(), rand());
    sprintf(inter_file_path, "%s/%s", dir_path, inter_file_name);

    //paste code
    if ((fd = open(inter_file_path, O_WRONLY | O_CREAT, ACCESS_MASK)) == ERROR
        || (fstream = fdopen(fd, "w")) == NULL) {
        fprintf(stderr, "cannot create interpretator file\n");
        exit(1);
    }
    fprintf(fstream, "#! /bin/sh\npython3 %s\nrm %s\nrm %s\n", py_file_path, py_file_path, inter_file_path);
    fflush(fstream);
    if (close(fd) == ERROR) {
        fprintf(stderr, "cannot close interpretator file\n");
        exit(1);
    }

    execl(inter_file_path, inter_file_name, NULL);
    fprintf(stderr, "failed executing file\n");
    _exit(1);
}
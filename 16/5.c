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
        ACCESS_MASK = 0600,
        FILE_NAME_MAX = 30,
        REJECT = -1,
        SUMMON = 0,
        DISQUALIFY = 1,
        SPECIFIER_LEN = 2,
        LEFT_BOUND_ARG = 1,
        RIGHT_BOUND_ARG = 2,
        N_ARG = 3,
        EXPR_ARG = 4,
    };

    char c_file_name[FILE_NAME_MAX] = {};
    char c_file_path[PATH_MAX] = {};
    char shared_file_path[PATH_MAX] = {};
    char *c_file_text = 
    "#include <math.h>\n\ndouble calc(double x){\n\treturn %s;\n}\n";
    char *dir_path = ".";
    FILE *fstream;
    int fd;
    unsigned int seed;
    void *handle;
    double (*calc) (double);
    double left = atof(argv[LEFT_BOUND_ARG]);
    double right = atof(argv[RIGHT_BOUND_ARG]);
    int n = atoi(argv[N_ARG]);
    double dx = (right - left) / n;
    double res = 0;

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
    if (fprintf(fstream, c_file_text, argv[EXPR_ARG]) != strlen(c_file_text) + strlen(argv[EXPR_ARG]) - SPECIFIER_LEN
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
        execlp(
            "gcc",
            "gcc",
            "-m32",
            "-lm",
            "-O2", 
            //"-Wall", 
            //"-Werror", 
            "-std=gnu11", 
            "-fPIC", 
            "-DPIC", 
            "-shared", 
            "-o", 
            shared_file_path, 
            c_file_path, 
            NULL
        );
        fprintf(stderr, "failed starting gcc\n");
    }
    wait(NULL);

    //retrieve symbol from dynamic library
    if ((handle = dlopen(shared_file_path, RTLD_LAZY)) == NULL
        || (calc = dlsym(handle, "calc")) == NULL) {
        fprintf(stderr, "failed retrieving symbol from dynamic library\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        res += calc(left + i * dx) * dx;
    }
    printf("%.10g\n", res);
    fflush(stdout);

    if (remove(c_file_path) != 0
        || remove(shared_file_path) != 0) {
        fprintf(stderr, "failed deleting temporary files\n");
        exit(1);
    }

    exit(0);
}
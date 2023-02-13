#include <stdio.h>

extern int copy_file(const char *srcpath, const char *dstpath);

int main(int argc, char *argv[]) 
{
    if (argc < 3) {
        printf("too little arguments\n");
        return 0;
    }
    printf("copy_file exit code: %d\n", copy_file(argv[1], argv[2]));

    return 0;
}


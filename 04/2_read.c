#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int
main (int argc, char *argv[]) {
    enum { NSIZE = sizeof(double) };
    int file = 0;
    if ((file = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    int n;
    double rd;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        if (read(file, &rd, NSIZE) != NSIZE) {
            fprintf(stderr, "Cannot read file\n");
            return 1;
        }
        printf("%lf\n", rd);
    }
    close(file);

    return 0;
}
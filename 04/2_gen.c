#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int
main (int argc, char *argv[]) {
    enum { NSIZE = sizeof(double) };
    int file = 0;
    if ((file = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0600)) == -1) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    double buf[1000];
    int n = 10;
    if (argc < 3 || argv[2][0] == '0') {
        for (int i = 0; i < 10; i++)
            buf[i] = i + 1;
    }
    else {
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
            scanf("%lf", &buf[i]);
    }
    for (int i = 0; i < n; i++) {
        if (write(file, &buf[i], NSIZE) != NSIZE) {
            fprintf(stderr, "Cannot write to file\n");
            return 1;
        }
    }
    close(file);

    return 0;
}
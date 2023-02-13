#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int
main (int argc, char *argv[]) {
    enum { 
    BUFSIZE = sizeof(unsigned int),
    SHIFT_1 = 8,
    SHIFT_2 = 12,
    SHIFT_3 = 20,
    MASK_1 = 0xFF,
    MASK_2 = 0x0F
    };
    int file = 0;
    if (argc == 0 || (file = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0600)) == -1) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    unsigned int cur = 0;
    char buf[BUFSIZE];
    while (scanf("%u", &cur) == 1) {
        buf[0] = (cur >> SHIFT_3) & MASK_2;
        buf[1] = (cur >> SHIFT_2) & MASK_1;
        buf[2] = (cur >> SHIFT_1) & MASK_2;
        buf[3] = cur & MASK_1;
        if (write(file, buf, BUFSIZE) != BUFSIZE) {
            fprintf(stderr, "Error writing to file\n");
            close(file);
            return 1;
        }
    }
    close(file);

    return 0;
}
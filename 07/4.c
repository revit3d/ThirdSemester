#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int 
main (int argc, char *argv[]) {
    enum {
        ERROR = -1,
        BIT_COUNT = 8,
    };
    int file;
    long long input;
    int inp_pos;
    unsigned char byte;
    struct stat file_info;
    if (argc < 1 || (file = open(argv[1], O_RDWR)) == ERROR) {
        return 1;
    }
    if (fstat(file, &file_info) == ERROR) {
        return 1;
    }
    while (scanf("%lld", &input) == 1) {
        if (input) {
            inp_pos = 1;
            if (input < 0) {
                inp_pos = 0;
                input = -input;
            }
            if (input <= file_info.st_size * BIT_COUNT) {
                errno = 0;
                lseek(file, (input - 1) / BIT_COUNT, SEEK_SET);
                if (errno || read(file, &byte, sizeof(char)) != sizeof(char)) {
                    return 1;
                }
                input = (input - 1) % BIT_COUNT;
                if (inp_pos) {
                    byte |= (1 << input);
                } else {
                    byte &= ~(1 << input);
                }
                errno = 0;
                lseek(file, -1, SEEK_CUR);
                if (errno || write(file, &byte, sizeof(char)) != sizeof(char)) {
                    return 1;
                }
            }
        }
    }
    if (close(file) == ERROR) {
        return 1;
    }

    return 0;
}
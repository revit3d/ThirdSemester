#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int
main (int argc, char *argv[]) {
    enum { 
        READN = sizeof(char),
        WRITEN = sizeof(int)
    };
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }
    int filein = 0;
    int fileout = 0;
    if ((filein = open(argv[1], O_RDONLY)) == -1
        || (fileout = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    int mod = atoi(argv[3]);
    char buf;
    int cur_num = 1;
    int cur_sum = 1;
    while (read(filein, &buf, READN) == READN) {
        for (int i = 0; i < CHAR_BIT; i++) {
            if ((buf >> i) & 1) {
                if (write(fileout, &cur_sum, WRITEN) != WRITEN) {
                    fprintf(stderr, "Cannot write to file\n");
                    return 1;
                }
            }
            cur_num++;
            cur_sum = (cur_sum + cur_num * (long long)cur_num) % mod;
        }
    }
    close(filein);
    close(fileout);

    return 0;
}
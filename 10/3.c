#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

int
main(int argc, char *argv[]) {
    enum {
        ERROR = -1,
        FILE_ARG = 1,
        ROWS_ARG = 2,
        COLS_ARG = 3,
        ARGCNT = 3,
        BASE = 10,
        RIGHT = 0,
        DOWN = 1,
        LEFT = 2,
        UP = 3,
    };
    int rows;
    int cols;
    int currow;
    int curcol;

    int right_bound;
    int down_bound;
    int up_bound;
    int left_bound;

    int filled;
    int mov_flag;
    int size;
    int file;
    int32_t *arr;
    errno = 0;
    rows = strtol(argv[ROWS_ARG], NULL, BASE);
    cols = strtol(argv[COLS_ARG], NULL, BASE);
    if (errno) {
        return 1;
    }
    if (argc < ARGCNT || (file = open(argv[FILE_ARG], O_RDWR | O_CREAT | O_TRUNC)) == ERROR) {
        return 1;
    }
    lseek(file, (rows * cols - 1) * sizeof(int32_t), SEEK_SET);
    if (write(file, "   ", sizeof(int32_t)) == ERROR) {
        return 1;
    }
    errno = 0;
    arr = mmap(NULL, rows * cols * sizeof(int32_t), PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    if (errno) {
        return 1;
    }
    close(file);
    currow = 0, curcol = 0;
    filled = 1;
    right_bound = cols;
    left_bound = -1;
    up_bound = -1;
    down_bound = rows;
    size = rows * cols;
    mov_flag = RIGHT;
    while (filled <= size) {
        if (mov_flag == RIGHT) {
            arr[currow * cols + curcol] = filled;
            filled++;
            if (curcol + 1 == right_bound) {
                mov_flag = DOWN;
                up_bound++;
                currow++;
            } else {
                curcol++;
            }
        } else if (mov_flag == DOWN) {
            arr[currow * cols + curcol] = filled;
            filled++;
            if (currow + 1 == down_bound) {
                mov_flag = LEFT;
                right_bound--;
                curcol--;
            } else {
                currow++;
            }
        } else if (mov_flag == LEFT) {
            arr[currow * cols + curcol] = filled;
            filled++;
            if (curcol - 1 == left_bound) {
                mov_flag = UP;
                down_bound--;
                currow--;
            } else {
                curcol--;
            }
        } else if (mov_flag == UP) {
            arr[currow * cols + curcol] = filled;
            filled++;
            if (currow - 1 == up_bound) {
                mov_flag = RIGHT;
                left_bound++;
                curcol++;
            } else {
                currow--;
            }
        }
    }
    if (munmap(arr, rows * cols * sizeof(int32_t)) == ERROR) {
        return 1;
    }

    return 0;
}
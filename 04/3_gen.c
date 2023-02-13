#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    NODESIZE = sizeof(int) * 3
};

int SIZE;
int arr[1000];

void create(int file, int cur) {
    int buf[3] = { arr[cur], 2 * cur, 2 * cur + 1 };
    lseek(file, cur * NODESIZE, SEEK_CUR);
    if (write(file, buf, NODESIZE) != NODESIZE) {
        fprintf(stderr, "Cannot write to file\n");
        exit(1);
    }
    create(file, cur * 2);
    create(file, cur * 2 + 1);
}

int
main (int argc, char *argv[]) {
    int file = 0;
    if ((file = open(argv[1], O_WRONLY)) == -1) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    scanf("%d", &SIZE);
    for (int i = 0; i < SIZE; i++)
        scanf("%d", &arr[i]);
    create(file, 0);
    close(file);

    return 0;
}
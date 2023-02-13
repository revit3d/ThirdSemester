#include <stdio.h>
#include <stdlib.h>

int main (void) {
    int size;
    int adr;
    int file_size;
    scanf("%d", &size);
    int *FAT = calloc(size, sizeof(int));
    for (int i = 2; i < size - 2; i++) {
        scanf("%d", &FAT[i]);
    }
    while (scanf("%d", &adr) > 0) {
        file_size = 0;
        while (FAT[adr] != 0) {
            adr = FAT[adr];
            file_size++;
        }
        printf("%d\n", file_size);
    }

    return 0;
}
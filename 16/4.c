#include <stdio.h>

int main(void) {
    unsigned size = 0, offset = 1;
    unsigned pole, mempole;
    unsigned pow;
    while (scanf("%u", &pole) > 0) {
        pow = 1;
        mempole = pole;
        while (pole % 2 == 0) {
            pow *= 2;
            pole /= 2;
        }
        if (offset < pow) {
            offset = pow;
        }
        size += mempole;
        if (size % pow != 0) {
            size += pow - (size % pow);
        }
    }

    if (size % offset != 0) {
        size += offset - (size % offset);
    }

    if (size == 0) {
        size = 1;
    }
    printf("%u %u\n", size, offset);

    return 0;
}
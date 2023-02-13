#include <stdio.h>

extern void normalize_path(char *buf);

int
main (void) {
    char str[100000];
    scanf("%s", str);
    normalize_path(str);
    printf("%s\n", str);

    return 0;
}
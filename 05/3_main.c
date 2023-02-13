#include <stdio.h>

extern int parse_rwx_permissions(const char *str);

int 
main (void) {
    char s[10005];
    scanf("%10000s", s);
    printf("%d\n", parse_rwx_permissions(s));

    return 0;
}
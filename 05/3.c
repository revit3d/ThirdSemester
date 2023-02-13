#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int
parse_rwx_permissions(const char *str) {
    char *temp1 = "rwxrwxrwx";
    int tlen = sizeof("rwxrwxrwx");
    if (str == NULL || strlen(str) != tlen - 1) {
        return -1;
    }
    for (int i = 0; i + 1 < tlen; i++) {
        if (str[i] != '-' && str[i] != temp1[i]) {
            return -1;
        }
    }
    int res = 0;
    for (int i = 0; i + 1 < tlen; i++) {
        if (str[i] != '-') {
            res += (1 << (tlen - i - 2));
        } else {
        }
    }
    
    return res;
}
#include <stdio.h>

void
gen(int pos, int len, int *used, char *buf){
    if(pos == len){
        printf("%s\n", buf);
        return;
    }
    for(int i = 1; i <= len; i++){
        if(!used[i]){
            used[i] = 1;
            buf[pos] = '0' + i;
            gen(pos + 1, len, used, buf);
            used[i] = 0;
        }
    }
}

int
main(void){
    enum { MAXN = 10 };
    int used[MAXN] = {};
    char buf[MAXN] = {};
    int n;
    if(scanf("%d", &n) != 1)
        return 1;
    if(n <= 0 || n >= MAXN)
        return 1;
    gen(0, n, used, buf);

    return 0;
}
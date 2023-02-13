#include <stdio.h>

extern void sort_even_odd(size_t count, int* data);

int 
main(void){
    enum { MAXN = 10 };
    int n;
    scanf("%d", &n);
    int arr[MAXN];
    for(int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    sort_even_odd(n, arr);
    for(int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
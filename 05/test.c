#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int comp (const int **i, const int **j)
{
    return (*i)[1] - (*j)[1];
}

int main(void)
{
    char srcpath[10000];
    //scanf("%s", srcpath);
    printf("%s\n", srcpath);
    struct stat src_info;
    if (access(srcpath, R_OK) == -1) {
        fprintf(stderr, "file is not accessible\n");
        exit(1);
    }
    printf("%d\n", S_ISDIR(src_info.st_mode));

    /*int *num[10];
    int i, a, b;
    for(i = 0; i < 10; i++) {
        scanf("%d %d", &a, &b);
        num[i] = calloc(2, sizeof(int));
        num[i][0] = a;
        num[i][1] = b;
    }
    printf("Original array: \n");
    for (i=0; i<10; i++) 
        printf("%d %d\n", num[i][0], num[i][1]);
    printf ("\n");
    int shift = 2;
    qsort(
        num + 2, 
        10 - 2, 
        sizeof(int*), 
        (int(*) (const void *, const void *)) comp
    );
    printf("Sorted array: \n");
    for(i = 0; i <10; i++) 
        printf("%d %d\n", num[i][0], num[i][1]);
    */
    return 0;
}
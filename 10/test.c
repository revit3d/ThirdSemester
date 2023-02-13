#include <stdio.h>

typedef union union_type { 
    int i; 
    double d; 
    char *s;
    char *byte[2];
} union_type;

typedef struct function_argument {
    union_type data;
    int data_type;
} function_argument;

double fuck_chernov(char *msg, double len, int times) {
    printf("%s %lf vot takim %d times\n", msg, len, times);
    return 101.35;
}

typedef double (*arbitrary)();
//typedef void* (*arbitrary)();

int main(void) {
    arbitrary prakman_pidor;
    char *msg = "Chernova viebali";
    function_argument args[16];
    union_type f = { }, s = { }, t = { };
    f.s = msg, s.i = 5, t.d = 30.5;
    args[0].data = f;
    args[1].data = t;
    args[2].data = s;
    args[0].data_type = 0;
    args[1].data_type = 1;
    args[2].data_type = 2;
    void *handle = (void *)fuck_chernov;
    *(void**)(&prakman_pidor) = handle;
    for (int i = 2; i >= 0; i--) {
        if (args[i].data_type == 1) {
            asm (
                "push %1\n"
                "push %0\n"
                : :
                "g" (args[i].data.byte[0]), "g" (args[i].data.byte[1])
            );
        } else {
            asm (
                "push %0\n"
                : :
                "g" (args[i].data.byte[0])
            );
        }
    }
    double res = prakman_pidor();
    for (int i = 0; i < 4; i++) {
        asm (
            "pop %eax\n\t"
        );
    }
    printf("%lf\n", res);

    return 0;
}
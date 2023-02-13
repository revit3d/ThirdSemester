/*
    Программе в аргументах командной строки передаются следующие параметры:

    Имя динамической библиотеки для загрузки
    Имя функции в этой библиотеке
    Сигнатура функции
    Далее идут аргументы функции (могут отсутствовать)

    Ваша программа должна вызвать указанную функцию, 
    передать ей указанные параметры в соответствии с 
    сигнатурой и вывести на стандартный поток вывода 
    результат, возвращенный этой функцией.
*/

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>

enum { MAX_BYTES = 2 };

typedef void   (*arbitrary_void)();
typedef int    (*arbitrary_int)();
typedef double (*arbitrary_double)();
typedef char*  (*arbitrary_string)();

typedef union union_type { 
    int     i; 
    double  d; 
    char    *s;
    char    *byte[MAX_BYTES];
} union_type;

typedef struct function_argument {
    union_type  data;
    int         data_type;
} function_argument;

int 
main(int argc, char *argv[]) {
    enum {
        LIBRARY_NAME = 1,
        SYMBOL_NAME = 2,
        FUNC_SIGNATURE = 3,
        MIN_ARGCNT = 3,
        VOID_TYPE = 0,
        INT_TYPE = 1,
        DOUBLE_TYPE = 2,
        STRING_TYPE = 3,
        BASE = 10,
        OFFSET = 16,
    };

    void *handle;
    void *sym;
    int spec_len = strlen(argv[FUNC_SIGNATURE]);
    int args_byte_size = 0;
    function_argument **args = calloc(spec_len, sizeof(function_argument *));

    //check errors and open dynamic library, get symbol
    if (args == NULL
        || argc < MIN_ARGCNT 
        || (handle = dlopen(argv[LIBRARY_NAME], RTLD_LAZY)) == NULL
        || (sym = dlsym(handle, argv[SYMBOL_NAME])) == NULL) {
        fprintf(stderr, "Error retrieving needed function from dynamic library\n");
        return 1;
    }

    //parse function arguments
    for (int i = 1; i < spec_len; i++) {
        args[i] = calloc(1, sizeof(function_argument));
        if (args[i] == NULL) {
            return 1;
        }
        if (argv[FUNC_SIGNATURE][i] == 'i') {
            errno = 0;
            args[i]->data_type = INT_TYPE;
            args[i]->data.i = strtol(argv[MIN_ARGCNT + i], NULL, BASE);
            if (errno) {
                return 1;
            }
            args_byte_size += sizeof(int);
        } else if (argv[FUNC_SIGNATURE][i] == 'd') {
            errno = 0;
            args[i]->data_type = DOUBLE_TYPE;
            args[i]->data.d = strtod(argv[MIN_ARGCNT + i], NULL);
            if (errno) {
                return 1;
            }
            args_byte_size += sizeof(double);
        } else if (argv[FUNC_SIGNATURE][i] == 's') {
            args[i]->data_type = STRING_TYPE;
            args[i]->data.s = argv[MIN_ARGCNT + i];
            args_byte_size += sizeof(char *);
        }
    }

    /* Et non est deus */

    //stack offset
    asm (
        "sub    %0, %%esp\n\t"
        : :
        "g" (OFFSET - (args_byte_size % OFFSET))
    );
    args_byte_size += OFFSET - (args_byte_size % OFFSET);

    //push function arguments on stack
    for (int i = spec_len - 1; i >= 1; i--) {
        if (args[i]->data_type == DOUBLE_TYPE) {
            asm (
                "push %1\n\t"
                "push %0\n\t"
                : :
                "g" (args[i]->data.byte[0]), "g" (args[i]->data.byte[1])
            );
        } else {
            asm (
                "push %0\n\t"
                : :
                "g" (args[i]->data.byte[0])
            );
        }
    }

    //call function with given return value
    if (argv[FUNC_SIGNATURE][0] == 'v') {
        arbitrary_void dfunc = sym;
        dfunc();
    } else if (argv[FUNC_SIGNATURE][0] == 'i') {
        arbitrary_int dfunc = sym;
        printf("%d\n", dfunc());
    } else if (argv[FUNC_SIGNATURE][0] == 'd') {
        arbitrary_double dfunc = sym;
        printf("%.10g\n", dfunc());
    } else if (argv[FUNC_SIGNATURE][0] == 's') {
        arbitrary_string dfunc = sym;
        printf("%s\n", dfunc());
    }

    //return stack pointer to its initial state
    asm (
        "add    %0, %%esp\n\t"
        : :
        "g" (args_byte_size)
    );

    dlclose(handle);
    for (int i = 1; i < spec_len; i++) {
        free(args[i]);
    }
    free(args);

    return 0;
}
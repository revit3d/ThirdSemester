#include <stdio.h>

int
main(void){
    enum Masks
    {
        SECOND_BIT = (1 << 2),
        THIRD_BIT = (1 << 3)
    };

    enum Shift
    {
        RESERVED_CODE_1 = 0,
        NUMBER_BEGIN = 1,
        NUMBER_END = 10,
        LOWER_CASE_BEGIN = 11,
        LOWER_CASE_END = 36,
        UPPER_CASE_BEGIN = 37,
        UPPER_CASE_END = 62,
        RESERVED_CODE_2 = 63
    };
    int ch;
    while((ch = getchar()) != EOF){
        unsigned int mask;
        if(ch >= '0' && ch <= '9')
            mask = ch - '0' + NUMBER_BEGIN;
        else if(ch >= 'a' && ch <= 'z')
            mask = ch - 'a' + LOWER_CASE_BEGIN;
        else if(ch >= 'A' && ch <= 'Z')
            mask = ch - 'A' + UPPER_CASE_BEGIN;
        else
            continue;
        mask ^= THIRD_BIT;
        mask &= ~SECOND_BIT;
        if(mask >= NUMBER_BEGIN && mask <= NUMBER_END)
            ch = '0' + mask - NUMBER_BEGIN;
        if(mask >= LOWER_CASE_BEGIN && mask <= LOWER_CASE_END)
            ch = 'a' + mask - LOWER_CASE_BEGIN;
        if(mask >= UPPER_CASE_BEGIN && mask <= UPPER_CASE_END)
            ch = 'A' + mask - UPPER_CASE_BEGIN;
        if(mask == RESERVED_CODE_1)
            ch = '@';
        if(mask == RESERVED_CODE_2)
            ch = '#';
        putchar(ch);
    }

    return 0;
}
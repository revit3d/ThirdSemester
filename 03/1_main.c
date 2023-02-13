#include <stdio.h>

typedef char STYPE;
typedef unsigned char UTYPE;

extern STYPE bit_reverse (STYPE value);

int main(void)
{
    long long input;
    scanf("%lld", &input);
    STYPE num = input;
    STYPE res = bit_reverse(num);
    for (int i = 0; i < sizeof(STYPE) * 8; i++)
    {
        printf("%d", num & 1);
        num >>= 1;
    }
    printf("\n");
    for (int i = 0; i < sizeof(STYPE) * 8; i++)
    {
        printf("%d", res & 1);
        res >>= 1;
    }
    printf("\n");

    return 0;
}
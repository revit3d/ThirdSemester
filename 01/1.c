#include <stdio.h>

int
main(void) {
    enum 
    {
    MOST_LEFT_X = 2,
    MIDDLE_X = 3,
    MOST_RIGHT_X = 5,
    HIGHEST_Y = 7,
    MIDDLE_Y = 3,
    LOWEST_Y = 1
    };
    double x, y;

    if (scanf("%lf %lf", &x, &y) != 2)
        return 1;
    if ((x >= MOST_LEFT_X && x <= MIDDLE_X && y >= LOWEST_Y && y <= HIGHEST_Y)
        || (x >= MIDDLE_X && x <= MOST_RIGHT_X && y >= MIDDLE_Y && y <= HIGHEST_Y) 
        || (x >= MIDDLE_X && x <= MOST_RIGHT_X && y >= LOWEST_Y && y <= MIDDLE_Y && (x - MOST_LEFT_X) <= y)) {
        printf("1\n");
        return 0;
    }
    printf("0\n");

    return 0;
}
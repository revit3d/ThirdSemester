#include <stdlib.h>

int comp(const int* left, const int* right) {
    if((*left & 1) != (*right & 1))
        return (*left & 1) - (*right & 1);
    long long diff = (long long)(*right) - *left;
    if(*left & 1)
        return (diff > 0) - (diff < 0);
    else
        return -((diff > 0) - (diff < 0));
    return 0;
}

void sort_even_odd(size_t count, int* data) {
    qsort(data, count, sizeof(int), (int(*) (const void*, const void*)) comp);
}
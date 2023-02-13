int 
satsum (int v1, int v2) {
    enum { MY_INT_MAX = (~0u) >> sizeof(char) };   // максимальное значение знакового 32-битного типа
    enum { MY_INT_MIN = ~((int)((~0u) >> sizeof(char))) };      // минимальное значение знакового 32-битного типа 
    if (__builtin_add_overflow(v1, v2, &v1)) {
        if (v2 < 0) {
            return MY_INT_MIN;
        } else {
            return MY_INT_MAX;
        }
    }
    return v1;
}
typedef char STYPE;
typedef unsigned char UTYPE;

STYPE 
bit_reverse (STYPE value) {
    UTYPE _unsigned = 1;
    UTYPE reversed = 0;
    while (_unsigned != 0) {
        reversed <<= 1;
        reversed += value & 1;
        value >>= 1;
        _unsigned <<= 1;
    }
    return (STYPE)reversed;
}
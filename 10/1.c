#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

int
main (int argc, char *argv[]) {
    enum {
        BASE = 16,
        PAGESIZE = 512,
        FILESIZE = 64 * 1024,
        ARGCNT = 2,
        ERROR = -1,
        MEMORY_DUMP_ARG = 1,
        TABLE_OFFSET_ARG = 2,
        SUCCESS = 1,
        BYTE = 8,
        OFFSET = 9,
    };
    if (argc < ARGCNT) {
        return 1;
    }
    unsigned short *memory;
    int file;
    size_t table_offset;
    unsigned short idx;
    if ((file = open(argv[MEMORY_DUMP_ARG], O_RDONLY)) == ERROR) {
        return 1;
    }
    errno = 0;
    table_offset = strtol(argv[TABLE_OFFSET_ARG], NULL, BASE);
    if (errno) {
        return 1;
    }
    table_offset >>= 1;
    errno = 0;
    memory = mmap(NULL, FILESIZE, PROT_READ, MAP_PRIVATE, file, 0);
    if (errno) {
        return 1;
    }
    close(file);
    while (scanf("%hx", &idx) == SUCCESS) {
        idx = (memory[table_offset + (idx >> OFFSET)] + (idx % PAGESIZE)) >> 1;
        printf("%hu\n", memory[idx]);
    }
    if (munmap(memory, FILESIZE) == ERROR) {
        return 1;
    }

    return 0;
}
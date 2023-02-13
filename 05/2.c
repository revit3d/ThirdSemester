#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *
parse(char num) {
    switch (num) {
    case '0':
        return "---";
    case '1':
        return "--x";
    case '2':
        return "-w-";
    case '3':
        return "-wx";
    case '4':
        return "r--";
    case '5':
        return "r-x";
    case '6':
        return "rw-";
    case '7':
        return "rwx";
    default:
        fprintf(stderr, "Wrong argument octal digit\n");
        exit(1);
    }
    return "";
}

int
main (int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        switch (strlen(argv[i])) {
        case 1:
            printf("------%s\n", parse(argv[i][0]));
            break;
        case 2:
            printf("---%s%s\n", parse(argv[i][0]), parse(argv[i][1]));
            break;
        case 3:
            printf("%s%s%s\n", parse(argv[i][0]), parse(argv[i][1]), parse(argv[i][2]));
            break;
        default:
            fprintf(stderr, "Wrong input\n");
            exit(1);
        }
    }

    return 0;
}
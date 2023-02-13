#include <sys/wait.h>
#include <unistd.h>

int 
call(char *command) {
    pid_t pid = fork();
    int status = 0;
    if (pid == 0) {
        execlp(command, command, NULL);
        _exit(1);
    }
    wait(&status);
    return WIFEXITED(status) && !WEXITSTATUS(status);
}

int 
main (int argc, char *argv[]) {
    if (argc < 4) {
        return 1;
    }
    return !((call(argv[1]) || call(argv[2])) && call(argv[3]));
}
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/eventfd.h>

int proc(void)
{
    int pid = fork();
    if (!pid) {
        write(1, "1\n", 2);
    }
    return pid;
}

int main(void)
{
    !proc() == !proc() ? proc() : proc();

    return 0;
}
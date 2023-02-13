#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

volatile int flag = 0;

void handler() {
    signal(SIGUSR1, handler);
    flag = 1;
}

void work(int ch_pos, int rfd, int wfd, int maxnum)
{
    int num;
    pid_t recv_pid;
    sigset_t ss;
    FILE *istream = fdopen(rfd, "r");
    FILE *ostream = fdopen(wfd, "a");
    signal(SIGUSR1, handler);
    sigemptyset(&ss); 
    sigaddset(&ss, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &ss, NULL);
    while (!flag) {
        pause();
    }
    flag = 0;
    fscanf(istream, "%d", &recv_pid);

    errno = 0;
    kill(getppid(), SIGUSR1);
    if (errno) {
        fprintf(stderr, "%s\n", strerror(errno));
        fflush(stderr);
        _exit(1);
    }

    while (1) {
        //wait for the signal to read from the buffer
        while (!flag) {
            pause();
        }
        flag = 0;
        fscanf(istream, "%d", &num);
        //exit condition
        if (num == maxnum) {
            fprintf(ostream, "%d\n", num);
            fflush(ostream);
            kill(recv_pid, SIGUSR1);
            return;
        }
        printf("%d %d\n", ch_pos, num);
        fflush(stdout);
        num++;
        //write next number to the buffer
        fprintf(ostream, "%d\n", num);
        fflush(ostream);
        //send signal to another child
        errno = 0;
        kill(recv_pid, SIGUSR1);
        if (errno) {
            fprintf(stderr, "%s\n", strerror(errno));
            fflush(stderr);
            _exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    int maxnum = atoi(argv[1]);
    sigset_t ss;
    int fd[2];
    signal(SIGUSR1, handler);
    sigemptyset(&ss); sigaddset(&ss, SIGUSR1);
    sigprocmask(SIG_BLOCK, &ss, NULL);
    pipe(fd);
    pid_t child1_pid;
    pid_t child2_pid;
    if ((child1_pid = fork()) == 0) {
        work(1, fd[0], fd[1], maxnum);
        _exit(0);
    }
    if ((child2_pid = fork()) == 0) {
        work(2, fd[0], fd[1], maxnum);
        _exit(0);
    }
    FILE *istream = fdopen(fd[1], "w");
    //send second child pid to first child
    fprintf(istream, "%d\n", child2_pid);
    fflush(istream);
    kill(child1_pid, SIGUSR1);
    //wait for the first child to read from buffer
    sigprocmask(SIG_UNBLOCK, &ss, NULL);
    while(!flag) {
        pause();
    }
    flag = 0;
    //send first child pid to second child
    fprintf(istream, "%d\n", child1_pid);
    fflush(istream);
    kill(child2_pid, SIGUSR1);
    //wait for the second child to read from buffer
    while(!flag) {
        pause();
    }
    flag = 0;
    //send first number to child 1
    fprintf(istream, "%d\n", 1);
    fflush(istream);
    kill(child1_pid, SIGUSR1);
    //wait for child 1 and child 2
    fflush(stdout);
    while (wait(NULL) > 0) {}

    close(fd[0]);
    close(fd[1]);
    printf("Done\n");

    return 0;
}
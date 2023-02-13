#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    enum {
        MAXN = 100,
        ACCESS_MASK = 0666,
    };
    setvbuf(stdin, NULL, _IONBF, 0);
    int num;
    int nproc = atoi(argv[1]);
    key_t key = ftok(argv[0], 's');
    int semid = semget(key, nproc, IPC_CREAT | ACCESS_MASK);
    struct sembuf sems[] = { { 0, 1, 0 }, };
    semop(semid, sems, 1);
    pid_t child[MAXN] = {};
    for (int i = 0; i < nproc; i++) {
        if ((child[i] = fork()) == 0) {
            struct sembuf down[] = { { (ushort)i, -1, 0 }, };
            while (1) {
                semop(semid, down, 1);
                if (scanf("%d", &num) < 1) {
                    _exit(0);
                }
                printf("%d %d\n", i, num);
                fflush(stdout);
                int next = (num % nproc + nproc) % nproc;
                struct sembuf up[] = { { (ushort)next, 1, 0 }, };
                semop(semid, up, 1);
            }
        }
    }
    int first_pid = wait(NULL);
    for (int i = 0; i < nproc; i++) {
        if (child[i] != first_pid) {
            kill(child[i], SIGKILL);
        }
    }
    while (wait(NULL) > 0) {}
    semctl(semid, 0, IPC_RMID, 0);

    return 0;
}
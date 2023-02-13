#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(int argc, char *argv[]) {
    int nproc = atoi(argv[1]);
    key_t key = atoi(argv[2]);
    int maxval = atoi(argv[3]);

    int semid = semget(key, nproc, IPC_CREAT | 0666);
    struct sembuf sems[] = { { 0, 1, 0 }, };
    semop(semid, sems, 1);
    int shmid = shmget(key, 2 * sizeof(unsigned int), IPC_CREAT | 0666);
    unsigned int *shmaddr = shmat(shmid, NULL, 0);
    shmaddr[0] = 0;
    shmaddr[1] = 0;

    for (int i = 0; i < nproc; i++) {
        if (fork() == 0) {
            struct sembuf buf[] = { { (unsigned short)i, -1, 0 }, };

            while (1) {
                if (semop(semid, buf, 1) < 0) {
                    return 0;
                }

                printf("%d %u %u\n", i + 1, shmaddr[0], shmaddr[1]);
                fflush(stdout);

                if (shmaddr[0] < maxval) {
                    shmaddr[0]++;
                    unsigned long long val = shmaddr[0] % nproc;
                    int next = (val * val * val * val) % nproc;
                    shmaddr[1] = i + 1;
                    struct sembuf sem_next[] = { { (unsigned short)next, 1, 0 }, };
                    semop(semid, sem_next, 1);
                } else {
                    return 0;
                }
            }
        }
    }
    wait(NULL);
    semctl(semid, 0, IPC_RMID, 0);
    while (wait(NULL) > 0) {}
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
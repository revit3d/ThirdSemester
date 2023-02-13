#include <unistd.h>
#include <stdio.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct Message
{
    long type;
    long long x1;
    long long x2;
} Message;

int main (int argc, char *argv[]) {
    enum {
        ERROR = -1,
        KEY_ARG = 1,
        NPROC_ARG = 2,
        VAL1_ARG = 3,
        VAL2_ARG = 4,
        MVAL_ARG = 5,
        ACCESS_MASK = 0666,
    };
    key_t key = atoi(argv[KEY_ARG]);
    int nproc = atoi(argv[NPROC_ARG]);
    Message msg = {
        .x1 = atoll(argv[VAL1_ARG]),
        .x2 = atoll(argv[VAL2_ARG]),
        .type = 1
    };
    long long maxval = atoll(argv[MVAL_ARG]);

    int msgid = msgget(key, IPC_CREAT | ACCESS_MASK);
    int msgsize = sizeof(msg) - sizeof(msg.type);

    for (int i = 0; i <= nproc; i++) {
        pid_t pid = fork();
        if (pid == ERROR) {
            msgctl(msgid, IPC_RMID, NULL);
            while (wait(NULL) > 0) {}
            exit(1);
        } else if (pid == 0) {
            while (1) {
                if (msgrcv(msgid, (void*)&msg, msgsize, i + 1, 0) == ERROR) {
                    _exit(0);
                }

                long long x3 = msg.x1 + msg.x2;
                printf("%d %lld\n", i, x3);
                fflush(stdout);

                if (llabs(x3) > maxval) {
                    msgctl(msgid, IPC_RMID, NULL);
                    _exit(0);
                } else {
                    msg.x1 = msg.x2;
                    msg.x2 = x3;
                    msg.type = x3 % nproc + 1;
                    msgsnd(msgid, (void*)&msg, msgsize, IPC_NOWAIT);
                }
            }
            _exit(0);
        }
    }
    msgsnd(msgid, (void *) &msg, msgsize, IPC_NOWAIT);
    while (wait(NULL) > 0) {}
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
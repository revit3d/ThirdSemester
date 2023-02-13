#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

volatile int flag;

void handler(int sig) {
    enum {
        BIT_0_SENT = 1,
        BIT_1_SENT = 2,
        BIT_RECV = 3,
        FILE_EOF = 4,
    };

    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    signal(SIGALRM, handler);
    signal(SIGIO, handler);

    if (sig == SIGUSR1) {
        flag = BIT_0_SENT;
    } else if (sig == SIGUSR2) {
        flag = BIT_1_SENT;
    } else if (sig == SIGALRM) {
        flag = BIT_RECV;
    } else if (sig == SIGIO) {
        flag = FILE_EOF;
    }
}

void fork_read(char *file, int rfd) {
    enum {
        BIT_0_SENT = 1,
        BIT_1_SENT = 2,
        BIT_RECV = 3,
        FILE_EOF = 4,
        BUF_SIZE = 4 * 1024,
        ERROR = -1,
        BYTE_SIZE = 8,
    };
    int fd;
    pid_t writer_pid;
    while (!flag) {
        pause();
    }
    flag = 0;
    if (read(rfd, &writer_pid, sizeof(pid_t)) == ERROR) {
        _exit(1);
    }
    if (close(rfd) == ERROR) {
        _exit(1);
    }
    kill(writer_pid, SIGUSR1);
    while (!flag) {
        pause();
    }
    flag = 0;
    int bytes_read;
    unsigned char buf[BUF_SIZE];
    if ((fd = open(file, O_RDONLY)) == ERROR) {
        fprintf(stderr, "failed to open the file\n");
        _exit(1);
    }
    do {
        if ((bytes_read = read(fd, buf, BUF_SIZE)) == ERROR) {
            _exit(1);
        }
        for (int byte = 0; byte < bytes_read; byte++) {
            for (int shift = 0; shift < BYTE_SIZE; shift++) {
                if ((buf[byte] >> shift) & 1) {
                    kill(writer_pid, SIGUSR2);
                } else {
                    kill(writer_pid, SIGUSR1);
                }
                while (flag != BIT_RECV) {
                    pause();
                }
                flag = 0;
            }
        }
    } while (bytes_read > 0);
    kill(writer_pid, SIGIO);
    if (close(fd) == ERROR) {
        _exit(1);
    }
}

void fork_write(int rfd) {
    enum {
        BIT_0_SENT = 1,
        BIT_1_SENT = 2,
        BIT_RECV = 3,
        FILE_EOF = 4,
        BUF_SIZE = 4 * 1024,
        BYTE_SIZE = 8,
        ERROR = -1,
    };
    pid_t reader_pid;
    while (!flag) {
        pause();
    }
    flag = 0;
    if (read(rfd, &reader_pid, sizeof(pid_t)) == ERROR) {
        _exit(1);
    }
    if (close(rfd) == ERROR) {
        _exit(1);
    }
    kill(reader_pid, SIGUSR1);
    int file_eof_rch = 0;
    unsigned char buf[BUF_SIZE] = { };
    int byte = 0;
    int shift = 0;
    while (!file_eof_rch) {
        while (flag != BIT_0_SENT && flag != BIT_1_SENT && flag != FILE_EOF) {
            pause();
        }
        if (flag == BIT_0_SENT) {
            buf[byte] &= ~(1u << shift);
        } else if (flag == BIT_1_SENT) {
            buf[byte] |= (1u << shift);
        } else if (flag == FILE_EOF) {
            file_eof_rch = 1;
        }
        flag = 0;
        shift++;
        if (shift == BYTE_SIZE) {
            shift = 0;
            byte++;
        }
        if (byte == BUF_SIZE) {
            byte = 0;
            shift = 0;
            if (write(STDOUT_FILENO, buf, BUF_SIZE) != BUF_SIZE) {
                _exit(1);
            }
        }
        if (!file_eof_rch) {
            kill(reader_pid, SIGALRM);
        }
    }
    if (byte) {
        if (write(STDOUT_FILENO, buf, byte) != byte) {
            _exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    enum {
        ERROR = -1,
    };
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    signal(SIGALRM, handler);
    signal(SIGIO, handler);
    int pipefd[2];
    pipe(pipefd);
    pid_t pid1, pid2;
    if ((pid1 = fork()) == 0) {
        if (close(pipefd[1]) == ERROR) {
            _exit(1);
        }
        fork_write(pipefd[0]);
        _exit(0);
    }
    if ((pid2 = fork()) == 0) {
        if (close(pipefd[1]) == ERROR) {
            _exit(1);
        }
        fork_read(argv[1], pipefd[0]);
        _exit(0);
    }
    if (write(pipefd[1], &pid1, sizeof(pid_t)) == ERROR
        || write(pipefd[1], &pid2, sizeof(pid_t)) == ERROR) {
        exit(1);
    }
    kill(pid2, SIGUSR1);
    if (close(pipefd[0]) == ERROR
        || close(pipefd[1]) == ERROR) {
        exit(1);
    }
    while (wait(NULL) > 0) {}

    return 0;
}
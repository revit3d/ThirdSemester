#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

volatile int int_flag = 0;

int is_prime(long long number) {
    enum { MIN_PRIME = 2 };
    if (number < MIN_PRIME) {
        return 0;
    }
    double sqr = sqrt(number);
    int b = MIN_PRIME;
    while (b < sqr + 1) {
        if (number % b == 0) {
            return 0;
        }
        b++;
    }
    return 1;
}

void handler(int sig) {
    enum { MAX_INT_RCVD = 4 };
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    if (sig == SIGTERM) {
        exit(0);
    } else if (sig == SIGINT) {
        int_flag++;
        if (int_flag == MAX_INT_RCVD) {
            exit(0);
        }
    }
}

int main(void) {
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    long long low, high, last = 0;
    int int_flag_svd = 0;
    scanf("%lld %lld", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);
    while (low < high) {
        if (is_prime(low)) {
            last = low;
        }
        if (int_flag_svd != int_flag) {
            printf("%lld\n", last);
            fflush(stdout);
            int_flag_svd++;
        }
        low++;
    }
    printf("-1\n");
    fflush(stdout);

    return 0;
}
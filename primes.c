#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

int is_prime(long long n) {
    if (n < 2) return 0;
    for (long long i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void mine_primes(long long start, int step) {
    long long number = start;
    while (keep_running) {
        if (is_prime(number)) {
            printf("%lld\n", number);
        }
        number += step;
    }
}

int main() {
    printf("Press Ctrl+C to stop.\n");
    sleep(1);

    signal(SIGINT, handle_sigint);

    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    pid_t pids[num_cores];

    for (int i = 0; i < num_cores; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            mine_primes(i + 2, num_cores);
            exit(0);
        } else if (pid > 0) {
            pids[i] = pid;
        } else {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 0; i < num_cores; i++) {
        waitpid(pids[i], NULL, 0);
    }

    printf("\nPrime mining stopped.\n");
    return 0;
}

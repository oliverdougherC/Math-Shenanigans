#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

void count_to_max(int start, long long *shared_max) {
    long long number = start;
    while (keep_running) {
        if (number % 1000000 == 0) {
            printf("\rCurrent number: %lld", number);
            fflush(stdout);
        }
        number++;
    }
    if (number > *shared_max) {
        *shared_max = number;
    }
}

int main() {
    printf("Press Ctrl+C to stop.\n");
    sleep(1);

    signal(SIGINT, handle_sigint);

    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    pid_t pids[num_cores];

    int shmid = shmget(IPC_PRIVATE, sizeof(long long), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    long long *shared_max = (long long *)shmat(shmid, NULL, 0);
    *shared_max = 0;

    for (int i = 0; i < num_cores; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            count_to_max(i, shared_max);
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

    printf("\nHighest number reached: %lld\n", *shared_max);

    shmdt(shared_max);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

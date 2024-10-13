#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

void count_up(int start, int step) {
    int number = start;
    while (keep_running) {
        printf("Process %d reached: %d\n", getpid(), number);
        number += step;
        usleep(100000);  // Sleep for 100ms to slow down output
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
            count_up(i + 1, num_cores);
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

    printf("\nCounting stopped.\n");
    return 0;
}

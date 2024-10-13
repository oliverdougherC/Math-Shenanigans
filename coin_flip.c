#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

int flip_coin() {
    return rand() % 2;  // 0 for tails, 1 for heads
}

int main() {
    srand(time(NULL));
    long long total_flips = 0;
    long long heads_count = 0;

    printf("Press Ctrl+C to stop the coin flipping.\n");
    sleep(1);

    signal(SIGINT, handle_sigint);

    while (keep_running) {
        if (flip_coin()) {
            heads_count++;
        }
        total_flips++;

        double heads_percentage = (double)heads_count / total_flips * 100;
        printf("\rTotal flips: %lld, Heads percentage: %.2f%%", total_flips, heads_percentage);
        fflush(stdout);

        usleep(1000);  // Sleep for 1ms to prevent excessive CPU usage
    }

    printf("\nCoin flipping stopped.\n");
    printf("Final results: Total flips: %lld, Heads percentage: %.10f%%\n", 
           total_flips, (double)heads_count / total_flips * 100);

    return 0;
}

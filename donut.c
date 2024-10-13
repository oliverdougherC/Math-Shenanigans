#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

void render_frame(float A, float B, int screen_width, int screen_height, char* output) {
    float size_factor = 1.5;
    int x_offset = screen_width / 2;
    int y_offset = screen_height / 2;
    int x_scale = (int)(fmin(screen_width, screen_height) * size_factor / 2);
    int y_scale = (int)(x_scale * 0.7);

    memset(output, ' ', screen_width * screen_height);
    float z[screen_width * screen_height];
    memset(z, 0, sizeof(float) * screen_width * screen_height);

    for (int j = 0; j < 628; j += 7) {
        for (int i = 0; i < 628; i += 2) {
            float c = sin(i * 0.01);
            float d = cos(j * 0.01);
            float e = sin(A);
            float f = sin(j * 0.01);
            float g = cos(A);
            float h = d + 2;
            float D = 1 / (c * h * e + f * g + 5);
            float l = cos(i * 0.01);
            float m = cos(B);
            float n = sin(B);
            float t = c * h * g - f * e;
            int x = (int)(x_offset + x_scale * D * (l * h * m - t * n));
            int y = (int)(y_offset + y_scale * D * (l * h * n + t * m));
            int o = x + screen_width * y;
            int N = (int)(8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n));
            if (y < screen_height && y > 0 && x > 0 && x < screen_width && D > z[o]) {
                z[o] = D;
                output[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
            }
        }
    }
}

int main() {
    struct winsize w;
    float A = 0, B = 0;

    signal(SIGINT, handle_sigint);

    while (keep_running) {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int width = w.ws_col;
        int height = w.ws_row - 1;

        char *frame = malloc(width * height);
        render_frame(A, B, width, height, frame);

        printf("\033[2J\033[H");  // Clear screen and move cursor to top-left
        for (int i = 0; i < height; i++) {
            fwrite(frame + i * width, 1, width, stdout);
            fputc('\n', stdout);
        }
        fflush(stdout);

        free(frame);

        A += 0.04;
        B += 0.02;
        usleep(30000);  // Sleep for 30ms
    }

    printf("\nDonut animation stopped.\n");
    return 0;
}

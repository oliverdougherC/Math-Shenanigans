import math
import time
import os
import shutil

def render_frame(A, B, screen_width, screen_height):
    b = [' '] * (screen_width * screen_height)
    z = [0] * (screen_width * screen_height)

    size_factor = 1.5 

    x_offset = screen_width // 2
    y_offset = screen_height // 2
    x_scale = int(min(screen_width, screen_height) * size_factor / 2)
    y_scale = int(x_scale * 0.7) 

    for j in range(0, 628, 7):
        for i in range(0, 628, 2):
            c = math.sin(i)
            d = math.cos(j)
            e = math.sin(A)
            f = math.sin(j)
            g = math.cos(A)
            h = d + 2
            D = 1 / (c * h * e + f * g + 5)
            l = math.cos(i)
            m = math.cos(B)
            n = math.sin(B)
            t = c * h * g - f * e
            x = int(x_offset + x_scale * D * (l * h * m - t * n))
            y = int(y_offset + y_scale * D * (l * h * n + t * m))
            o = int(x + screen_width * y)
            N = int(8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n))
            if y < screen_height and y > 0 and x > 0 and x < screen_width and D > z[o]:
                z[o] = D
                b[o] = ".,-~:;=!*#$@"[N if N > 0 else 0]

    return ''.join(b)

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

def get_terminal_size():
    return shutil.get_terminal_size()

def main():
    A = 0
    B = 0
    try:
        while True:
            clear_screen()
            width, height = get_terminal_size()
            frame = render_frame(A, B, width, height - 1) 
            print('\n'.join(frame[i:i+width] for i in range(0, len(frame), width)))
            A += 0.04
            B += 0.02
            time.sleep(0.03)
    except KeyboardInterrupt:
        print("\nDonut animation stopped.")

if __name__ == "__main__":
    main()

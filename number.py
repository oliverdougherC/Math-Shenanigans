import multiprocessing
import time

def count_up(start, step):
    number = start
    try:
        while True:
            print(f"Process {multiprocessing.current_process().name} reached: {number}")
            number += step
    except KeyboardInterrupt:
        pass

def main():
    print("Press Ctrl+C to stop.")
    time.sleep(1)
    num_cores = multiprocessing.cpu_count()
    processes = []

    try:
        for i in range(num_cores):
            p = multiprocessing.Process(target=count_up, args=(i + 1, num_cores))
            processes.append(p)
            p.start()

        for p in processes:
            p.join()
    except KeyboardInterrupt:
        print("\nCounting stopped.")
        for p in processes:
            p.terminate()

if __name__ == "__main__":
    main()

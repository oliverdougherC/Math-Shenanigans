import multiprocessing
import time
import sys

def count_to_max(start):
    number = start
    try:
        while True:
            if number % 1000000 == 0:
                print(f"\rCurrent number: {number}", end="", flush=True)
            number += 1
    except KeyboardInterrupt:
        return number - 1

def main():
    print("Press Ctrl+C to stop.")
    time.sleep(1)
    num_cores = multiprocessing.cpu_count()
    pool = multiprocessing.Pool(processes=num_cores)

    try:
        results = pool.map_async(count_to_max, range(num_cores))
        pool.close()
        pool.join()
    except KeyboardInterrupt:
        print("\nCounting stopped.")
        pool.terminate()
    finally:
        pool.join()
        max_number = max(results.get())
        print(f"\nHighest number reached: {max_number}")

if __name__ == "__main__":
    main()

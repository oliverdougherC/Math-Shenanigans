import multiprocessing
import time

def is_prime(n):
    if n < 2:
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    return True

def mine_primes(start, step):
    number = start
    try:
        while True:
            if is_prime(number):
                print(f"{number}\r")
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
            p = multiprocessing.Process(target=mine_primes, args=(i + 2, num_cores))
            processes.append(p)
            p.start()

        for p in processes:
            p.join()
    except KeyboardInterrupt:
        print("\nPrime mining stopped.")
        for p in processes:
            p.terminate()

if __name__ == "__main__":
    main()

import random
import time

def flip_coin():
    return random.choice(['heads', 'tails'])

def main():
    total_flips = 0
    heads_count = 0

    print("Press Ctrl+C to stop the coin flipping.")
    time.sleep(1)

    try:
        while True:
            if flip_coin() == 'heads':
                heads_count += 1
            total_flips += 1
            
            heads_percentage = (heads_count / total_flips) * 100
            print(f"\rTotal flips: {total_flips}, Heads percentage: {heads_percentage:.2f}%", end="", flush=True)
            
    except KeyboardInterrupt:
        print("\nCoin flipping stopped.")
        print(f"Final results: Total flips: {total_flips}, Heads percentage: {heads_percentage:.10f}%")

if __name__ == "__main__":
    main()

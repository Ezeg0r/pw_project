import subprocess
import time
import csv
from pathlib import Path
from statistics import mean

threads = [4, 8, 16, 32, 64, 128]
blocks = [8, 32]

tests = sorted(Path("tests").glob("input_*.txt"),
               key=lambda p: int(p.stem.split("_")[1]))

def benchmark(cmd, data):
    times = []
    answers = []

    for i in range(5):
        start = time.perf_counter()
        p = subprocess.run(cmd, input=data, capture_output=True, check=True)
        times.append(time.perf_counter() - start)
        answers.append(p.stdout.strip())

    assert len(set(answers)) == 1
    return mean(times), answers[0]

with open("results.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["test", "n", "threads", "block", "time", "speedup"])

    for test in tests:
        data = test.read_bytes()
        n = int(data.split()[0])

        slow_time, answer = benchmark(["./slow"], data)
        writer.writerow([test.name, n, 0, 0, slow_time, 1])

        print(test.name, "slow:", round(slow_time, 4))

        for t in threads:
            for block in blocks:
                avg, result = benchmark(["./fast", str(t), str(block)], data)

                assert result == answer

                speedup = slow_time / avg
                writer.writerow([test.name, n, t, block, avg, speedup])

                print(t, block, round(avg, 4), round(speedup, 2))

        f.flush()
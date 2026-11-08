from pathlib import Path
from subprocess import check_output
from sys import argv
from time import perf_counter

modes = [("seq", 1), ("equal", 16), ("balanced", 8), ("dynamic", 8)]
tests = argv[1:] or sorted(
    Path("tests").glob("test-*.txt"),
    key=lambda path: int(path.stem.split("-")[1]),
)

for test in tests:
    print(f"\n{Path(test).name}", flush=True)
    expected = None

    for mode, threads in modes:
        with open(test) as data:
            start = perf_counter()
            count = check_output(
                ["./triangles", mode, str(threads)], stdin=data, text=True
            ).strip()
            elapsed = perf_counter() - start

        print(f"  {mode:8} {threads:2} threads  {elapsed:7.3f} s  {count}", flush=True)

        if expected is None:
            expected = count
        elif count != expected:
            raise SystemExit(f"{test}: {mode} returned {count}, expected {expected}")

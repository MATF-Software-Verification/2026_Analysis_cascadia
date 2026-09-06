# Unit tests and coverage

Tests use **Qt6 Test**, while coverage is collected with **GCC/gcov and LCOV**.

## Setup

```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev lcov
git submodule update --init --recursive
```

## Run tests

Without coverage:

```bash
bash unit_tests/run_tests.sh
```

With coverage:

```bash
bash unit_tests/coverage.sh
```

The coverage script:

1. Builds the tests with coverage instrumentation.
2. Resets previous counters with `lcov --zerocounters`.
3. Captures an initial zero-count baseline.
4. Runs the tests.
5. Collects line, function, and branch coverage.
6. Merges and filters the results to `tileData.cpp`.
7. Generates an HTML report.

Resetting counters ensures that results from previous runs are not accumulated. Creating and merging an initial baseline is a standard LCOV workflow for preserving information about instrumented code that was not executed.

## Output

Each run creates:

```text
unit_tests/reports/run_XXXXXXXX/
```

including the test log, LCOV `.info` files, and:

```text
html/index.html
```

The current test suite covers `TileData` construction, getters/setters, serialization, and serialization-deserialization round trips.

Coverage is reported only for `tileData.cpp`, not for the entire application.

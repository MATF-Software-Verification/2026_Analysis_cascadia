# Unit tests and coverage

## Installation and execution

Dependencies on Ubuntu: `build-essential cmake qt6-base-dev lcov`.
Run these commands from the analysis repository root:

```bash
git submodule update --init --recursive
bash unit_tests/run_tests.sh
bash unit_tests/coverage.sh
```

## Implemented cases

| Suite | Cases |
|---|---|
| TileData | Four existing cases; independent explicit-map deserialization; list round trips for empty, single and multiple entries; replacing and clearing old lists; mandatory serialized keys |
| PlayerData | Constructor fields and tile order; round trips for 0, 1 and 3 tiles, including a Unicode name; independent explicit-map deserialization; replacing and clearing old tiles |
| Turn | Constructor must preserve the supplied placedToken |

There are 3 CTest suites and 17 Qt Test executions excluding lifecycle callbacks:
10 TileData, 6 PlayerData, 1 Turn. Data-provider methods are not additional cases.

Qt macros used: QCOMPARE, QVERIFY, QVERIFY2, QFETCH and QTest data rows.

## Expected constructor finding

Analyzed upstream revision: `c32895403185b6ae4a968d4517d8a634adfe80ee`.

`Turn::Turn(...)` receives a placedToken argument but does not assign it to its
QString member. The test passes "bear" and compares the getter with "bear".
Static inspection predicts a failure: actual empty QString, expected "bear".

## Coverage

The coverage script:

1. Builds the tests with coverage instrumentation.
2. Resets previous counters with `lcov --zerocounters`.
3. Captures an initial zero-count baseline.
4. Runs the tests.
5. Collects line, function, and branch coverage.
6. Generates an HTML report.
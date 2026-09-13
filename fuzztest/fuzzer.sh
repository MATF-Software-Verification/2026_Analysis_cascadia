#!/usr/bin/env bash
set -euo pipefail

FUZZ_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$FUZZ_DIR/.." && pwd)"
CFG_FILE="$FUZZ_DIR/fuzztest.cfg"
BUILD_DIR="$PROJECT_ROOT/build_fuzz"
REPORT_DIR="$FUZZ_DIR/reports/run_$(date +%d%m%Y_%H%M%S)"

[[ -f "$CFG_FILE" ]] || { echo "Copy fuzztest.cfg.example to fuzztest.cfg first."; exit 1; }
source "$CFG_FILE"
CC="${CC:-clang}"
CXX="${CXX:-clang++}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
JOBS="${JOBS:-2}"
FUZZ_DURATION="${FUZZ_DURATION:-60}"
FUZZ_TARGETS="${FUZZ_TARGETS:-}"

for cmd in cmake git "$CC" "$CXX"; do
    command -v "$cmd" >/dev/null || { echo "$cmd not found"; exit 1; }
done
[[ "$FUZZ_DURATION" =~ ^[1-9][0-9]*$ ]] || { echo "FUZZ_DURATION must be a positive number of seconds."; exit 1; }
[[ -f "$PROJECT_ROOT/cascadia++/cascadia/game/entities/tileData.cpp" ]] || {
    echo "Game sources not found. Run: git submodule update --init --recursive"
    exit 1
}

mkdir -p "$FUZZ_DIR/reports"
mkdir "$REPORT_DIR"
cp "$CFG_FILE" "$REPORT_DIR/config.cfg"

# Qt graphics objects need an application, even when no window is shown.
export QT_QPA_PLATFORM=offscreen
export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1}"

# Compile both the fuzz tests and the selected original game sources.
cmake -S "$FUZZ_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DFUZZTEST_FUZZING_MODE=ON 2>&1 | tee "$REPORT_DIR/build.log"
cmake --build "$BUILD_DIR" --target fuzz_cascadia --parallel "$JOBS" \
    2>&1 | tee -a "$REPORT_DIR/build.log"

# Discover registered tests from the executable, or use the configured list.
"$BUILD_DIR/fuzz_cascadia" --list_fuzz_tests > "$REPORT_DIR/targets.txt"
if [[ -n "$FUZZ_TARGETS" ]]; then
    read -ra TARGETS <<< "$FUZZ_TARGETS"
else
    mapfile -t TARGETS < <(sed -n 's/^\[\*\] Fuzz test: //p' "$REPORT_DIR/targets.txt")
fi
(( ${#TARGETS[@]} > 0 )) || { echo "No fuzz tests found."; exit 1; }

printf 'target\texit_code\n' > "$REPORT_DIR/summary.tsv"
FAILED=0
for target in "${TARGETS[@]}"; do
    TEST_DIR="$REPORT_DIR/$target"
    mkdir -p "$TEST_DIR/corpus" "$TEST_DIR/crashes"
    echo "Fuzzing $target for up to ${FUZZ_DURATION}s. Log: $TEST_DIR/output.log"

    # A failed property stops this process; the next target still runs.
    if FUZZTEST_TESTSUITE_OUT_DIR="$TEST_DIR/corpus" \
       FUZZTEST_REPRODUCERS_OUT_DIR="$TEST_DIR/crashes" \
       "$BUILD_DIR/fuzz_cascadia" --fuzz="$target" \
           --fuzz_for="${FUZZ_DURATION}s" --time_limit_per_input=10s \
           > "$TEST_DIR/output.log" 2>&1; then
        STATUS=0
    else
        STATUS=$?
        FAILED=1
    fi
    printf '%s\t%s\n' "$target" "$STATUS" >> "$REPORT_DIR/summary.tsv"
    echo "$target: exit code $STATUS"
    # Preserve an explicit interruption instead of starting another test.
    if [[ "$STATUS" == 130 || "$STATUS" == 143 ]]; then exit "$STATUS"; fi
done

echo "Results: $REPORT_DIR"
exit "$FAILED"

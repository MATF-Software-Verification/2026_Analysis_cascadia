#!/usr/bin/env bash

set -euo pipefail

FUZZ_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$FUZZ_DIR/.." && pwd)"
CFG_FILE="$FUZZ_DIR/fuzztest.cfg"
BUILD_DIR="$PROJECT_ROOT/build_fuzz"
REPORT_DIR="$FUZZ_DIR/reports/run_$(date +%d%m%Y_%H%M%S)"

if [[ ! -f "$CFG_FILE" ]]; then
    echo "Copy fuzztest.cfg.example to fuzztest.cfg first."
    exit 1
fi
source "$CFG_FILE"

BUILD_TYPE="${BUILD_TYPE:-Debug}"
JOBS="${JOBS:-2}"
FUZZ_DURATION="${FUZZ_DURATION:-60}"
[[ "$FUZZ_DURATION" =~ ^[1-9][0-9]*$ ]] || { echo "FUZZ_DURATION must be positive seconds."; exit 1; }

mkdir -p "$REPORT_DIR"
cp "$CFG_FILE" "$REPORT_DIR/config.cfg"
exec > >(tee -a "$REPORT_DIR/output.log") 2>&1

# Read FUZZ_TEST registrations from one source file
if [[ -n "${FUZZ_TARGETS:-}" ]]; then
    read -ra TARGETS <<< "$FUZZ_TARGETS"
else
    mapfile -t TARGETS < <(
        sed -nE 's/^FUZZ_TEST\(([[:alnum:]_]+),[[:space:]]*([[:alnum:]_]+)\).*/\1.\2/p' \
            "$FUZZ_DIR/fuzz_cascadia.cc"
    )
fi
(( ${#TARGETS[@]} > 0 )) || { echo "No fuzz tests found."; exit 1; }
printf '%s\n' "${TARGETS[@]}" > "$REPORT_DIR/targets.txt"

# Qt graphics objects need an application even without a visible window
export QT_QPA_PLATFORM=offscreen
export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1}"

cmake -S "$FUZZ_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_CXX_COMPILER="${CXX:-clang++}" \
    -DCMAKE_C_COMPILER="${CC:-clang}" \
    -DFUZZTEST_FUZZING_MODE=ON
cmake --build "$BUILD_DIR" --target fuzz_cascadia --parallel "$JOBS"

printf 'target\texit_code\n' > "$REPORT_DIR/summary.tsv"
FAILED=0
for target in "${TARGETS[@]}"; do
    TEST_DIR="$REPORT_DIR/$target"
    mkdir -p "$TEST_DIR/corpus" "$TEST_DIR/crashes"
    echo "=== Fuzzing $target for up to ${FUZZ_DURATION}s"

    # A finding stops one fuzz target; continue with the remaining targets
    if FUZZTEST_TESTSUITE_OUT_DIR="$TEST_DIR/corpus" \
       FUZZTEST_REPRODUCERS_OUT_DIR="$TEST_DIR/crashes" \
       "$BUILD_DIR/fuzz_cascadia" --fuzz="$target" \
           --fuzz_for="${FUZZ_DURATION}s" --time_limit_per_input=10s \
           2>&1 | tee "$TEST_DIR/output.log"; then
        STATUS=0
    else
        STATUS=${PIPESTATUS[0]}
        FAILED=1
    fi
    printf '%s\t%s\n' "$target" "$STATUS" >> "$REPORT_DIR/summary.tsv"
    echo "$target: exit code $STATUS"
    if [[ "$STATUS" == 130 || "$STATUS" == 143 ]]; then exit "$STATUS"; fi
done

echo "Results: $REPORT_DIR"
exit "$FAILED"
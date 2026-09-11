#!/usr/bin/env bash
set -euo pipefail

TOOL_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$TOOL_DIR/../.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build_memcheck"
CFG_FILE="$TOOL_DIR/memcheck.cfg"
REPORT_DIR="$TOOL_DIR/reports/run_$(date +%d%m%Y_%H%M%S)"

# Load the existing configuration.
[[ -f "$CFG_FILE" ]] || { echo "Copy memcheck.cfg.example to memcheck.cfg first."; exit 1; }
TESTS=()
source "$CFG_FILE"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
CXX="${CXX:-g++}"
JOBS="${JOBS:-$(nproc)}"
export QT_QPA_PLATFORM="${QT_QPA_PLATFORM:-offscreen}"

for cmd in cmake valgrind "$CXX"; do
    command -v "$cmd" >/dev/null || { echo "$cmd not found"; exit 1; }
done
[[ -n "${TESTS[*]:-}" ]] || { echo "Select at least one executable in TESTS."; exit 1; }

# Keep the build separate from coverage and sanitizer builds.
cmake -S "$PROJECT_ROOT/unit_tests" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_CXX_FLAGS_DEBUG="-O0 -g" \
    -DENABLE_SANITIZERS=OFF \
    -DENABLE_COVERAGE=OFF
cmake --build "$BUILD_DIR" --parallel "$JOBS"

# Refuse to overwrite a report directory from a previous run.
mkdir -p "$TOOL_DIR/reports"
mkdir "$REPORT_DIR"
cp "$CFG_FILE" "$REPORT_DIR/config_used.txt"
printf 'executable\texit_code\n' > "$REPORT_DIR/summary.tsv"
overall_status=0
cd "$BUILD_DIR"

for test_name in "${TESTS[@]}"; do
    echo "Running Memcheck: $test_name"

    # An if statement captures failures without stopping the remaining suites.
    if [[ ! -x "$BUILD_DIR/$test_name" ]]; then
        echo "Missing executable: $test_name" > "$REPORT_DIR/${test_name}_output.log"
        status=127
    elif valgrind --tool=memcheck \
        --leak-check=full \
        --show-leak-kinds=all \
        --errors-for-leak-kinds=definite,possible \
        --track-origins=yes \
        --num-callers=30 \
        --error-exitcode=99 \
        --log-file="$REPORT_DIR/memcheck_${test_name}.log" \
        "$BUILD_DIR/$test_name" > "$REPORT_DIR/${test_name}_output.log" 2>&1
    then
        status=0
    else
        status=$?
    fi

    printf '%s\t%s\n' "$test_name" "$status" >> "$REPORT_DIR/summary.tsv"
    echo "$test_name: exit code $status"
    [[ "$status" -eq 0 ]] || overall_status=1

    # Respect interruption instead of starting another suite.
    if [[ "$status" -eq 130 || "$status" -eq 143 ]]; then
        echo "Interrupted. Partial reports: $REPORT_DIR"
        exit "$status"
    fi
done

echo "Valgrind Memcheck reports: $REPORT_DIR"
exit "$overall_status"
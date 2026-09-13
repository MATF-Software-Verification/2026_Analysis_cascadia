#!/usr/bin/env bash
set -euo pipefail

TOOL_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$TOOL_DIR/.." && pwd)"
LLVM_VERSION="${LLVM_VERSION:-18}"
BUILD_DIR="$PROJECT_ROOT/build_mull_$LLVM_VERSION"
MULL_RUNNER="mull-runner-$LLVM_VERSION"
MULL_PLUGIN="${MULL_PLUGIN:-/usr/lib/mull-ir-frontend-$LLVM_VERSION}"

# Use a small set of complete, previously passing Qt Test suites by default.
TARGETS=("$@")
if (( ${#TARGETS[@]} == 0 )); then
    TARGETS=(cascadia_unit_tests player_data_tests tile_storage_tests hexagon_grid_tests)
fi

# The compiler, plugin and runner must use the same LLVM major version.
for TOOL in cmake "clang++-$LLVM_VERSION" "$MULL_RUNNER" "llvm-profdata-$LLVM_VERSION" "llvm-cov-$LLVM_VERSION"; do
    command -v "$TOOL" >/dev/null || { echo "Nedostaje $TOOL. Pogledaj README.md."; exit 1; }
done
[[ -f "$MULL_PLUGIN" ]] || { echo "Nedostaje plugin: $MULL_PLUGIN"; exit 1; }

REPORT_DIR="$TOOL_DIR/reports/run_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$TOOL_DIR/reports"
mkdir "$REPORT_DIR"
echo "Izvestaji: $REPORT_DIR"

export MULL_CONFIG="$TOOL_DIR/mull.yml"
export QT_QPA_PLATFORM=offscreen
export LLVM_PROFILE_FILE="$REPORT_DIR/coverage-%p.profraw"
ulimit -c 0

# Record the exact tool versions and requested test suites.
{
    "clang++-$LLVM_VERSION" --version
    "$MULL_RUNNER" --version
    printf 'Targets: %s\n' "${TARGETS[*]}"
    git -C "$PROJECT_ROOT/cascadia++" rev-parse HEAD 2>/dev/null || true
} > "$REPORT_DIR/environment.log" 2>&1
cp "$MULL_CONFIG" "$REPORT_DIR/mull.yml"

# Reuse the existing unit-test CMake project; no source patches are needed.
FLAGS="-O0 -g -grecord-command-line -fpass-plugin=$MULL_PLUGIN"
FLAGS+=" -fprofile-instr-generate -fcoverage-mapping"
cmake -S "$PROJECT_ROOT/unit_tests" -B "$BUILD_DIR" \
    -DCMAKE_CXX_COMPILER="clang++-$LLVM_VERSION" \
    -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="$FLAGS" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DENABLE_COVERAGE=OFF -DENABLE_SANITIZERS=OFF \
    2>&1 | tee "$REPORT_DIR/configure.log"
cmake --build "$BUILD_DIR" --clean-first --target "${TARGETS[@]}" --parallel "${JOBS:-2}" \
    2>&1 | tee "$REPORT_DIR/build.log"

# Stop if any selected suite already fails without an activated mutant.
for TARGET in "${TARGETS[@]}"; do
    echo "Provera originala: $TARGET"
    if ! timeout 30 "$BUILD_DIR/$TARGET" > "$REPORT_DIR/${TARGET}_baseline.log" 2>&1; then
        cat "$REPORT_DIR/${TARGET}_baseline.log"
        echo "Originalni testovi nisu prosli. Mull nije pokrenut."
        exit 1
    fi
done

# Keep independent reports for each test executable, as in the regex example.
for TARGET in "${TARGETS[@]}"; do
    echo "Mutaciono testiranje: $TARGET"
    "$MULL_RUNNER" --workers 2 --timeout "${MULL_TIMEOUT_MS:-3000}" \
        --allow-surviving --reporters IDE --reporters Elements \
        --report-dir "$REPORT_DIR" --report-name "$TARGET" \
        "$BUILD_DIR/$TARGET" 2>&1 | tee "$REPORT_DIR/${TARGET}_mull.log"
done

echo "Zavrseno. Rezultati: $REPORT_DIR"

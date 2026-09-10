#!/usr/bin/env bash
# Stop on errors, unset variables, and failed pipeline commands
set -euo pipefail

# Resolve this script's directory regardless of where it is launched
TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$TEST_DIR/.." && pwd)"
CFG_FILE="$TEST_DIR/coverage.cfg"

# Require a local configuration file
if [[ ! -f "$CFG_FILE" ]]; then
    echo "Copy unit_tests/coverage.cfg.example to unit_tests/coverage.cfg first." >&2
    exit 1
fi

# Load trusted local configuration as Bash assignments.
source "$CFG_FILE"

# Apply defaults for settings omitted from the configuration.
BUILD_TYPE="${BUILD_TYPE:-Debug}"
CXX="${CXX:-g++}"
GCOV_TOOL="${GCOV_TOOL:-gcov}"
JOBS="${JOBS:-$(nproc)}"
ENABLE_SANITIZERS="${ENABLE_SANITIZERS:-ON}"
OPEN_REPORT="${OPEN_REPORT:-1}"
EARLY_FILTER="${EARLY_FILTER:-1}"

# Keep instrumented outputs separate from ordinary builds.
BUILD_DIR="$PROJECT_ROOT/build_coverage"
REPORT_ROOT="$TEST_DIR/reports"

# Define the production files included in the final report.
SOURCE_FILES=(
    "$PROJECT_ROOT/cascadia++/cascadia/game/entities/tileData.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/game/entities/tile.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/game/entities/tileStorage.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/game/entities/player.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/game/entities/token.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/game/engine/scoring.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/game/board/hexagon.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/game/board/hexagonGrid.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/common/playerData.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/common/turn.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/controller/setupGame.cpp"
    "$PROJECT_ROOT/cascadia++/cascadia/controller/setupPlayers.cpp"
)

# Verify required tools before configuring the project
for tool in cmake ctest "$CXX" "$GCOV_TOOL" lcov genhtml; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "Required tool not found: $tool" >&2
        exit 1
    fi
done

# Verify source availability and optionally exclude dependencies during capture
CAPTURE_FILTERS=()
for source_file in "${SOURCE_FILES[@]}"; do
    if [[ ! -f "$source_file" ]]; then
        echo "Missing source: $source_file; initialize the submodule." >&2
        exit 1
    fi

    if [[ "$EARLY_FILTER" == "1" ]]; then
        CAPTURE_FILTERS+=(--include "$source_file")
    fi
done

# Configure coverage and the optional sanitizer instrumentation
cmake -S "$TEST_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DENABLE_COVERAGE=ON \
    -DENABLE_SANITIZERS="$ENABLE_SANITIZERS"

# Build all registered test executables
cmake --build "$BUILD_DIR" --parallel "$JOBS"

# Reset previous execution counters without deleting the build
lcov --zerocounters --directory "$BUILD_DIR"

# Preserve each run in its own local directory
mkdir -p "$REPORT_ROOT"
REPORT_DIR="$(mktemp -d "$REPORT_ROOT/run_XXXXXXXX")"
echo "Coverage output: $REPORT_DIR"

# Allow failing tests so their coverage can still be collected
set +e

# Save complete test output while displaying it in the terminal
ctest --test-dir "$BUILD_DIR" --verbose \
    2>&1 | tee "$REPORT_DIR/tests.log"

# Preserve both pipeline statuses immediately
RUN_STATUS=("${PIPESTATUS[@]}")

# Restore automatic error handling
set -e

# Stop if saving the log failed
if (( RUN_STATUS[1] != 0 )); then
    echo "Failed to save the test log." >&2
    exit "${RUN_STATUS[1]}"
fi

# Record the test outcome before running coverage tools
printf '%s\n' "${RUN_STATUS[0]}" > "$REPORT_DIR/test_exit_code.txt"

# Collect execution data only; no initial zero-count baseline is generated
lcov --branch-coverage \
    --gcov-tool "$GCOV_TOOL" \
    --capture \
    "${CAPTURE_FILTERS[@]}" \
    --directory "$BUILD_DIR" \
    --output-file "$REPORT_DIR/executed.info"

# Restrict the final report to the three analyzed implementations
lcov --branch-coverage \
    --extract "$REPORT_DIR/executed.info" \
    "${SOURCE_FILES[@]}" \
    --output-file "$REPORT_DIR/coverage.info"

# Generate HTML with line, function, and branch coverage
genhtml --branch-coverage \
    --flat \
    --legend \
    --title "Cascadia++ - Unit test coverage" \
    --output-directory "$REPORT_DIR/html" \
    "$REPORT_DIR/coverage.info"

# Preserve the previous published HTML before replacing it
if [[ -d "$REPORT_ROOT/html" ]]; then
    mv "$REPORT_ROOT/html" "$REPORT_DIR/previous_published_html"
fi

# Publish only the latest HTML and test log for Git
cp -a "$REPORT_DIR/html" "$REPORT_ROOT/html"
cp "$REPORT_DIR/tests.log" "$REPORT_ROOT/tests.log"

# Print both published and archived locations
echo "Latest HTML report: $REPORT_ROOT/html/index.html"
echo "Archived run: $REPORT_DIR"

# Optionally open the report without changing the test exit status
if [[ "$OPEN_REPORT" == "1" ]]; then
    if command -v xdg-open >/dev/null 2>&1; then
        xdg-open "$REPORT_ROOT/html/index.html" \
            >"$REPORT_DIR/browser.log" 2>&1 &
    else
        echo "xdg-open not found; open the HTML report manually."
    fi
fi

# Explain a failing test outcome even when coverage generation succeeded
if (( RUN_STATUS[0] != 0 )); then
    echo "Tests failed; see tests.log. Coverage may be incomplete after a crash." >&2
fi

# Return the original CTest status
exit "${RUN_STATUS[0]}"
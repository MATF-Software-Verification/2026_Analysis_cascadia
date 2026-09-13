#!/usr/bin/env bash
set -euo pipefail

TOOL_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$TOOL_DIR/build"
MODE="${1:-scenario}"

case "$MODE" in
    scenario) TARGET="cascadia_profile" ;;
    game) TARGET="cascadia" ;;
    *) echo "Upotreba: bash tracy/tracy.sh [scenario|game]"; exit 1 ;;
esac

# Build the instrumented program and the matching Tracy capture tool.
cmake -S "$TOOL_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build "$BUILD_DIR" --target "$TARGET" --parallel 2
cmake -S "$BUILD_DIR/_deps/tracy-src/capture" -B "$BUILD_DIR/capture" \
    -DCMAKE_BUILD_TYPE=Release -DLEGACY=ON -DNO_PARALLEL_STL=ON
cmake --build "$BUILD_DIR/capture" --target tracy-capture --parallel 2

# Each run has its own directory, so earlier recordings are preserved.
REPORT_DIR="$TOOL_DIR/reports/${MODE}_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$TOOL_DIR/reports"
mkdir "$REPORT_DIR"
echo "Snimanje: $REPORT_DIR"

# Start the recorder before the program and stop it if the script fails.
"$BUILD_DIR/capture/tracy-capture" -a 127.0.0.1 \
    -o "$REPORT_DIR/capture.tracy" > "$REPORT_DIR/capture.log" 2>&1 &
CAPTURE_PID=$!

# Keep short programs alive until Tracy has transferred the recorded events.
TRACY_NO_EXIT=1 "$BUILD_DIR/$TARGET" > "$REPORT_DIR/program.log" 2>&1 &
PROGRAM_PID=$!
trap 'kill "$CAPTURE_PID" "$PROGRAM_PID" 2>/dev/null || true' EXIT

# Stop on failure of either process; otherwise wait for both to finish.
wait -n "$CAPTURE_PID" "$PROGRAM_PID"
wait "$PROGRAM_PID"
wait "$CAPTURE_PID"
trap - EXIT

echo "Snimak je sacuvan: $REPORT_DIR/capture.tracy"

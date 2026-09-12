#!/usr/bin/env bash
set -euo pipefail

TOOL_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$TOOL_DIR/../.." && pwd)"
SOURCE_DIR="$PROJECT_ROOT/cascadia++/cascadia"
BUILD_DIR="$PROJECT_ROOT/build_memcheck_game"
SCENARIO="${1:-interactive}"
REPORT_DIR="$TOOL_DIR/reports/$SCENARIO"
CXX="${CXX:-g++}"
JOBS="${JOBS:-4}"

[[ "$SCENARIO" =~ ^[a-zA-Z0-9_-]+$ ]] || { echo "Use letters, digits, underscores or hyphens for the session name."; exit 1; }
[[ ! -e "$REPORT_DIR" && ! -L "$REPORT_DIR" ]] || { echo "Session already exists: $REPORT_DIR. Choose another name."; exit 1; }

for cmd in cmake "$CXX" valgrind; do
    command -v "$cmd" >/dev/null || { echo "$cmd not found"; exit 1; }
done

# Use the desktop so the game window is visible.
if [[ "${QT_QPA_PLATFORM:-}" == offscreen || "${QT_QPA_PLATFORM:-}" == minimal ]]; then
    unset QT_QPA_PLATFORM
fi
[[ -n "${DISPLAY:-}" || -n "${WAYLAND_DISPLAY:-}" ]] || { echo "Run in a graphical desktop session."; exit 1; }

# Build separately, with debug symbols and no added instrumentation.
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_CXX_STANDARD=20 \
    -DCMAKE_CXX_STANDARD_REQUIRED=ON \
    -DCMAKE_CXX_FLAGS="" \
    -DCMAKE_CXX_FLAGS_DEBUG="-O0 -g" \
    -DCMAKE_EXE_LINKER_FLAGS="" \
    -DCMAKE_EXE_LINKER_FLAGS_DEBUG=""
cmake --build "$BUILD_DIR" --target cascadia --parallel "$JOBS"

# Create a report folder only after a successful build.
mkdir -p "$TOOL_DIR/reports"
mkdir "$REPORT_DIR"
: > "$REPORT_DIR/memcheck.log"
cd "$SOURCE_DIR"

cat > "$REPORT_DIR/session.md" <<EOF
# Session: $SCENARIO

## Automatic metadata

- Started at: $(date -Iseconds)
- Valgrind: $(valgrind --version)
- Build: Debug, C++20, -O0 -g
- Working directory: $SOURCE_DIR
- Qt platform override: ${QT_QPA_PLATFORM:-automatic}
- QT_ENABLE_REGEXP_JIT: ${QT_ENABLE_REGEXP_JIT:-not set}
- Game commit: $(git -C "$PROJECT_ROOT/cascadia++" rev-parse HEAD 2>/dev/null || printf 'unavailable')
EOF

echo "Reports: $REPORT_DIR"
echo "Play the scenario, then close the game normally."

# Save output directly; capture nonzero exit codes without stopping early.
if valgrind --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=all \
    --errors-for-leak-kinds=definite,possible \
    --track-origins=yes \
    --num-callers=30 \
    --error-exitcode=99 \
    --log-file="$REPORT_DIR/memcheck.log" \
    "$BUILD_DIR/cascadia" > "$REPORT_DIR/game_output.log" 2>&1
then
    status=0
else
    status=$?
fi

cat >> "$REPORT_DIR/session.md" <<EOF
- Finished at: $(date -Iseconds)
- Valgrind/game exit code: $status

## Akcije

1. ...

## Zaključci

...

## Kako se sesija završila

Exit button / window close / crash / forced termination: ...

## Pronađeni problemi

...
EOF

echo "Session finished with exit code $status."
printf 'Document your actions: nano %q\n' "$REPORT_DIR/session.md"
exit "$status"

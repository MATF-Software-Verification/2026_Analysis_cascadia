#!/usr/bin/env bash
set -euo pipefail

TOOL_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$TOOL_DIR/.." && pwd)"
CFG_FILE="$TOOL_DIR/cppcheck.cfg"
SOURCE_DIR="$PROJECT_ROOT/cascadia++/cascadia"
BUILD_DIR="$PROJECT_ROOT/build_cppcheck"
REPORT_DIR="$TOOL_DIR/reports/run_$(date +%d%m%Y_%H%M%S)"
XML_FILE="$REPORT_DIR/cppcheck.xml"

[[ -f "$CFG_FILE" ]] || { echo "Copy cppcheck.cfg.example to cppcheck.cfg first."; exit 1; }
source "$CFG_FILE"
CXX="${CXX:-g++}"
JOBS="${JOBS:-4}"
OPEN_REPORT="${OPEN_REPORT:-1}"

for cmd in "$CXX" cmake cppcheck cppcheck-htmlreport; do
    command -v "$cmd" >/dev/null || { echo "$cmd not found"; exit 1; }
done
[[ -f "$SOURCE_DIR/CMakeLists.txt" ]] || { echo "Game CMakeLists.txt not found"; exit 1; }

mkdir -p "$TOOL_DIR/reports"
mkdir "$REPORT_DIR"

# Prepare the compilation database and generated Qt headers.
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_CXX_STANDARD=20 \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build "$BUILD_DIR" --target cascadia_autogen --parallel "$JOBS"

# The analysis cache supports checks across files during parallel execution.
mkdir -p "$BUILD_DIR/cppcheck_cache"

# Analyze the original game sources using the Qt library model.
cppcheck \
    -j "$JOBS" \
    --cppcheck-build-dir="$BUILD_DIR/cppcheck_cache" \
    --project="$BUILD_DIR/compile_commands.json" \
    --file-filter="$SOURCE_DIR/*" \
    --enable=all \
    --inconclusive \
    --library=qt \
    --suppress=missingIncludeSystem \
    --language=c++ \
    --std=c++20 \
    --platform=unix64 \
    --xml \
    --xml-version=2 \
    2>"$XML_FILE"

cppcheck-htmlreport \
    --file="$XML_FILE" \
    --report-dir="$REPORT_DIR/html" \
    --source-dir="$SOURCE_DIR"

echo "XML report: $XML_FILE"
echo "HTML report: $REPORT_DIR/html/index.html"

if [[ "$OPEN_REPORT" == 1 ]] && command -v xdg-open >/dev/null; then
    xdg-open "$REPORT_DIR/html/index.html" >/dev/null 2>&1 || true
fi

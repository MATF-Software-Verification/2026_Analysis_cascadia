#!/usr/bin/env bash

# Stop on errors, unset variables, and failed pipeline commands.
set -euo pipefail

# Absolute path to the directory containing this script.
TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Project root is one level above the unit_tests directory.
PROJECT_ROOT="$(cd "$TEST_DIR/.." && pwd)"

# Separate build directory used for unit tests.
BUILD_DIR="$PROJECT_ROOT/build_unit_tests"

# Build type can be overridden through the BUILD_TYPE environment variable.
# Debug is used by default.
BUILD_TYPE="${BUILD_TYPE:-Debug}"

# C++ compiler can be overridden through the CXX environment variable.
# g++ is used by default.
CXX="${CXX:-g++}"

# Number of parallel build jobs.
# By default, use the number of available CPU cores.
JOBS="${JOBS:-$(nproc)}"

# Verify that CMake is installed and available in PATH.
command -v cmake >/dev/null || {
    echo "cmake not found"
    exit 1
}

# Verify that the selected C++ compiler is available.
command -v "$CXX" >/dev/null || {
    echo "$CXX not found"
    exit 1
}

# Configure the CMake project for unit testing.
# Coverage instrumentation is explicitly disabled because this script
# is intended only for regular test execution.
cmake \
    -S "$TEST_DIR" \
    -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DENABLE_COVERAGE=OFF

# Build the unit tests using parallel compilation.
cmake --build "$BUILD_DIR" --parallel "$JOBS"

# Run all tests registered with CTest.
# Detailed output is displayed if any test fails.
ctest \
    --test-dir "$BUILD_DIR" \
    --output-on-failure
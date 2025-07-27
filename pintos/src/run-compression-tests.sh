#!/bin/bash

# Compression Test Runner Script
# Builds and runs the optimized compression test suite

set -e  # Exit on any error

echo "=== RLE Compression Test Suite ==="
echo "Building and running optimized compression tests..."
echo

# Configuration
CC="gcc"
CFLAGS="-O2 -Wall -Wextra -std=c99 -I."
TEST_SOURCE="tests/compression-test.c"
LIB_SOURCE="lib/compression.c"
TEST_BINARY="compression-test"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if source files exist
if [ ! -f "$TEST_SOURCE" ]; then
    print_error "Test source file not found: $TEST_SOURCE"
    exit 1
fi

if [ ! -f "$LIB_SOURCE" ]; then
    print_error "Library source file not found: $LIB_SOURCE"
    exit 1
fi

# Clean previous build
print_status "Cleaning previous build..."
rm -f "$TEST_BINARY"

# Build the test
print_status "Building compression test..."
$CC $CFLAGS -o "$TEST_BINARY" "$TEST_SOURCE" "$LIB_SOURCE" -lm

if [ $? -ne 0 ]; then
    print_error "Build failed!"
    exit 1
fi

print_status "Build successful!"

# Run the test
echo
print_status "Running compression tests..."
echo

# Run with timeout to prevent hanging
timeout 30s ./"$TEST_BINARY"
EXIT_CODE=$?

echo

if [ $EXIT_CODE -eq 0 ]; then
    print_status "All tests passed! ✅"
    echo
    print_status "Test summary:"
    echo "  - Compression algorithm: RLE (Run-Length Encoding)"
    echo "  - Block size: 512 bytes"
    echo "  - Performance: Optimized for speed and memory efficiency"
    echo "  - Coverage: Edge cases, performance, various data patterns"
else
    if [ $EXIT_CODE -eq 124 ]; then
        print_error "Tests timed out after 30 seconds!"
    else
        print_error "Some tests failed! ❌"
    fi
    exit $EXIT_CODE
fi

# Clean up
print_status "Cleaning up..."
rm -f "$TEST_BINARY"

print_status "Test suite completed successfully!"
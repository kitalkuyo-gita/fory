#!/bin/bash

# Script to run C++ cross-language tests
# This script builds and runs the C++ cross-language tests
# and provides instructions for running the corresponding Python tests

echo "Apache Fory™ C++ Cross-Language Test Runner"
echo "=========================================="

# Check if bazel is available
if ! command -v bazel &> /dev/null
then
    echo "Error: bazel is not installed or not in PATH"
    echo "Please install Bazel 6.3.2 and try again"
    exit 1
fi

echo "Building C++ cross-language tests..."
bazel build //cpp/fory:xlang_test

if [ $? -ne 0 ]; then
    echo "Error: Failed to build C++ cross-language tests"
    exit 1
fi

echo "Running C++ cross-language tests..."
bazel test //cpp/fory:xlang_test --test_output=all

if [ $? -ne 0 ]; then
    echo "Error: C++ cross-language tests failed"
    exit 1
fi

echo ""
echo "C++ cross-language tests completed successfully!"
echo ""
echo "Next steps for full cross-language testing:"
echo "1. Ensure Python pyfory is installed:"
echo "   cd ../python && pip install -e ."
echo ""
echo "2. Run Python cross-language tests:"
echo "   cd ../python && python -m pytest pyfory/tests/test_cross_language.py -v"
echo ""
echo "Note: The C++ tests create .data files that are verified by Python tests."
echo "      Set ENABLE_CROSS_LANGUAGE_TESTS=true environment variable to enable Python cross-language tests."
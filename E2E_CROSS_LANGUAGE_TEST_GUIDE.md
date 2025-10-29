# End-to-End Cross-Language Test Guide: C++ and Python

## Overview

This guide explains how to run a complete end-to-end cross-language test between the C++ and Python implementations of Apache Fory™. This test verifies that data serialized by C++ can be correctly deserialized by Python and vice versa.

## Prerequisites

Before running the end-to-end test, ensure you have:

1. **Bazel 6.3.2** installed and in your PATH
2. **Python 3.8+** with pip
3. **Git** for repository management (if not already cloned)

## Setup Instructions

### 1. Clone and Navigate to Repository

```bash
git clone https://github.com/apache/fory.git
cd fory
```

### 2. Install Python Dependencies

```bash
cd python
pip install -e .
cd ..
```

### 3. Verify Bazel Installation

```bash
bazel version
```

You should see version 6.3.2 or compatible version.

## Running the End-to-End Test

### Step 1: Build and Run C++ Cross-Language Tests

The C++ tests will generate data files that Python can verify.

```bash
# Navigate to the root directory
cd /path/to/fory

# Build the C++ cross-language test
bazel build //cpp/fory:xlang_test

# Run the C++ cross-language test
bazel test //cpp/fory:xlang_test --test_output=all
```

Expected output:
```
INFO: Analyzed target //cpp/fory:xlang_test (0 packages loaded, 0 targets configured).
INFO: Found 1 test target...
PASS: //cpp/fory:xlang_test (see /path/to/test.log)
Target //cpp/fory:xlang_test up-to-date:
  bazel-bin/cpp/fory/xlang_test
INFO: Elapsed time: X.XXs, Critical Path: X.XXs
INFO: 1 process: 1 local.
INFO: Build completed successfully, 1 total action
//cpp/fory:xlang_test                                               PASSED in 0.Xs

Executed 1 out of 1 test: 1 test passes.
INFO: Build completed successfully, 1 total action
```

### Step 2: Verify Data Files Creation

Check that the C++ test created the expected data files:

```bash
ls -la cpp/fory/test_*.data
```

You should see files like:
- `test_buffer.data`
- `test_cross_language_serializer.data`
- `test_cross_language_reference.data`

### Step 3: Run Python Cross-Language Tests

The Python tests will read the files created by C++ and verify their contents.

```bash
# Set environment variable to enable cross-language tests
export ENABLE_CROSS_LANGUAGE_TESTS=true

# Run Python cross-language tests
cd python
python -m pytest pyfory/tests/test_cross_language.py::test_buffer -v
python -m pytest pyfory/tests/test_cross_language.py::test_cross_language_serializer -v
```

On Windows:
```cmd
set ENABLE_CROSS_LANGUAGE_TESTS=true
cd python
python -m pytest pyfory/tests/test_cross_language.py::test_buffer -v
python -m pytest pyfory/tests/test_cross_language.py::test_cross_language_serializer -v
```

Expected output:
```
============================= test session starts ==============================
platform linux -- Python 3.x.x, pytest-x.x.x, pluggy-x.x.x
rootdir: /path/to/fory/python
plugins: ...
collected 1 item

pyfory/tests/test_cross_language.py::test_buffer PASSED                  [100%]

============================== 1 passed in 0.XXs ===============================
```

### Step 4: Verify Round-trip Compatibility

The Python tests not only verify the C++ data but also write modified data back to the files. Run the C++ tests again to verify round-trip compatibility:

```bash
# Navigate back to root directory
cd ..

# Run C++ tests again to verify Python-modified data
bazel test //cpp/fory:xlang_test --test_output=all
```

## Understanding the Test Flow

### Data Flow

1. **C++ Serialization**:
   - C++ test creates data structures
   - Serializes them using Fory C++ implementation
   - Writes serialized data to `.data` files

2. **Python Verification**:
   - Python test reads `.data` files
   - Deserializes data using Fory Python implementation
   - Verifies data integrity
   - Modifies data and writes back to files

3. **Round-trip Verification**:
   - C++ test reads Python-modified files
   - Deserializes and verifies consistency
   - Confirms cross-language compatibility

### File Details

- **`test_buffer.data`**: Contains primitive types (bool, int, float, string)
- **`test_cross_language_serializer.data`**: Contains comprehensive type set
- **`test_cross_language_reference.data`**: Contains reference structures

## Troubleshooting Common Issues

### Issue 1: Bazel Not Found
```
bash: bazel: command not found
```
**Solution**: Install Bazel 6.3.2 and ensure it's in your PATH.

### Issue 2: Python Module Not Found
```
ModuleNotFoundError: No module named 'pyfory'
```
**Solution**: Install Python dependencies:
```bash
cd python
pip install -e .
```

### Issue 3: Cross-Language Tests Skipped
```
SKIP: Pass ENABLE_CROSS_LANGUAGE_TESTS to enable cross-language tests
```
**Solution**: Set the environment variable:
```bash
export ENABLE_CROSS_LANGUAGE_TESTS=true
```

### Issue 4: File Not Found Errors
```
FileNotFoundError: [Errno 2] No such file or directory: 'test_buffer.data'
```
**Solution**: Ensure C++ tests run successfully before Python tests.

### Issue 5: Data Verification Failures
```
AssertionError: assert False == True
```
**Solution**: This indicates a serialization compatibility issue. Check:
1. Data format consistency between languages
2. Type mappings between C++ and Python
3. Endianness handling

## Advanced Testing

### Run All Cross-Language Tests

To run all cross-language tests at once:

```bash
# C++ side
bazel test //cpp/fory:xlang_test

# Python side
cd python
export ENABLE_CROSS_LANGUAGE_TESTS=true
python -m pytest pyfory/tests/test_cross_language.py -v
```

### Test Specific Functions

You can test specific cross-language functions:

```bash
# Test buffer operations
python -m pytest pyfory/tests/test_cross_language.py::test_buffer -v

# Test serializer functionality
python -m pytest pyfory/tests/test_cross_language.py::test_cross_language_serializer -v

# Test reference handling
python -m pytest pyfory/tests/test_cross_language.py::test_cross_language_reference -v
```

## Expected Results

A successful end-to-end test should show:

1. **C++ Tests**: All pass without errors
2. **Python Tests**: All pass without errors
3. **File Creation**: Data files created in `cpp/fory/` directory
4. **Round-trip**: C++ can read Python-modified files correctly

## Cleaning Up

To clean up test files:

```bash
# Remove generated test files
rm cpp/fory/test_*.data

# Clean Bazel cache (optional)
bazel clean
```

## Next Steps

After successfully running the end-to-end test:

1. **Explore Other Languages**: Test compatibility with Java, Go, or Rust implementations
2. **Add New Tests**: Extend cross-language tests with custom data structures
3. **Performance Testing**: Compare serialization performance across languages
4. **Integration Testing**: Test cross-language serialization in real applications

## Conclusion

This end-to-end test demonstrates the cross-language compatibility of Apache Fory™ between C++ and Python implementations. The successful execution confirms that:

- Data serialized by C++ can be correctly deserialized by Python
- Data serialized by Python can be correctly deserialized by C++
- The binary protocol is consistent across language implementations
- Reference tracking works correctly in cross-language scenarios

This compatibility is essential for multi-language systems using Apache Fory™ for data serialization and exchange.
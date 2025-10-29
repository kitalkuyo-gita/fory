# Cross-Language Testing in Apache Fory™ C++

## Overview

Apache Fory™ is a multi-language serialization framework that enables cross-language data exchange. This document explains how to run and develop cross-language tests for the C++ implementation.

## Prerequisites

Before running cross-language tests, ensure you have:

1. **Bazel 6.3.2** installed and in your PATH
2. **Python 3.8+** with `pyfory` installed
3. **Java 8+** for running Java-based tests
4. **Go 1.16+** for running Go-based tests (optional)

## Building and Running C++ Tests

### Building Tests

To build all C++ tests including the cross-language tests:

```bash
cd /path/to/fory
bazel build //cpp/fory:xlang_test
```

### Running Tests

To run the C++ cross-language tests:

```bash
bazel test //cpp/fory:xlang_test
```

## Cross-Language Test Structure

The C++ cross-language tests follow a similar pattern to other language implementations:

1. **Data Serialization**: Tests serialize data using C++ Fory implementation
2. **File Output**: Serialized data is written to files with specific naming conventions
3. **Cross-Language Verification**: Python tests are executed to verify the serialized data
4. **Round-trip Testing**: Data is read back and verified for consistency

### Test File Naming Convention

Cross-language test files follow these naming conventions:
- `test_buffer.data` - Basic buffer serialization test
- `test_cross_language_serializer.data` - Comprehensive type serialization
- `test_cross_language_reference.data` - Reference tracking test

## How Cross-Language Tests Work

### 1. C++ Test Execution

When a C++ cross-language test runs:
1. Data is serialized using C++ Fory implementation
2. Serialized data is written to `.data` files
3. Test verifies file creation and basic structure

### 2. Python Verification

The Python test suite verifies the C++ serialized data:
1. Python reads the `.data` files created by C++ tests
2. Data is deserialized using Python Fory implementation
3. Results are verified against expected values
4. Python may modify the data and write it back for round-trip testing

### 3. Round-trip Verification

Cross-language tests ensure data consistency:
1. C++ serializes data → file
2. Python deserializes file → verifies data
3. Python serializes modified data → file
4. C++ deserializes file → verifies consistency

## Example Test Workflow

Here's how the cross-language test workflow works:

```cpp
// C++ test creates data and writes to file
TEST(XLangTest, Buffer) {
  // Serialize data using C++ Fory
  std::shared_ptr<Buffer> buffer = CreateTestData();
  
  // Write to file for cross-language testing
  std::ofstream file("test_buffer.data", std::ios::binary);
  file.write(buffer->data(), buffer->size());
  file.close();
  
  // File is now available for Python verification
}
```

```python
# Python test (pyfory/tests/test_cross_language.py)
@cross_language_test
def test_buffer(data_file_path):
    # Read data file created by C++ test
    with open(data_file_path, "rb") as f:
        data_bytes = f.read()
        buffer = pyfory.Buffer(data_bytes)
        
        # Verify data using Python Fory
        assert buffer.read_bool() is True
        assert buffer.read_int32() == 2**31 - 1
        # ... more verifications
        
        # Write back modified data for round-trip testing
        buffer.write_bool(True)
        # ... more writes
        
    with open(data_file_path, "wb+") as f:
        f.write(buffer.get_bytes(0, buffer.writer_index))
```

## Adding New Cross-Language Tests

To add new cross-language tests:

1. **Create Test Data**: Define the data structures you want to test
2. **Serialize Data**: Use C++ Fory to serialize the data
3. **Write to File**: Save serialized data to a file with appropriate naming
4. **Add Python Test**: Create corresponding test in `pyfory/tests/test_cross_language.py`
5. **Verify Round-trip**: Ensure data consistency between languages

### Example New Test

```cpp
// In xlang_test.cc
TEST(XLangTest, NewDataType) {
  // Create test data
  MyStruct data = CreateTestStruct();
  
  // Serialize using C++ Fory
  std::shared_ptr<Buffer> buffer = SerializeData(data);
  
  // Write to file
  std::string filename = "test_new_data_type.data";
  std::ofstream file(filename, std::ios::binary);
  file.write(buffer->data(), buffer->size());
  file.close();
  
  std::cout << "Created test file: " << filename << std::endl;
}
```

## Debugging Cross-Language Issues

When cross-language tests fail:

1. **Check File Creation**: Verify that `.data` files are created correctly
2. **Validate Data Format**: Ensure serialization format matches other languages
3. **Test Individual Types**: Isolate specific data types that may be causing issues
4. **Compare with Working Tests**: Use existing working tests as reference

## Best Practices

1. **Consistent Naming**: Use the same test function names across languages
2. **Matching Data**: Ensure test data is identical across language implementations
3. **File Cleanup**: Remove temporary files after tests complete
4. **Error Handling**: Implement proper error handling for file operations
5. **Logging**: Provide clear logging for debugging cross-language issues

## Environment Variables

Cross-language tests use these environment variables:

- `ENABLE_CROSS_LANGUAGE_TESTS=true` - Enable cross-language tests
- `PYTHON_EXECUTABLE=python` - Specify Python executable path

## Troubleshooting

### Bazel Build Issues

If you encounter build issues:

```bash
# Clean Bazel cache
bazel clean --expunge

# Rebuild with verbose output
bazel build --verbose_failures //cpp/fory:xlang_test
```

### Python Module Not Found

If Python cannot find the `pyfory` module:

```bash
# Install pyfory in development mode
cd python
pip install -e .
```

### Cross-Language Test Failures

If cross-language tests fail:

1. Verify Python `pyfory` is installed: `python -c "import pyfory"`
2. Check that test files are created in the correct location
3. Ensure environment variables are set correctly
4. Run individual tests to isolate issues

## Related Documentation

- [Python Cross-Language Testing](../python/README.md)
- [Java Cross-Language Testing](../java/README.md)
- [Go Cross-Language Testing](../go/README.md)
- [Fory Cross-Language Specification](../docs/specification/fory_xlang_serialization_spec.md)
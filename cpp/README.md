# Apache Fory™ C++

Apache Fory™ is a blazingly-fast multi-language serialization framework powered by just-in-time compilation and zero-copy.

## Build Apache Fory™ C++

```bash
# Build all projects
bazel build //:all
# Run all tests
bazel test //:all
```

## Cross-Language Testing

Apache Fory™ supports cross-language serialization testing to ensure compatibility between different language implementations.

```bash
# Run cross-language tests
bazel test //cpp/fory:xlang_test
```

For more information about cross-language testing, see [Cross-Language Testing Guide](CROSS_LANGUAGE_TESTING.md).

## Environment

- Bazel version: 6.3.2

## Benchmark

```bash
bazel build //cpp/fory/benchmark:all
bazel test //cpp/fory/benchmark:all
# You can also run a single benchmark to see how efficient it is.
# For example
bazel run //cpp/fory/benchmark:benchmark_string_util
```

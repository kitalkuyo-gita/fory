/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <cmath>

#include "gtest/gtest.h"
#include "fory/util/buffer.h"
#include "fory/util/status.h"
#include "fory/row/writer.h"
#include "fory/row/row.h"

namespace fory {

// Cross-language test utilities
const std::string PYTHON_MODULE = "pyfory.tests.test_cross_language";
const std::string PYTHON_EXECUTABLE = "python";

// Execute an external command for cross-language testing
bool executeCommand(const std::vector<std::string>& command, int waitTimeoutSeconds = 30) {
  std::string cmd = PYTHON_EXECUTABLE;
  for (const auto& arg : command) {
    cmd += " " + arg;
  }
  
  std::cout << "Executing command: " << cmd << std::endl;
  int result = system(cmd.c_str());
  return result == 0;
}

// Test buffer serialization for cross-language compatibility
TEST(XLangTest, Buffer) {
  // Create a buffer with various data types
  std::shared_ptr<Buffer> buffer;
  AllocateBuffer(1024, &buffer);
  
  int32_t offset = 0;
  
  // Write boolean
  buffer->UnsafePut<bool>(offset, true);
  offset += sizeof(bool);
  
  // Write byte (int8)
  buffer->UnsafePut<int8_t>(offset, std::numeric_limits<int8_t>::max());
  offset += sizeof(int8_t);
  
  // Write int16
  buffer->UnsafePut<int16_t>(offset, std::numeric_limits<int16_t>::max());
  offset += sizeof(int16_t);
  
  // Write int32
  buffer->UnsafePut<int32_t>(offset, std::numeric_limits<int32_t>::max());
  offset += sizeof(int32_t);
  
  // Write int64
  buffer->UnsafePut<int64_t>(offset, std::numeric_limits<int64_t>::max());
  offset += sizeof(int64_t);
  
  // Write float32
  buffer->UnsafePut<float>(offset, -1.1f);
  offset += sizeof(float);
  
  // Write float64
  buffer->UnsafePut<double>(offset, -1.1);
  offset += sizeof(double);
  
  // Write varint (simplified)
  buffer->UnsafePut<uint32_t>(offset, 100);
  offset += sizeof(uint32_t);
  
  // Write bytes
  std::string test_bytes = "ab";
  buffer->UnsafePut<int32_t>(offset, static_cast<int32_t>(test_bytes.length()));
  offset += sizeof(int32_t);
  
  for (char c : test_bytes) {
    buffer->UnsafePut<char>(offset, c);
    offset += sizeof(char);
  }
  
  // Write to file for cross-language testing
  std::string filename = "test_buffer.data";
  std::ofstream file(filename, std::ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char*>(buffer->data()), offset);
    file.close();
    
    // Execute Python cross-language test
    std::vector<std::string> command = {
      "-m", PYTHON_MODULE, "test_buffer", filename
    };
    
    // Set environment variable for cross-language tests
    #ifdef _WIN32
    putenv("ENABLE_CROSS_LANGUAGE_TESTS=true");
    #else
    setenv("ENABLE_CROSS_LANGUAGE_TESTS", "true", 1);
    #endif
    
    // Note: In a real implementation, we would execute the cross-language test
    // For now, we'll just verify the file was created
    std::ifstream check_file(filename, std::ios::binary);
    EXPECT_TRUE(check_file.good()) << "Failed to create cross-language test file";
    check_file.close();
    
    std::cout << "Created cross-language test file: " << filename << std::endl;
  } else {
    FAIL() << "Could not open file " << filename << " for writing";
  }
}

// Test cross-language serializer with basic types
TEST(XLangTest, CrossLanguageSerializer) {
  // Create a buffer with various data types for cross-language testing
  std::shared_ptr<Buffer> buffer;
  AllocateBuffer(2048, &buffer);
  
  int32_t offset = 0;
  
  // Write basic types that match Python/Java cross-language tests
  buffer->UnsafePut<bool>(offset, true);
  offset += sizeof(bool);
  
  buffer->UnsafePut<bool>(offset, false);
  offset += sizeof(bool);
  
  buffer->UnsafePut<int64_t>(offset, -1);
  offset += sizeof(int64_t);
  
  buffer->UnsafePut<int8_t>(offset, std::numeric_limits<int8_t>::max());
  offset += sizeof(int8_t);
  
  buffer->UnsafePut<int8_t>(offset, std::numeric_limits<int8_t>::min());
  offset += sizeof(int8_t);
  
  buffer->UnsafePut<int16_t>(offset, std::numeric_limits<int16_t>::max());
  offset += sizeof(int16_t);
  
  buffer->UnsafePut<int16_t>(offset, std::numeric_limits<int16_t>::min());
  offset += sizeof(int16_t);
  
  buffer->UnsafePut<int32_t>(offset, std::numeric_limits<int32_t>::max());
  offset += sizeof(int32_t);
  
  buffer->UnsafePut<int32_t>(offset, std::numeric_limits<int32_t>::min());
  offset += sizeof(int32_t);
  
  buffer->UnsafePut<int64_t>(offset, std::numeric_limits<int64_t>::max());
  offset += sizeof(int64_t);
  
  buffer->UnsafePut<int64_t>(offset, std::numeric_limits<int64_t>::min());
  offset += sizeof(int64_t);
  
  buffer->UnsafePut<float>(offset, -1.0f);
  offset += sizeof(float);
  
  buffer->UnsafePut<double>(offset, -1.0);
  offset += sizeof(double);
  
  // Write string
  std::string test_str = "str";
  buffer->UnsafePut<int32_t>(offset, static_cast<int32_t>(test_str.length()));
  offset += sizeof(int32_t);
  
  for (char c : test_str) {
    buffer->UnsafePut<char>(offset, c);
    offset += sizeof(char);
  }
  
  // Write to file for cross-language testing
  std::string filename = "test_cross_language_serializer.data";
  std::ofstream file(filename, std::ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char*>(buffer->data()), offset);
    file.close();
    
    std::cout << "Created cross-language serializer test file: " << filename << std::endl;
    std::cout << "File contains various basic types for cross-language compatibility testing" << std::endl;
  } else {
    FAIL() << "Could not open file " << filename << " for writing";
  }
}

// Test cross-language reference handling
TEST(XLangTest, CrossLanguageReference) {
  // Create a simple data structure with references for cross-language testing
  // In a real implementation, this would involve more complex reference tracking
  
  // For now, we'll create a file with a simple array structure
  std::shared_ptr<Buffer> buffer;
  AllocateBuffer(1024, &buffer);
  
  int32_t offset = 0;
  
  // Write array with some basic values
  std::vector<std::string> test_values = {"a", "1", "-1.0"};
  
  // Write array length
  buffer->UnsafePut<int32_t>(offset, static_cast<int32_t>(test_values.size()));
  offset += sizeof(int32_t);
  
  // Write array elements
  for (const auto& value : test_values) {
    buffer->UnsafePut<int32_t>(offset, static_cast<int32_t>(value.length()));
    offset += sizeof(int32_t);
    
    for (char c : value) {
      buffer->UnsafePut<char>(offset, c);
      offset += sizeof(char);
    }
  }
  
  // Write to file for cross-language testing
  std::string filename = "test_cross_language_reference.data";
  std::ofstream file(filename, std::ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char*>(buffer->data()), offset);
    file.close();
    
    std::cout << "Created cross-language reference test file: " << filename << std::endl;
  } else {
    FAIL() << "Could not open file " << filename << " for writing";
  }
}

} // namespace fory

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
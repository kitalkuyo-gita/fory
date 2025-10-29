#!/usr/bin/env python3
"""
Verification script for C++ cross-language test implementation.
This script checks if all required files have been created correctly.
"""

import os
import sys

def check_file_exists(filepath, description):
    """Check if a file exists and print status."""
    if os.path.exists(filepath):
        print(f"✓ {description} - FOUND")
        return True
    else:
        print(f"✗ {description} - NOT FOUND")
        return False

def check_file_content(filepath, expected_strings, description):
    """Check if a file contains expected content."""
    if not os.path.exists(filepath):
        print(f"✗ {description} - NOT FOUND")
        return False
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        all_found = True
        for expected in expected_strings:
            if expected in content:
                print(f"  ✓ Contains: {expected}")
            else:
                print(f"  ✗ Missing: {expected}")
                all_found = False
        
        if all_found:
            print(f"✓ {description} - CONTENT OK")
            return True
        else:
            print(f"✗ {description} - CONTENT ISSUES")
            return False
    except Exception as e:
        print(f"✗ {description} - ERROR reading file: {e}")
        return False

def main():
    print("Apache Fory™ C++ Cross-Language Test Implementation Verification")
    print("=" * 65)
    
    # Change to the repository root
    repo_root = os.path.dirname(os.path.abspath(__file__))
    os.chdir(repo_root)
    
    all_checks_passed = True
    
    # Check core implementation files
    print("\n1. Core Implementation Files:")
    checks = [
        ("cpp/fory/xlang_test.cc", "C++ cross-language test implementation"),
        ("cpp/fory/BUILD", "C++ BUILD file with xlang_test target"),
    ]
    
    for filepath, description in checks:
        if not check_file_exists(filepath, description):
            all_checks_passed = False
    
    # Check documentation files
    print("\n2. Documentation Files:")
    doc_checks = [
        ("cpp/CROSS_LANGUAGE_TESTING.md", "Cross-language testing guide"),
        ("cpp/README.md", "Updated C++ README"),
        ("CPP_CROSS_LANGUAGE_TEST_SUMMARY.md", "Implementation summary"),
        ("E2E_CROSS_LANGUAGE_TEST_GUIDE.md", "End-to-end testing guide"),
    ]
    
    for filepath, description in doc_checks:
        if not check_file_exists(filepath, description):
            all_checks_passed = False
    
    # Check helper scripts
    print("\n3. Helper Scripts:")
    script_checks = [
        ("cpp/run_xlang_tests.sh", "Unix/Linux/macOS test runner"),
        ("cpp/run_xlang_tests.bat", "Windows test runner"),
    ]
    
    for filepath, description in script_checks:
        if not check_file_exists(filepath, description):
            all_checks_passed = False
    
    # Check content of key files
    print("\n4. Content Verification:")
    
    # Check xlang_test.cc has proper test functions
    xlang_test_content = [
        "TEST(XLangTest, Buffer)",
        "TEST(XLangTest, CrossLanguageSerializer)",
        "TEST(XLangTest, CrossLanguageReference)",
        "#include \"gtest/gtest.h\""
    ]
    
    if not check_file_content("cpp/fory/xlang_test.cc", xlang_test_content, 
                             "C++ test implementation content"):
        all_checks_passed = False
    
    # Check BUILD file has xlang_test target
    build_content = [
        "cc_test(",
        "name = \"xlang_test\"",
        "srcs = [\"xlang_test.cc\"]"
    ]
    
    if not check_file_content("cpp/fory/BUILD", build_content, 
                             "BUILD file xlang_test target"):
        all_checks_passed = False
    
    # Check README has cross-language info
    readme_content = [
        "Cross-Language Testing",
        "CROSS_LANGUAGE_TESTING.md"
    ]
    
    if not check_file_content("cpp/README.md", readme_content, 
                             "README cross-language info"):
        all_checks_passed = False
    
    # Summary
    print("\n" + "=" * 65)
    if all_checks_passed:
        print("✓ ALL CHECKS PASSED - Implementation appears correct!")
        print("\nTo run the tests when Bazel is available:")
        print("  1. Install Bazel 6.3.2")
        print("  2. Run: bazel build //cpp/fory:xlang_test")
        print("  3. Run: bazel test //cpp/fory:xlang_test")
        print("  4. For full cross-language testing, also run Python tests")
        return 0
    else:
        print("✗ SOME CHECKS FAILED - Please review the implementation")
        return 1

if __name__ == "__main__":
    sys.exit(main())
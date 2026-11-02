cmake_minimum_required(VERSION 3.20)
project(sap1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Lets VS Code's IntelliSense see the exact compiler flags and include paths.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Tests. Every test file listed here is compiled into one program, sap1_tests.
# When you write a new test file, add its name to this list.
# ---------------------------------------------------------------------------
enable_testing()

add_executable(sap1_tests
    tests/test_main.cpp
    tests/test_smoke.cpp
    tests/test_control_word.cpp
    tests/test_register.cpp
    tests/test_program_counter.cpp
)

# doctest.h lives in third_party/doctest; the core headers you write will live
# in core/include, and are included in code as  #include "sap1/<name>.hpp"
target_include_directories(sap1_tests PRIVATE
    third_party/doctest
    core/include
)

# Warnings are free bug reports. Read them.
target_compile_options(sap1_tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion)

add_test(NAME sap1_tests COMMAND sap1_tests)

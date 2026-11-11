cmake_minimum_required(VERSION 3.20)
project(sap1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Lets VS Code's IntelliSense see the exact compiler flags and include paths.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Core library: the machine itself. Only code that needs a .cpp file goes in
# this list; header-only pieces (registers, ALU, ...) need no entry here.
# Built under microcontroller-style rules: no exceptions, no RTTI.
# ---------------------------------------------------------------------------
add_library(sap1_core STATIC
    core/src/ram.cpp
)

# PUBLIC: the core's own files AND anything that links to it can include
# "sap1/<name>.hpp" from core/include.
target_include_directories(sap1_core PUBLIC core/include)

target_compile_options(sap1_core PRIVATE
    -fno-exceptions -fno-rtti
    -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
)

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
    tests/test_instruction_register.cpp
    tests/test_alu.cpp
    tests/test_ram.cpp
    tests/test_microcode.cpp
)

# Linking sap1_core gives the tests the core's include path automatically.
target_link_libraries(sap1_tests PRIVATE sap1_core)
target_include_directories(sap1_tests PRIVATE third_party/doctest)

# Warnings are free bug reports. Read them.
target_compile_options(sap1_tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion)

add_test(NAME sap1_tests COMMAND sap1_tests)
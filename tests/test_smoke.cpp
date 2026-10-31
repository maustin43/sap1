#include "doctest.h"

// If this builds and passes, your compiler, CMake, and doctest are all working.
TEST_CASE("toolchain works") {
    CHECK(1 + 1 == 2);
}

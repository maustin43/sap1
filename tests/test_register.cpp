#include "doctest.h"
#include <type_traits>
#include "sap1/register.hpp"

using namespace sap1;

TEST_CASE("a new register holds zero") {
    Register8 r;
    CHECK(r.value() == 0);
}

TEST_CASE("Register8 keeps all eight bus bits") {
    Register8 r;
    r.load(0xAB);
    CHECK(r.value() == 0xAB);
}

TEST_CASE("Register4 sees only the low nibble of the bus") {
    Register4 r;
    r.load(0xAB);
    CHECK(r.value() == 0x0B);
}

TEST_CASE("Register<1> keeps a single bit") {
    Register<1> r;
    r.load(0x03);
    CHECK(r.value() == 1);
    r.load(0x02);
    CHECK(r.value() == 0);
}

TEST_CASE("a register holds its value until loaded, and load replaces it") {
    Register8 r;
    r.load(0xFF);
    CHECK(r.value() == 0xFF);
    CHECK(r.value() == 0xFF);  // reading does not disturb it
    r.load(0x00);
    CHECK(r.value() == 0x00);  // a new load replaces the old value, not ORs
}

TEST_CASE("reset clears the register") {
    Register8 r;
    r.load(0x5A);
    r.reset();
    CHECK(r.value() == 0);
}

TEST_CASE("masks are computed at compile time") {
    static_assert(Register<1>::kMask == 0x01);
    static_assert(Register<4>::kMask == 0x0F);
    static_assert(Register<8>::kMask == 0xFF);
    CHECK(true);  // the real checks are the static_asserts above
}

TEST_CASE("different widths are different types, but the same size") {
    static_assert(!std::is_same_v<Register4, Register8>);
    static_assert(sizeof(Register4) == 1);
    static_assert(sizeof(Register8) == 1);
    CHECK(true);
}
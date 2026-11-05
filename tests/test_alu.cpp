#include "doctest.h"
#include "sap1/alu.hpp"

using namespace sap1;

TEST_CASE("ALU adds when subtract is false")
{
    CHECK(alu_result(5, 3, false) == 8);
    CHECK(alu_result(0, 0, false) == 0);
    CHECK(alu_result(28, 14, false) == 42);
}

TEST_CASE("ALU subtracts when subtract is true")
{
    CHECK(alu_result(5, 3, true) == 2);
    CHECK(alu_result(5, 5, true) == 0);
    CHECK(alu_result(42, 14, true) == 28);
}

TEST_CASE("results wrap modulo 256, like an 8-bit adder with no carry out")
{
    CHECK(alu_result(255, 1, false) == 0);
    CHECK(alu_result(200, 100, false) == 44);
    CHECK(alu_result(0, 1, true) == 255); // -1 as an unsigned byte
    CHECK(alu_result(3, 5, true) == 254); // -2 as an unsigned byte
}

TEST_CASE("the ALU never changes its inputs' meaning: B of zero")
{
    CHECK(alu_result(77, 0, false) == 77);
    CHECK(alu_result(77, 0, true) == 77);
}

TEST_CASE("A + B is correct for every pair of bytes")
{
    for (unsigned a = 0; a < 256; ++a)
    {
        for (unsigned b = 0; b < 256; ++b)
        {
            const auto expected = static_cast<std::uint8_t>((a + b) & 0xFF);
            REQUIRE(alu_result(static_cast<std::uint8_t>(a),
                               static_cast<std::uint8_t>(b), false) == expected);
        }
    }
}

TEST_CASE("A - B is correct for every pair of bytes")
{
    for (unsigned a = 0; a < 256; ++a)
    {
        for (unsigned b = 0; b < 256; ++b)
        {
            const auto expected = static_cast<std::uint8_t>((a - b) & 0xFF);
            REQUIRE(alu_result(static_cast<std::uint8_t>(a),
                               static_cast<std::uint8_t>(b), true) == expected);
        }
    }
}

TEST_CASE("the ALU works at compile time")
{
    static_assert(alu_result(5, 3, true) == 2);
    static_assert(alu_result(255, 1, false) == 0);
    CHECK(true); // the real checks are the static_asserts above
}
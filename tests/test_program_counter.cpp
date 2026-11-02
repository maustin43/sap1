#include "doctest.h"
#include "sap1/program_counter.hpp"

using namespace sap1;

TEST_CASE("a new program counter points at address zero")
{
    ProgramCounter pc;
    CHECK(pc.value() == 0);
}

TEST_CASE("count advances by one")
{
    ProgramCounter pc;
    pc.count();
    CHECK(pc.value() == 1);
    pc.count();
    CHECK(pc.value() == 2);
}

TEST_CASE("count wraps from 15 back to 0")
{
    ProgramCounter pc;
    for (int i = 0; i < 15; ++i)
    {
        pc.count();
    }
    CHECK(pc.value() == 15);
    pc.count();
    CHECK(pc.value() == 0);
    pc.count();
    CHECK(pc.value() == 1);
}

TEST_CASE("jump loads a new address from the bus")
{
    ProgramCounter pc;
    pc.jump(9);
    CHECK(pc.value() == 9);
}

TEST_CASE("jump sees only the low four bus bits")
{
    ProgramCounter pc;
    pc.jump(0xA7);
    CHECK(pc.value() == 0x7);
}

TEST_CASE("counting continues from the jump target")
{
    ProgramCounter pc;
    pc.jump(14);
    pc.count();
    CHECK(pc.value() == 15);
    pc.count();
    CHECK(pc.value() == 0);
}

TEST_CASE("reset returns to address zero")
{
    ProgramCounter pc;
    pc.jump(12);
    pc.reset();
    CHECK(pc.value() == 0);
}

namespace
{
    constexpr std::uint8_t count_n_times(int n)
    {
        ProgramCounter pc;
        for (int i = 0; i < n; ++i)
        {
            pc.count();
        }
        return pc.value();
    }
} // namespace

TEST_CASE("the program counter works at compile time")
{
    static_assert(count_n_times(5) == 5);
    static_assert(count_n_times(16) == 0);
    static_assert(count_n_times(21) == 5);
    CHECK(true); // the real checks are the static_asserts above
}
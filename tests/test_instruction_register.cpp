#include "doctest.h"
#include "sap1/instruction_register.hpp"

using namespace sap1;

TEST_CASE("a new instruction register holds zero")
{
    InstructionRegister ir;
    CHECK(ir.value() == 0);
    CHECK(ir.opcode() == 0);
    CHECK(ir.operand() == 0);
}

TEST_CASE("load splits the byte into opcode and operand")
{
    InstructionRegister ir;
    ir.load(0x1E); // LDA 14
    CHECK(ir.value() == 0x1E);
    CHECK(ir.opcode() == 0x1);
    CHECK(ir.operand() == 0xE);
}

TEST_CASE("high nibble is the opcode, low nibble is the operand")
{
    InstructionRegister ir;
    ir.load(0xF0); // HLT
    CHECK(ir.opcode() == 0xF);
    CHECK(ir.operand() == 0x0);
    ir.load(0x0F);
    CHECK(ir.opcode() == 0x0);
    CHECK(ir.operand() == 0xF);
}

TEST_CASE("every possible byte splits correctly and recombines")
{
    InstructionRegister ir;
    for (unsigned b = 0; b < 256; ++b)
    {
        ir.load(static_cast<std::uint8_t>(b));
        REQUIRE(ir.value() == b);
        REQUIRE(ir.opcode() == (b >> 4));
        REQUIRE(ir.operand() == (b & 0x0F));
        REQUIRE(ir.opcode() <= 0xF);
        REQUIRE(ir.operand() <= 0xF);
        REQUIRE(((ir.opcode() << 4) | ir.operand()) == b);
    }
}

TEST_CASE("a new load replaces the previous instruction")
{
    InstructionRegister ir;
    ir.load(0xFF);
    ir.load(0x20);
    CHECK(ir.opcode() == 0x2);
    CHECK(ir.operand() == 0x0);
}

TEST_CASE("reset clears the instruction register")
{
    InstructionRegister ir;
    ir.load(0x5A);
    ir.reset();
    CHECK(ir.value() == 0);
    CHECK(ir.opcode() == 0);
    CHECK(ir.operand() == 0);
}

namespace
{
    constexpr std::uint8_t opcode_after_load(std::uint8_t byte)
    {
        InstructionRegister ir;
        ir.load(byte);
        return ir.opcode();
    }
} // namespace

TEST_CASE("the instruction register works at compile time")
{
    static_assert(opcode_after_load(0x1E) == 0x1);
    static_assert(opcode_after_load(0xF0) == 0xF);
    CHECK(true); // the real checks are the static_asserts above
}
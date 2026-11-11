#include "doctest.h"
#include "sap1/microcode.hpp"

#include <bit>

using namespace sap1;

namespace
{
    // Looks up one microcode word: row for an opcode, column for a step.
    ControlWord word(Opcode op, std::size_t step)
    {
        return kMicrocode[static_cast<std::size_t>(op)][step];
    }
    ControlWord word(std::size_t op, std::size_t step)
    {
        return kMicrocode[op][step];
    }
} // namespace

TEST_CASE("table dimensions match the hardware: 16 opcodes by 8 steps")
{
    static_assert(kOpcodeCount == 16);
    static_assert(kTableSteps == 8);
    static_assert(kStepsPerInstruction == 5);
    CHECK(kMicrocode.size() == 16);
    CHECK(kMicrocode[0].size() == 8);
}

TEST_CASE("opcode numbers match the instruction encoding")
{
    CHECK(static_cast<int>(Opcode::NOP) == 0x0);
    CHECK(static_cast<int>(Opcode::LDA) == 0x1);
    CHECK(static_cast<int>(Opcode::ADD) == 0x2);
    CHECK(static_cast<int>(Opcode::SUB) == 0x3);
    CHECK(static_cast<int>(Opcode::STA) == 0x4);
    CHECK(static_cast<int>(Opcode::LDI) == 0x5);
    CHECK(static_cast<int>(Opcode::JMP) == 0x6);
    CHECK(static_cast<int>(Opcode::OUT) == 0xE);
    CHECK(static_cast<int>(Opcode::HLT) == 0xF);
}

TEST_CASE("every opcode begins with the same fetch cycle")
{
    for (std::size_t op = 0; op < kOpcodeCount; ++op)
    {
        CHECK(word(op, 0) == (ctl::CO | ctl::MI));
        CHECK(word(op, 1) == (ctl::RO | ctl::II | ctl::CE));
    }
}

TEST_CASE("NOP does nothing after the fetch")
{
    for (std::size_t step = 2; step < kTableSteps; ++step)
    {
        CHECK(word(Opcode::NOP, step) == 0);
    }
}

TEST_CASE("LDA: address to MAR, then RAM into A")
{
    CHECK(word(Opcode::LDA, 2) == (ctl::IO | ctl::MI));
    CHECK(word(Opcode::LDA, 3) == (ctl::RO | ctl::AI));
    CHECK(word(Opcode::LDA, 4) == 0);
}

TEST_CASE("ADD: address to MAR, RAM into B, ALU sum into A")
{
    CHECK(word(Opcode::ADD, 2) == (ctl::IO | ctl::MI));
    CHECK(word(Opcode::ADD, 3) == (ctl::RO | ctl::BI));
    CHECK(word(Opcode::ADD, 4) == (ctl::EO | ctl::AI));
}

TEST_CASE("SUB: same as ADD, but with the subtract line asserted")
{
    CHECK(word(Opcode::SUB, 2) == (ctl::IO | ctl::MI));
    CHECK(word(Opcode::SUB, 3) == (ctl::RO | ctl::BI));
    CHECK(word(Opcode::SUB, 4) == (ctl::EO | ctl::AI | ctl::SU));
}

TEST_CASE("STA: address to MAR, then A into RAM")
{
    CHECK(word(Opcode::STA, 2) == (ctl::IO | ctl::MI));
    CHECK(word(Opcode::STA, 3) == (ctl::AO | ctl::RI));
    CHECK(word(Opcode::STA, 4) == 0);
}

TEST_CASE("LDI: operand straight into A")
{
    CHECK(word(Opcode::LDI, 2) == (ctl::IO | ctl::AI));
    CHECK(word(Opcode::LDI, 3) == 0);
}

TEST_CASE("JMP: operand into the program counter")
{
    CHECK(word(Opcode::JMP, 2) == (ctl::IO | ctl::J));
    CHECK(word(Opcode::JMP, 3) == 0);
}

TEST_CASE("OUT: A into the output register")
{
    CHECK(word(Opcode::OUT, 2) == (ctl::AO | ctl::OI));
    CHECK(word(Opcode::OUT, 3) == 0);
}

TEST_CASE("HLT: halts the clock")
{
    CHECK(word(Opcode::HLT, 2) == ctl::HLT);
    CHECK(word(Opcode::HLT, 3) == 0);
}

TEST_CASE("steps beyond T4 are always empty")
{
    for (std::size_t op = 0; op < kOpcodeCount; ++op)
    {
        for (std::size_t step = kStepsPerInstruction; step < kTableSteps; ++step)
        {
            CHECK(word(op, step) == 0);
        }
    }
}

TEST_CASE("unassigned opcodes behave like NOP")
{
    for (std::size_t op = 0x7; op <= 0xD; ++op)
    {
        for (std::size_t step = 2; step < kTableSteps; ++step)
        {
            CHECK(word(op, step) == 0);
        }
    }
}

TEST_CASE("no step ever has two devices driving the bus")
{
    for (std::size_t op = 0; op < kOpcodeCount; ++op)
    {
        for (std::size_t step = 0; step < kTableSteps; ++step)
        {
            CHECK(std::popcount(static_cast<ControlWord>(word(op, step) & ctl::kBusDrivers)) <= 1);
        }
    }
}

TEST_CASE("special signals appear only where they should")
{
    for (std::size_t op = 0; op < kOpcodeCount; ++op)
    {
        for (std::size_t step = 0; step < kTableSteps; ++step)
        {
            const ControlWord w = word(op, step);
            CHECK(((w & ctl::FI) != 0) == false); // reserved for the flags add-on
            CHECK(((w & ctl::HLT) != 0) ==
                  (op == static_cast<std::size_t>(Opcode::HLT) && step == 2));
            CHECK(((w & ctl::SU) != 0) ==
                  (op == static_cast<std::size_t>(Opcode::SUB) && step == 4));
        }
    }
}

TEST_CASE("the table is built at compile time")
{
    static_assert(kMicrocode[0x1][2] == (ctl::IO | ctl::MI));
    static_assert(kMicrocode[0xF][2] == ctl::HLT);
    CHECK(true); // the real checks are the static_asserts above
}
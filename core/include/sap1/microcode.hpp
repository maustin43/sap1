#pragma once

#include "sap1/control_word.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace sap1 {

enum class Opcode : std::uint8_t {
    NOP = 0x0,
    LDA = 0x1,
    ADD = 0x2,
    SUB = 0x3,
    STA = 0x4,
    LDI = 0x5,
    JMP = 0x6,
    OUT = 0xE,
    HLT = 0xF
};

inline constexpr std::size_t kOpcodeCount = 16;
inline constexpr std::size_t kTableSteps = 8;
inline constexpr std::size_t kStepsPerInstruction = 5;

using Microcode = std::array<std::array<ControlWord, kTableSteps>, kOpcodeCount>;

constexpr Microcode build_microcode() noexcept {
    Microcode table{};

    auto set = [&](Opcode op, std::size_t step, ControlWord cw) {
        table[static_cast<std::size_t>(op)][step] = cw;
    };

    constexpr ControlWord kFetch0 = ctl::CO | ctl::MI;
    constexpr ControlWord kFetch1 = ctl::RO | ctl::II | ctl::CE;

    // Use std::size_t instead of Opcode to make sure all opcodes are covered, even those
    // not yet defined
    for (std::size_t op = 0; op < kOpcodeCount; ++op) {
        table[op][0] = kFetch0;
        table[op][1] = kFetch1;
    }

    // LDA
    set(Opcode::LDA, 2, ctl::IO | ctl::MI);
    set(Opcode::LDA, 3, ctl::RO | ctl::AI);

    // ADD
    set(Opcode::ADD, 2, ctl::IO | ctl::MI);
    set(Opcode::ADD, 3, ctl::RO | ctl::BI);
    set(Opcode::ADD, 4, ctl::EO | ctl::AI);

    // SUB
    set(Opcode::SUB, 2, ctl::IO | ctl::MI);
    set(Opcode::SUB, 3, ctl::RO | ctl::BI);
    set(Opcode::SUB, 4, ctl::EO | ctl::SU | ctl::AI);

    // STA
    set(Opcode::STA, 2, ctl::IO | ctl::MI);
    set(Opcode::STA, 3, ctl::AO | ctl::RI);

    // LDI
    set(Opcode::LDI, 2, ctl::IO | ctl::AI);

    // JMP
    set(Opcode::JMP, 2, ctl::IO | ctl::J);

    // OUT
    set(Opcode::OUT, 2, ctl::AO | ctl::OI);

    // HLT
    set(Opcode::HLT, 2, ctl::HLT);

    return table;
}
inline constexpr Microcode kMicrocode = build_microcode();
} // namespace sap1
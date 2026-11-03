#pragma once
#include "sap1/register.hpp"
#include <cstdint>

namespace sap1 {
class InstructionRegister {

public:
    constexpr std::uint8_t value() const noexcept {
        return reg_.value();
    }

    constexpr void load(std::uint8_t bus) noexcept {
        reg_.load(bus);
    }

    constexpr void reset() noexcept {
        reg_.reset();
    }

    constexpr std::uint8_t opcode() const noexcept {
        return static_cast<std::uint8_t>(reg_.value() >> 4);
    }
    constexpr std::uint8_t operand() const noexcept {
        return static_cast<std::uint8_t>(reg_.value() & 0xF);
    }

private:
    Register8 reg_;
};

} // namespace sap1

#pragma once
#include <cstdint>

namespace sap1 {
constexpr std::uint8_t alu_result(std::uint8_t a, std::uint8_t b,
                                  bool subtract) noexcept {
    const std::uint8_t operand = subtract ? static_cast<std::uint8_t>(~b) : b;

    // Compiler promotes to int for the calculation. static_cast
    // aknowledges narrowing cast to 8 bit to wraparound and
    // begin counting again. The discarded ninth bit is the addr's carry-out
    return static_cast<std::uint8_t>(a + operand + (subtract ? 1 : 0));
}

} // namespace sap1
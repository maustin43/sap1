#pragma once
#include "sap1/register.hpp"
#include <cstdint>

namespace sap1 {

class ProgramCounter {
public:
    constexpr std::uint8_t value() const noexcept {
        return reg_.value();
    }

    constexpr void count() noexcept {
        reg_.load(static_cast<std::uint8_t>(reg_.value() + 1));
    }

    constexpr void jump(std::uint8_t bus) noexcept {
        reg_.load(bus);
    }

    constexpr void reset() noexcept {
        reg_.reset();
    }

private:
    Register4 reg_;
};

} // namespace sap1
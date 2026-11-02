#pragma once
#include "sap1/register.hpp"
#include <cstdint>

namespace sap1 {
public:
constexpr std::uint8_t value() const noexcept {
    return reg_;
}

constexpr void reset() noexcept {
    reg_ = 0;
}

private:
Register4 reg_

} // namespace sap1
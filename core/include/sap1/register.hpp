#pragma once
#include <cstdint>

namespace sap1 {

template <unsigned Bits> class Register {
    static_assert(Bits >= 1 && Bits <= 8, "Register must have between 1 and 8 bits");

public:
    static constexpr std::uint8_t kMask = static_cast<std::uint8_t>((1u << Bits) - 1);
    constexpr std::uint8_t value() const noexcept {
        return value_;
    }
    constexpr void load(std::uint8_t bus) noexcept {
        value_ = static_cast<std::uint8_t>(bus & kMask);
    }

    constexpr void reset() noexcept {
        value_ = 0;
    }

private:
    std::uint8_t value_ = 0;
}; // class Register

using Register4 = Register<4>; // MAR is 4 bits wide, so we can use this type for it
using Register8 =
    Register<8>; // A and B registers are 8 bits wide, so we can use this type for them

} // namespace sap1
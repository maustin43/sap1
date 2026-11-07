#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace sap1 {

class Ram {
public:
    static constexpr std::size_t kSize = 16;
    using Image = std::array<std::uint8_t, kSize>;

    std::uint8_t read(std::uint8_t address) const noexcept;

    void write(std::uint8_t address, std::uint8_t value) noexcept;

    void clear() noexcept;

    bool load_image(std::span<const std::uint8_t> bytes) noexcept;

    const Image &contents() const noexcept {
        return data_;
    }

private:
    Image data_{};

}; // Ram

} // namespace sap1
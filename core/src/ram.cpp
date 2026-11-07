#include "sap1/ram.hpp"
#include <algorithm>
#include <span>

namespace sap1
{

    std::uint8_t Ram::read(std::uint8_t address) const noexcept
    {
        return data_[address & 0x0F];
    }

    void Ram::write(std::uint8_t address, std::uint8_t value) noexcept
    {
        data_[address & 0x0F] = value;
    }

    void Ram::clear() noexcept
    {
        data_.fill(0);
    }

    bool Ram::load_image(std::span<const std::uint8_t> bytes) noexcept
    {
        if (bytes.size() > kSize)
        {
            return false;
        }

        Ram::clear();

        std::copy(bytes.begin(), bytes.end(), data_.begin());
        return true;
    }
} // Namespace sap1
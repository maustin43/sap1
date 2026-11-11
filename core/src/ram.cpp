#include "sap1/ram.hpp"
#include <algorithm>
#include <span>

namespace
{
    static constexpr std::size_t kAddressMask = sap1::Ram::kSize;
}

namespace sap1
{

    std::uint8_t Ram::read(std::uint8_t address) const noexcept
    {
        return data_[address & (kAddressMask - 1)];
    }

    void Ram::write(std::uint8_t address, std::uint8_t value) noexcept
    {
        data_[address & (kAddressMask - 1)] = value;
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
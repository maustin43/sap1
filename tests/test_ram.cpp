#include "doctest.h"
#include "sap1/ram.hpp"

#include <array>

using namespace sap1;

TEST_CASE("RAM starts zeroed")
{
    Ram ram;
    for (std::size_t i = 0; i < Ram::kSize; ++i)
    {
        CHECK(ram.read(static_cast<std::uint8_t>(i)) == 0);
    }
}

TEST_CASE("RAM reads back what was written")
{
    Ram ram;
    ram.write(14, 28);
    CHECK(ram.read(14) == 28);
}

TEST_CASE("every address holds its own value")
{
    Ram ram;
    for (std::uint8_t i = 0; i < Ram::kSize; ++i)
    {
        ram.write(i, static_cast<std::uint8_t>(i * 7 + 1));
    }
    for (std::uint8_t i = 0; i < Ram::kSize; ++i)
    {
        CHECK(ram.read(i) == static_cast<std::uint8_t>(i * 7 + 1));
    }
}

TEST_CASE("RAM addresses are four bits wide")
{
    Ram ram;
    ram.write(3, 0x55);
    CHECK(ram.read(0x13) == 0x55); // upper nibble ignored: same cell as address 3
    ram.write(0xF2, 0x66);
    CHECK(ram.read(2) == 0x66);
}

TEST_CASE("clear zeroes every cell")
{
    Ram ram;
    ram.write(0, 1);
    ram.write(15, 2);
    ram.clear();
    CHECK(ram.read(0) == 0);
    CHECK(ram.read(15) == 0);
}

TEST_CASE("load_image copies the bytes and zero-fills the rest")
{
    Ram ram;
    ram.write(15, 0xFF); // stale data that load_image must wipe
    const std::array<std::uint8_t, 4> program = {0x1E, 0x2F, 0xE0, 0xF0};
    CHECK(ram.load_image(program));
    CHECK(ram.read(0) == 0x1E);
    CHECK(ram.read(3) == 0xF0);
    CHECK(ram.read(4) == 0);
    CHECK(ram.read(15) == 0);
}

TEST_CASE("load_image accepts exactly 16 bytes and rejects 17")
{
    Ram ram;
    std::array<std::uint8_t, 16> full{};
    full[15] = 0xAA;
    CHECK(ram.load_image(full));
    CHECK(ram.read(15) == 0xAA);

    std::array<std::uint8_t, 17> too_big{};
    too_big[0] = 0x77;
    CHECK_FALSE(ram.load_image(too_big));
    CHECK(ram.read(15) == 0xAA); // unchanged by the failed load
    CHECK(ram.read(0) == 0);
}

TEST_CASE("contents exposes the whole image for display")
{
    Ram ram;
    ram.write(2, 9);
    const Ram::Image &image = ram.contents();
    CHECK(image.size() == Ram::kSize);
    CHECK(image[2] == 9);
}
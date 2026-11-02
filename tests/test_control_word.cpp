#include "doctest.h"
#include <array>
#include <bit>
#include "sap1/control_word.hpp"

using namespace sap1;

TEST_CASE("every control signal is exactly one bit") {
    const std::array<ControlWord, 16> all = {
        ctl::HLT, ctl::MI, ctl::RI, ctl::RO, ctl::IO, ctl::II, ctl::AI, ctl::AO,
        ctl::EO,  ctl::SU, ctl::BI, ctl::OI, ctl::CE, ctl::CO, ctl::J,  ctl::FI};
    for (ControlWord w : all) {
        CHECK(std::popcount(w) == 1);
    }
}

TEST_CASE("signals are distinct, fill the word, and sit at the right ends") {
    const ControlWord combined =
        ctl::HLT | ctl::MI | ctl::RI | ctl::RO | ctl::IO | ctl::II | ctl::AI |
        ctl::AO | ctl::EO | ctl::SU | ctl::BI | ctl::OI | ctl::CE | ctl::CO |
        ctl::J | ctl::FI;
    CHECK(combined == 0xFFFF);
    CHECK(ctl::HLT == 0x8000);
    CHECK(ctl::FI == 0x0001);
}

TEST_CASE("BusDrivers contains exactly RO, IO, AO, EO, CO") {
    const ControlWord expected = ctl::RO | ctl::IO | ctl::AO | ctl::EO | ctl::CO;
    CHECK(ctl::kBusDrivers == expected);
    CHECK(std::popcount(ctl::kBusDrivers) == 5);
}
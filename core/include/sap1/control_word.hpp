#pragma once
#include <cstdint>


namespace sap1 {

using ControlWord = std::uint16_t;
namespace ctl{
inline constexpr ControlWord HLT = (1u << 15);
inline constexpr ControlWord MI = (1u << 14);
inline constexpr ControlWord RI = (1u << 13);
inline constexpr ControlWord RO = (1u << 12);
inline constexpr ControlWord IO = (1u << 11);
inline constexpr ControlWord II = (1u << 10);
inline constexpr ControlWord AI = (1u << 9);
inline constexpr ControlWord AO = (1u << 8);
inline constexpr ControlWord EO = (1u << 7);
inline constexpr ControlWord SU = (1u << 6);
inline constexpr ControlWord BI = (1u << 5);
inline constexpr ControlWord OI = (1u << 4);
inline constexpr ControlWord CE = (1u << 3);
inline constexpr ControlWord CO = (1u << 2);
inline constexpr ControlWord J = (1u << 1);
inline constexpr ControlWord FI = (1u << 0);

inline constexpr ControlWord kBusDrivers = (RO | IO | AO | EO | CO);



} // namespace ctl


} // namespace sap1
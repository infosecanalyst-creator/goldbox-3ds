// platform/keys.h - IBM BIOS key codes as coab's engine expects them
// (port of coab Main/Keyboard.cs).  High byte = scan code, low byte = ASCII.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#pragma once

#include <cstdint>

namespace keys {

constexpr uint16_t Enter = 0x1C0D;
constexpr uint16_t Space = 0x0020;
constexpr uint16_t Delete = 0x5300;
constexpr uint16_t Backspace = 0x0008;
constexpr uint16_t Home = 0x4700;  // also numpad 7 / '['  (diagonal up-left)
constexpr uint16_t Up = 0x4800;
constexpr uint16_t PageUp = 0x4900;  // numpad 9 (diagonal up-right)
constexpr uint16_t Left = 0x4B00;
constexpr uint16_t Center = 0x4C00;  // numpad 5
constexpr uint16_t Right = 0x4D00;
constexpr uint16_t End = 0x4F00;  // also numpad 1 / ']' (diagonal down-left)
constexpr uint16_t Down = 0x5000;
constexpr uint16_t PageDown = 0x5100;  // numpad 3 (diagonal down-right)
constexpr uint16_t Minus = 0x2D00;
constexpr uint16_t Escape = 0x001B;
constexpr uint16_t Comma = 0x002C;
constexpr uint16_t Period = 0x002E;

// Letters are sent as upper-case ASCII, digits as ASCII, exactly like coab.
constexpr uint16_t Letter(char c) { return (uint16_t)((c >= 'a' && c <= 'z') ? c - 32 : c); }
constexpr uint16_t Digit(int d) { return (uint16_t)('0' + d); }

}  // namespace keys

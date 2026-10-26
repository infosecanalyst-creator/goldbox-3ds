// Classes/HillsFarPlayer.h - port of Classes/HillsFarPlayer.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class HillsFarPlayer_ {
public:
    static constexpr int StructSize = 0xBC;

    String name; // 0x04 name char[?]
    byte stat_str = 0; // 0x14 str
    byte stat_str00 = 0; // 0x15 str 100
    byte stat_int = 0; // 0x16 int
    byte stat_wis = 0; // 0x17 wis
    byte stat_dex = 0; // 0x18 dex
    byte stat_con = 0; // 0x19 con
    byte stat_cha = 0; // 0x1A cha
    byte alignment = 0; // 0x1C Alignment
    byte field_1D = 0;
    short age = 0; // 0x1E Age
    byte field_20 = 0; // HP current
    byte field_21 = 0; // HP Max
    byte field_23 = 0;
    byte field_26 = 0;
    int field_28 = 0; // money
    byte field_2C = 0; // Sex
    byte field_2D = 0; // Race
    int field_2E = 0; // Exp

    byte field_35 = 0;

    byte field_86 = 0;
    byte field_87 = 0;

    byte field_B7 = 0; // cleric skill
    byte field_B8 = 0; // magic-usr skill
    byte field_B9 = 0; // fighter skill
    byte field_BA = 0; // thief skill

    HillsFarPlayer_();

    HillsFarPlayer_(Array<byte> data);
};

}  // namespace Classes

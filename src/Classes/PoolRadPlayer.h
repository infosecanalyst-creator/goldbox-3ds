// Classes/PoolRadPlayer.h - port of Classes/PoolRadPlayer.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class PoolRadPlayer_ {
public:
    static constexpr int StructSize = 0x011D;

    String name; // 0x0	16
    byte stat_str = 0; // 0x10
    byte stat_int = 0; // 0x11
    byte stat_wis = 0; // 0x12
    byte stat_dex = 0; // 0x13
    byte stat_con = 0; // 0x14
    byte stat_cha = 0; // 0x15
    byte stat_str00 = 0; // 0x16
    sbyte thac0 = 0; // 0x2D
    byte race = 0; // 0x2e
    byte _class = 0; // 0x2F
    short age = 0; // 0x30
    byte hp_max = 0; // 0x32
    Array<byte> field_33; // 0x33 Array 0x38, 0x33 - 0x6A
    byte field_6B = 0; // 0x6B
    byte field_6C = 0; // 0x6C
    Array<byte> saveVerse; // 0x6D Array 5, 0x6D - 0x71
    byte field_72 = 0; // 0x72
    byte field_73 = 0; // 0x73
    byte field_74 = 0; // 0x74
    byte field_75 = 0; // 0x75
    byte field_76 = 0; // 0x76
    Array<byte> field_77; // 0x77 Array 8, 0x77 - 0x7E
    byte field_83 = 0; // 0x83
    byte field_84 = 0; // 0x84
    byte field_85 = 0; // 0x85
    byte field_86 = 0; // 0x86
    byte field_87 = 0; // 0x87

    Array<byte> field_96; // 0x96 Array 8 0x96 - 0x9D
    byte sex = 0; // 0x9E
    byte field_9F = 0; // 0x9F
    byte field_A0 = 0; // 0xA0

    byte field_A1 = 0; // 0xA1
    byte field_A2 = 0; // 0xA2
    byte field_A3 = 0; // 0xA3
    byte field_A4 = 0; // 0xA4
    byte field_A5 = 0; // 0xA5
    byte field_A6 = 0; // 0xA6
    byte field_A7 = 0; // 0xA7
    byte field_A8 = 0; // 0xA8

    byte field_A9 = 0; // 0xA9
    byte field_AA = 0; // 0xAA
    byte field_AB = 0; // 0xAB

    int field_AC = 0; //0xAC
    byte field_B0 = 0; // 0xB0
    byte field_B1 = 0; // 0xB1
    Array<byte> field_B2; // 0xB2 - 3
    Array<byte> field_B5; // 0xB5 - 3

    short field_B8 = 0; // 0xB8

    byte field_BA = 0; // 0xBA
    byte field_BB = 0; // 0xBB
    byte field_BC = 0; // 0xBC
    byte field_BD = 0; // 0xBD
    byte field_BE = 0; // 0xBE
    byte field_C0 = 0; // 0xC0

    Array<byte> field_C1; // 0xC1
    byte field_C7 = 0; // 0xC7

    byte field_100 = 0; // 0x100
    byte field_101 = 0; // 0x101
    short field_102 = 0; // 0x102

    byte field_10C = 0; // 0x10C
    byte field_10D = 0; // 0x10D
    byte field_10E = 0; // 0x10E
    sbyte field_110 = 0; // 0x110

    byte field_111 = 0; // 0x111
    byte field_112 = 0; // 0x112
    byte field_113 = 0; // 0x113
    byte field_114 = 0; // 0x114
    byte field_115 = 0; // 0x115
    byte field_116 = 0; // 0x116
    byte field_117 = 0; // 0x117
    byte field_118 = 0; // 0x118
    byte field_119 = 0; // 0x119
    byte field_11A = 0; // 0x11a
    byte field_11B = 0; // 0x11b
    sbyte field_11C = 0; // 0x11c

    PoolRadPlayer_();

    // C#: PoolRadPlayer(byte[] data) : this()  -> delegating constructor
    PoolRadPlayer_(Array<byte> data);
};

}  // namespace Classes

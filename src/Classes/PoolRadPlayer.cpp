// Classes/PoolRadPlayer.cpp - port of Classes/PoolRadPlayer.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

PoolRadPlayer_::PoolRadPlayer_()
{
    field_33 = Array<byte>(0x38);
    saveVerse = Array<byte>(5);
    field_77 = Array<byte>(8);
    field_96 = Array<byte>(8);
    field_B2 = Array<byte>(3);
    field_B5 = Array<byte>(3);
    field_C1 = Array<byte>(6);
}

PoolRadPlayer_::PoolRadPlayer_(Array<byte> data)
    : PoolRadPlayer_()
{
    name = Sys::ArrayToString(data, 0, 15);

    stat_str = data[0x10]; // 0x10
    stat_int = data[0x11]; // 0x11
    stat_wis = data[0x12]; // 0x12
    stat_dex = data[0x13]; // 0x13
    stat_con = data[0x14]; // 0x14
    stat_cha = data[0x15]; // 0x15
    stat_str00 = data[0x16]; // 0x16
    thac0 = (sbyte)data[0x2D];
    race = data[0x2e];
    _class = data[0x2F];
    age = Sys::ArrayToShort(data, 0x30);
    hp_max = data[0x32];

    Array<byte>::Copy(data, 0x33, field_33, 0, 0x38);

    field_6B = data[0x6B];
    field_6C = data[0x6C];
    Array<byte>::Copy(data, 0x6D, saveVerse, 0, 5);
    field_72 = data[0x72];
    field_73 = data[0x73];
    field_74 = data[0x74];
    field_75 = data[0x75];
    field_76 = data[0x76];
    Array<byte>::Copy(data, 0x77, field_77, 0, 8);
    field_83 = data[0x83];
    field_84 = data[0x84];
    field_85 = data[0x85];
    field_86 = data[0x86];
    field_87 = data[0x87];
    Array<byte>::Copy(data, 0x96, field_96, 0, 8);

    sex = data[0x9E];
    field_9F = data[0x9F];
    field_A0 = data[0xA0];

    field_A1 = data[0xA1];
    field_A2 = data[0xA2];
    field_A3 = data[0xA3];
    field_A4 = data[0xA4];
    field_A5 = data[0xA5];
    field_A6 = data[0xA6];
    field_A7 = data[0xA7];
    field_A8 = data[0xA8];

    field_A9 = data[0xA9];
    field_AA = data[0xAA];
    field_AB = data[0xAB];

    field_AC = Sys::ArrayToInt(data, 0xAC);
    field_B0 = data[0xB0];
    field_B1 = data[0xB1];
    Array<byte>::Copy(data, 0xB2, field_B2, 0, 3);
    Array<byte>::Copy(data, 0xB5, field_B5, 0, 3);

    field_B8 = Sys::ArrayToShort(data, 0xb8);

    field_BA = data[0xBA];
    field_BB = data[0xBB];
    field_BC = data[0xBC];
    field_BD = data[0xBD];
    field_BE = data[0xBE];
    field_C0 = data[0xC0];
    Array<byte>::Copy(data, 0xC1, field_C1, 0, 6);

    field_C7 = data[0xC7];

    field_100 = data[0x100];
    field_101 = data[0x101];
    field_102 = Sys::ArrayToShort(data, 0x102);

    field_10C = data[0x10C];
    field_10D = data[0x10D];
    field_10E = data[0x10E];
    field_110 = (sbyte)data[0x110];

    field_111 = data[0x111];
    field_112 = data[0x112];
    field_113 = data[0x113];
    field_114 = data[0x114];
    field_115 = data[0x115];
    field_116 = data[0x116];
    field_117 = data[0x117];
    field_118 = data[0x118];
    field_119 = data[0x119];
    field_11A = data[0x11a];
    field_11B = data[0x11b];
    field_11C = (sbyte)data[0x11c];
}

}  // namespace Classes

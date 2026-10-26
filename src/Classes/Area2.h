// Classes/Area2.h - port of Classes/Area2.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/DataIO.h"

#include <vector>

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Area2.
/// </summary>
class Area2_ {
    static constexpr int Area2Size = 0x800;

public:
    Area2_();
    Area2_(Array<byte> data, int offset);

    void Clear();

protected:
    Array<byte> origData = Array<byte>(Area2Size);

public:
    ushort field_170 = 0; // [DataOffset(0x170, DataType.Word)] // 0x170

    ushort field_218 = 0; // [DataOffset(0x218, DataType.Word)] // 0x218

    byte training_class_mask = 0; // [DataOffset(0x550, DataType.Byte)] // 0x550
    ushort max_encounter_distance = 0; // [DataOffset(0x580, DataType.Word)] // 0x580
    ushort encounter_distance = 0; // [DataOffset(0x582, DataType.Word)] // 0x582
    ushort field_58C = 0; // [DataOffset(0x58C, DataType.Word)] // 0x58c
    short field_58E = 0; // [DataOffset(0x58E, DataType.SWord)] // 0x58e
    short field_590 = 0; // [DataOffset(0x590, DataType.SWord)] // 0x590
    short field_592 = 0; // [DataOffset(0x592, DataType.SWord)] // 0x592
    ushort search_flags = 0; // [DataOffset(0x594, DataType.Word)] // 0x594 - field_594 bit flags 1 - searching, 2 - looking
    short field_596 = 0; // [DataOffset(0x596, DataType.SWord)] // 0x596
    short rest_incounter_period = 0; // [DataOffset(0x5A4, DataType.SWord)] // 0x5a4
    short rest_incounter_percentage = 0; // [DataOffset(0x5A6, DataType.SWord)] // 0x5a6
    bool tried_to_exit_map = false; // [DataOffset(0x5AA, DataType.Bool)] // 0x5aa
    byte HeadBlockId = 0; // [DataOffset(0x5C2, DataType.Byte)] // 0x5c2
    ushort EnterTemple = 0; // [DataOffset(0x5C4, DataType.Word)] // 0x5c4
    short field_5C6 = 0; // [DataOffset(0x5C6, DataType.SWord)] // 0x5c6
    bool isDuel = false; // NOT a DataIO field (attribute commented out in C#: //[DataOffset(0x5CC, DataType.SWord)]) // 0x5cc field_5CC
    byte game_area = 0; // [DataOffset(0x624, DataType.Byte)] // 0x624
    short field_666 = 0; // [DataOffset(0x666, DataType.SWord)] // 0x666
    byte party_size = 0; // [DataOffset(0x67C, DataType.Byte)] // 0x67c field_67C
    short field_67E = 0; // [DataOffset(0x67E, DataType.SWord)] // 0x67e
    ushort EnterShop = 0; // [DataOffset(0x6D8, DataType.Word)] // 0x6d8
    short field_6DA = 0; // [DataOffset(0x6DA, DataType.SWord)] // 0x6da
    short field_6E0 = 0; // [DataOffset(0x6E0, DataType.SWord)] // 0x6e0
    short field_6E2 = 0; // [DataOffset(0x6E2, DataType.SWord)] // 0x6e2
    short field_6E4 = 0; // [DataOffset(0x6E4, DataType.SWord)] // 0x6e4

    ushort field_6F2 = 0; // [DataOffset(0x6F2, DataType.Word)] // 0x6F2
    ushort field_6F4 = 0; // [DataOffset(0x6F4, DataType.Word)] // 0x6F4
    ushort field_6F6 = 0; // [DataOffset(0x6F6, DataType.Word)] // 0x6F6
    ushort field_6F8 = 0; // [DataOffset(0x6F8, DataType.Word)] // 0x6F8
    ushort field_6FA = 0; // [DataOffset(0x6FA, DataType.Word)] // 0x6FA
    ushort field_6FC = 0; // [DataOffset(0x6FC, DataType.Word)] // 0x6FC
    ushort field_6FE = 0; // [DataOffset(0x6FE, DataType.Word)] // 0x6FE
    ushort field_700 = 0; // [DataOffset(0x700, DataType.Word)] // 0x700
    ushort field_702 = 0; // [DataOffset(0x702, DataType.Word)] // 0x702
    ushort field_704 = 0; // [DataOffset(0x704, DataType.Word)] // 0x704

    byte field_799 = 0; // [DataOffset(0x799, DataType.Byte)] // 0x799
    byte field_79A = 0; // [DataOffset(0x79A, DataType.Byte)] // 0x79a
    byte field_79B = 0; // [DataOffset(0x79B, DataType.Byte)] // 0x79b
    byte field_79C = 0; // [DataOffset(0x79C, DataType.Byte)] // 0x79c
    byte field_79D = 0; // [DataOffset(0x79D, DataType.Byte)] // 0x79d
    byte field_79E = 0; // [DataOffset(0x79E, DataType.Byte)] // 0x79e
    byte field_79F = 0; // [DataOffset(0x79F, DataType.Byte)] // 0x79f
    byte field_7A0 = 0; // [DataOffset(0x7A0, DataType.Byte)] // 0x7a0
    byte field_7A1 = 0; // [DataOffset(0x7A1, DataType.Byte)] // 0x7a1
    byte field_7A2 = 0; // [DataOffset(0x7A2, DataType.Byte)] // 0x7a2
    byte field_7A3 = 0; // [DataOffset(0x7A3, DataType.Byte)] // 0x7a3
    byte field_7A4 = 0; // [DataOffset(0x7A4, DataType.Byte)] // 0x7a4
    byte field_7A5 = 0; // [DataOffset(0x7A5, DataType.Byte)] // 0x7a5
    byte field_7A6 = 0; // [DataOffset(0x7A6, DataType.Byte)] // 0x7a6
    byte field_7A7 = 0; // [DataOffset(0x7A7, DataType.Byte)] // 0x7a7
    byte field_7A8 = 0; // [DataOffset(0x7A8, DataType.Byte)] // 0x7a8
    byte field_7A9 = 0; // [DataOffset(0x7A9, DataType.Byte)] // 0x7a9
    byte field_7AA = 0; // [DataOffset(0x7AA, DataType.Byte)] // 0x7aa
    byte field_7AB = 0; // [DataOffset(0x7AB, DataType.Byte)] // 0x7ab
    ushort field_7EC = 0; // [DataOffset(0x7EC, DataType.Word)]



    Array<byte> ToByteArray();

    ushort field_800_Get(int index);
    void field_800_Set(int index, ushort value);

    void RestField6F2Values();

    // DataIO field table ([DataOffset] fields in declaration order); defined in Area2.cpp.
    static const std::vector<FieldDesc>& DataFields();
};

}  // namespace Classes

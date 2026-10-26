// Classes/Struct_1A35E.h - port of Classes/Struct_1A35E.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/Enums.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class SubStruct_1A35E_ {
public:
    SubStruct_1A35E_(short f0, byte f2, byte f3);

    short base_age = 0;
    byte dice_count = 0;
    byte dice_size = 0;
};

class Struct_1A35E_ {
    // 0x1C long.
    Array<SubStruct_1A35E> field_00;

public:
    Struct_1A35E_();

    Struct_1A35E_(Array<SubStruct_1A35E> values);

    // C# get-only indexers this[int] and this[ClassId]
    SubStruct_1A35E operator[](int i) const { return field_00[i]; }
    SubStruct_1A35E operator[](ClassId i) const { return field_00[(int)i]; }

    /*
    private short field_0;   // seg600:404E unk_1A35E
    private byte field_2;    // seg600:4050 unk_1A360
    private byte field_3;    // seg600:4051 unk_1A361
    private int field_4;     // not used.
    private short field_8;   // seg600:4056 unk_1A366
    private byte field_A;    // seg600:4058 unk_1A368
    private byte field_B;    // seg600:4059 unk_1A369
    private short field_14;  // seg600:4062 unk_1A372
    private byte field_16;   // seg600:4064 unk_1A374
    private byte field_17;   // seg600:4065 unk_1A375
    */
};

}  // namespace Classes

// Classes/ClassStatsMin.h - port of Classes/ClassStatsMin.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Struct_1A484.
/// </summary>
class ClassStatsMin_ {
public:
    ClassStatsMin_(byte v0, byte v1, byte v2, byte v3, byte v4, byte cha);

    byte str_min = 0; // seg600:4174 unk_1A484
    byte int_min = 0; // seg600:4175 unk_1A485
    byte wis_min = 0; // seg600:4176 unk_1A486
    byte dex_min = 0; // seg600:4177 unk_1A487
    byte con_min = 0; // seg600:4178 unk_1A488
    byte cha_min = 0; // field_5 seg600:4179 unk_1A489

    // C# get-only indexer `byte this[int index]` (switch; throws ArgumentOutOfRangeException).
    byte operator[](int index) const;
};

}  // namespace Classes

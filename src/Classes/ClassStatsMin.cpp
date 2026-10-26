// Classes/ClassStatsMin.cpp - port of Classes/ClassStatsMin.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Struct_1A484.
/// </summary>
ClassStatsMin_::ClassStatsMin_(byte v0, byte v1, byte v2, byte v3, byte v4, byte cha)
{
    str_min = v0;
    int_min = v1;
    wis_min = v2;
    dex_min = v3;
    con_min = v4;
    cha_min = cha;
}

byte ClassStatsMin_::operator[](int index) const
{
    switch (index)
    {
        case 0:
            return str_min;

        case 1:
            return int_min;

        case 2:
            return wis_min;

        case 3:
            return dex_min;

        case 4:
            return con_min;

        case 5:
            return cha_min;

        default:
            throw ArgumentOutOfRangeException("index");
    }
}

}  // namespace Classes

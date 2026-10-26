// Classes/Struct_189B4.h - port of Classes/Struct_189B4.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Struct_189B4.
/// </summary>
class Struct_189B4_ {
public:
    byte move_cost = 0; /* field_0 189B4 */
    byte field_1 = 0; /* 189B5 */
    byte field_2 = 0; /* 189B6 */
    byte tile_index = 0; /* 189B7 field_3 */

    Struct_189B4_(byte MoveCost, byte v1, byte v2, byte v3);
};

}  // namespace Classes

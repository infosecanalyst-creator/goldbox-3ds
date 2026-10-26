// Classes/DownedPlayerTile.h - port of Classes/DownedPlayerTile.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/GblTypes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class DownedPlayerTile_ {
public:
    DownedPlayerTile_(); // Struct_1D183

    void Clear();

    Player target; // unk_1D17C field_0
    Point map; // unk_1D180 field_4 was sbyte
    int originalBackgroundTile = 0; // unk_1D182 field_6 was byte
};

}  // namespace Classes

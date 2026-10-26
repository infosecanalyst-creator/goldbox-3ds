// Classes/GasCloud.h - port of Classes/GasCloud.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/GblTypes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class GasCloud_ {
public:
    GasCloud_(Player _player, int count, Point pos);

    Player player; // 0x00;

    Array<int> groundTile; //0x07 base-1 array
    Array<bool> present; // 0x10 base-1 array

    Point targetPos;

    int field_1C = 0;// 0x1C - field_1C
    bool field_1D = false;// 0x1D
};

}  // namespace Classes

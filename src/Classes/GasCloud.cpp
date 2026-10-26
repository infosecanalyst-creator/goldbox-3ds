// Classes/GasCloud.cpp - port of Classes/GasCloud.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

GasCloud_::GasCloud_(Player _player, int count, Point pos)
{
    player = _player;
    field_1C = count;
    targetPos = Point(pos);
    field_1D = false;

    // zero the rest.
    groundTile = Array<int>(10);
    present = Array<bool>(10);
}

}  // namespace Classes

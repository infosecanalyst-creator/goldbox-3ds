// Classes/DownedPlayerTile.cpp - port of Classes/DownedPlayerTile.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

DownedPlayerTile_::DownedPlayerTile_() // Struct_1D183
{
    Clear();
}

void DownedPlayerTile_::Clear()
{
    target = nullptr;
    map.x = 0;
    map.y = 0;
    originalBackgroundTile = 0;
}

}  // namespace Classes

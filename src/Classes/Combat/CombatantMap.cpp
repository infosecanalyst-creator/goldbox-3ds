// Classes/Combat/CombatantMap.cpp - port of Classes/Combat/CombatantMap.cs
#include "Classes/Classes.h"

namespace Classes {
namespace Combat {
using namespace cs;
using namespace cs::enum_ops;

String CombatantMap_::ToString() const
{
    return Format("size: {0} pos: {1},{2}", size, pos.x, pos.y);
}

}  // namespace Combat
}  // namespace Classes

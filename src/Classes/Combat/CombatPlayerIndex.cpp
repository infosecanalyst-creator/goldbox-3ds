// Classes/Combat/CombatPlayerIndex.cpp - port of Classes/Combat/CombatPlayerIndex.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {
namespace Combat {

CombatPlayerIndex_::CombatPlayerIndex_(Player Player, Point Pos)
{
    player = Player;
    pos = Pos;
}

String CombatPlayerIndex_::ToString() const
{
    return Format("{0} pos: {1}", player, pos);
}

}  // namespace Combat
}  // namespace Classes

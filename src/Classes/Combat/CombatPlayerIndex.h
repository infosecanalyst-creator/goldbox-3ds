// Classes/Combat/CombatPlayerIndex.h - port of Classes/Combat/CombatPlayerIndex.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/GblTypes.h"

namespace Classes {
namespace Combat {
using namespace cs;
using namespace cs::enum_ops;

class CombatPlayerIndex_ {
public:
    Player player; // { get; protected set; }
    Point pos;//{ get; protected set; }

    CombatPlayerIndex_(Player Player, Point Pos);

    String ToString() const;
};

}  // namespace Combat
}  // namespace Classes

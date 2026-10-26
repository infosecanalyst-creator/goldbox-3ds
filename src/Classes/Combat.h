// Classes/Combat.h - port of Classes/Combat.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/GblTypes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class SortedCombatant_ /*Struct_1D1C1*/ {
public:
    SortedCombatant_(Player Player, Point Pos, int Steps, int Direction);

    Player player;
    Point pos;

    /// <summary>
    /// steps to counted in 2's, so that diagional steps can be 3 (thus 1.5)
    /// </summary>
    int steps = 0;

private:
    int direction = 0;

public:
    String ToString() const;

    // IComparable<SortedCombatant>.CompareTo (explicit interface implementation)
    int CompareTo(const SortedCombatant& other) const;
};

}  // namespace Classes

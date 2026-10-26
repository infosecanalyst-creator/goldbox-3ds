// Classes/Combat.cpp - port of Classes/Combat.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

SortedCombatant_::SortedCombatant_(Player Player, Point Pos, int Steps, int Direction)
{
    player = Player;
    pos = Pos;
    steps = Steps;
    direction = Direction;
}

String SortedCombatant_::ToString() const
{
    // PORT-NOTE: C# `"index: " + player` calls Player.ToString() (null -> "").
    return String("index: ") + cs::ToString(player) + " pos: " + pos.ToString() + " dir: " + cs::ToString(direction) + " steps: " + cs::ToString(steps);
}

int SortedCombatant_::CompareTo(const SortedCombatant& other) const
{
    if (steps == other->steps)
    {
        if (direction == other->direction)
        {
            return (direction % 2) - (other->direction % 2);
        }
        else
        {
            return direction - other->direction;
        }
    }
    else
    {
        return steps - other->steps;
    }
}

}  // namespace Classes

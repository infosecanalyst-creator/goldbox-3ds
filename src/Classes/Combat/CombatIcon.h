// Classes/Combat/CombatIcon.h - port of Classes/Combat/CombatIcon.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
namespace Combat {
using namespace cs;
using namespace cs::enum_ops;

enum class Icon : int {
    Normal,
    Attack,
};

class CombatIcon_ {
    DaxBlock normal = nullptr;
    DaxBlock attack = nullptr;

    // cheat and cache the flipped copy, so we don't damage the original, then we present this one later.
    DaxBlock normal_f = nullptr;
    DaxBlock attack_f = nullptr;

public:
    void Release();

private:
    static DaxBlock LoadIconHelper(int maskColor, int masked, int block_id, String file_text);

public:
    void LoadIcons(int maskColor, int masked, String file_text, int normal_id, int attack_id);

    void Recolor(bool p, Array<byte> newColors, Array<byte> oldColors);

    DaxBlock GetIcon(Icon iconState, int direction);

    void MergeIcon(CombatIcon combatIcon); // used to blend head ad body icons.

    void DuplicateIcon(bool Recolour, CombatIcon combatIcon, Player player);
};

}  // namespace Combat
}  // namespace Classes

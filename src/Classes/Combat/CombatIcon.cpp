// Classes/Combat/CombatIcon.cpp - port of Classes/Combat/CombatIcon.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {
namespace Combat {

void CombatIcon_::Release()
{
    normal = nullptr;
    normal_f = nullptr;
    attack = nullptr;
    attack_f = nullptr;
}

DaxBlock CombatIcon_::LoadIconHelper(int maskColor, int masked, int block_id, String file_text)
{
    auto data = Classes::DaxFiles::DaxCache::LoadDax(file_text + ".dax", block_id);
    return New<DaxBlock_>(data, masked, maskColor);
}

void CombatIcon_::LoadIcons(int maskColor, int masked, String file_text, int normal_id, int attack_id)
{
    normal = LoadIconHelper(maskColor, masked, normal_id, file_text);
    normal_f = LoadIconHelper(maskColor, masked, normal_id, file_text);
    normal_f->FlipIconLeftToRight();
    attack = LoadIconHelper(maskColor, masked, attack_id, file_text);
    attack_f = LoadIconHelper(maskColor, masked, attack_id, file_text);
    attack_f->FlipIconLeftToRight();
}

void CombatIcon_::Recolor(bool p, Array<byte> newColors, Array<byte> oldColors)
{
    normal->Recolor(p, newColors, oldColors);
    normal_f->Recolor(p, newColors, oldColors);
    attack->Recolor(p, newColors, oldColors);
    attack_f->Recolor(p, newColors, oldColors);
}

DaxBlock CombatIcon_::GetIcon(Icon iconState, int direction)
{
    if (iconState == Icon::Normal)
    {
        return direction > 3 ? normal_f : normal;
    }
    else
    {
        return direction > 3 ? attack_f : attack;
    }
}

void CombatIcon_::MergeIcon(CombatIcon combatIcon) // used to blend head ad body icons.
{
    normal->MergeIcons(combatIcon->normal);
    normal_f->MergeIcons(combatIcon->normal_f);
    attack->MergeIcons(combatIcon->attack);
    attack_f->MergeIcons(combatIcon->attack_f);
}

void CombatIcon_::DuplicateIcon(bool Recolour, CombatIcon combatIcon, Player player)
{
    int bitPerPixel = normal->bpp;

    Array<byte>::Copy(combatIcon->normal->data, normal->data, combatIcon->normal->data.Length());
    Array<byte>::Copy(combatIcon->normal_f->data, normal_f->data, combatIcon->normal_f->data.Length());
    Array<byte>::Copy(combatIcon->attack->data, attack->data, combatIcon->attack->data.Length());
    Array<byte>::Copy(combatIcon->attack_f->data, attack_f->data, combatIcon->attack_f->data.Length());

    if (Recolour)
    {
        Array<byte> newColors = Array<byte>(16);
        Array<byte> oldColors = Array<byte>(16);

        for (byte i = 0; i < 16; i++)
        {
            oldColors[i] = i;
            newColors[i] = i;
        }

        for (byte i = 0; i < 6; i++)
        {
            newColors[gbl::default_icon_colours[i]] = (byte)(player->icon_colours[i] & 0x0F);
            newColors[gbl::default_icon_colours[i] + 8] = (byte)((player->icon_colours[i] & 0xF0) >> 4);
        }

        Recolor(false, newColors, oldColors);
    }
}

}  // namespace Combat
}  // namespace Classes

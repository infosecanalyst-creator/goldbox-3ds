// engine/ovr026.cpp - translation of coab engine/ovr026.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

// Static tables ClericSpellLevels, PaladinSpellLevels, unk_1A758 are defined in decl/ovr026.h.

void ovr026::sub_6A00F(Player player) // sub_6A00F
{
    for (int i = 0; i < 5; i++)
    {
        player->spellCastCount(0, i) = 0;
        player->spellCastCount(1, i) = 0;
        player->spellCastCount(2, i) = 0;
    }

    for (SkillType skill = SkillType::Cleric; skill <= SkillType::Monk; skill++)
    {
        int skillLevel = player->SkillLevel(skill);

        if (skillLevel > 0)
        {
            switch (skill)
            {
                case SkillType::Cleric:
                    player->spellCastCount(0, 0) += 1;

                    for (int PlayerLvl = 0; PlayerLvl <= (skillLevel - 2); PlayerLvl++)
                    {
                        for (int sp_lvl = 0; sp_lvl < 5; sp_lvl++)
                        {
                            player->spellCastCount(0, sp_lvl) += ClericSpellLevels(PlayerLvl, sp_lvl);
                        }
                    }

                    calc_cleric_spells(false, player);

                    // PORT-NOTE: foreach (Spells spell in System.Enum.GetValues(typeof(Spells))) -
                    // the Spells enumerators are exactly 1..100, ascending (GetValues order).
                    for (Spells spell = (Spells)1; spell <= (Spells)100; spell++)
                    {
                        SpellEntry se = gbl::spellCastingTable[(int)spell];

                        int sp_class = (se->spellLevel - 1) / 5;
                        int sp_lvl = (se->spellLevel - 1) % 5;

                        if (se->spellClass == (SpellClass)0 &&
                            player->spellCastCount(sp_class, sp_lvl) > 0 &&
                            spell != Spells::animate_dead)
                        {
                            player->LearnSpell(spell);
                        }
                    }
                    break;

                case SkillType::Paladin:
                    if (skillLevel > 8)
                    {
                        for (int addLvl = 8; addLvl < skillLevel; addLvl++)
                        {
                            for (int spellLvl = 0; spellLvl < 5; spellLvl++)
                            {
                                player->spellCastCount(0, spellLvl) += PaladinSpellLevels(addLvl, spellLvl);
                            }
                        }

                        // PORT-NOTE: Enum.GetValues(typeof(Spells)) -> 1..100 (see above).
                        for (Spells spell = (Spells)1; spell <= (Spells)100; spell++)
                        {
                            SpellEntry se = gbl::spellCastingTable[(int)spell];
                            int sp_class = (se->spellLevel - 1) / 5;
                            int sp_lvl = (se->spellLevel - 1) % 5;

                            if (se->spellClass == (SpellClass)0 &&
                                player->spellCastCount(sp_class, sp_lvl) > 0)
                            {
                                player->LearnSpell(spell);
                            }
                        }
                    }
                    break;

                case SkillType::Ranger:
                    if (skillLevel > 7)
                    {
                        for (int var_3 = 8; var_3 <= skillLevel; var_3++)
                        {
                            for (int sp_lvl = 0; sp_lvl < 3; sp_lvl++)
                            {
                                player->spellCastCount(1, sp_lvl) += unk_1A758(var_3, sp_lvl);
                            }

                            for (int sp_lvl = 3; sp_lvl < 5; sp_lvl++)
                            {
                                player->spellCastCount(2, sp_lvl - 3) += unk_1A758(var_3, sp_lvl);
                            }
                        }

                        // PORT-NOTE: Enum.GetValues(typeof(Spells)) -> 1..100 (see above).
                        for (Spells spell = (Spells)1; spell <= (Spells)100; spell++)
                        {
                            if (gbl::spellCastingTable[(int)spell]->spellClass == SpellClass::Druid)
                            {
                                player->LearnSpell(spell);
                            }
                        }
                    }

                    break;

                case SkillType::MagicUser:
                    player->spellCastCount(2, 0) += 1;

                    for (int lvl = 0; lvl <= (skillLevel - 2); lvl++)
                    {
                        /* unk_1A7C6 = seg600:44B6 */
                        player->spellCastCount(2, 0) += ovr020::MU_spell_lvl_learn(lvl, 0);
                        player->spellCastCount(2, 1) += ovr020::MU_spell_lvl_learn(lvl, 1);
                        player->spellCastCount(2, 2) += ovr020::MU_spell_lvl_learn(lvl, 2);
                        player->spellCastCount(2, 3) += ovr020::MU_spell_lvl_learn(lvl, 3);
                        player->spellCastCount(2, 4) += ovr020::MU_spell_lvl_learn(lvl, 4);
                    }
                    break;

                default: // PORT-NOTE: added to silence -Wswitch; C# has no default (no-op).
                    break;
            }
        }
    }

    for (Item item : player->items)
    {
        if (item->affect_3 == Affects::protect_magic && item->readied)
        {
            for (int sp_lvl = 0; sp_lvl < 3; sp_lvl++)
            {
                player->spellCastCount(2, sp_lvl) *= 2;
            }
        }
    }
}


void ovr026::ReclacClassBonuses(Player player) // sub_6A3C6
{
    player->thac0 = 0;

    for (int _class = 0; _class <= 7; _class++)
    {
        byte class_lvl = player->ClassLevel[_class];

        player->thac0 = Math::Max(ovr018::thac0_table(_class, class_lvl), player->thac0);
        player->HitDice = Math::Max(class_lvl, player->HitDice);
    }

    if (player->fighter_lvl() >= 7 ||
        player->paladin_lvl() >= 7 ||
        player->ranger_lvl() >= 8)
    {
        player->attacksCount = 3;
    }

    sub_6A00F(player);
    reclac_saving_throws(player);

    if (player->thief_lvl() > 0)
    {
        reclac_thief_skills(player);
    }

    player->classFlags = 0;

    for (int skill = 0; skill <= 7; skill++)
    {
        if (player->ClassLevel[skill] > 0 ||
            (player->ClassLevelsOld[skill] > 0 && player->ClassLevelsOld[skill] < player->HitDice))
        {
            player->classFlags += ovr018::unk_1A1B2[skill];
        }
    }

    player->activeItems->UndreadyAll(player->classFlags);


    if (DualClassExceedLastLevel(player) == true)
    {
        for (int class_index = 0; class_index <= 7; class_index++)
        {
            byte skill_lvl = player->ClassLevelsOld[class_index];

            if (class_index == 2 || class_index == 3)
            {
                if (skill_lvl > 6)
                {
                    player->attacksCount = 3;
                }
            }
            else if (class_index == 4)
            {
                if (skill_lvl > 7)
                {
                    player->attacksCount = 3;
                }
            }

            if (ovr018::thac0_table(class_index, skill_lvl) > player->thac0)
            {
                player->thac0 = ovr018::thac0_table(class_index, skill_lvl);
            }
        }

        if (player->fighter_old_lvl() > 6 ||
            player->ranger_old_lvl() > 7 ||
            player->paladin_old_lvl() > 6)
        {
            player->attacksCount = 3;
        }

        if (player->thief_old_lvl() > 0)
        {
            reclac_thief_skills(player);
        }
    }
}


void ovr026::calc_cleric_spells(bool ResetSpellLevels, Player player) /* sub_6A686 */
{
    int clericLvl = player->SkillLevel(SkillType::Cleric);

    if (clericLvl > 0)
    {
        if (ResetSpellLevels == true)
        {
            for (int sp_lvl = 1; sp_lvl < 5; sp_lvl++)
            {
                player->spellCastCount(0, sp_lvl) = 0;
            }

            player->spellCastCount(0, 0) = 1;

            for (int playerLvl = 0; playerLvl <= (clericLvl - 2); playerLvl++)
            {
                for (int spellLvl = 0; spellLvl < 5; spellLvl++)
                {
                    player->spellCastCount(0, spellLvl) += ClericSpellLevels(playerLvl, spellLvl);
                }
            }
        }

        if (player->stats2->Wis.full > 12 && player->spellCastCount(0, 0) > 0)
        {
            player->spellCastCount(0, 0) += 1;
        }

        if (player->stats2->Wis.full > 13 && player->spellCastCount(0, 0) > 0)
        {
            player->spellCastCount(0, 0) += 1;
        }

        if (player->stats2->Wis.full > 14 && player->spellCastCount(0, 1) > 0)
        {
            player->spellCastCount(0, 1) += 1;
        }

        if (player->stats2->Wis.full > 15 && player->spellCastCount(0, 1) > 0)
        {
            player->spellCastCount(0, 1) += 1;
        }

        if (player->stats2->Wis.full > 16 && player->spellCastCount(0, 2) > 0)
        {
            player->spellCastCount(0, 2) += 1;
        }

        if (player->stats2->Wis.full > 17 && player->spellCastCount(0, 3) > 0)
        {
            player->spellCastCount(0, 3) += 1;
        }
    }
}

// Static table SaveThrowValues is defined in decl/ovr026.h.

void ovr026::reclac_saving_throws(Player player) // sub_6A7FB
{
    Item item = player->items.Find([&](const Item& i) { return i->affect_3 == Affects::item_affect_6 && i->readied; });
    bool applyBonus = item != nullptr;

    for (int save = 0; save < 5; save++)
    {
        int _class;

        player->saveVerse[save] = 20;
        for (_class = 0; _class <= 7; _class++)
        {
            if (player->ClassLevel[_class] > 0)
            {
                byte dl = SaveThrowValues(_class, player->ClassLevel[_class], save);

                if (player->saveVerse[save] > dl)
                {
                    player->saveVerse[save] = dl;
                }
            }
        }

        _class = 7; /* Orignal code had a post use test and would exit on 7,
                * but this for loops uses are pre-test increment so fix var_2 */

        if (player->ClassLevel[_class] > player->ClassLevelsOld[_class])
        {
            byte dl = SaveThrowValues(_class, player->ClassLevelsOld[_class], save);

            if (player->saveVerse[save] > dl)
            {
                player->saveVerse[save] = dl;
            }
        }

        if (save == 0)
        {
            SaveVersePoisonBonus(player, applyBonus);
        }
    }
}

void ovr026::SaveVersePoisonBonus(Player player, bool applyBonus)
{
    int poison = (int)SaveVerseType::Poison;

    if (player->race == Race::dwarf ||
        player->race == Race::halfling ||
        applyBonus == true)
    {
        if (player->stats2->Con.full >= 4 && player->stats2->Con.full <= 6)
        {
            player->saveVerse[poison] += 1;
        }
        else if (player->stats2->Con.full >= 7 && player->stats2->Con.full <= 10)
        {
            player->saveVerse[poison] += 2;
        }
        else if (player->stats2->Con.full >= 11 && player->stats2->Con.full <= 13)
        {
            player->saveVerse[poison] += 3;
        }
        else if (player->stats2->Con.full >= 14 && player->stats2->Con.full <= 17)
        {
            player->saveVerse[poison] += 4;
        }
        else if (player->stats2->Con.full == 18)
        {
            player->saveVerse[poison] += 5;
        }
    }

    if (player->stats2->Con.full == 19 || player->stats2->Con.full == 20)
    {
        player->saveVerse[poison] += 1;
    }
    else if (player->stats2->Con.full == 21 || player->stats2->Con.full == 22)
    {
        player->saveVerse[poison] += 2;
    }
    else if (player->stats2->Con.full == 23 || player->stats2->Con.full == 24)
    {
        player->saveVerse[poison] += 3;
    }
    else if (player->stats2->Con.full == 25)
    {
        player->saveVerse[poison] += 4;
    }
}

// Static tables unk_1A230, unk_1A243, base_chance are defined in decl/ovr026.h.

void ovr026::reclac_thief_skills(Player player) // sub_6AAEA
{
    byte var_2 = 0; //Simeon

    auto item_found = player->items.Find([&](const Item& item) { return item->readied && (item->ScrollLearning(3, 2) || item->ScrollLearning(3, 11)); });
    auto var_A = item_found != nullptr && item_found->ScrollLearning(3, 11);
    auto var_B = item_found != nullptr && item_found->ScrollLearning(3, 2);

    int thiefLvl = player->SkillLevel(SkillType::Thief);

    if (thiefLvl < 4 && var_B == true)
    {
        thiefLvl = 4;
        var_B = false;
    }

    int orig_thief_lvl = thiefLvl;

    for (int skill = 1; skill <= 8; skill++)
    {
        if (var_A == true)
        {
            switch (skill)
            {
                case 1:
                    if (thiefLvl < 5)
                    {
                        thiefLvl = 5;
                        var_2 = 0;
                    }
                    else
                    {
                        var_2 = 5;
                    }
                    break;

                case 2:
                    if (thiefLvl < 7)
                    {
                        thiefLvl = 7;
                        var_2 = 0;
                    }
                    else
                    {
                        var_2 = 5;
                    }
                    break;
            }
        }

        if (unk_1A230((int)player->race, skill) < 0 &&
            base_chance(thiefLvl, skill) < (Math::Abs(unk_1A230((int)player->race, skill)) + var_2))
        {
            player->thief_skills[skill - 1] = 0;
        }
        else
        {
            player->thief_skills[skill - 1] = (byte)(var_2 + base_chance(thiefLvl, skill) + unk_1A230((int)player->race, skill));

            if (skill < 6)
            {
                player->thief_skills[skill - 1] += (byte)unk_1A243(player->stats2->Dex.full, skill);
            }

        }

        if (var_B == true)
        {
            player->thief_skills[skill - 1] += 10;
        }

        thiefLvl = orig_thief_lvl;
    }
}


bool ovr026::SecondClassAllowed(ClassId _class, Player player) /* sub_6AD3E */
{
    auto firstClass = HumanCurrentClass_Unknown(player);
    bool var_2 = _class != firstClass;

    int var_3 = 0;
    while (var_3 <= 5 &&
        (gbl::class_stats_min[(int)firstClass][var_3] < 9 || player->stats2[var_3].cur > 14))
    {
        var_3++;
    }

    var_2 = (var_2 == true && var_3 > 5);
    var_3 = 0;

    while (var_3 <= 5 &&
        (gbl::class_stats_min[(int)_class][var_3] < 9 || player->stats2[var_3].cur > 16))
    {
        var_3++;
    }

    var_2 = (var_2 == true && var_3 > 5);

    byte var_4 = 1;

    while (gbl::class_alignments((int)_class, 0) >= var_4 &&
        gbl::class_alignments((int)_class, var_4) != player->alignment)
    {
        var_4++;
    }

    if (var_2 == false ||
        gbl::class_alignments((int)_class, 0) < var_4)
    {
        var_2 = false;
    }
    else
    {
        var_2 = true;
    }

    return var_2;
}


void ovr026::DuelClass(Player player)
{
    List<MenuItem> list = List<MenuItem>();

    list.Add(New<MenuItem_>("Pick New Class", true));

    for (auto _class : gbl::RaceClasses[(int)player->race])
    {
        if (SecondClassAllowed(_class, player) == true)
        {
            list.Add(New<MenuItem_>(ovr020::classString[(int)_class]));
        }
    }

    if (list.Count() == 1)
    {
        seg041::DisplayStatusText(15, 4, player->name + " doesn't qualify.");
        list.Clear();
        return;
    }

    MenuItem list_ptr;
    int index = 1;
    bool show_exit = true;
    bool var_F = true;

    char input_key;

    do
    {
        input_key = ovr027::sl_select_item(list_ptr, index, var_F, show_exit, list,
            0x16, 0x26, 2, 1, gbl::defaultMenuColors, "Select", String::Empty);

        if (input_key == 0)
        {
            return;
        }
    } while (input_key != 'S');

    player->exp = 0;
    player->attacksCount = 2;
    int newClass = 0;

    while (newClass <= 7 && ovr020::classString[newClass] != list_ptr->Text)
    {
        newClass++;
    }

    list.Clear();

    player->ClassLevelsOld[(int)HumanCurrentClass_Unknown(player)] = HumanCurrentClassLevel_Zero(player);

    player->multiclassLevel = player->HitDice;
    player->HitDice = 1;

    player->ClassLevel[(int)HumanCurrentClass_Unknown(player)] = 0;
    player->ClassLevel[newClass] = 1;

    for (int i = 0; i < 5; i++)
    {
        player->spellCastCount(0, i) = 0;
        player->spellCastCount(1, i) = 0;
        player->spellCastCount(2, i) = 0;
    }

    if (newClass == 0)
    {
        player->spellCastCount(0, 0) = 1;
    }
    else if (newClass == 5)
    {
        player->spellCastCount(2, 0) = 1;
        player->LearnSpell(Spells::detect_magic_MU);
        player->LearnSpell(Spells::read_magic);
        player->LearnSpell(Spells::sleep);
    }

    player->_class = (ClassId)newClass;

    seg041::DisplayStatusText(0, 10, player->name + " is now a 1st level " + ovr020::classString[newClass] + ".");

    player->spellList->Clear();

    ReclacClassBonuses(player);
    calc_cleric_spells(true, player);
    reclac_saving_throws(player);
    reclac_thief_skills(player);

    for (auto item : player->items)
    {
        if ((gbl::ItemDataTable[item->type]->classFlags & player->classFlags) == 0 &&
            item->cursed == false)
        {
            item->readied = false;
        }
    }
}


ClassId ovr026::HumanCurrentClass_Unknown(Player player) // getFirstSkill
{
    if (player->race != Race::human)
    {
        return ClassId::unknown;
    }

    for (ClassId index = ClassId::cleric; index <= ClassId::monk; index++)
    {
        if (player->ClassLevel[(int)index] > 0)
        {
            return index;
        }
    }

    return ClassId::unknown;
}

byte ovr026::HumanCurrentClassLevel_Zero(Player player) /* hasAnySkills */
{
    if (player->race != Race::human)
    {
        return 0;
    }

    int loop_var = 0;

    while (loop_var < 7 &&
        player->ClassLevel[loop_var] == 0)
    {
        loop_var++;
    }

    return player->ClassLevel[loop_var];
}


bool ovr026::DualClassExceedLastLevel(Player player) // sub_6B3D1
{
    return HumanCurrentClassLevel_Zero(player) > player->multiclassLevel;
}

}  // namespace engine

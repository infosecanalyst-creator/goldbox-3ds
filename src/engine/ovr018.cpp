// engine/ovr018.cpp - translation of coab engine/ovr018.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

void ovr018::FreePlayer(Player player) // free_player
{
    if (player->actions != nullptr)
    {
        player->actions = nullptr;
    }

    player->items.Clear();
    player->affects.Clear();
}

void ovr018::startGameMenu()
{
    auto gameStateBackup = gbl::game_state;
    gbl::game_state = GameState::StartGameMenu;
    bool reclac_menus = true;

    while (true)
    {
        if (reclac_menus == true)
        {
            seg037::DrawFrame_Outer();
            if (gbl::SelectedPlayer != nullptr)
            {
                ovr025::PartySummary(gbl::SelectedPlayer);
                menuFlags[allow_drop] = true;
                menuFlags[allow_modify] = true;

                if (gbl::area2_ptr->training_class_mask > 0 || Cheats::free_training == true)
                {
                    menuFlags[allow_training] = true;
                    menuFlags[allow_duelclass] = gbl::SelectedPlayer->CanDuelClass();
                }
                else
                {
                    menuFlags[allow_training] = false;
                    menuFlags[allow_duelclass] = false;
                }

                menuFlags[allow_view] = true;
                menuFlags[allow_remove] = true;
                menuFlags[allow_load] = false;
                menuFlags[allow_save] = true;
                menuFlags[allow_begin] = true;
            }
            else
            {
                menuFlags[allow_drop] = false;
                menuFlags[allow_modify] = false;
                menuFlags[allow_training] = false;
                menuFlags[allow_duelclass] = false;
                menuFlags[allow_view] = false;
                menuFlags[allow_remove] = false;
                menuFlags[allow_load] = true;
                menuFlags[allow_save] = false;
                menuFlags[allow_begin] = false;
            }

            int yCol = 0;
            for (int i = 0; i <= 11; i++)
            {
                if (menuFlags[i] == true)
                {
                    seg041::displayString(ToString(menuStrings[i][0]), 0, 15, yCol + 12, 2);

                    String var_111 = seg051::Copy(menuStrings[i].Length(), 1, menuStrings[i]);
                    seg041::displayString(var_111, 0, 10, yCol + 12, 3);
                    yCol++;
                }
            }

            reclac_menus = false;
        }

        bool controlKey;

        char inputkey = ovr027::displayInput(controlKey, false, 1, New<MenuColorSet_>(0, 0, 13), "C D M T H V A R L S B E J", "Choose a function ");

        ovr027::ClearPromptArea();

        if (controlKey == true)
        {
            if (gbl::SelectedPlayer != nullptr && unk_4C13D->MemberOf(inputkey) == true)
            {
                bool previousDuelClassState = gbl::SelectedPlayer->CanDuelClass();

                ovr020::scroll_team_list(inputkey);
                ovr025::PartySummary(gbl::SelectedPlayer);

                previousDuelClassState ^= gbl::SelectedPlayer->CanDuelClass();

                reclac_menus = previousDuelClassState && gbl::area2_ptr->training_class_mask > 0;
            }
        }
        else
        {
            if (unk_4C15D->MemberOf(inputkey) == false)
            {
                gbl::gameSaved = false;
            }

            switch (inputkey)
            {
                case 'C':
                    if (menuFlags[allow_create] == true)
                    {
                        createPlayer();
                    }
                    break;

                case 'D':
                    if (menuFlags[allow_drop] == true)
                    {
                        dropPlayer();
                    }
                    break;
                case 'M':
                    if (menuFlags[allow_modify] == true)
                    {
                        modifyPlayer();
                    }
                    break;
                case 'T':
                    if (menuFlags[allow_training] == true)
                    {
                        train_player();
                    }
                    break;
                case 'H':
                    if (menuFlags[allow_duelclass] == true)
                    {
                        ovr026::DuelClass(gbl::SelectedPlayer);
                    }
                    break;

                case 'V':
                    if (menuFlags[allow_view] == true)
                    {
                        ovr020::viewPlayer();
                    }
                    break;

                case 'A':
                    if (menuFlags[allow_add] == true)
                    {
                        AddPlayer();
                    }
                    break;

                case 'R':
                    if (menuFlags[allow_remove] == true &&
                        gbl::SelectedPlayer != nullptr)
                    {
                        if (gbl::SelectedPlayer->control_morale < Control::NPC_Base)
                        {
                            ovr017::SavePlayer(String::Empty, gbl::SelectedPlayer);
                            gbl::SelectedPlayer = FreeCurrentPlayer(gbl::SelectedPlayer, true, false);
                        }
                        else
                        {
                            dropPlayer();
                        }
                    }
                    break;

                case 'L':
                    if (menuFlags[allow_load] == true)
                    {
                        ovr017::loadGameMenu();
                    }
                    break;

                case 'S':
                    if (menuFlags[allow_save] == true &&
                        gbl::TeamList.Count() > 0)
                    {
                        ovr017::SaveGame();
                    }

                    break;

                case 'B':
                    if (menuFlags[allow_begin] == true)
                    {
                        if ((gbl::TeamList.Count() > 0 && gbl::inDemo == true) ||
                            gbl::area_ptr->field_3FA == 0 || gbl::inDemo == true)
                        {
                            gbl::game_state = gameStateBackup;

                            if (gbl::reload_ecl_and_pictures == false &&
                                gbl::lastDaxBlockId != 0x50)
                            {
                                if (gbl::game_state == GameState::WildernessMap)
                                {
                                    seg037::DrawFrame_WildernessMap();
                                }
                                else
                                {
                                    seg037::draw8x8_03();
                                }
                                ovr025::PartySummary(gbl::SelectedPlayer);
                            }
                            else
                            {
                                if (gbl::area_ptr->LastEclBlockId == 0)
                                {
                                    seg037::draw8x8_03();
                                }
                            }

                            ovr027::ClearPromptArea();
                            gbl::area2_ptr->training_class_mask = 0;

                            return;
                        }
                    }
                    break;

                case 'E':
                    if (menuFlags[allow_exit] == true)
                    {
                        inputkey = ovr027::yes_no(gbl::alertMenuColors, "Quit to DOS ");

                        if (inputkey == 'Y')
                        {
                            if (gbl::TeamList.Count() > 0 &&
                                gbl::gameSaved == false)
                            {

                                inputkey = ovr027::yes_no(gbl::alertMenuColors, "Game not saved.  Quit anyway? ");
                                if (inputkey == 'N')
                                {
                                    ovr017::SaveGame();
                                }
                            }

                            if (inputkey == 'Y')
                            {
                                seg043::print_and_exit();
                            }
                        }
                    }
                    break;
            }

            reclac_menus = true;
        }
    }
}

//static byte[] /*seg600:45B3 */ unk_1A8C3 = { 3, 3, 5, 5, 5, 2, 2, 5 };
//static byte[] /*seg600:45B4 */ unk_1A8C4 = { 6, 6, 4, 4, 4, 4, 6, 4 };

void ovr018::createPlayer()
{
    bool menuRedraw;
    bool showExit;
    byte var_20;
    short var_1E;
    byte var_1B;

    char input_key;
    int index;
    MenuItem selected;

    Player player = New<Player_>();

    for (int i = 0; i < 6; i++)
    {
        player->icon_colours[i] = (byte)(((gbl::default_icon_colours[i] + 8) << 4) + gbl::default_icon_colours[i]);
    }

    player->base_ac = 50;
    player->thac0 = 40;
    player->health_status = Status::okey;
    player->in_combat = true;
    player->field_DE = 1;
    player->mod_id = (byte)seg051::Random(256);
    player->icon_id = 0x0A;

    List<MenuItem> var_C = List<MenuItem>();
    var_C.Add(New<MenuItem_>("Pick Race", true));

    var_C.Add(New<MenuItem_>(String("  ") + ovr020::raceString[1]));
    var_C.Add(New<MenuItem_>(String("  ") + ovr020::raceString[2]));
    var_C.Add(New<MenuItem_>(String("  ") + ovr020::raceString[3]));
    var_C.Add(New<MenuItem_>(String("  ") + ovr020::raceString[4]));
    var_C.Add(New<MenuItem_>(String("  ") + ovr020::raceString[5]));
    var_C.Add(New<MenuItem_>(String("  ") + ovr020::raceString[7]));

    index = 0;
    menuRedraw = true;
    showExit = true;

    do
    {
        input_key = ovr027::sl_select_item(selected, index, menuRedraw, showExit, var_C,
            22, 38, 2, 1, gbl::defaultMenuColors, "Select", String::Empty);

        if (input_key == '\0')
        {
            var_C.Clear();
            return;
        }
    } while (input_key != 'S');

    if (index == 6)
    {
        index++;
    }

    player->race = (Race)index;

    switch (player->race)
    {
        case Race::halfling:
            player->icon_size = 1;
            ovr024::add_affect(false, 0xff, 0, Affects::con_saving_bonus, player);
            break;

        case Race::dwarf:
            player->icon_size = 1;
            ovr024::add_affect(false, 0xff, 0, Affects::con_saving_bonus, player);
            ovr024::add_affect(false, 0xff, 0, Affects::dwarf_vs_orc, player);
            ovr024::add_affect(false, 0xff, 0, Affects::dwarf_and_gnome_vs_giants, player);
            break;

        case Race::gnome:
            player->icon_size = 1;
            ovr024::add_affect(false, 0xff, 0, Affects::con_saving_bonus, player);
            ovr024::add_affect(false, 0xff, 0, Affects::gnome_vs_man_sized_giant, player);
            ovr024::add_affect(false, 0xff, 0, Affects::dwarf_and_gnome_vs_giants, player);
            ovr024::add_affect(false, 0xff, 0, Affects::affect_30, player);
            break;

        case Race::elf:
            player->icon_size = 2;
            ovr024::add_affect(false, 0xff, 0, Affects::elf_resist_sleep, player);

            break;

        case Race::half_elf:
            player->icon_size = 2;
            ovr024::add_affect(false, 0xff, 0, Affects::halfelf_resistance, player);
            break;

        default:
            player->icon_size = 2;
            break;
    }

    /* Gender */

    var_C.Clear();

    var_C.Add(New<MenuItem_>("Pick Gender", true));
    var_C.Add(New<MenuItem_>(String("  ") + ovr020::sexString[0]));
    var_C.Add(New<MenuItem_>(String("  ") + ovr020::sexString[1]));

    index = 1;
    showExit = true;
    menuRedraw = true;

    do
    {
        input_key = ovr027::sl_select_item(selected, index, menuRedraw, showExit, var_C,
            22, 38, 2, 1, gbl::defaultMenuColors, "Select", String::Empty);

        if (input_key == '\0')
        {
            var_C.Clear();
            player = nullptr;
            return;
        }

    } while (input_key != 'S');


    player->sex = (byte)(index - 1);
    var_C.Clear();

    var_C.Add(New<MenuItem_>("Pick Class", true));

    auto ClassList = gbl::RaceClasses[(int)player->race];
    if (player->race != Race::human && Cheats::no_race_class_restrictions)
    {
        ClassList = gbl::RaceClasses[(int)Race::human + 1];
    }

    for (auto _class : ClassList)
    {
        var_C.Add(New<MenuItem_>(String("  ") + ovr020::classString[(int)_class]));
    }

    index = 1;
    showExit = true;
    menuRedraw = true;

    do
    {
        input_key = ovr027::sl_select_item(selected, index, menuRedraw, showExit, var_C,
            22, 38, 2, 1, gbl::defaultMenuColors, "Select", String::Empty);

        if (input_key == '\0')
        {
            var_C.Clear();
            player = nullptr;
            return;
        }
    } while (input_key != 'S');

    player->exp = 25000;
    player->_class = ClassList[index - 1];
    player->HitDice = 1;

    if (player->_class >= ClassId::cleric && player->_class <= ClassId::fighter)
    {
        player->ClassLevel[(int)player->_class] = 1;
    }
    else if (player->_class >= ClassId::magic_user && player->_class <= ClassId::monk)
    {
        player->ClassLevel[(int)player->_class] = 1;
    }
    else if (player->_class == ClassId::paladin)
    {
        player->paladinCuresLeft = 1;
        player->paladin_lvl() = 1;
        ovr024::add_affect(false, 0xff, 0, Affects::protection_from_evil, player);
    }
    else if (player->_class == ClassId::ranger)
    {
        player->ranger_lvl() = 1;
        ovr024::add_affect(false, 0xff, 0, Affects::ranger_vs_giant, player);
    }
    else if (player->_class == ClassId::mc_c_f)
    {
        player->cleric_lvl() = 1;
        player->fighter_lvl() = 1;
        player->exp = 12500;
    }
    else if (player->_class == ClassId::mc_c_f_m)
    {
        player->cleric_lvl() = 1;
        player->fighter_lvl() = 1;
        player->magic_user_lvl() = 1;
        player->exp = 8333;
    }
    else if (player->_class == ClassId::mc_c_r)
    {
        player->cleric_lvl() = 1;
        player->ranger_lvl() = 1;
        ovr024::add_affect(false, 0xff, 0, Affects::ranger_vs_giant, player);
        player->exp = 12500;
    }
    else if (player->_class == ClassId::mc_c_mu)
    {
        player->cleric_lvl() = 1;
        player->magic_user_lvl() = 1;
        player->exp = 12500;
    }
    else if (player->_class == ClassId::mc_c_t)
    {
        player->cleric_lvl() = 1;
        player->thief_lvl() = 1;
        player->exp = 12500;
    }
    else if (player->_class == ClassId::mc_f_mu)
    {
        player->fighter_lvl() = 1;
        player->magic_user_lvl() = 1;
        player->exp = 12500;
    }
    else if (player->_class == ClassId::mc_f_t)
    {
        player->fighter_lvl() = 1;
        player->thief_lvl() = 1;
        player->exp = 12500;
    }
    else if (player->_class == ClassId::mc_f_mu_t)
    {
        player->fighter_lvl() = 1;
        player->magic_user_lvl() = 1;
        player->thief_lvl() = 1;
        player->exp = 8333;
    }
    else if (player->_class == ClassId::mc_mu_t)
    {
        player->magic_user_lvl() = 1;
        player->thief_lvl() = 1;
        player->exp = 8333;
    }

    if (player->thief_lvl() > 0)
    {
        ovr026::reclac_thief_skills(player);
    }

    player->classFlags = 0;
    player->thac0 = 0;

    for (int class_idx = 0; class_idx <= 7; class_idx++)
    {
        if (player->ClassLevel[class_idx] > 0)
        {
            int skill_lvl = player->ClassLevel[class_idx];

            if (thac0_table(class_idx, skill_lvl) > player->thac0)
            {
                player->thac0 = thac0_table(class_idx, skill_lvl);
            }

            player->classFlags += unk_1A1B2[class_idx];
        }
    }

    ovr026::reclac_saving_throws(player);
    var_C.Clear();

    int alignments = gbl::class_alignments((int)player->_class, 0);

    var_C.Add(New<MenuItem_>("Pick Alignment", true));

    for (int i = 1; i <= alignments; i++)
    {
        var_C.Add(New<MenuItem_>(String("  ") + ovr020::alignmentString[gbl::class_alignments((int)player->_class, i)]));
    }

    index = 1;
    showExit = true;
    menuRedraw = true;

    do
    {
        input_key = ovr027::sl_select_item(selected, index, menuRedraw, showExit, var_C,
            22, 38, 2, 1, gbl::defaultMenuColors, "Select", String::Empty);


        if (input_key == '\0')
        {
            var_C.Clear();

            player = nullptr;
            return;
        }
    } while (input_key != 'S');

    player->alignment = gbl::class_alignments((int)player->_class, index);

    var_C.Clear();

    if (player->_class <= ClassId::monk)
    {
        SubStruct_1A35E v5 = gbl::race_ages[(int)player->race][player->_class];

        player->age = (short)(ovr024::roll_dice(v5->dice_size, v5->dice_count) + v5->base_age);
    }
    else
    {
        int race = (int)player->race;

        switch (player->_class)
        {
            case ClassId::mc_c_f:
            case ClassId::mc_c_f_m:
            case ClassId::mc_c_t:
            case ClassId::mc_c_r:
                player->age = (short)(gbl::race_ages[race][0]->base_age + (gbl::race_ages[race][0]->dice_count * gbl::race_ages[race][0]->dice_size));
                break;

            case ClassId::mc_f_mu:
            case ClassId::mc_f_mu_t:
            case ClassId::mc_mu_t:
                player->age = (short)(gbl::race_ages[race][6]->base_age + (gbl::race_ages[race][6]->dice_count * gbl::race_ages[race][6]->dice_size));
                break;

            case ClassId::mc_f_t:
                player->age = (short)(gbl::race_ages[race][2]->base_age + (gbl::race_ages[race][2]->dice_count * gbl::race_ages[race][2]->dice_size));
                break;

            default: // PORT-NOTE: added only to silence -Wswitch; C# has no default (no-op).
                break;
        }
    }

    Player gblPlayerPtrBkup = gbl::SelectedPlayer;
    gbl::SelectedPlayer = player;
    ovr020::playerDisplayFull(player);

    do
    {
        for (int class_idx = 0; class_idx <= 7; class_idx++)
        {
            if (player->ClassLevel[class_idx] > 0)
            {
                player->ClassLevel[class_idx] = 1;
            }
        }

        player->stats2->Str.full = 0;
        player->stats2->Int.full = 0;
        player->stats2->Wis.full = 0;
        player->stats2->Dex.full = 0;
        player->stats2->Con.full = 0;
        player->stats2->Cha.full = 0;
        player->stats2->Str00.full = 0;

        for(int i = 0; i < 6; i++)
        {
            player->stats2->Str.full = Math::Max(player->stats2->Str.full, ovr024::roll_dice(6, 3) + 1);
            player->stats2->Int.full = Math::Max(player->stats2->Int.full, ovr024::roll_dice(6, 3) + 1);
            player->stats2->Wis.full = Math::Max(player->stats2->Wis.full, ovr024::roll_dice(6, 3) + 1);
            player->stats2->Dex.full = Math::Max(player->stats2->Dex.full, ovr024::roll_dice(6, 3) + 1);
            player->stats2->Con.full = Math::Max(player->stats2->Con.full, ovr024::roll_dice(6, 3) + 1);
            player->stats2->Cha.full = Math::Max(player->stats2->Cha.full, ovr024::roll_dice(6, 3) + 1);
        }

        int race = (int)player->race;
        int sex = player->sex;

        for (var_1B = 0; var_1B < 6; var_1B++)
        {
            switch ((Stat)var_1B)
            {
                case Stat::STR:
                    player->stats2->Str.AgeEffects(race, player->age);
                    player->stats2->Str.EnforceRaceSexLimits(race, sex);
                    player->stats2->Str.EnforceClassLimits((int)player->_class);

                    if (player->stats2->Str.full == 18)
                    {
                        if (player->fighter_lvl() > 0 ||
                            player->ranger_lvl() > 0 ||
                            player->paladin_lvl() > 0)
                        {
                            player->stats2->Str00.Load(seg051::Random(100) + 1);
                            player->stats2->Str00.EnforceRaceSexLimits(race, sex);
                        }
                    }
                    break;

                case Stat::INT:
                    player->stats2->Int.AgeEffects(race, player->age);
                    player->stats2->Int.EnforceRaceSexLimits(race, sex);
                    player->stats2->Int.EnforceClassLimits((int)player->_class);
                    break;

                case Stat::WIS:
                    player->stats2->Wis.AgeEffects(race, player->age);
                    player->stats2->Wis.EnforceRaceSexLimits(race, sex);
                    player->stats2->Wis.EnforceClassLimits((int)player->_class);

                    if (player->stats2->Wis.full < 13 &&
                        player->_class >= ClassId::mc_c_f && player->_class <= ClassId::mc_c_t)
                    {
                        // Multi-Class Cleric
                        player->stats2->Wis.full = 13;
                    }
                    break;

                case Stat::DEX:
                    player->stats2->Dex.AgeEffects(race, player->age);
                    player->stats2->Dex.EnforceRaceSexLimits(race, sex);
                    player->stats2->Dex.EnforceClassLimits((int)player->_class);
                    break;

                case Stat::CON:
                    player->stats2->Con.AgeEffects(race, player->age);
                    player->stats2->Con.EnforceRaceSexLimits(race, sex);
                    player->stats2->Con.EnforceClassLimits((int)player->_class);
                    break;

                case Stat::CHA:
                    player->stats2->Cha.AgeEffects(race, player->age);
                    player->stats2->Cha.EnforceRaceSexLimits(race, sex);
                    player->stats2->Cha.EnforceClassLimits((int)player->_class);
                    break;
            }

            ovr020::display_stat(false, var_1B);
        }

        player->hit_point_current = player->hit_point_max;
        player->attacksCount = 2;
        player->attack1_DiceCountBase = 1;
        player->attack1_DiceSizeBase = 2;
        player->field_125 = 1;
        player->base_movement = 12;
        var_20 = 0;

        for (int i = 0; i < 5; i++)
        {
            player->spellCastCount(0, i) = 0;
            player->spellCastCount(1, i) = 0;
            player->spellCastCount(2, i) = 0;
        }

        for (int class_idx = 0; class_idx <= 7; class_idx++)
        {
            if (player->ClassLevel[class_idx] > 0)
            {
                if (class_idx == 0)
                {
                    player->spellCastCount(0, 0) = 1;
                }
                else if (class_idx == 5)
                {
                    player->spellCastCount(2, 0) = 1;
                }

                //var_21 += ovr024.roll_dice(unk_1A8C4[class_idx], unk_1A8C3[class_idx]);
                //TODO this was not used in original code.

                if (class_idx == 0)
                {
                    ovr026::calc_cleric_spells(false, player);

                    // PORT-NOTE: C# `foreach (Spells spell in System.Enum.GetValues(typeof(Spells)))`.
                    // The Spells enumerators are exactly 0x01..0x64 (bless..bestow_curse_MU),
                    // contiguous and in ascending order, so this loop visits the same values.
                    for (int spell_i = (int)Spells::bless; spell_i <= (int)Spells::bestow_curse_MU; spell_i++)
                    {
                        Spells spell = (Spells)spell_i;
                        SpellEntry stru = gbl::spellCastingTable[(int)spell];

                        if (stru->spellClass == (SpellClass)0 && stru->spellLevel == 1)
                        {
                            player->LearnSpell(spell);
                        }
                    }
                }
                else if (class_idx == 5)
                {
                    player->LearnSpell(Spells::detect_magic_MU);
                    player->LearnSpell(Spells::read_magic);
                    player->LearnSpell(Spells::enlarge);
                    player->LearnSpell(Spells::sleep);
                }

                var_20++;
            }
        }

        player->Money->SetCoins(Money::Platinum, 300);
        player->hit_point_rolled = sub_509E0(0xff, player);
        player->hit_point_max = player->hit_point_rolled;

        var_1E = get_con_hp_adj(player);

        if (var_1E < 0)
        {
            if (player->hit_point_max > (Math::Abs(var_1E) + var_20))
            {
                player->hit_point_max = (byte)((player->hit_point_max + var_1E) / var_20);
            }
            else
            {
                player->hit_point_max = 1;
            }
        }
        else
        {
            player->hit_point_max = (byte)((player->hit_point_max + var_1E) / var_20);
        }

        player->hit_point_current = player->hit_point_max;
        player->hit_point_rolled = (byte)(player->hit_point_rolled / var_20);
        byte trainingClassMaskBackup = gbl::area2_ptr->training_class_mask;

        ovr017::SilentTrainPlayer();

        gbl::area2_ptr->training_class_mask = trainingClassMaskBackup;
        bool first_lvl = true;
        String text = String::Empty;

        for (int class_idx = 0; class_idx <= 7; class_idx++)
        {
            if (player->ClassLevel[class_idx] > 0 ||
                (player->ClassLevelsOld[class_idx] < ovr026::HumanCurrentClassLevel_Zero(player) &&
                 player->ClassLevelsOld[class_idx] > 0))
            {
                if (first_lvl == false)
                {
                    text += "/";
                }

                byte b = player->ClassLevelsOld[class_idx];
                b += player->ClassLevel[class_idx];

                text += ToString(b);

                first_lvl = false;
            }
        }

        seg041::displayString(text, 0, 15, 15, 7);
        ovr020::display_player_stats01();
        ovr020::displayMoney();

        input_key = ovr027::yes_no(gbl::defaultMenuColors, "Reroll stats? ");

    } while (input_key != 'N');

    ovr020::playerDisplayFull(player);

    do
    {
        player->name = seg041::getUserInputString(15, 0, 13, "Character name: ");
    } while (player->name.Length() == 0);

    icon_builder();

    //for (var_1B = 0; var_1B <= 5; var_1B++)
    //{
    //    player.stats2[var_1B].cur = player.stats2[var_1B].full;
    //}

    player->stats2->Str00.full = player->stats2->Str00.cur;

    input_key = ovr027::yes_no(gbl::defaultMenuColors, String("Save ") + player->name + "? ");

    if (input_key == 'Y')
    {
        ovr017::SavePlayer(String::Empty, player);
    }

    gbl::SelectedPlayer = gblPlayerPtrBkup;
}


int ovr018::con_bonus(ClassId classId)
{
    int bonus;
    int stat = gbl::SelectedPlayer->stats2->Con.full;

    if (stat == 3)
    {
        bonus = -2;
    }
    else if (stat >= 4 && stat <= 6)
    {
        bonus = -1;
    }
    else if (stat >= 7 && stat <= 14)
    {
        bonus = 0;
    }
    else if (stat == 15)
    {
        bonus = 1;
    }
    else if (stat == 16)
    {
        bonus = 1;
    }
    else if (classId == ClassId::fighter || classId == ClassId::ranger || classId == ClassId::paladin)
    {
        bonus = stat - 14;
    }
    else
    {
        bonus = 2;
    }

    return bonus;
}


void ovr018::dropPlayer()
{
    if (gbl::SelectedPlayer != nullptr)
    {
        Player player = gbl::SelectedPlayer;

        if (ovr027::yes_no(gbl::alertMenuColors, String("Drop ") + player->name + " forever? ") == 'Y' &&
            ovr027::yes_no(gbl::alertMenuColors, "Are you sure? ") == 'Y')
        {
            if (player->in_combat == false)
            {
                ovr025::string_print01(String("You dump ") + player->name + " out back.");
            }
            else
            {
                ovr025::string_print01(player->name + " bids you farewell.");
            }

            ovr017::remove_player_file(player);
            gbl::SelectedPlayer = FreeCurrentPlayer(gbl::SelectedPlayer, true, false);
        }
        else
        {
            ovr025::string_print01(player->name + " breathes a sigh of relief.");
        }
    }

    ovr025::PartySummary(gbl::SelectedPlayer);
}

/// <summary>
/// nested function, has not been fix to be not nested.
/// </summary>
void ovr018::draw_highlight_stat(bool highlighted, byte edited_stat, int name_cursor_pos) /* sub_4E6F2 */
{
    if (edited_stat >= 0 && edited_stat <= 5)
    {
        ovr020::display_stat(highlighted, edited_stat);
    }
    else if (edited_stat == 6)
    {
        ovr025::display_hp(highlighted, 18, 4, gbl::SelectedPlayer);
    }
    else if (edited_stat == 7)
    {
        if (highlighted == true)
        {
            seg041::displaySpaceChar(1, gbl::SelectedPlayer->name.Length() + 1);
            seg041::displayString(gbl::SelectedPlayer->name, 0, 13, 1, 1);

            if (name_cursor_pos > gbl::SelectedPlayer->name.Length() || gbl::SelectedPlayer->name[name_cursor_pos - 1] == ' ')
            {
                seg041::displayString("%", 0, 15, 1, name_cursor_pos);
            }
            else
            {
                seg041::displayString(ToString(gbl::SelectedPlayer->name[name_cursor_pos - 1]), 0, 15, 1, name_cursor_pos);
            }
        }
        else
        {
            seg041::displayString(gbl::SelectedPlayer->name, 0, 10, 1, 1);
        }
    }
}


void ovr018::modifyPlayer()
{
    bool controlkey;
    char inputkey;

    // PORT-NOTE: outer parentheses added to silence -Wparentheses; C# precedence
    // (&& binds tighter than ||) is unchanged.
    if ((Cheats::allow_player_modify == false &&
        (gbl::SelectedPlayer->exp != 0 &&
        gbl::SelectedPlayer->exp != 8333 &&
        gbl::SelectedPlayer->exp != 12500 &&
        gbl::SelectedPlayer->exp != 25000)) ||
        gbl::SelectedPlayer->multiclassLevel != 0)
    {
        seg041::DisplayStatusText(0, 14, gbl::SelectedPlayer->name + " can't be modified.");
        return;
    }

    ovr020::playerDisplayFull(gbl::SelectedPlayer);

    PlayerStats stats_bkup = New<PlayerStats_>();
    stats_bkup->Assign(gbl::SelectedPlayer->stats2);

    byte orig_hp_max = gbl::SelectedPlayer->hit_point_max;

    String nameBackup = gbl::SelectedPlayer->name;

    int name_cursor_pos = 1;
    byte edited_stat = 7;
    draw_highlight_stat(false, edited_stat, name_cursor_pos);
    edited_stat = 0;
    draw_highlight_stat(true, edited_stat, name_cursor_pos);
    Player player = gbl::SelectedPlayer;

    do
    {
        if (edited_stat == 7)
        {
            while (seg049::KEYPRESSED() == false)
            {
                /* empty */
            }

            inputkey = (char)seg043::GetInputKey();

            if (inputkey == 0)
            {
                inputkey = (char)seg043::GetInputKey();
                controlkey = true;
            }
            else
            {
                controlkey = false;
            }

            if (inputkey == 0x1B)
            {
                inputkey = '\0';
            }
        }
        else
        {
            inputkey = ovr027::displayInput(controlkey, false, 1, gbl::defaultMenuColors, "Keep Exit", "Modify: ");
        }

        draw_highlight_stat(false, edited_stat, name_cursor_pos);

        if (controlkey == true)
        {
            switch (inputkey)
            {
                case 'S':
                    if (edited_stat == 7 && gbl::SelectedPlayer->name.Length() > 1)
                    {
                        if (name_cursor_pos == gbl::SelectedPlayer->name.Length())
                        {
                            gbl::SelectedPlayer->name = gbl::SelectedPlayer->name.Substring(0, gbl::SelectedPlayer->name.Length() - 1);
                            name_cursor_pos = (byte)gbl::SelectedPlayer->name.Length();
                        }
                        else
                        {
                            String part_a = gbl::SelectedPlayer->name.Substring(0, name_cursor_pos);
                            String part_b = gbl::SelectedPlayer->name.Substring(name_cursor_pos + 1, gbl::SelectedPlayer->name.Length() - name_cursor_pos);
                            gbl::SelectedPlayer->name = part_a + part_b;
                        }
                    }
                    break;

                case 'O':
                    edited_stat++;

                    if (edited_stat > 7)
                    {
                        edited_stat = 0;
                    }
                    break;

                case 'G':
                    edited_stat -= 1;

                    if (edited_stat == 0xff)
                    {
                        edited_stat = 7;
                    }
                    break;

                case 'K':
                    if (edited_stat < 6)
                    {
                        int stat_var = edited_stat;
                        int race = (int)player->race;
                        int sex = player->sex;

                        player->stats2->Dec(stat_var);

                        switch ((Stat)stat_var)
                        {
                            case Stat::STR:
                                if (player->stats2->Str00.cur > 0)
                                {
                                    player->stats2->Str00.Dec();
                                    player->stats2->Str.Inc();
                                }
                                else
                                {
                                    player->stats2->Str.EnforceRaceSexLimits(race, sex);
                                }
                                player->stats2->Str.EnforceClassLimits((int)player->_class);
                                break;

                            case Stat::INT:
                                player->stats2->Int.EnforceRaceSexLimits(race, sex);
                                player->stats2->Int.EnforceClassLimits((int)player->_class);
                                break;

                            case Stat::WIS:
                                player->stats2->Wis.EnforceRaceSexLimits(race, sex);
                                player->stats2->Wis.EnforceClassLimits((int)player->_class);

                                if (player->spellCastCount(0, 0) > 0)
                                {
                                    player->spellCastCount(0, 0) = 1;
                                }
                                break;

                            case Stat::DEX:
                                player->stats2->Dex.EnforceRaceSexLimits(race, sex);
                                player->stats2->Dex.EnforceClassLimits((int)player->_class);
                                break;

                            case Stat::CON:
                            { // PORT-NOTE: braces added; C++ forbids jumping past `int max_hp`'s initialisation.
                                player->stats2->Con.EnforceRaceSexLimits(race, sex);
                                player->stats2->Con.EnforceClassLimits((int)player->_class);

                                int max_hp = calc_max_hp(gbl::SelectedPlayer);
                                if (max_hp < player->hit_point_max)
                                {
                                    player->hit_point_max = (byte)max_hp;
                                }

                                player->hit_point_current = player->hit_point_max;
                                edited_stat = 6;
                                draw_highlight_stat(false, edited_stat, name_cursor_pos);
                                edited_stat = 4;
                                break;
                            }

                            case Stat::CHA:
                                player->stats2->Cha.EnforceRaceSexLimits(race, sex);
                                player->stats2->Cha.EnforceClassLimits((int)player->_class);
                                break;
                        }
                    }
                    else if (edited_stat == 6)
                    {
                        player->hit_point_max -= 1;

                        if (sub_506BA(gbl::SelectedPlayer) > player->hit_point_max)
                        {
                            player->hit_point_max = (byte)sub_506BA(player);
                        }

                        player->hit_point_current = player->hit_point_max; ;
                    }
                    else
                    {
                        if (name_cursor_pos == 1)
                        {
                            name_cursor_pos = (byte)gbl::SelectedPlayer->name.Length();
                        }
                        else
                        {
                            name_cursor_pos -= 1;
                        }
                    }
                    break;

                case 'M':
                    if (edited_stat < 6)
                    {
                        int stat_var = edited_stat;
                        int race = (int)player->race;
                        int sex = player->sex;

                        player->stats2->Inc(stat_var);
                        switch ((Stat)stat_var)
                        {
                            case Stat::STR:
                                player->stats2->Str.EnforceRaceSexLimits(race, sex);

                                if( player->stats2->Str.full == 18 &&
                                    (player->fighter_lvl() >0 || player->ranger_lvl() > 0 || player->paladin_lvl() > 0) )
                                {
                                    player->stats2->Str00.Inc();
                                    player->stats2->Str00.EnforceRaceSexLimits(race, sex);
                                }
                                else
                                {
                                    player->stats2->Str00.Load(0);
                                }
                                break;

                            case Stat::INT:
                                player->stats2->Int.EnforceRaceSexLimits(race, sex);
                                break;

                            case Stat::WIS:
                                player->stats2->Wis.EnforceRaceSexLimits(race, sex);

                                if (player->spellCastCount(0, 0) > 0)
                                {
                                    player->spellCastCount(0, 0) = 1;
                                }
                                break;

                            case Stat::DEX:
                                player->stats2->Dex.EnforceRaceSexLimits(race, sex);
                                break;

                            case Stat::CON:
                                player->stats2->Con.EnforceRaceSexLimits(race, sex);

                                if (sub_506BA(gbl::SelectedPlayer) > player->hit_point_max)
                                {
                                    player->hit_point_max = (byte)sub_506BA(player);
                                }

                                player->hit_point_current = player->hit_point_max;
                                edited_stat = 6;
                                draw_highlight_stat(false, edited_stat, name_cursor_pos);
                                edited_stat = 4;
                                break;

                            case Stat::CHA:
                                player->stats2->Cha.EnforceRaceSexLimits(race, sex);
                                break;
                        }
                    }
                    else
                    {
                        if (edited_stat == 6)
                        {
                            player->hit_point_max += 1;

                            if (calc_max_hp(gbl::SelectedPlayer) < player->hit_point_max)
                            {
                                player->hit_point_max = (byte)calc_max_hp(gbl::SelectedPlayer);
                            }

                            player->hit_point_current = player->hit_point_max;
                        }
                        else
                        {
                            if (name_cursor_pos == player->name.Length() + 1)
                            {
                                name_cursor_pos = 1;
                            }
                            else
                            {
                                name_cursor_pos++;
                            }
                        }
                    }
                    break;
            }
        }
        else
        {
            if (inputkey == 0x0d)
            {
                edited_stat++;

                if (edited_stat > 7)
                {
                    edited_stat = 0;
                }
            }
            else if (inputkey == 0x08)
            {
                if (name_cursor_pos > 1 && edited_stat > 6)
                {
                    int len = gbl::SelectedPlayer->name.Length();
                    int del = name_cursor_pos - 1;

                    /* delete char from name */
                    String s = String::Empty;
                    if (del > 0)
                    {
                        s = gbl::SelectedPlayer->name.Substring(0, del);
                    }

                    if ((len - del) > 0)
                    {
                        s += gbl::SelectedPlayer->name.Substring(del + 1);
                    }

                    gbl::SelectedPlayer->name = s;

                    if (name_cursor_pos > gbl::SelectedPlayer->name.Length())
                    {
                        name_cursor_pos = (byte)gbl::SelectedPlayer->name.Length();
                    }
                }
            }
            else if (inputkey >= 0x20 && inputkey <= 0x7A)
            {
                if (edited_stat > 6)
                {
                    if (name_cursor_pos <= 15)
                    {
                        String s = String::Empty;
                        int len = player->name.Length();
                        int insert = name_cursor_pos - 1;

                        if (insert > 0)
                        {
                            s = player->name.Substring(0, insert);
                        }
                        s += inputkey;
                        if (len - insert > 0)
                        {
                            s += player->name.Substring(insert + 1);
                        }

                        player->name = s;

                        name_cursor_pos++;
                        if (name_cursor_pos > 15)
                        {
                            name_cursor_pos = 15;
                        }

                        if (name_cursor_pos > player->name.Length())
                        {
                            // PORT-NOTE: result discarded, as in coab (no effect).
                            player->name.PadRight(name_cursor_pos, ' ');
                        }
                        inputkey = '\0';
                    }
                }
                else if (inputkey == 0x45)
                {
                    gbl::SelectedPlayer->stats2->Assign(stats_bkup);

                    gbl::SelectedPlayer->hit_point_max = orig_hp_max;
                    gbl::SelectedPlayer->hit_point_current = gbl::SelectedPlayer->hit_point_max;

                    gbl::SelectedPlayer->name = nameBackup;

                    ovr025::reclac_player_values(gbl::SelectedPlayer);
                    return;
                }
            }
            else if (inputkey == 0)
            {
                gbl::SelectedPlayer->stats2->Assign(stats_bkup);

                gbl::SelectedPlayer->hit_point_max = orig_hp_max;
                gbl::SelectedPlayer->name = nameBackup;

                gbl::SelectedPlayer->hit_point_current = gbl::SelectedPlayer->hit_point_max;
                ovr025::reclac_player_values(gbl::SelectedPlayer);
                return;
            }
        }

        ovr025::reclac_player_values(gbl::SelectedPlayer);
        ovr020::display_player_stats01();

        draw_highlight_stat(true, edited_stat, name_cursor_pos);
    } while (controlkey == true || inputkey != 0x4B);

    ovr026::calc_cleric_spells(true, gbl::SelectedPlayer);

    gbl::SelectedPlayer->npcTreasureShareCount = 1;

    player = gbl::SelectedPlayer;
    orig_hp_max = 0;
    byte hp_count = 0;

    for (int var_33 = 0; var_33 < 8; var_33++)
    {
        if (player->ClassLevel[var_33] > 0)
        {
            if (player->ClassLevel[var_33] < gbl::max_class_hit_dice[var_33])
            {
                if ((ClassId)var_33 == ClassId::ranger)
                {
                    orig_hp_max += (byte)((player->ClassLevel[var_33] + 1) * (con_bonus((ClassId)var_33)));
                }
                else
                {
                    orig_hp_max += (byte)(player->ClassLevel[var_33] * (con_bonus((ClassId)var_33)));
                }
            }
            else
            {
                orig_hp_max += (byte)((gbl::max_class_hit_dice[var_33] - 1) * con_bonus((ClassId)var_33));
            }
            hp_count++;
        }
    }

    orig_hp_max /= hp_count;

    player->hit_point_rolled = (byte)(player->hit_point_max - orig_hp_max);

    //for (int stat_var = 0; stat_var <= 5; stat_var++)
    //{
    //    gbl.SelectedPlayer.stats2[stat_var].cur = gbl.SelectedPlayer.stats2[stat_var].full;
    //}

    //gbl.SelectedPlayer.stats2.Str00.full = gbl.SelectedPlayer.stats2.Str00.cur;
}


void ovr018::AddPlayer()
{
    seg037::draw8x8_clear_area(0x16, 0x26, 1, 1);

    char input_key = ovr027::displayInput(false, 0, gbl::defaultMenuColors, "Curse Pool Hillsfar Exit", "Add from where? ");

    switch (input_key)
    {
        case 'C':
            gbl::import_from = ImportSource::Curse;
            break;

        case 'P':
            gbl::import_from = ImportSource::Pool;
            break;

        case 'H':
            gbl::import_from = ImportSource::Hillsfar;
            break;

        case 'E':
        case '\0':
            return;
    }

    List<MenuItem> strList;
    List<MenuItem> nameList;
    ovr017::BuildLoadablePlayersLists(strList, nameList);

    if (nameList.Count() > 0)
    {
        int pc_count = 0;

        int strList_index = 0;
        MenuItem select_sl;
        bool menuRedraw = true;

        do
        {
            bool showExit = true;
            input_key = ovr027::sl_select_item(select_sl, strList_index, menuRedraw, showExit, nameList,
                22, 38, 2, 1, gbl::defaultMenuColors, "Add", "Add a character: ");

            if ((input_key == 13 || input_key == 'A') &&
                select_sl->Text[0] != '*')
            {
                ovr027::ClearPromptArea();

                Player new_player = New<Player_>();

                MenuItem var_10 = ovr027::getStringListEntry(strList, strList_index);

                ovr017::import_char01(new_player, var_10->Text);

                select_sl->Text = String("* ") + select_sl->Text;
                pc_count = 0;

                if (gbl::TeamList.Count() == 0)
                {
                    gbl::area2_ptr->party_size = 0;
                    ovr017::AssignPlayerIconId(new_player);

                    ovr017::LoadPlayerCombatIcon(true);
                }
                else
                {
                    bool paladin_present = false;
                    String paladins_name = "";
                    bool evil_present = false;
                    int ranger_count = 0;
                    bool found = false;

                    for (Player tmp_player : gbl::TeamList)
                    {
                        if (tmp_player->name == new_player->name &&
                            tmp_player->mod_id == new_player->mod_id)
                        {
                            found = true;
                            break;
                        }

                        if (tmp_player->control_morale < Control::NPC_Base)
                        {
                            pc_count++;
                        }

                        if (tmp_player->ranger_lvl() > 0)
                        {
                            ranger_count++;
                        }

                        if ((tmp_player->alignment + 1) % 3 == 0)
                        {
                            evil_present = true;
                        }

                        if (tmp_player->paladin_lvl() > 0)
                        {
                            paladin_present = true;
                            paladins_name = tmp_player->name;
                        }
                    }

                    if (found == false &&
                        ((new_player->control_morale < Control::NPC_Base && pc_count < 6) ||
                         (new_player->control_morale >= Control::NPC_Base && gbl::area2_ptr->party_size < 8)) &&
                        (new_player->paladin_lvl() == 0 || evil_present == false) &&
                        (new_player->ranger_lvl() == 0 || ranger_count < 3) &&
                        (((new_player->alignment + 1) % 3) != 0 || paladin_present == false))
                    {
                        ovr017::AssignPlayerIconId(new_player);
                        ovr017::LoadPlayerCombatIcon(true);

                        if (new_player->control_morale < Control::NPC_Base)
                        {
                            pc_count++;
                        }
                    }
                    else
                    {
                        select_sl->Text = select_sl->Text.Substring(2);

                        if (new_player->paladin_lvl() > 0 && evil_present == true)
                        {
                            ovr025::string_print01("paladins do not join with evil scum");
                            seg041::GameDelay();
                        }
                        else if (new_player->ranger_lvl() > 0 && ranger_count > 2)
                        {
                            ovr025::string_print01("too many rangers in party");
                        }
                        else if (((new_player->alignment + 1) % 3) == 0 &&
                                paladin_present == true)
                        {
                            ovr025::string_print01(paladins_name + " will tolerate no evil!");
                        }

                        new_player = nullptr; // FreeMem( Player.StructSize, player_ptr1 );
                    }
                }
            }

        } while (input_key != 0x45 && input_key != '\0' && pc_count <= 5 && gbl::area2_ptr->party_size <= 7);

        nameList.Clear();
    }
}


Player ovr018::FreeCurrentPlayer(Player player, bool free_icon, bool leave_party_size) // free_players
{
    int index = gbl::TeamList.IndexOf(player);

    if (index >= 0)
    {
        gbl::TeamList.RemoveAt(index);

        if (free_icon)
        {
            ovr034::ReleaseCombatIcon(player->icon_id);
        }

        if (leave_party_size == false)
        {
            gbl::area2_ptr->party_size--;
        }

        FreePlayer(player);

        index = index > 0 ? index - 1 : 0;
        if (gbl::TeamList.Count() > 0)
        {
            return gbl::TeamList[index];
        }
    }

    return nullptr;
}


void ovr018::drawIconEditorIcons(sbyte titleY, sbyte titleX) /* sub_4FB7C */
{
    seg040::DrawColorBlock(0, 24, 12, titleY * 24, titleX * 3);

    ovr034::draw_combat_icon(25, Icon::Normal, 0, titleY, titleX);
    ovr034::draw_combat_icon(25, Icon::Attack, 0, titleY, titleX + 3);

    ovr034::draw_combat_icon(12, Icon::Normal, 0, titleY, titleX);
    ovr034::draw_combat_icon(12, Icon::Attack, 0, titleY, titleX + 3);

    seg040::DrawOverlay();
}


void ovr018::duplicateCombatIcon(bool recolour, byte destIndex, byte sourceIndex) /* sub_4FC5B */
{
    gbl::combat_icons[destIndex]->DuplicateIcon(recolour, gbl::combat_icons[sourceIndex], gbl::SelectedPlayer);
}

void ovr018::icon_builder()
{
    Player player_ptr2;
    Player player;
    char var_1B = '\0'; /* Simeon */
    byte var_1A = 0; /* Simeon */
    bool second_color = false;
    byte color_index = 0;
    Array<byte> bkup_colours = Array<byte>(6);
    byte var_8;
    byte weaponIcon;
    byte headIcon;
    char inputKey;

    Array<String> iconStrings = {   "",
                                   "Parts 1st-color 2nd-color Size Exit",
                                   "Head Weapon Exit",
                                   "Weapon Body xxxx Shield Arm Leg Exit",
                                   " Keep Exit",
                                   "Next Prev Keep Exit" };

    seg037::DrawFrame_Outer();
    ovr033::Color_0_8_inverse();

    do
    {
        ovr017::LoadPlayerCombatIcon(false);

        player = gbl::SelectedPlayer;

        var_8 = 1;
        Array<byte>::Copy(player->icon_colours, bkup_colours, 6);

        byte bkup_icon_id = player->icon_id;
        player->icon_id = 0x0C;
        ovr017::LoadPlayerCombatIcon(false);
        player->icon_id = bkup_icon_id;

        headIcon = player->head_icon;
        weaponIcon = player->weapon_icon;
        byte bkup_size = player->icon_size;

        duplicateCombatIcon(true, 12, player->icon_id);
        drawIconEditorIcons(2, 1);

        seg041::displayString("old", 0, 15, 6, 8);
        seg041::displayString("ready   action", 0, 15, 10, 3);
        seg041::displayString("new", 0, 15, 12, 8);
        seg041::displayString("ready   action", 0, 15, 16, 3);

        do
        {
            drawIconEditorIcons(4, 1);

            String text;
            if (var_8 == 4)
            {
                if (player->icon_size == 2)
                {
                    text = String("Small") + iconStrings[4];
                }
                else
                {
                    text = String("Large") + iconStrings[4];
                }
            }
            else
            {
                text = iconStrings[var_8];
            }

            bool specialKey;

            inputKey = ovr027::displayInput(specialKey, false, 0, gbl::defaultMenuColors, text, String::Empty);

            if (specialKey == false)
            {
                switch (var_8)
                {
                    case 1:
                        var_1A = 1;

                        switch (inputKey)
                        {
                            case 'P':
                                var_8 = 2;
                                break;

                            case '1':
                                var_8 = 3;
                                second_color = false;

                                iconStrings[3] = "Weapon Body Hair Shield Arm Leg Exit";
                                break;

                            case '2':
                                var_8 = 3;
                                second_color = true;

                                iconStrings[3] = "Weapon Body Face Shield Arm Leg Exit";
                                break;

                            case 'S':
                                var_8 = 4;
                                break;

                            case 'E':
                                var_1A = 0;
                                break;
                        }

                        break;

                    case 2:
                        var_1A = 2;
                        if (unk_4FE94->MemberOf(inputKey) == true)
                        {
                            var_8 = 1;
                        }
                        else
                        {
                            var_1B = inputKey;
                            var_8 = 5;
                        }
                        break;

                    case 3:
                        var_1A = 3;

                        switch (inputKey)
                        {
                            case 'W':
                                color_index = 5;
                                break;

                            case 'B':
                                color_index = 0;
                                break;

                            case 'H':
                                color_index = 3;
                                break;

                            case 'F':
                                color_index = 3;
                                break;

                            case 'S':
                                color_index = 4;
                                break;

                            case 'A':
                                color_index = 1;
                                break;

                            case 'L':
                                color_index = 2;
                                break;

                            default:
                                color_index = 0;
                                break;
                        }

                        if (unk_4FE94->MemberOf(inputKey) == true)
                        {
                            var_8 = 1;
                        }
                        else
                        {
                            var_8 = 5;
                        }
                        break;

                    case 4:
                        switch (inputKey)
                        {
                            case 'L':
                                player->icon_size = 2;
                                ovr017::LoadPlayerCombatIcon(false);
                                break;

                            case 'S':
                                player->icon_size = 1;
                                ovr017::LoadPlayerCombatIcon(false);
                                break;

                            case 'K':
                                bkup_size = player->icon_size;
                                var_8 = 1;
                                inputKey = ' ';
                                break;

                            case 'E':
                                // PORT-NOTE: C# `goto case '\0';` -> fallthrough into the next case.
                                [[fallthrough]];

                            case '\0':
                                player->icon_size = bkup_size;
                                var_8 = 1;
                                inputKey = ' ';
                                break;
                        }

                        ovr017::LoadPlayerCombatIcon(false);
                        break;

                    case 5:
                        if (var_1A == 2)
                        {
                            if (var_1B == 'H')
                            {
                                if (inputKey == 0x50)
                                {
                                    player->head_icon = (byte)Sys::WrapMinMax(player->head_icon - 1, 0, 13);
                                }
                                else if (inputKey == 'N')
                                {
                                    player->head_icon = (byte)Sys::WrapMinMax(player->head_icon + 1, 0, 13);
                                }
                                else if (inputKey == 'K')
                                {
                                    player_ptr2 = gbl::SelectedPlayer;
                                    headIcon = player_ptr2->head_icon;
                                    var_8 = var_1A;
                                    inputKey = ' ';
                                }
                                else if (inputKey == 'E' || inputKey == '\0')
                                {
                                    player->head_icon = headIcon;
                                    var_8 = var_1A;
                                    inputKey = ' ';
                                }

                                ovr017::LoadPlayerCombatIcon(false);
                            }
                            else if (var_1B == 'W')
                            {
                                if (inputKey == 'P')
                                {
                                    if (player->weapon_icon > 0)
                                    {
                                        player->weapon_icon -= 1;
                                    }
                                    else
                                    {
                                        player->weapon_icon = 0x1F;
                                    }
                                }
                                else if (inputKey == 'N')
                                {
                                    if (player->weapon_icon < 0x1F)
                                    {
                                        player->weapon_icon += 1;
                                    }
                                    else
                                    {
                                        player->weapon_icon = 0;
                                    }
                                }
                                else if (inputKey == 'K')
                                {
                                    player_ptr2 = gbl::SelectedPlayer;
                                    weaponIcon = player_ptr2->weapon_icon;
                                    var_8 = var_1A;
                                    inputKey = ' ';
                                }
                                else if (inputKey == 'E' || inputKey == '\0')
                                {
                                    player->weapon_icon = weaponIcon;
                                    var_8 = var_1A;
                                    inputKey = ' ';
                                }

                                ovr017::LoadPlayerCombatIcon(false);
                            }
                        }
                        else if (var_1A == 3)
                        {
                            byte low_color = (byte)(player->icon_colours[color_index] & 0x0F);
                            byte high_color = (byte)((player->icon_colours[color_index] & 0xF0) >> 4);

                            if (inputKey == 'N')
                            {
                                if (second_color == true)
                                {
                                    high_color = (byte)((high_color + 1) % 16);
                                }
                                else
                                {
                                    low_color = (byte)((low_color + 1) % 16);
                                }

                                player->icon_colours[color_index] = (byte)(low_color + (high_color << 4));
                            }
                            else if (inputKey == 'P')
                            {
                                if (second_color == true)
                                {
                                    high_color = (byte)((high_color - 1) & 0x0F);
                                }
                                else
                                {
                                    low_color = (byte)((low_color - 1) & 0x0F);
                                }

                                player->icon_colours[color_index] = (byte)(low_color + (high_color << 4));
                            }
                            else if (inputKey == 'K')
                            {
                                Array<byte>::Copy(player->icon_colours, bkup_colours, 6);
                                var_8 = var_1A;
                                inputKey = ' ';
                            }
                            else if (inputKey == 'E' || inputKey == '\0')
                            {
                                Array<byte>::Copy(bkup_colours, player->icon_colours, 6);
                                var_8 = var_1A;
                                inputKey = ' ';
                            }
                        }
                        break;
                }
            }

            duplicateCombatIcon(true, 12, player->icon_id);

        } while (var_1A != 0 || unk_4FE94->MemberOf(inputKey) == false);

        player->head_icon = headIcon;
        player->weapon_icon = weaponIcon;
        player->icon_size = bkup_size;

        Array<byte>::Copy(bkup_colours, player->icon_colours, 6);

        duplicateCombatIcon(true, 12, player->icon_id);
        duplicateCombatIcon(false, player->icon_id, 12);

        ovr027::ClearPromptArea();
        ovr034::ReleaseCombatIcon(12);

        inputKey = ovr027::yes_no(gbl::defaultMenuColors, "Is this icon ok? ");

    } while (inputKey != 'Y');

    ovr033::Color_0_8_normal();
}

/// <summary> seg600:4281 </summary> (con_hp_adj table is in decl/ovr018.h)

sbyte ovr018::get_con_hp_adj(Player player)
{
    sbyte hp_adj = 0;

    for (int class_index = 0; class_index <= (byte)ClassId::monk; class_index++)
    {
        if (player->ClassLevel[class_index] > 0 &&
            player->ClassLevel[class_index] < gbl::max_class_hit_dice[class_index])
        {
            hp_adj += con_hp_adj[player->stats2->Con.full];

            if (player->_class == ClassId::fighter ||
                player->_class == ClassId::paladin ||
                player->_class == ClassId::ranger)
            {
                int con = player->stats2->Con.full;

                if (con == 17)
                {
                    hp_adj++;
                }
                else if (con == 18)
                {
                    hp_adj += 2;
                }
                else if (con == 19 || con == 20)
                {
                    hp_adj += 3;
                }
                else if (con >= 21 && con <= 23)
                {
                    hp_adj += 4;
                }
                else if (con == 24 || con == 25)
                {
                    hp_adj += 5;
                }
            }

            if (class_index == (byte)ClassId::ranger &&
                player->ClassLevel[class_index] == 1)
            {
                hp_adj *= 2;
            }
        }
    }

    return hp_adj;
}


int ovr018::sub_506BA(Player player)
{
    int class_count = 0;
    int levels_total = 0;

    for (int class_index = 0; class_index <= (int)ClassId::monk; class_index++)
    {
        if (player->ClassLevel[class_index] > 0)
        {
            levels_total += player->ClassLevel[class_index] + hp_calc_table[class_index]->lvl_bonus;
            class_count++;
        }
    }

    int con_adj = get_con_hp_adj(player);

    if (con_adj < 0)
    {
        if (levels_total > (Math::Abs(con_adj) + class_count))
        {
            levels_total = (levels_total + con_adj) / class_count;
        }
        else
        {
            levels_total = 1;
        }
    }
    else
    {
        levels_total = (levels_total + con_adj) / class_count;
    }

    return levels_total;
}

ovr018::hp_calc_::hp_calc_(int _dice, int _lvl, int _base, int _mult) { dice = _dice; lvl_bonus = _lvl; max_base = _base; max_mult = _mult; }

int ovr018::calc_max_hp(Player player) /* sub_50793 */
{
    int class_count = 0;
    int max_hp = 0;

    for (int class_index = 0; class_index <= 7; class_index++)
    {
        if (player->ClassLevel[class_index] > 0)
        {
            hp_calc hpt = hp_calc_table[class_index];

            int var_4 = con_bonus((ClassId)class_index);

            if (player->ClassLevel[class_index] < gbl::max_class_hit_dice[class_index])
            {
                class_count++;
                max_hp += (var_4 + hpt->dice) * (player->ClassLevel[class_index] + hpt->lvl_bonus);
            }
            else
            {
                class_count++;
                int over_count = (player->ClassLevel[class_index] - gbl::max_class_hit_dice[class_index]) + 1;

                max_hp = hpt->max_base + (over_count * hpt->max_mult);
            }
        }
    }

    max_hp /= class_count;

    return max_hp;
}


byte ovr018::sub_509E0(byte arg_0, Player player)
{
    byte var_4 = 0;

    for (int _class = 0; _class <= 7; _class++)
    {
        if (player->ClassLevel[_class] > 0 &&
            (classMasks[_class] & arg_0) != 0)
        {
            if (player->ClassLevel[_class] < gbl::max_class_hit_dice[_class])
            {
                int var_5 = unk_16B2A[_class];

                if (player->ClassLevel[_class] > 1)
                {
                    var_5 = 1;
                }

                byte var_2 = ovr024::roll_dice(unk_16B32[_class], var_5);
                byte var_3 = ovr024::roll_dice(unk_16B32[_class], var_5);

                if (var_3 > var_2)
                {
                    var_2 = var_3;
                }

                var_4 += var_2;
            }
            else
            {
                if (_class == 2 || _class == 3)
                {
                    var_4 = 3;
                }
                else if (_class == 4 || _class == 0 || _class == 6)
                {
                    var_4 = 2;
                }
                else if (_class == 5)
                {
                    var_4 = 1;
                }
            }
        }
    }

    return var_4;
}


void ovr018::train_player()
{
    if (gbl::SelectedPlayer->health_status != Status::okey &&
        Cheats::free_training == false)
    {
        seg041::DisplayStatusText(0, 14, "we only train conscious people");
        return;
    }

    if (gbl::SelectedPlayer->Money->GetGoldWorth() < 1000 &&
        Cheats::free_training == false &&
        gbl::silent_training == false &&
        gbl::gameWon == false)
    {
        seg041::DisplayStatusText(0, 14, "Training costs 1000 gp.");
        return;
    }


    byte classesExpTrainMask = 0;
    byte classesToTrainMask = 0;
    byte class_lvl = 123; /* Simeon */

    byte trainerClassMask = gbl::area2_ptr->training_class_mask;
    Player player = gbl::SelectedPlayer;

    int var_5 = 0;

    for (int _class = 0; _class <= 7; _class++)
    {
        if (player->ClassLevel[_class] > 0)
        {
            classesToTrainMask += classMasks[_class];
            class_lvl = player->ClassLevel[_class];

            if (Limits::RaceClassLimit(class_lvl, player, (ClassId)_class) == false)
            {
                if ((exp_table(_class, class_lvl) > 0) &&
                    (exp_table(_class, class_lvl) <= player->exp ||
                     Cheats::free_training == true))
                {
                    if (Cheats::free_training == true)
                    {
                        int tmpExp = exp_table(_class, class_lvl);
                        if (tmpExp > 0)
                        {
                            if (tmpExp > player->exp)
                            {
                                player->exp = tmpExp;
                            }
                        }
                    }

                    classesExpTrainMask += classMasks[_class];

                    int next_lvl_exp = exp_table(_class, (class_lvl + 1));

                    if (next_lvl_exp > 0)
                    {
                        if (player->exp >= next_lvl_exp &&
                            next_lvl_exp > var_5)
                        {
                            var_5 = next_lvl_exp - 1;
                        }
                    }
                }
            }
        }
    }

    if (gbl::silent_training == false)
    {
        int max_class = 0;
        int max_exp = 0;

        for (int _class = 0; _class <= 7; _class++)
        {
            if ((classMasks[_class] & classesExpTrainMask) != 0)
            {
                if (exp_table(_class, class_lvl) > max_exp)
                {
                    max_exp = exp_table(_class, class_lvl);
                    max_class = _class;
                }
            }
        }


        if (max_exp > 0)
        {
            classesExpTrainMask = classMasks[max_class];
            int var_9 = exp_table(max_class, class_lvl + 1);

            if (var_9 > 0 &&
                player->exp >= var_9 &&
                var_9 > var_5)
            {
                var_5 = var_9 - 1;
            }
        }
    }

    if (var_5 > 0 && gbl::silent_training == false)
    {
        //player_ptr.exp = var_5;
    }

    if ((classesToTrainMask & trainerClassMask) == 0 &&
            gbl::silent_training == false &&
            Cheats::free_training == false)
    {
        seg041::DisplayStatusText(0, 14, "We don't train that class here");
        return;
    }

    if ((classesExpTrainMask & trainerClassMask) == 0)
    {
        if (gbl::silent_training == true)
        {
            gbl::can_train_no_more = true;
        }

        if (gbl::silent_training == false &&
             Cheats::free_training == false)
        {
            seg041::DisplayStatusText(0, 14, "Not Enough Experience");
            return;
        }
    }


    byte actualTrainingClassesMask;
    if (Cheats::free_training == false)
    {
        actualTrainingClassesMask = (byte)(classesExpTrainMask & trainerClassMask);
    }
    else
    {
        actualTrainingClassesMask = classesExpTrainMask;
    }

    bool skipBits = false;
    if (gbl::silent_training == true)
    {
        skipBits = true;
    }

    if (skipBits == false)
    {
        seg037::draw8x8_clear_area(0x16, 0x26, 1, 1);

        int y_offset = 4;

        ovr025::displayPlayerName(false, y_offset, 4, gbl::SelectedPlayer);

        seg041::displayString(" will become:", 0, 10, y_offset, player->name.Length() + 4);

        for (int _class = 0; _class <= 7; _class++)
        {
            if (player->ClassLevel[_class] > 0 &&
                (classMasks[_class] & actualTrainingClassesMask) != 0)
            {
                y_offset++;

                if (y_offset == 5)
                {
                    String text = Format("    a level {0} {1}",
                        player->ClassLevel[_class] + 1, ovr020::classString[_class]);

                    seg041::displayString(text, 0, 10, y_offset, 6);
                }
                else
                {
                    String text = Format("and a level {0} {1}",
                        player->ClassLevel[_class] + 1, ovr020::classString[_class]);

                    seg041::displayString(text, 0, 10, y_offset, 6);
                }
            }
        }
    }

    if (skipBits || ovr027::yes_no(gbl::defaultMenuColors, "Do you wish to train? ") == 'Y')
    {
        if (skipBits == false)
        {
            ovr025::string_print01("Congratulations...");

            if (Cheats::free_training == false &&
                gbl::gameWon == false)
            {
                player->Money->SubtractGoldWorth(1000);
            }
        }

        byte class_count = 0;
        byte oldMagicUserLvl = player->magic_user_lvl();
        byte oldRangeLevel = player->ranger_lvl();
        player->classFlags = 0;

        for (int _class = 0; _class <= 7; _class++)
        {
            if (player->ClassLevel[_class] > 0)
            {
                class_count++;

                if ((classMasks[_class] & actualTrainingClassesMask) != 0)
                {
                    player->ClassLevel[_class] += 1;
                    if (player->lost_lvls > 0)
                    {
                        player->lost_hp -= (byte)(player->lost_hp / player->lost_lvls);
                        player->lost_lvls -= 1;
                    }
                }
            }
        }

        ovr026::ReclacClassBonuses(gbl::SelectedPlayer);

        if (gbl::silent_training == false)
        {
            if (player->magic_user_lvl() > oldMagicUserLvl ||
                player->ranger_lvl() > 8)
            {
                int index = -1;
                byte newSpellId;
                bool var_1D;

                do
                {
                    newSpellId = ovr020::spell_menu2(var_1D, index, SpellSource::Learn, SpellLoc::choose);
                } while (newSpellId <= 0 && var_1D == true);

                if (newSpellId > 0)
                {
                    player->LearnSpell((Spells)newSpellId);
                }
            }
        }

        if (gbl::silent_training == true)
        {
            switch (player->magic_user_lvl())
            {
                case 2:
                    player->LearnSpell(Spells::magic_missile);
                    break;

                case 3:
                    player->LearnSpell(Spells::stinking_cloud);
                    player->LearnSpell(Spells::protect_from_evil_MU);
                    break;

                case 4:
                    player->LearnSpell(Spells::knock);
                    break;

                case 5:
                    player->LearnSpell(Spells::fireball);
                    break;
            }
        }

        if (player->HitDice <= player->multiclassLevel)
        {
            return;
        }

        short var_F = sub_509E0(actualTrainingClassesMask, gbl::SelectedPlayer);

        int max_hp_increase = var_F / class_count;

        if (max_hp_increase == 0)
        {
            max_hp_increase = 1;
        }

        player->hit_point_rolled += (byte)max_hp_increase;

        int var_15 = get_con_hp_adj(gbl::SelectedPlayer);

        max_hp_increase = (var_F + var_15) / class_count;

        if (max_hp_increase < 1)
        {
            max_hp_increase = 1;
        }

        int hp_lost = player->hit_point_max - player->hit_point_current;

        player->hit_point_max += (byte)max_hp_increase;
        player->hit_point_current = (byte)(player->hit_point_max - hp_lost);
    }
}

}  // namespace engine

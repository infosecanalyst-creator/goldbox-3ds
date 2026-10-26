// Classes/Cheats.cpp - port of Classes/Cheats.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

void Cheats::AllowPlayerModifySet(bool value)
{
    allow_player_modify = value;
}

void Cheats::AlwayShowAreaMapSet(bool value)
{
    always_show_areamap = value;
}

void Cheats::AllowGodsInterveneSet(bool value)
{
    allow_gods_intervene = value;
}

void Cheats::DisplayFullItemNamesSet(bool value)
{
    display_full_item_names = value;
}

void Cheats::FreeTrainingSet(bool value)
{
    free_training = value;
}

void Cheats::ImprovedAreaMapSet(bool value)
{
    improved_area_map = value;
}

void Cheats::NoRaceLevelLimits(bool value)
{
    no_race_level_limits = value;
}

void Cheats::NoRaceClassRestrictions(bool value)
{
    no_race_class_restrictions = value;
}

void Cheats::PlayerAlwaysSavesSet(bool value)
{
    player_always_saves = value;
}

void Cheats::SkipCopyProtectionSet(bool value)
{
    skip_copy_protection = value;
}

void Cheats::SkipTitleScreenSet(bool value)
{
    skip_title_screen = value;
}

void Cheats::ViewItemStatsSet(bool value)
{
    view_item_stats = value;
}

void Cheats::SortTreasureSet(bool value)
{
    sort_treasure = value;
}

}  // namespace Classes

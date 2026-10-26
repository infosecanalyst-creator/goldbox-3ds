// Classes/Cheats.h - port of Classes/Cheats.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

struct Cheats {
    inline static bool allow_player_modify = true;
    static void AllowPlayerModifySet(bool value);

    inline static bool always_show_areamap = false;
    static void AlwayShowAreaMapSet(bool value);

    inline static bool allow_gods_intervene = false;
    static void AllowGodsInterveneSet(bool value);

    inline static bool allow_keyboard_exit = false;

    inline static bool display_full_item_names = false;
    static void DisplayFullItemNamesSet(bool value);

    inline static bool free_training = false;
    static void FreeTrainingSet(bool value);

    inline static bool improved_area_map = false;
    static void ImprovedAreaMapSet(bool value);

    inline static bool no_race_level_limits = false;
    static void NoRaceLevelLimits(bool value);

    inline static bool no_race_class_restrictions = false;
    static void NoRaceClassRestrictions(bool value);

    inline static bool player_always_saves = false;
    static void PlayerAlwaysSavesSet(bool value);

    inline static bool skip_copy_protection = true;
    static void SkipCopyProtectionSet(bool value);

    inline static bool skip_title_screen = false;
    static void SkipTitleScreenSet(bool value);

    inline static bool view_item_stats = false;
    static void ViewItemStatsSet(bool value);

    inline static bool sort_treasure = false;
    static void SortTreasureSet(bool value);
};

}  // namespace Classes

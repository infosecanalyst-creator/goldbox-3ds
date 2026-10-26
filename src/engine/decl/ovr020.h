// engine/decl/ovr020.h - declarations for engine/ovr020.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr020.cpp.
// ---------------------------------------------------------------------------
// ovr020.cs
// ---------------------------------------------------------------------------
struct ovr020 {
    inline static Array<String> sexString = { "Male", "Female" };
    inline static Array<String> raceString = { "Monster", "Dwarf", "Elf", "Gnome",
                                         "Half-Elf", "Halfling", "Half-Orc", "Human" };
    inline static Array<String> alignmentString = { "Lawful Good", "Lawful Neutral", "Lawful Evil",
                                              "Neutral Good", "True Neutral", "Neutral Evil",
                                              "Chaotic Good", "Chaotic Neutral", "Chaotic Evil" };
    inline static Array<String> classString = { "Cleric", "Druid", "Fighter", "Paladin", "Ranger",
                                          "Magic-User", "Thief", "Monk", "Cleric/Fighter",
                                          "Cleric/Fighter/Magic-User", "Cleric/Ranger",
                                          "Cleric/Magic-User","Cleric/Thief", "Fighter/Magic-User",
                                          "Fighter/Thief", "Fighter/Magic-User/Thief",
                                          "Magic-User/Thief" };
    inline static Array<String> statShortString = { "STR ", "INT ", "WIS ", "DEX ", "CON ", "CHA " };
    inline static Array<String> statusString = { "Okay", "Animated", "tempgone", "Running",
                                         "Unconscious", "Dying", "Dead", "Stoned",
                                         "Gone" };
    inline static Array<String> moneyString = { "Copper", "Silver", "Electrum", "Gold", "Platinum",
                                        "Gems", "Jewelry" };
    static void playerDisplayFull(Player player);
    static void displayMoney();
    static void display_player_stats01();
    static void display_stat(bool highlighted, int stat_index);
    inline static Set asc_54B50 = New<Set_>(Array<int>{73, 83, 84 });
    inline static Set unk_54B03 = New<Set_>(Array<int>{0, 69 });
    static bool viewPlayer();
    static bool CanSellDropTradeItem(Item item); // sub_54EC1
    static void ItemDisplayStats(Item arg_0); /*sub_550A6*/
    inline static Set unk_554EE = New<Set_>(Array<int>{0, 69});
    static void PlayerItemsMenu(bool& arg_0); /*use_item*/
    inline static const Array2<byte> MU_spell_lvl_learn = {
            {1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {1, 1, 0, 0, 0},
            {1, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 1, 0, 1, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 1},
            {0, 1, 0, 0, 1},
            {0, 0, 1, 1, 1},
            {0, 0, 0, 1, 1}  };
    static void calc_items_effects(bool add_item, Item item); /*sub_55B04*/
    enum class Weld : int {
        Ok = 0,
        WrongClass = 1,
        AlreadyUsingX = 2,
        HandsFull = 3
    };

    static void ready_Item(Item item);
    static void trade_item(Item item);
    static void halve_items(Item item);
    static void join_items(Item item); /*sub_56285*/
    static void UseMagicItem(bool& arg_0, Item item); // sub_56478
    static void ShopSellItem(Item item); // sell_Item
    static void IdentifyItem(bool& arg_0, Item item);
    static void tradeCoin();
    static void drop_coin();
    static bool canCarry(Item item, Player player);
    static void scroll_team_list(char input_key);
    static byte spell_menu2(bool& arg_0, int& index, SpellSource arg_8, SpellLoc spl_location);
    static bool CanCastHeal(Player player); /* sub_575F0 */
    static bool CanCastCureDiseases(Player player); /* sub_57655 */
    static void PaladinHeal(Player player);
    inline static Array<Affects> paladinCureableDiseases = { // unk_16B39
            Affects::helpless,
            Affects::cause_disease_1,
            Affects::weaken,
            Affects::cause_disease_2,
            Affects::hot_fire_shield,
            Affects::affect_39 };
    static void PaladinCureDisease(Player player); /* sub_577EC */
};


// engine/decl/ovr020.h - declarations for engine/ovr020.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr020.cpp.
// ---------------------------------------------------------------------------
// ovr020.cs
// ---------------------------------------------------------------------------
struct ovr020 {
    static Array<String> sexString;
    static Array<String> raceString;
    static Array<String> alignmentString;
    static Array<String> classString;
    static Array<String> statShortString;
    static Array<String> statusString;
    static Array<String> moneyString;
    static void playerDisplayFull(Player player);
    static void displayMoney();
    static void display_player_stats01();
    static void display_stat(bool highlighted, int stat_index);
    static Set asc_54B50;
    static Set unk_54B03;
    static bool viewPlayer();
    static bool CanSellDropTradeItem(Item item); // sub_54EC1
    static void ItemDisplayStats(Item arg_0); /*sub_550A6*/
    static Set unk_554EE;
    static void PlayerItemsMenu(bool& arg_0); /*use_item*/
    static const Array2<byte> MU_spell_lvl_learn;
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
    static Array<Affects> paladinCureableDiseases;
    static void PaladinCureDisease(Player player); /* sub_577EC */
};


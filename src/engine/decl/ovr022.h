// engine/decl/ovr022.h - declarations for engine/ovr022.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr022.cpp.
// ---------------------------------------------------------------------------
// ovr022.cs
// ---------------------------------------------------------------------------
struct ovr022 {
    static int get_max_load(Player player);
    static bool willOverload(int item_weight, Player player);
    static bool willOverload(int& weight, int item_weight, Player player);
    static void addPlayerGold(int item_weight);
    static short AskNumberValue(byte fgColor, String prompt, int maxValue); // sub_592AD
    static void trade_money(int money_slot, short num_coins, Player dest, Player source); /* add_object */
    static void poolMoney();
    static int GetPartyCount(); /* sub_595FF */
    static void share_pooled();
    static void DropCoins(int money_slot, int num_coins, Player player); /* sub_59A19 */
    static void PickupCoins(int money_slot, int num_coins, Player player); /* sub_59AA0 */
    static int GetMoneyIndexFromString(String& displayText, String input); // sub_59BAB
    static void TakePoolMoney(); // takeItems
    static void treasureOnGround(bool& items, bool& money);
    static sbyte randomBonus(); // sub_59FCF
    static Array2<short> preconfiiguredItems; /*seg600:082E unk_16B3E */
    static Item create_item(ItemType item_type); /* sub_5A007 */
    static bool appraiseGemsJewels();
};


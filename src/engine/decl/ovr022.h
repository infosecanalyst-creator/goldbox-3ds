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
    inline static Array2<short> preconfiiguredItems = {
        {0x00B9, 0x00BB, 0x0040, 0x0001, 0x0320, 0x0003, 0x0063, 0x0000},
        {0x00EF, 0x00A7, 0x0040, 0x0001, 0x044C, 0x0001, 0x003b, 0x0000},
        {0x00b9, 0x00a7, 0x0040, 0x0001, 0x0190, 0x0001, 0x0003, 0x0000},
        {0x00AD, 0x00a7, 0x0040, 0x0001, 0x01C2, 0x0001, 0x0030, 0x0000},
        {0x00CE, 0x00A7, 0x0045, 0x0001, 0x2AF8, 0x001E, 0x000F, 0x0000},
        {0x00E2, 0x00A7, 0x0064, 0x000A, 0x3A98, 0x0000, 0x0026, 0x0083},
        {0x009d, 0x00a7, 0x0015, 0x0014, 0x0bb8, 0x0001, 0x0033, 0x0000} }; /*seg600:082E unk_16B3E */
    static Item create_item(ItemType item_type); /* sub_5A007 */
    static bool appraiseGemsJewels();
};


// engine/decl/ovr006.h - declarations for engine/ovr006.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr006.cpp.
// ---------------------------------------------------------------------------
// ovr006.cs
// ---------------------------------------------------------------------------
struct ovr006 {
    static int calc_battle_exp();
    static void addExp(int exp_to_add);
    static Array<Affects> affects_array;
    static void CleanupPlayersStateAfterCombat(); // sub_2D556
    static void displayCombatResults(int exp); /* sub_2DABC */
    static void select_treasure(int& index, Item& selectedItem, char& key); /* sub_2DD2B */
    static void take_items_treasure(); /* sub_2DDFC */
    static void take_treasure(bool& items_present, bool& money_present); /* sub_2DF2E */
    static void distributeCombatTreasure(); /* sub_2E0C3 */
    static void DeallocateNonTeamMemebers(); // sub_2E3C7
    static void distributeNpcTreasure(); /*sub_2E50E*/
    static void AfterCombatExpAndTreasure(); // sub_2E7A2
};


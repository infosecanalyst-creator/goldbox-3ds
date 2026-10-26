// engine/decl/ovr010.h - declarations for engine/ovr010.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr010.cpp.
// ---------------------------------------------------------------------------
// ovr010.cs
// ---------------------------------------------------------------------------
struct ovr010 {
    static void PlayerQuickFight(Player player); // sub_3504B
    static bool turn_undead(Player player);
    static bool ShouldCastSpellX_sub1(int spell_id, Point pos); // sub_352AF
    static bool ShouldCastSpellX(int minPriority, int spellId, Player attacker); // sub_353B1
    static bool sub_354AA(Player player);
    static bool sub_3560B(Player player);
    inline static Array2<int> data_2B8 = {
            {8, 7, 6, 1, 2, 8}, {8, 1, 2, 7, 6, 7}, {7, 1, 8, 6, 2, 1}, {1, 7, 8, 2, 6, 8}, {8, 7, 6, 5, 4, 8},
            {8, 1, 2, 3, 4, 8}, {8, 4, 6, 2, 8, 6}, {6, 4, 0, 8, 0, 6}, {6, 2, 8, 2, 0, 4}, {4, 0, 0, 2, 6, 2},
            {2, 2, 0, 4, 4, 4} /*, {4, 2, 6, 6}*/ }; /* actual from seg600:02BD - seg600:02F8 */
    static bool CanMove(bool& groundClear, int baseDirecction, int dirStep, Player player); // sub_3573B
    static void moralFailureEscape(Player player); // sub_359D1
    inline static int byte_1AB18 = 0; // byte_1AB18
    inline static int byte_1AB19 = 0; // byte_1AB19
    static bool sub_35DB1(Player player);
    static void TryGuarding(Player player); // sub_361F7
    static bool process_input_in_monsters_turn(Player player); /* sub_36269 */
    static bool FleeCheck_001(Player player); // sub_3637F
    static int CalcItemPowerRating(Item item, Player player); // sub_36535
    static void AI_items_selection(Player player); // sub_36673
};


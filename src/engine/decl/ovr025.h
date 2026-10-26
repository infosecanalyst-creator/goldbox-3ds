// engine/decl/ovr025.h - declarations for engine/ovr025.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr025.cpp.
// ---------------------------------------------------------------------------
// ovr025.cs
// ---------------------------------------------------------------------------
struct ovr025 {
    static void CalculateAttackValues(Player player); // sub_66023
    static void CalcArmorWeightEffect(Item item, Player player); // sub_6621E
    static void sub_662A6(byte& output, Array<sbyte>& bonus, Item item, Player player);
    static void calc_movement(Player player); /* sub_663C4 */
    static void ItemDisplayNameBuild(bool display_new_name, bool displayReadied, int yCol, int xCol, Item item); /*id_item*/
    static void PartySummary(Player player);
    static void display_AC(int y_offset, int x_offset, Player player);
    static void display_hp(bool hightlighted, int y_pos, int x_pos, Player player);
    static void CombatDisplayPlayerSummary(Player player); /* hitpoint_ac */
    static void reclac_player_values(Player player); /* sub_66C20 */
    static sbyte DexAcBonus(Player player); /* stat_bonus */
    static int DexReactionAdj(Player player);
    static int player_strength_group(Player player); /* playerStrengh */
    static int strengthHitBonus(Player player);
    static sbyte strengthDamBonus(Player player);
    static int max_encumberance(Player player); /* strength_bonus */
    static void lose_item(Item item, Player player);
    static void string_print01(String text);
    static void DisplayPlayerStatusString(bool clearDisplay, int lineY, String text, Player player); /* sub_67788 */
    static void ClearPlayerTextArea(); /* sub_6786F */
    static void displayPlayerName(bool pural, int y_offset, int x_offset, Player player); /*sub_678A2*/
    static void load_missile_dax(bool flipIcon, byte iconOffset, Icon iconAction, int iconIdx); /* sub_67924 */
    static void load_missile_icons(int iconIdx); /* sub_67A59 */
    static void draw_missile_attack(int delay, int frameCount, Point target, Point attacker); /* sub_67AA4 */
    static void MagicAttackDisplay(String text, bool showMagicStars, Player player); // sub_6818A
    static bool FindAffect(Affect& affectFound, Affects affect_type, Player player);
    static void damage_player(int damage, Player player);
    static void DescribeHealing(Player player); /* sub_684F7 */
    static void CountCombatTeamMembers(); // count_teams
    static List<CombatPlayerIndex> BuildNearTargets(int max_range, Player player); /*near_enermy*/
    static int getTargetRange(Player target, Player attacker); /* sub_68708 */
    static void clear_actions(Player player);
    static void guarding(Player player);
    static int spellMaxTargetCount(int spell_id); /* sub_6886F */
    static void LoadPic(); // load_pic
    static String direction(int dir);
    static void display_map_position_time(); // camping_search
    static void RedrawCombatScreen(); // sub_68DC0
    inline static Set unk_68DFA = New<Set_>(Array<int>{13, 27, 69, 83});
    static void selectAPlayer(Player& player, bool showExit, String prompt);
    static bool item_is_ranged_melee(Item item);
    static bool is_weapon_ranged(Player player); /* offset_above_1 */
    static bool is_weapon_ranged_melee(Player player); /* offset_equals_20 */
    static bool GetCurrentAttackItem(Item& found_item, Player player); // sub_6906C
    static bool bandage(bool applyBandage);
};


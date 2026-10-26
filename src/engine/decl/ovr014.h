// engine/decl/ovr014.h - declarations for engine/ovr014.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr014.cpp.
// ---------------------------------------------------------------------------
// ovr014.cs
// ---------------------------------------------------------------------------
struct ovr014 {
    static void CalculateInitiative(Player player); // sub_3E000
    static int CalcMoves(Player player); // sub_3E124
    static void sub_3E192(int index, Player target, Player attacker);
    enum class AttackType : int {
        Normal = 0,
        Behind = 1,
        Backstab = 2,
        Slay = 3
    };

    static void DisplayAttackMessage(bool attackHits, int attackDamge, int actualDamage, AttackType attack, Player target, Player attacker); /* backstab */
    static void move_step_into_attack(Player target); /* sub_3E65D */
    static void sub_3E748(int direction, Player player);
    static void move_step_away_attack(int direction, Player player); /* sub_3E954 */
    static void flee_battle(Player player);
    static void reclac_attacks(Player player); // sub_3EDD4
    static int ThisRoundActionCount(int halfActionsLeft); // sub_3EF0D
    static bool TrySweepAttack(Player target, Player attacker); // sub_3EF3D
    static bool CanSeeTargetA(Player targetA, Player targetB); //sub_3F143
    inline static Array<sbyte> unk_16679 = { 0,
        17, 17,  0,  0,  1, 17,  0,  0, 32, 32,
        10,  7,  4,  1,  1,  0,  0, -1, -1, -1 }; /*seg600:0369*/
    static void turns_undead(Player player);
    static bool FindLowestE9Target(Player& output, Player player); /* sub_3F433 */
    static bool AttackTarget01(Item item, int arg_8, Player target, Player attacker); // sub_3F4EB
    static void RecalcAttacksReceived(Player target, Player attacker); // sub_3F94D
    static bool AttackTarget(Item rangedWeapon, int attackType, Player target, Player attacker); // sub_3F9DB
    static int RangedDefenseBonus(Player target, Player attacker); /* sub_3FCED */
    static bool find_healing_target(Player& target, Player healer); /* sub_3FDFE */
    inline static Array<Affects> unk_18ADB = { Affects::bless, Affects::snake_charm, Affects::paralyze, Affects::sleep, Affects::helpless }; // seg600:27CB first is filler (off by 1)
    static bool sub_4001C(DownedPlayerTile arg_0, bool canTargetEmptyGround, QuickFight quick_fight, int spellId);
    static bool target(QuickFight quick_fight, int spellId);
    static void spell_menu3(bool& casting_spell, QuickFight quick_fight, int spell_id);
    static bool CanBackStabTarget(Player target, Player attacker); /* sub_408D7 */
    static byte getTargetDirection(Player playerB, Player playerA); /* sub_409BC */
    static void DrawRangedAttack(Item item, Player target, Player attacker); /* sub_40BF1 */
    static void calc_enemy_health_percentage(); /* sub_40E00 */
    static int MaxOppositionMoves(Player player); // sub_40E8F
    static bool can_attack_target(Player target, Player attacker); /* sub_40F1F */
    static char aim_sub_menu(bool showTarget, bool showRange, int maxRange, Player target, Player attacker); /* Aim_menu */
    static bool sub_411D8(DownedPlayerTile arg_0, bool showRange, Player target, Player attacker);
    inline static Set asc_41342 = New<Set_>(Array<int>{0, 69, 84});
    static bool Target(DownedPlayerTile arg_0, bool allowTarget, bool canTargetEmptyGround, bool showRange, int maxRange, Player target, Player player01);
    static Array<SortedCombatant> copy_sorted_players(Player player); /* sub_4188F */
    static Player step_combat_list(bool arg_2, int step, int& list_index, Point& attackerPos, Array<SortedCombatant> sorted_list); /* sub_41932 */
    inline static Set unk_41AE5 = New<Set_>(Array<int>{0, 69});
    inline static Set unk_41B05 = New<Set_>(Array<int>{71, 72, 73, 75, 77, 79, 80, 81});
    static bool aim_menu(DownedPlayerTile arg_0, bool allowTarget, bool canTargetEmptyGround, bool showRange, int maxRange, Player attacker); /* sub_41B25 */
    static bool find_target(bool clear_target, byte arg_2, int max_range, Player player); /* sub_41E44 */
    static void engulfs(Effect arg_0, object param, Player attacker);
    static void LoadMissleIconAndDraw(int icon_id, Player target, Player attacker); //sub_42159
    static bool sub_421C1(bool clear_target, int& range, Player player); // sub_421C1
    static void attack_or_kill(Effect arg_0, object param, Player attacker);
    static void sub_425C6(Effect add_remove, object param, Player player);
    static void AffectOwlbearHugRoundAttack(Effect arg_0, object param, Player player); // sub_426FC
    static void AffectOwlbearHugAttackCheck(Effect arg_0, object param, Player player); // hugs
    static bool god_intervene();
};


// engine/decl/ovr009.h - declarations for engine/ovr009.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr009.cpp.
// ---------------------------------------------------------------------------
// ovr009.cs
// ---------------------------------------------------------------------------
struct ovr009 {
    static void free_combat_stuff(); /* sub_3304B */
    static void MainCombatLoop(); //sub_33100
    static Player FindNextCombatant(); // PORT-NOTE: C# IEnumerable<Player> + yield; see ENGINE_NOTES.md. Returns the next combatant or nullptr. /* sub_331BC */
    static void DoPlayerCombatTurn(Player player); // sub_33281
    static void combat_menu(Player player); /* camp_menu */
    inline static Set unk_33748 = New<Set_>(Array<int>{16, 19, 45, 50, 71, 72, 73, 75, 77, 79, 80, 81});
    inline static Set unk_33768 = New<Set_>(Array<int>{16, 19, 32, 45, 50, 65, 67, 68, 71, 72, 73, 75, 77, 79, 80, 81, 84, 85, 86});
    static void combat_menu(char& arg_0, Player player);
    static bool BattleRoundChecks(); // battle01
    static void sub_33B26(bool& arg_0, char arg_4, Player player);
    static void sub_33F03(bool& arg_0, Player target, Player player);
    static void delay_menu(bool& turnEnded, Player player);
    static void set_gamespeed();
    static void SetPlayerQuickFight(Player player); // sub_3432F
};


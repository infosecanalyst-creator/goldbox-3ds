// engine/decl/ovr021.h - declarations for engine/ovr021.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr021.cpp.
// ---------------------------------------------------------------------------
// ovr021.cs
// ---------------------------------------------------------------------------
struct ovr021 {
    inline static Array<int> timeScales = { 10, 10, 6, 24, 30, 12, 0x100 }; //word_1A13C
    static void CheckAffectsTimingOut(int timeSlot, int timeSteps); // sub_5801E
    static void NormalizeClock(RestTime arg_0); /* sub_58317 */
    static void clock_583C8(); /* sub_583C8 */
    static void step_game_time(int time_slot, int amount); /* sub_583FA */
    static void rest_time_5849F(int time_index, byte arg_2); /* sub_5849F */
    static String format_time(int value); /* sub_5858A */
    static void display_resting_time(int highlight_time); /* sub_58615 */
    inline static Set unk_58731 = New<Set_>(Array<int>{0, 69, 82 });
    static bool resting_time_menu(); /* sub_58751 */
    static void rest_heal(bool show_text); /* reset_heal */
    static int rest_memorize(bool& findNext, Player player);
    static int rest_scribe(bool& findNext, Player player);
    inline static Array<int> spellLaernTimeout = Array<int>(9); // seg600:758D
    static void CheckForSpellLearning(); // sub_58B4D
    static void sub_58C03(int& arg_0);
    static bool resting(bool interactive_resting); /* reseting */
};


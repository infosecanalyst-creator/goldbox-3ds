// engine/decl/ovr016.h - declarations for engine/ovr016.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr016.cpp.
// ---------------------------------------------------------------------------
// ovr016.cs
// ---------------------------------------------------------------------------
struct ovr016 {
    static int sub_44032(Player player);
    static void cancel_memorize(Player player);
    static void cancel_scribes(Player player);
    static void cancel_spells();
    static int HowManySpellsPlayerCanLearn(SpellClass spellClass, int spellLevel); //sub_4428E
    static bool sub_443A0(byte learn_type);
    static void cast_spell();
    static bool BuildMemorizeSpellText(); // sub_445D4
    static bool rest_menu();
    static void memorize_spell();
    static void scribe_spell();
    inline static Dictionary<Affects, String> EffectNameMap;
    static void BuildEffectNameMap();
    static void DisplayMagicEffects();
    static bool magic_menu();
    static void MoveCurrentPlayerUp(); // sub_4558D
    static void MoveCurrentPlayerDown(); // sub_456E5
    inline static Array<String> reorderStrings = { "Select Exit", "Place Exit" }; //seg600_04A6
    inline static Set reorderSet = New<Set_>(Array<int>{13,80,83});
    static void reorder_party();
    static void DropPlayer(); // drop_player
    static void game_speed();
    inline static Set AlterSet = New<Set_>(Array<int>{0, 69});
    static void alter_menu();
    static int CalculateInitialHealing(); // sub_45F22
    static void CalculateHealing(int& healingAvailable, int numCureLight, int numCureSerious, int numCureCritical); // sub_45FDD
    static int TotalHitpointsLost(); /* sub_4608F */
    static void CalculateTimeAndSpellNumbers(int& numCureCritical, int& numCureSerious, int& numCureLight); //sub_460ED
    static void DoTeamHealing(int& healingAvailable); //sub_46280
    static bool FixTeam(); // fix_menu
    inline static Set unk_463F4 = New<Set_>(Array<int>{0, 69});
    static bool MakeCamp(); // make_camp
};


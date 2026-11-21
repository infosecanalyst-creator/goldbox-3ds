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
    static Dictionary<Affects, String> EffectNameMap;
    static void BuildEffectNameMap();
    static void DisplayMagicEffects();
    static bool magic_menu();
    static void MoveCurrentPlayerUp(); // sub_4558D
    static void MoveCurrentPlayerDown(); // sub_456E5
    static Array<String> reorderStrings; //seg600_04A6
    static Set reorderSet;
    static void reorder_party();
    static void DropPlayer(); // drop_player
    static void game_speed();
    static Set AlterSet;
    static void alter_menu();
    static int CalculateInitialHealing(); // sub_45F22
    static void CalculateHealing(int& healingAvailable, int numCureLight, int numCureSerious, int numCureCritical); // sub_45FDD
    static int TotalHitpointsLost(); /* sub_4608F */
    static void CalculateTimeAndSpellNumbers(int& numCureCritical, int& numCureSerious, int& numCureLight); //sub_460ED
    static void DoTeamHealing(int& healingAvailable); //sub_46280
    static bool FixTeam(); // fix_menu
    static Set unk_463F4;
    static bool MakeCamp(); // make_camp
};


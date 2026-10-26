// engine/decl/ovr023.h - declarations for engine/ovr023.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr023.cpp.
// ---------------------------------------------------------------------------
// ovr023.cs
// ---------------------------------------------------------------------------
struct ovr023 {
    inline static Array<String> SpellNames = { /* AffectNames */
                            "",
                            "Bless",
                            "Curse",
                            "Cure Light Wounds",
                            "Cause Light Wounds",
                            "Detect Magic",
                            "Protection from Evil",
                            "Protection from Good",
                            "Resist Cold",
                            "Burning Hands",
                            "Charm Person",
                            "Detect Magic",
                            "Enlarge",
                            "Reduce",
                            "Friends",
                            "Magic Missile",
                            "Protection From Evil",
                            "Protection From Good",
                            "Read Magic",
                            "Shield",
                            "Shocking Grasp",
                            "Sleep",
                            "Find Traps",
                            "Hold Person",
                            "Resist Fire",
                            "Silence, 15' Radius",
                            "Slow Poison",
                            "Snake Charm",
                            "Spiritual Hammer",
                            "Detect Invisibility",
                            "Invisibility",
                            "Knock",
                            "Mirror Image",
                            "Ray of Enfeeblement",
                            "Stinking Cloud",
                            "Strength",
                            "Animate Dead",
                            "Cure Blindness",
                            "Cause Blindness",
                            "Cure Disease",
                            "Cause Disease",
                            "Dispel Magic",
                            "Prayer",
                            "Remove Curse",
                            "Bestow Curse",
                            "Blink",
                            "Dispel Magic",
                            "Fireball",
                            "Haste",
                            "Hold Person",
                            "Invisibility, 10' Radius",
                            "Lightning Bolt",
                            "Protection From Evil, 10' Radius",
                            "Protection From Good, 10' Radius",
                            "Protection From Normal Missiles",
                            "Slow",
                            "Restoration",
                            "",
                            "Cure Serious Wounds",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "Cause Serious Wounds",
                            "Neutralize Poison",
                            "Poison",
                            "Protection Evil, 10' Radius",
                            "Sticks to Snakes",
                            "Cure Critical Wounds",
                            "Cause Critical Wounds",
                            "Dispel Evil",
                            "Flame Strike",
                            "Raise Dead",
                            "Slay Living",
                            "Detect Magic",
                            "Entangle",
                            "Faerie Fire",
                            "Invisibility to Animals",
                            "Charm Monsters",
                            "Confusion",
                            "Dimension Door",
                            "Fear",
                            "Fire Shield",
                            "Fumble",
                            "Ice Storm",
                            "Minor Globe Of Invulnerability",
                            "Remove Curse",
                            "Animate Dead",
                            "Cloud Kill",
                            "Cone of Cold",
                            "Feeblemind",
                            "Hold Monsters",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "Bestow Curse" };
    inline static Array<String> LevelStrings = {
                            "",
                            "1st Level",
                            "2nd Level",
                            "3rd Level",
                            "4th Level",
                            "5th Level",
                            "6th Level",
                            "7th Level",
                            "8th Level",
                            "9th Level"
                        };
    static bool can_learn_spell(int spell_id, Player player); /* sub_5C01E */
    inline static Set unk_5C1A2 = New<Set_>(Array<int>{1,2,3,4});
    inline static Set asc_5C1D1 = New<Set_>(Array<int>{0, 67, 69, 76, 77, 83});
    inline static Set unk_5C1F1 = New<Set_>(Array<int>{0, 69});
    static byte spell_menu(int& index, SpellSource spellSource);
    static void add_spell_to_list(byte spell_id); /* sub_5C3ED */
    static void add_spell_to_learning_list(int spell_id); /* sub_5C5B9 */
    static void scroll_5C912(bool learning); /* sub_5C912 */
    static void BuildScrollSpellLists(bool showLearning); // sub_5C9F4
    static bool BuildSpellList(SpellLoc spl_location); // sub_5CA74
    static int SpellRange(int spellId); // sub_5CDE5
    static ushort GetSpellAffectTimeout(Spells spellId); // sub_5CE92
    static void DoSpellCastingWork(String text, DamageType damageFlags, int damage, bool call_affect_table, int TargetCount, int spell_id); // sub_5CF7F
    static bool NonCombatSpellCast(QuickFight quick_fight, int spellId); // cast_spell_on
    static void sub_5D2E1(bool showCastingText, QuickFight quick_fight, int spell_id); // sub_5D2E1
    static void sub_5D2E1(bool& arg_0, bool showCastingText, QuickFight quick_fight, int spell_id); // sub_5D2E1
    static void localSteppingPathInit(Point target, Point caster, SteppingPath path); /* sub_5D676 */
    static int find_players_on_path(SteppingPath path, List<int> player_list); /* sub_5D702 */
    inline static Array<Point> unk_16D22 = { Point(-1, 0), Point(0, -1), Point(0, -1), Point(1, 0), Point(1, 0), Point(0, 1), Point(0, 1), Point(-1, 0) };
    inline static Array<Point> unk_16D32 = { Point(1, 0), Point(1, 0), Point(0, 1), Point(0, 1), Point(-1, 0), Point(-1, 0), Point(0, -1), Point(0, -1) };
    static void BuildAreaDamageTargets(int max_range, int playerSize, Point targetPos, Point casterPos); // sub_5D7CF
    static void MultiTargetedSpell(String text, int save_bonus); // sub_5DB24
    static void CastTeamSpell(String text, CombatTeam team); // sub_5DCA0
    static void cleric_bless(); /* is_Blessed */
    static void cleric_curse(); /* is_Cursed */
    static void SpellCureLight(); /* sub_5DDBC */
    static void SpellCauseLight(); /* sub_5DDF8 */
    static void is_affected();
    static void SpellProtectionFromX(); // is_protected
    static void SpellResistCold(); // is_cold_resistant
    static void SpellBuringHands(); // sub_5DEE1
    static void SpellCharm(); // is_charmed
    static void SpellEnlarge(); // is_stronger
    static void SpellReduce(); // has_been_reduced
    static void SpellFriends(); // is_friendly
    static void SpellMagicMissile(); // sub_5E221
    static void SpellShield(); // is_shielded
    static void SpellShockingGrasp(); // sub_5E2B2
    static void SpellSleep(); // falls_asleep
    static int CalcSleepCost(Player target);
    static void SpellHoldX(); // is_held
    static void SpellFireResistant(); // is_fire_resistant
    static void SpellSilence15Radius(); // is_silenced
    static void is_affected2();
    static void SpellSnakeCharm(); // is_charmed2
    static void SpellSpiritualHammer(); // sub_5E681
    static void is_invisible();
    static void SpellKnock();
    static void SpellMirrorImage(); // is_duplicated
    static void SpellRayOfEnfeeblement(); // is_weakened
    static constexpr int StinkingCloudMaxTargets = 4;
    static void SpellStinkingCloud(); //TODO similar to spell_poisonous_cloud
    static void SpellStrength(); // sub_5EC5B
    static void SpellAnimateDead(); // is_animated
    static void SpellCureBlindness(); // can_see
    static void SpellCauseBlindness(); // is_blind
    static bool sub_5F037();
    static void SpellCureDisease(); // sub_5F0DC
    static void SpellCauseDisease(); // is_diseased
    static bool sub_5F126(Player arg_2, int target_count); // sub_5F126
    static void SpellDispelMagic(); // is_affected3
    static void SpellPrayer(); // is_praying
    static void SpellRemoveCurse(); // uncurse
    static void curse();
    static void spell_blinking();
    static void sub_5F782();
    static void RemoveComplimentSpellFirst(String text, CombatTeam combatTeam, Affects affect); //sub_5F87B
    static void cast_haste();
    static void DoElecDamage(int player_index, SaveVerseType bonusType, int damage, Point pos);
    static void DoActualElecDamage(int player_index, SaveVerseType bonusType, int damage, int playerIndex);
    static bool DoElecDamage(bool arg_0, int player_index, SaveVerseType bonusType, int damage, Point pos); // sub_5F986
    static void sub_5FA44(byte arg_0, SaveVerseType bonusType, int damage, byte arg_6);
    static void SpellLightningBolt(); // sub_5FCD9
    static void SpellSlow(); // sub_5FD2E
    static void SpellRestoration(); // cast_restore
    static void cast_speed();
    static void SpellCureSeriousWounds(); // sub_5FF6D
    static void cast_strength();
    static void sub_6003C();
    static void cast_paralyzed();
    static void cast_heal();
    static void cast_invisible();
    static void dam2d4plus2();
    static void SpellCauseSeriousWounds(); // sub_60185
    static void SpellNeutralizePoison(); // cure_poison
    static void SpellPoison(); // sub_602D0
    static void SpellSticksToSnakes(); // cast_flattern
    static void SpellCureCriticalWounds(); // sub_603F0
    static void SpellCauseCriticalWounds(); // sub_60431
    static void SpellDispelEvil(); // is_affected4
    static void SpellFlameStrike(); // sub_604DA
    static void SpellRaiseDead(); // cast_raise
    static void SpellSlayLiving(); //cast_slay
    static void SpellEntangle(); // cast_entangle
    static void SpellFaerieFire(); /* cast_highlisht */
    static void SpellInvisToAnimals(); // cast_invisible2
    static void SpellCharmMonsters(); // cast_charmed
    static void SpellConfusion(); // cast_confuse
    static void SpellDimensionDoor(); // cast_teleport
    static void SpellFear(); /* cast_terror */
    static void SpellFireProtection(); // cast_protection
    static void SpellFumble(); // spell_slow
    static void SpellIceStorm(); // sub_60F0B
    static void SpellMinorGlobeOfInvulnerability(); // sub_60F4E
    static void SpellCloudKill(); // spell_poisonous_cloud // similar to create_noxious_cloud
    static void SpellConeOfCold(); // sub_61550
    static void SpellFeeblemind(); // sub_615F2
    static void sub_616CC();
    static void sub_61727();
    static void cast_heal2();
    static void AffectParalizingGaze(Effect arg_0, object param, Player player); /* spell_stone */
    static void DragonBreathElec(Effect arg_0, object param, Player player); // cast_breath
    static void AffectSpitAcid(Effect arg_0, object param, Player player); // spell_spit_acid
    static void DragonBreathAcid(Effect arg_0, object param, Player attacker); // spell_breathes_acid
    static void DragonBreathFire(Effect arg_0, object param, Player attacker); // spell_breathes_fire
    static void cast_breath_fire(Effect arg_0, object param, Player arg_6);
    static void cast_throw_lightening(Effect arg_0, object param, Player caster); /* cast_throw_lightning */
    static void cast_gaze_paralyze(Effect arg_0, object param, Player arg_6);
    static void remove_spell_from_scroll(int affect, Item item, Player player); /* sub_623FF */
    static void DisplayCaseSpellText(int spellId, String arg_2, Player player); /* cast_a_spell */
    static void setup_spells();
};


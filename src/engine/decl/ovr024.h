// engine/decl/ovr024.h - declarations for engine/ovr024.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr024.cpp.
// ---------------------------------------------------------------------------
// ovr024.cs
// ---------------------------------------------------------------------------
struct ovr024 {
    static void KillPlayer(String text, Status new_health_status, Player player); // sub_63014
    static void remove_affect(Affect affect, Affects affect_id, Player player);
    inline static Array<Affects> unk_6325A = { Affects::silence_15_radius, Affects::prot_from_evil_10_radius, Affects::prot_from_good_10_radius, Affects::prayer };
    static void calc_affect_effect(Affects affect_type, Player player);
    static void CheckAffectsEffect(Player player, CheckType type); // work_on_00
    static Player sub_63D03(Array<byte> directions, int arraySize, List<GasCloud> list, Point mapPos); // sub_63D03
    static void in_poison_cloud(byte arg_0, Player player);
    static bool CanHitTarget(int bonus, Player target); // sub_641DD
    static bool PC_CanHitTarget(int target_ac, Player target, Player attacker); /* sub_64245 */
    static bool RollSavingThrow(int saveBonus, SaveVerseType saveType, Player player); // do_saving_throw
    static byte roll_dice(int dice_size, int dice_count);
    static int roll_dice_save(int dice_size, int dice_count);
    static void add_affect(bool call_spell_jump_list, int data, ushort minutes, Affects type, Player player);
    static void RemoveFromCombat(String msg, Status health_status, Player player); // sub_644A7
    static void remove_invisibility(Player player);
    static void RemoveCombatAffects(Player player); // sub_645AB
    static void RemoveAttackersAffects(Player player); // sub_6460D
    static bool cure_affect(Affects affectId, Player player); /* is_cured */
    static int encode_strength(int str_00, int str); /* odd_math */
    static void decode_strength(int& str_00, int& str, Affect arg_8); /* sub_646D9 */
    static bool TryEncodeStrength(int& encoded_str, int str_100, int str, Player player); // sub_64728
    static void max_strength(int& str_a, int str_b, int& str_00_a, int str_00_b); /* sub_64771 */
    static int ConHitPointBonus(int classLvl, SkillType class_index, int cons, Player player); // sub_647BE
    static void CalcStatBonuses(Stat stat_index, Player player); // sub_648D9
    static void damage_person(bool change_damage, DamageOnSave arg_2, int damage, Player player);
    static void TryLooseSpell(Player player);
    static void ApplyAttackSpellAffect(String text, bool saved, DamageOnSave can_save, bool call_affect_table, int data, ushort time, Affects affect_id, Player target); // is_unaffected
    static bool heal_player(byte arg_0, int amount_healed, Player player);
    static bool combat_heal(byte arg_0, Player player);
};


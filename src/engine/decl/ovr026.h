// engine/decl/ovr026.h - declarations for engine/ovr026.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr026.cpp.
// ---------------------------------------------------------------------------
// ovr026.cs
// ---------------------------------------------------------------------------
struct ovr026 {
    static Array2<byte> ClericSpellLevels; /*seg600:42BC*/
    static Array2<byte> PaladinSpellLevels; /*seg600:43E5*/
    static Array2<byte> unk_1A758; /*seg600:4448*/
    static void sub_6A00F(Player player); // sub_6A00F
    static void ReclacClassBonuses(Player player); // sub_6A3C6
    static void calc_cleric_spells(bool ResetSpellLevels, Player player); /* sub_6A686 */
    static Array3<byte> SaveThrowValues;
    static void reclac_saving_throws(Player player); // sub_6A7FB
    static void SaveVersePoisonBonus(Player player, bool applyBonus);
    static Array2<sbyte> unk_1A230; /*seg600:3F20 */
    static Array2<sbyte> unk_1A243; /*seg600:3F33 */
    static Array2<byte> base_chance; /*seg600:3EC0 unk_1A1D0 */
    static void reclac_thief_skills(Player player); // sub_6AAEA
    static bool SecondClassAllowed(ClassId _class, Player player); /* sub_6AD3E */
    static void DuelClass(Player player);
    static ClassId HumanCurrentClass_Unknown(Player player); // getFirstSkill
    static byte HumanCurrentClassLevel_Zero(Player player); /* hasAnySkills */
    static bool DualClassExceedLastLevel(Player player); // sub_6B3D1
};


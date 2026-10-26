// engine/decl/ovr013.h - declarations for engine/ovr013.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr013.cpp.
// ---------------------------------------------------------------------------
// ovr013.cs
// ---------------------------------------------------------------------------
struct ovr013 {
    static void ProtectedIf(Affects affect); /* sub_3A019 */
    static void Protected();
    static bool addAffect(ushort time, int data, Affects affect_type, Player player);
    static void sub_3A071(Effect arg_0, object param, Player player);
    static void Bless(Effect add_remove, object param, Player player);
    static void Curse(Effect arg_0, object param, Player player);
    static void SticksToSnakes(Effect arg_0, object param, Player player);
    static void DispelEvil(Effect arg_0, object param, Player player);
    static void BonusVsMonstersX(Effect arg_0, object param, Player player); // sub_3A17A
    static void FaerieFire(Effect arg_0, object param, Player player);
    static void affect_protect_evil(Effect arg_0, object param, Player player); /* sub_3A224 */
    static void affect_protect_good(Effect arg_0, object param, Player player); /* sub_3A259 */
    static void affect_resist_cold(Effect arg_0, object param, Player player); /* sub_3A28E */
    static void affect_charm_person(Effect arg_0, object param, Player player); /* sub_3A2AD */
    static void Suffocates(Effect arg_0, object param, Player player);
    static void AffectPoisonDamage(Effect arg_0, object param, Player player); // sub_3A3BC
    static void AffectShield(Effect arg_0, object param, Player player); /* sub_3A41F */
    static void AffectGnomeVsManSizedGiant(Effect arg_0, object param, Player player); // sub_3A44A
    static void AffectResistFire(Effect add_remove, object param, Player player); /* sub_3A480 */
    static void is_silenced1(Effect arg_0, object param, Player player);
    static void AffectSlowPoison(Effect arg_0, object param, Player player); // sub_3A517
    static void affect_spiritual_hammer(Effect add_remove, object param, Player player); /* sub_3A583 */
    static void sub_3A6C6(Effect arg_0, object param, Player player);
    static void AffectDwarfVsOrc(Effect arg_0, object param, Player player); // sub_3A7E8
    static void MirrorImage(Effect arg_0, object param, Player player);
    static void three_quarters_damage(Effect arg_0, object param, Player player);
    static void StinkingCloud(Effect arg_0, object param, Player player);
    static void sub_3A89E(Effect arg_0, object param, Player player);
    static void AffectBlinded(Effect arg_0, object param, Player player); // sub_3A951
    static void AffectCauseDisease(Effect add_remove, object param, Player player); // sub_3A974
    static void AffectConfuse(Effect arg_0, object arg_2, Player player); // sub_3A9D9
    static void affect_curse(Effect arg_0, object param, Player player); /* sub_3AB6F */
    static void AffectBlink(Effect arg_0, object param, Player player); // has_action_timedout
    static void AffectHaste(Effect arg_0, object param, Player player); // spl_age
    static void StinkingCloudAffect(Effect arg_0, object param, Player player); // sub_3AC1D
    static void AvoidMissleAttack(int percentage, Player player); // sub_3AF06
    static Item get_primary_weapon(Player player); /* sub_3AF77 */
    static void AffectProtNormalMissles(Effect arg_0, object param, Player player); // sub_3AFE0
    static void AffectSlow(Effect arg_0, object param, Player player); //sub_3B01B
    static void weaken(Effect arg_0, object param, Player player);
    static void sub_3B0C2(Effect arg_0, object param, Player player);
    static void AffectDwarfGnomeVsGiants(Effect arg_0, object param, Player player);
    static void sub_3B1A2(Effect arg_0, object param, Player player);
    static void AffectPrayer(Effect arg_0, object param, Player player); // sub_3B1C9
    static void HotFireShield(Effect arg_0, object param, Player player); // sub_3B212
    static void ColdFireShield(Effect arg_0, object param, Player player); // sub_3B243
    static void sub_3B27B(Effect arg_0, object param, Player player); // sub_3B27B
    static void AffectClearMovement(Effect arg_0, object param, Player player); //sub_3B29A
    static void AffectRegenration(Effect arg_0, object param, Player player);
    static void AffectResistWeapons(Effect arg_0, object param, Player player); // sub_3B2D8
    static void AffectFireResist(Effect arg_0, object param, Player player);
    static void AffectHighConRegen(Effect arg_0, object param, Player player); /* sub_3B386 */
    static void AffectMinorGlobeOfInvulnerability(Effect arg_0, object param, Player player); /* sub_3B3CA */
    static void PoisonAttack(int save_bonus, Player player);
    static void AffectPoisonPlus0(Effect arg_0, object param, Player player); // sub_3B520
    static void AffectPoisonPlus4(Effect arg_0, object param, Player player); // sub_3B534
    static void AffectPoisonPlus2(Effect arg_0, object param, Player player); // sub_3B548
    static void ThriKreenParalyze(Effect arg_0, object param, Player player); // sub_3B55C
    static void AffectFeebleMind(Effect arg_0, object param, Player player); // spell_stupid
    static void AffectInvisToAnimals(Effect arg_0, object param, Player player); // sub_3B636
    static void AffectPoisonNeg2(Effect arg_0, object param, Player player); // sub_3B671
    static void AffectInvisible(Effect arg_0, object param, Player player); // sub_3B685
    static void AffectCamouflage(Effect arg_0, object param, Player player); // sub_3B696
    static void ProtDragonsBreath(Effect arg_0, object param, Player player);
    static void AffectDragonSlayer(Effect arg_0, object param, Player player); // sub_3B71A
    static void AffectFrostBrand(Effect arg_0, object param, Player player); // sub_3B772
    static void AffectBerzerk(Effect arg_0, object param, Player player);
    static void sub_3B8D9(Effect arg_0, object param, Player player);
    static void MagicFireAttack_2d10(Effect arg_0, object param, Player player); // sub_3B919
    static void AnkhegAcidAttack(Effect arg_0, object param, Player player); // sub_3B94C
    static void half_damage(Effect arg_0, object param, Player player); /* sub_3B97F */
    static void AffectResistFireAndCold(Effect arg_0, object param, Player player); // sub_3B990
    static void AffectShamblerAbsorbLightning(Effect arg_0, object param, Player player); // sub_3B9E1
    static void sub_3BA14(Effect arg_0, object param, Player player);
    static void AffectDisplace(Effect arg_0, object param, Player player); /*sub_3BA55*/
    static void CloudKillAffect(Effect arg_0, object param, Player player); // sub_3BAB9
    static void half_fire_damage(Effect arg_0, object param, Player arg_6); // sub_3BD98
    static void sub_3BDB2(Effect arg_0, object param, Player arg_6);
    static void sub_3BE06(Effect arg_0, object param, Player player);
    static void con_saving_bonus(Effect arg_0, object param, Player player); /* sub_3BE42 */
    static void AffectRegen3Hp(Effect arg_0, object param, Player player); // sub_3BEB8
    static void sub_3BEE8(Effect arg_0, object param, Player player);
    static void AffectTrollFireOrAcid(Effect arg_0, object param, Player player);
    static void AffectTrollRegenerate(Effect arg_0, object param, Player player); // sp_regenerate
    static void AffectTrollRegen(Effect arg_0, object param, Player player); // sub_3C01E
    static void AffectSalamanderHeatDamage(Effect arg_0, object param, Player player); // sub_3C05D
    static void sub_3C0DA(Effect arg_0, object param, Player player);
    static void ResistMagicPercent(int rollBase); // sub_3C0EE
    static void ResistMagic50Percent(Effect arg_0, object param, Player arg_6); // sub_3C14F
    static void ResistMagic15Percent(Effect arg_0, object param, Player arg_6); // sub_3C15D
    static void AffectElfRisistSleep(Effect arg_0, object param, Player arg_6); // sub_3C16B
    static void AffectProtCharmSleep(Effect arg_0, object param, Player arg_6); // sub_3C18F
    static void ResistParalyze(Effect arg_0, object param, Player arg_6); // sub_3C1A4
    static void AffectImmuneToCold(Effect arg_0, object param, Player arg_6); // sub_3C1B2
    static void sub_3C1C9(Effect arg_0, object param, Player arg_6);
    static void AffectImmuneToFire(Effect arg_0, object param, Player arg_6); // sub_3C1EA
    static void sub_3C201(Effect arg_0, object param, Player arg_6);
    static void AffectProtectionFromElectricity(Effect arg_0, object param, Player player); // sub_3C246
    static void sub_3C260(Effect arg_0, object param, Player player);
    static void half_damage_if_weap_magic(Effect arg_0, object param, Player player); /* sub_3C2BF */
    static void sub_3C2F9(Effect arg_0, object param, Player player); // sub_3C2F9
    static void AffectProtCold(Effect arg_0, object param, Player player); // sub_3C33C
    static void AffectProtNonMagicWeapons(Effect arg_0, object param, Player player); // sub_3C356
    static void sub_3C3A2(Effect arg_0, object param, Player player);
    static void sub_3C3F6(Effect arg_0, object param, Player player);
    static void AffectDracolichParalysis(Effect arg_0, object param, Player player); // spl_paralyze
    static void sub_3C59D(Effect arg_0, object param, Player player);
    static void AffectHalfElfResistance(Effect arg_0, object param, Player player); // sub_3C5D0
    static void sub_3C5F4(Effect arg_0, object param, Player player);
    static void AffectProtMagic(Effect arg_0, object param, Player player); // sub_3C623
    static void sub_3C643(Effect arg_0, object arg_2, Player player);
    static void do_items_affect(Effect remove_affect, object param, Player player); /* sub_3C6D3 */
    static void AffectDracolichA(Effect arg_0, object param, Player player); //sub_3C750
    static void AffectRangerVsGiant(Effect arg_0, object param, Player player); // sub_3C77C
    static void AffectProtElec(Effect arg_0, object param, Player player); //sub_3C7B5
    static void AffectEntangle(Effect arg_0, object param, Player player); // sub_3C7CC
    static void sub_3C7E0(Effect arg_0, object param, Player player); // sub_3C7E0
    static void add_affect_19(Effect arg_0, object param, Player player);
    static void PaladinCastCureRefresh(Effect add_remove, object param, Player player); // sub_3C8EF
    static void AffectFear(Effect add_remove, object param, Player player); /* sub_3C932 */
    static void sub_3C975(Effect arg_0, object arg_2, Player target);
    static void sp_dispel_evil(Effect arg_0, object param, Player player);
    static void empty(Effect arg_0, object param, Player player);
    inline static Dictionary<Affects, affectDelegate> affect_table = nullptr; // null until SetupAffectTables()
    static void SetupAffectTables(); // setup_spells2
    static void CallAffectTable(Effect add_remove, object parameter, Player player, Affects affect); /* sub_630C7 */
};


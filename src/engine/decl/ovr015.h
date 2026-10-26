// engine/decl/ovr015.h - declarations for engine/ovr015.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr015.cpp.
// ---------------------------------------------------------------------------
// ovr015.cs
// ---------------------------------------------------------------------------
struct ovr015 {
    static void MapSetDoorUnlocked(int mapDir, int mapY, int mapX); /*sub_43148*/
    static bool AnyPlayerHasSkill(SkillType skill); // any_player_has_skill
    static bool bash_door();
    static bool pick_lock(); /*sub_435B6*/
    static bool TeamMemberHasSpell(Spells spellId);
    static bool RemoveKnockSpell();
    static void TryStepForward(); // sub_43765
    static void MovePartyForward(); /* sub_43813 */
    static char main_3d_world_menu(); /* sub_438DF */
    static void locked_door();
};


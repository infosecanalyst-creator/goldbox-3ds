// engine/decl/ovr005.h - declarations for engine/ovr005.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr005.cpp.
// ---------------------------------------------------------------------------
// ovr005.cs
// ---------------------------------------------------------------------------
struct ovr005 {
    static Array<Affects> disease_types;
    static Array<String> temple_sl;
    static bool CastCureAnyway(String text);
    static bool buy_cure(int cost, String cure_name); /* buy_cure */
    static void cure_blindness();
    static void cure_disease();
    static void cure_wounds(int healType);
    static void raise_dead();
    static void cure_poison2();
    static void remove_curse();
    static void stone_to_flesh();
    static void temple_heal();
    static void temple_shop();
};


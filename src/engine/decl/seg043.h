// engine/decl/seg043.h - declarations for engine/seg043.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg043.cpp.
// ---------------------------------------------------------------------------
// seg043.cs
// ---------------------------------------------------------------------------
// goldbox-3ds: thrown when a key is pressed during the demo (port
// enhancement), so the demo ends and the title menu comes back.
struct DemoAbort {};

struct seg043 {
    inline static bool in_print_and_exit = false;
    static void print_and_exit();
    static byte GetInputKey();
    static void DumpPlayerAffects();
    static void ToggleCommandDebugging();
    static void clear_keyboard();
    static void clear_one_keypress();
    static void DumpTreasureItems();
    static void TxtDumpPlayer(Player p, int area, int id);
    static void HtmlTableDumpPlayer(Ref<Logging::DebugWriter> dw, Player p, byte area, int id);
    static void DumpMonstersFiltered();
    static void DumpMonstersFilteredSub(Ref<Logging::DebugWriter> dw, std::function<bool(Player)> filter);
    static void DumpMonsters();
};


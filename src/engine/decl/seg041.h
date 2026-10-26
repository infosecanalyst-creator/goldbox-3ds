// engine/decl/seg041.h - declarations for engine/seg041.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg041.cpp.
// ---------------------------------------------------------------------------
// seg041.cs
// ---------------------------------------------------------------------------
struct seg041 {
    static void DrawRectangle(byte color, int yEnd, int xEnd, int yStart, int xStart);
    static void Load8x8Tiles(); // load_8x8d1_201
    static void display_char01(char ch, int repeatCount, int bgColor, int fgColor, int YCol, int XCol); // display_char01
    static void displaySpaceChar(int yCol, int xCol);
    static void displayString(String str, int bgColor, int fgColor, int yCol, int xCol);
    static int displayStringSlow(String text, int text_index, int text_length, int fgColor); // sub_107DE
    static void text_skip_space(String text, int text_max, int& text_index); /* sub_10854 */
    inline static Array2<int> bounds = {
            { 0x16, 0x26, 0x11, 1 },
            { 0x16, 0x26, 0x15, 1 },
            { 0x15, 0x26, 1, 0x17 } // TextRegion.CombatSummary
        };
    static void press_any_key(String text, bool clearArea, int fgColor, TextRegion region);
    inline static Set puncutation = New<Set_>(Array<int>{33, 44, 45, 46, 58, 59, 63}); // "!,-.:;?" // 33,44,45,46,58,59,63
    static void press_any_key(String text, bool clearArea, int fgColor, int yEnd, int xEnd, int yStart, int xStart);
    static String getUserInputString(byte inputLen, byte bgColor, byte fgColor, String prompt);
    static ushort getUserInputShort(byte bgColor, byte fgColor, String prompt);
    static void DisplayAndPause(String txt, byte fgColor); // displayAndDebug
    static int time01(); // time01
    static void ClearScreen();
    static void DisplayStatusText(byte bgColor, byte fgColor, String text); /* sub_10ECF */
    static void GameDelay();
};


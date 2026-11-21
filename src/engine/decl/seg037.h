// engine/decl/seg037.h - declarations for engine/seg037.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg037.cpp.
// ---------------------------------------------------------------------------
// seg037.cs
// ---------------------------------------------------------------------------
struct seg037 {
    static Array<int> outer_frame_bottom; /*unk_16EB0*/
    static Array<byte> unk_16ED6;
    static Array<byte> unk_16F0A;
    static Array<int> outer_frame_top; /*byte_16E60*/
    static Array<byte> x8x8_07;
    static Array<byte> unk_16EE3;
    static Array<int> outer_frame_left; /*unk_16EF2*/
    static Array<int> outer_frame_right; /*unk_16F1B*/
    static Array<byte> unk_16F31;
    static Array<byte> unk_16F3E;
    static Array<byte> unk_16F4D;
    static Array<byte> unk_16F64;
    static Array<byte> unk_16F7B;
    static void DrawFrame_Outer(); /* draw8x8_01 */
    static void draw8x8_02();
    static void draw8x8_03();
    static void DrawFrame_WildernessMap(); // draw8x8_04
    static void draw8x8_05();
    static void DrawFrame_Combat(); // draw8x8_06
    static void draw8x8_07();
    static void draw8x8_clear_area(TextRegion region);
    static void draw8x8_clear_area(int yEnd, int xEnd, int yStart, int xStart);
};


// engine/decl/seg037.h - declarations for engine/seg037.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg037.cpp.
// ---------------------------------------------------------------------------
// seg037.cs
// ---------------------------------------------------------------------------
struct seg037 {
    inline static Array<int> outer_frame_bottom = { 1, 8, 6, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 6, 8, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3 }; /*unk_16EB0*/
    inline static Array<byte> unk_16ED6 = { 4, 3, 0, 6, 1, 1, 1, 1, 8, 1, 1, 4, 1, 1, 2, 1, 4 };
    inline static Array<byte> unk_16F0A = { 0, 7, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 2, 9, 4 };
    inline static Array<int> outer_frame_top = { 0, 6, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 4, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 6, 1, 2 }; /*byte_16E60*/
    inline static Array<byte> x8x8_07 = { 0, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 8, 4,
                                    1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 6, 1,
                                    1, 1, 1, 1, 8, 2 };
    inline static Array<byte> unk_16EE3 = { 1, 2, 1, 4, 1, 1, 1, 1, 1, 1, 8, 4, 1, 1, 3 };
    inline static Array<int> outer_frame_left = { 0, 2, 9, 5, 2, 2, 2, 2, 2, 2, 5, 7, 2, 2, 2, 2, 2, 9, 7, 2, 2, 2, 7, 1 }; /*unk_16EF2*/
    inline static Array<int> outer_frame_right = { 2, 2, 9, 7, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 5, 2 }; /*unk_16F1B*/
    inline static Array<byte> unk_16F31 = { 5, 2, 0, 2, 7, 2, 2, 2, 2, 5, 2, 2, 2, 2, 1 };
    inline static Array<byte> unk_16F3E = { 2, 1, 2, 5, 9, 2, 2, 2, 7, 5, 2, 2, 2, 2, 3 };
    inline static Array<byte> unk_16F4D = { 0, 2, 9, 5, 2, 2, 2, 2, 2, 2, 5, 7, 2, 2, 2, 2, 2, 9, 7, 2, 2, 2, 1 };
    inline static Array<byte> unk_16F64 = { 0, 7, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 5, 2, 2, 2, 2, 2, 2, 5, 2, 4 };
    inline static Array<byte> unk_16F7B = { 2, 2, 9, 7, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2 };
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


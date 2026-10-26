// engine/decl/seg040.h - declarations for engine/seg040.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg040.cpp.
// ---------------------------------------------------------------------------
// seg040.cs
// ---------------------------------------------------------------------------
struct seg040 {
    static DaxBlock LoadDax(byte mask_colour, byte masked, int block_id, String fileName); // load_dax
    static void OverlayUnbounded(DaxBlock source, int arg_8, int itemIdex, int rowY, int colX);
    static void OverlayBounded(DaxBlock source, int arg_8, int itemIndex, int rowY, int colX); /* sub_E353 */
    static void ega_backup(DaxBlock dax_block, int rowY, int colX); /* ega_01 */
    inline static int color_no_draw = 17;
    inline static int color_re_color_from = 17;
    inline static int color_re_color_to = 17;
    static void draw_clipped_recolor(int from, int to);
    static void draw_clipped_nodraw(int color);
    static void draw_clipped_picture(DaxBlock dax_block, int rowY, int colX, int index, int clipMinX, int clipMaxX, int clipMinY, int clipMaxY);
    static void draw_combat_picture(DaxBlock dax_block, int rowY, int colX, int index);
    static void draw_picture(DaxBlock dax_block, int rowY, int colX, int index);
    static void DrawOverlay();
    static void SetPaletteColor(int color, int index);
    static void DrawColorBlock(int color, int lineCount, int colWidth, int lineY, int colX);
};


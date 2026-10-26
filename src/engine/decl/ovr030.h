// engine/decl/ovr030.h - declarations for engine/ovr030.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr030.cpp.
// ---------------------------------------------------------------------------
// ovr030.cs
// ---------------------------------------------------------------------------
struct ovr030 {
    inline static Array<byte> fadeOldColors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    inline static Array<byte> fadeNewColors = { 12, 12, 12, 12, 4, 5, 6, 7, 12, 12, 10, 12, 12, 12, 14, 12 };
    inline static Array<byte> transparentOldColors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    inline static Array<byte> transparentNewColors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 14, 15 };
    static void DrawMaybeOverlayed(DaxBlock dax_block, bool useOverlay, int rowY, int colX); // sub_7000A
    static void load_pic_final(DaxArray& daxArray, byte masked, byte block_id, String file_name);
    static void DaxArrayFreeDaxBlocks(DaxArray animation);
    static void head_body(byte body_id, byte head_id);
    static void draw_head_and_body(bool draw_body, byte rowY, byte colX); /* sub_706DC */
    static void Show3DSprite(DaxArray arg_0, int sprite_index);
    static void load_bigpic(byte block_id); /* bigpic */
    static void draw_bigpic(); /* sub_7087A */
};


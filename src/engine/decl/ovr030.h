// engine/decl/ovr030.h - declarations for engine/ovr030.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr030.cpp.
// ---------------------------------------------------------------------------
// ovr030.cs
// ---------------------------------------------------------------------------
struct ovr030 {
    static Array<byte> fadeOldColors;
    static Array<byte> fadeNewColors;
    static Array<byte> transparentOldColors;
    static Array<byte> transparentNewColors;
    static void DrawMaybeOverlayed(DaxBlock dax_block, bool useOverlay, int rowY, int colX); // sub_7000A
    static void load_pic_final(DaxArray& daxArray, byte masked, byte block_id, String file_name);
    static void DaxArrayFreeDaxBlocks(DaxArray animation);
    static void head_body(byte body_id, byte head_id);
    static void draw_head_and_body(bool draw_body, byte rowY, byte colX); /* sub_706DC */
    static void Show3DSprite(DaxArray arg_0, int sprite_index);
    static void load_bigpic(byte block_id); /* bigpic */
    static void draw_bigpic(); /* sub_7087A */
};


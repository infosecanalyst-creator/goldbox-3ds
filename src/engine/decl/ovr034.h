// engine/decl/ovr034.h - declarations for engine/ovr034.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr034.cpp.
// ---------------------------------------------------------------------------
// ovr034.cs
// ---------------------------------------------------------------------------
struct ovr034 {
    static void Load24x24Set(int cellCount, int destCellOffset, int block_id, String fileName);
    static void DrawIsoTile(int tileIndex, int rowY, int colX); /* sub_760F7 */
    static void ReleaseCombatIcon(int index); // free_icon
    static Array<byte> unk_16E30; // seg600:0B20
    static Array<byte> unk_16E40; // seg600:0B30
    static Array<byte> unk_16E50; // seg600:0B40
    static void chead_cbody_comspr_icon(byte combat_icon_index, int block_id, String fileText);
    static void draw_combat_icon(int iconIndex, Icon iconState, int direction, int tileY, int tileX); /* sub_76504 */
};


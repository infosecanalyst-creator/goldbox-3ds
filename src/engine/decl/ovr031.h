// engine/decl/ovr031.h - declarations for engine/ovr031.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr031.cpp.
// ---------------------------------------------------------------------------
// ovr031.cs
// ---------------------------------------------------------------------------
struct ovr031 {
    static constexpr int Column_A = 5; //word_16E08
    static constexpr int Column_B = 4; //word_16E0A
    static constexpr int Column_C = 6; // word_16E0C
    static constexpr int Column_D = 4; // word_16E0E
    static constexpr int Column_E = 2; // word_16E10
    static constexpr int Column_F = 7; // word_16E12
    static constexpr int Column_G = 2; // word_16E14
    static constexpr int Column_H = 0; // word_16E16
    static constexpr int Column_I = 9; // word_16E18
    static constexpr int Column_J = 5; // word_16E1A
    static constexpr int Row_A = 4; // byte_16E1C
    static constexpr int Row_B = 3; // byte_16E1E
    static constexpr int Row_C = 3; // byte_16E20
    static constexpr int Row_D = 3; // byte_16E22
    static constexpr int Row_E = 1; // byte_16E24
    static constexpr int Row_F = 1; // byte_16E26
    static constexpr int Row_G = 1; // byte_16E28
    static constexpr int Row_H = 0; // byte_16E2A
    static constexpr int Row_I = 0; // byte_16E2C
    static constexpr int Row_J = 4; // byte_16E2E
    static void DrawAreaMap(int partyDir, int partyMapY, int partyMapX); /* sub_7100F */
    static void Draw3dWorldBackground();
    inline static Array<byte> idxOffset = { 0, 2, 6, 10, 22, 38, 54, 110, 132, 154, 1 }; // seg600:0ADA
    inline static Array<int> colCount = { 1, 1, 1, 3, 2, 2, 7, 2, 2, 1 }; // seg600:0AE4
    inline static Array<int> rowCount = { 2, 4, 4, 4, 8, 8, 8, 11, 11, 2 }; // seg600:0AEE
    static void draw_3D_8x8_titles(int offsetIndex, int arg_2, int rowStart, int colStart); /* sub_71434 */
    static constexpr int MapSize = 16; // 16x16 so 0-15
    static bool MapCoordIsValid(int mapY, int mapX); /*sub_71542*/
    static byte WallDoorFlagsGet(int mapDir, int mapY, int mapX); /*sub_71573*/
    static byte getMap_wall_type(int direction, int mapY, int mapX);
    static MapInfo getMap_XXX(int mapY, int mapX);
    static byte get_wall_x2(int mapY, int mapX); /* sub_717A5 */
    static void Draw3dWorld(byte partyDir, int partyPosY, int partyPosX); /* sub_71820 */
    static void Draw3dWorldFar(byte partyDir, int dir_left, int dir_right, int drawX, int drawY);
    static void Draw3dWorldMid(byte partyDir, int dir_left, int dir_right, int var_5, int var_7);
    static void Draw3dWorldNear(byte partyDir, int dir_left, int dir_right, int var_5, int var_7);
    static void LoadWalldef(int symbolSet, int block_id);
    static void Load3DMap(int blockId);
};


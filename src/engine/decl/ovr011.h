// engine/decl/ovr011.h - declarations for engine/ovr011.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr011.cpp.
// ---------------------------------------------------------------------------
// ovr011.cs
// ---------------------------------------------------------------------------
struct ovr011 {
    inline static Array4<byte> unk_1AB1C = Array4<byte>(2, 4, 6, 11); //seg600:480C unk_1AB1C
    static void set_background_tile(int tileId, int y, int x); /* sub_37046 */
    inline static Array<int> dir_x_offset = { 0, 1, 0, -1 }; /*seg600:034C unk_1665B*/
    inline static Array<int> dir_y_offset = { -1, 0, 1, 0 }; /*seg600:0350 unk_1665F*/
    static void sub_370D3();
    static int sub_37306(int dir, int mapY, int mapX);
    static int get_dir_flags(int dir, int mapX, int mapY); /* sub_37388 */
    static void build_background_tiles_1(); /* sub_373FC */
    static void build_background_tiles_2(); /* sub_374A1 */
    static void build_backgrould_tiles_3(int mapX, int mapY); /* sub_3751E */
    static void build_background_tiles_4(int mapX, int mapY); /* sub_376F6 */
    static void SetupDungeonFloor(); // sub_378CD0
    inline static Array<int> CityInfo = { /* unk_16664 seg600:0354 */
            0x00, 0x18, 0x11, 0x15, 0x01, 0x01, 0x60, 0x14, // 354 - 35B
            0x08, 0x01, 0x00, 0x21, 0x71, 0x09, 0x06, 0x04, // 35C - 363
            0x01, 0x09, 0x09, 0x08, 0x59, 0x00, 0x11, 0x11, // 364 - 36B
            0x00, 0x00, 0x01, 0x11, 0x00, 0x00, 0x20, 0x20, // 36C - 373
            0x0A};
    static int GetCityInfo(); // sub_37991
    static void SetGroundTile_40(int map_x, int map_y); // sub_379AC
    static void SetupWildernessFloor01(); // sub_37A00
    static void SetupWildernessFloor02(); // sub_37B0B
    static void SetGroupMapStepped(int stepE, int stepD, int stepC, int stepB, int stepA, int map_y, int map_x); // sub_37CA2
    static void SetupWildernessFloor03(); // sub_37E4A
    static void SetupWildernessFloor(); // sub_37FC8
    static void SetupGroundTiles(); // sub_38030
    static void SetupCombatActions(); // sub_380E0
    inline static int min_placement_column = 0;
    inline static int max_placement_column = 10;
    inline static int min_placement_row = 0;
    inline static int max_placement_row = 5;
    static bool row_column_both_out_of_range(int row, int column); /* sub_38202 */
    static bool try_place_combatant(int arg_0, int arg_2, int arg_4, int arg_6, int arg_8, int player_index); /* sub_38233 */
    inline static Array2<int> direction_165EC = { { 8, 4, 6, 2 }, { 8, 6, 4, 0 }, { 8, 0, 6, 2 }, { 8, 2, 0, 4 } }; /*seg600:02DC unk_165EC*/
    inline static Array2<int> direction_165FC = { { 0, 0, 2, 6 }, { 2, 2, 0, 4 }, { 4, 4, 2, 6 }, { 6, 6, 4, 0 } }; /*seg600:02EC unk_165FC*/
    inline static Array<int> HalfDirToIso = { 7, 2, 3, 6 }; /*seg600:02FC unk_1660C */
    inline static Array<byte> unk_16610 = { 5, 4, 5, 6, 3, 8, 7, 2 }; /*seg600:0300*/
    inline static Array<byte> unk_16618 = { 3, 2, 2, 3, 0, 2, 5, 3 }; /*seg600:0308*/
    inline static Array3<byte> unk_16620 = { // unk_16620 seg600:0310
                {{1,0},{1,0},{1,0},{2,9},{3,10},{4,10}}, // 310 - 31B
                {{0,2},{0,3},{1,4},{2,5},{3,6},{4,7}}, // 31C - 327
                {{0,6},{0,7},{1,8},{1,0},{1,0},{1,0}}, // 328 - 333
                {{3,6},{4,7},{5,8},{6,9},{7,10},{8,10}}, // 334 - 33F
                {{0,6},{0,7},{1,8},{2,9},{3,10},{4,10}}, // 340 - 31B
        };
    enum class tri_state : int {
        start = 1,
        right = 2,
        left = 3
    };

    static bool place_combatant(int player_index); /* sub_38380 */
    static void PlaceCombatants(); /* sub_387FE */
    static void BattleSetup(); // battle_begins
};


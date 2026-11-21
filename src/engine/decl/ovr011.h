// engine/decl/ovr011.h - declarations for engine/ovr011.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr011.cpp.
// ---------------------------------------------------------------------------
// ovr011.cs
// ---------------------------------------------------------------------------
struct ovr011 {
    static Array4<byte> unk_1AB1C; //seg600:480C unk_1AB1C
    static void set_background_tile(int tileId, int y, int x); /* sub_37046 */
    static Array<int> dir_x_offset; /*seg600:034C unk_1665B*/
    static Array<int> dir_y_offset; /*seg600:0350 unk_1665F*/
    static void sub_370D3();
    static int sub_37306(int dir, int mapY, int mapX);
    static int get_dir_flags(int dir, int mapX, int mapY); /* sub_37388 */
    static void build_background_tiles_1(); /* sub_373FC */
    static void build_background_tiles_2(); /* sub_374A1 */
    static void build_backgrould_tiles_3(int mapX, int mapY); /* sub_3751E */
    static void build_background_tiles_4(int mapX, int mapY); /* sub_376F6 */
    static void SetupDungeonFloor(); // sub_378CD0
    static Array<int> CityInfo;
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
    static Array2<int> direction_165EC; /*seg600:02DC unk_165EC*/
    static Array2<int> direction_165FC; /*seg600:02EC unk_165FC*/
    static Array<int> HalfDirToIso; /*seg600:02FC unk_1660C */
    static Array<byte> unk_16610; /*seg600:0300*/
    static Array<byte> unk_16618; /*seg600:0308*/
    static Array3<byte> unk_16620;
    enum class tri_state : int {
        start = 1,
        right = 2,
        left = 3
    };

    static bool place_combatant(int player_index); /* sub_38380 */
    static void PlaceCombatants(); /* sub_387FE */
    static void BattleSetup(); // battle_begins
};


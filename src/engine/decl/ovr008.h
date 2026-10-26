// engine/decl/ovr008.h - declarations for engine/ovr008.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr008.cpp.
// ---------------------------------------------------------------------------
// ovr008.cs
// ---------------------------------------------------------------------------
struct ovr008 {
    static void vm_LoadCmdSets(int numberOfSets); // parse_command_sub
    static ushort vm_GetCmdValue(int arg_0); // sub_30168
    static void vm_init_ecl(); // sub_301E8
    static void load_ecl_dax(byte block_id);
    static byte sub_304B4(int map_dir, int map_y, int map_x);
    static void set_and_draw_head_body(byte body_id, byte head_id); /* sub_30543 */
    static void sub_30580(Array<bool> flags, int encounter_distance, byte pic_block_id, byte sprite_block_id);
    static char inflateChar(uint arg_0);
    static uint deflateChar(char ch);
    static int vm_GetMemoryValueType(ushort arg_0); // sub_30723
    static ushort find_gbl_player_index(Player player);
    static ushort get_player_values(bool& arg_0, ushort arg_4);
    static void alter_character(ushort set_value, ushort switch_var);
    static void vm_SetMemoryValue(ushort value, ushort location); // cmd_table01
    static ushort vm_GetMemoryValue(ushort loc); // sub_30F16
    static void vm_WriteStringToMemory(String text, ushort loc); // sub_3105D
    static Array<byte> compressString(String input);
    static String DecompressString(Array<byte> data);
    static void LoadCompressedEclString(int strIndex, int inputLength);
    static void vm_CopyStringFromMemory(ushort location, byte strIndex); // sub_31421
    inline static Set unk_31673 = New<Set_>(Array<int>{48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90});
    static String buildMenuStrings(String& MenuString);
    inline static Set validkeys = New<Set_>(Array<int>{48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90}); // unk_3178A
    static int sub_317AA(bool useOverlay, bool acceptReturn, MenuColorSet colors, String displayString, String extraString);
    static int VertMenuSelect(int index, bool menuRedraw, bool showExit, List<MenuItem> list, sbyte endY, sbyte endX, int startY, sbyte startX);
    static void compare_strings(String string_a, String string_b); /* sub_3193B */
    static void compare_variables(ushort arg_0, ushort arg_2); /* sub_31A11 */
    static void MovePositionForward(); // sub_31B01
    static int DecrimentWrap(int value, int max);
    static int IncrementWrap(int value, int max);
    static void SetupDuel(bool isDuel);
    static void RobMoney(Player player, double scale); /* sub_31DEF */
    static void RobItems(Player player, int robChance); /* sub_31F1C */
    static void calc_group_movement(byte& mov_min, byte& mov_max); /* calc_group_inituative */
    static void sub_32200(Player player, int damage); /* sub_32200 */
};


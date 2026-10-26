// engine/decl/ovr017.h - declarations for engine/ovr017.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr017.cpp.
// ---------------------------------------------------------------------------
// ovr017.cs
// ---------------------------------------------------------------------------
struct ovr017 {
    static void BuildLoadablePlayersLists(List<MenuItem>& fileNames, List<MenuItem>& displayNames, short playerFileSize, int npcOffset, int nameOffset, String fileFilter); // sub_4708B
    inline static Array<int> PlayerNameOffset = { 0, 0, 4 };
    inline static Array<int> NpcFileOffset = { 0xf7, 0x84, 0x13 };
    static void BuildLoadablePlayersLists(List<MenuItem>& fileNames, List<MenuItem>& displayNames); // sub_47465
    inline static Set unk_47635 = New<Set_>(Array<int>{0, 5});
    static void LoadPlayerCombatIcon(bool recolour); /* sub_47A90 */
    static void remove_player_file(Player player);
    static void SavePlayer(String arg_0, Player player); // sub_47DFC
    static bool PlayerFileExists(String fileExt, String player_name); // sub_483AE
    static Player ConvertPoolRadPlayer(PoolRadPlayer bp_var_1C0);
    static void TransferHillsFarCharacter(HillsFarPlayer hf_player, Player player, Player previousSelectPlayer); // sub_48F35
    static void SilentTrainPlayer();
    inline static Set asc_49280 = New<Set_>(Array<int>{18, 26, 47, 48, 97, 107, 124});
    inline static Array<ClassId> HillsFarClassMap = {
    ClassId::unknown,    ClassId::thief,      ClassId::fighter,    ClassId::mc_f_t, ClassId::magic_user,
    ClassId::mc_mu_t,    ClassId::mc_f_mu,    ClassId::mc_f_mu_t,  ClassId::cleric, ClassId::mc_c_t,
    ClassId::mc_c_f,     ClassId::unknown,    ClassId::mc_c_mu,    ClassId::unknown, ClassId::mc_c_f_m,
    ClassId::unknown};
    static void import_char01(Player& player, String arg_8);
    static Player ConvertHillsFarPlayer(HillsFarPlayer hf_player, String arg_8);
    static Player load_mob(int monster_id);
    static Player load_mob(int monster_id, bool exit);
    static void load_npc(int monster_id); // sub_4A57D
    static void AssignPlayerIconId(Player player); // sub_4A60A
    inline static Set save_game_keys = New<Set_>(Array<int>{65, 66, 67, 68, 69, 70, 71, 72, 73, 74}); // asc_4A761
    static void loadGameMenu(); // loadGame
    static void loadSaveGame(String file_name);
    inline static Set unk_4AEA0 = New<Set_>(Array<int>{0, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74});
    inline static Set unk_4AEEF = New<Set_>(Array<int>{0, 2, 18});
    static void SaveGame();
};


// engine/decl/ovr017.h - declarations for engine/ovr017.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr017.cpp.
// ---------------------------------------------------------------------------
// ovr017.cs
// ---------------------------------------------------------------------------
struct ovr017 {
    static void BuildLoadablePlayersLists(List<MenuItem>& fileNames, List<MenuItem>& displayNames, short playerFileSize, int npcOffset, int nameOffset, String fileFilter); // sub_4708B
    static Array<int> PlayerNameOffset;
    static Array<int> NpcFileOffset;
    static void BuildLoadablePlayersLists(List<MenuItem>& fileNames, List<MenuItem>& displayNames); // sub_47465
    static Set unk_47635;
    static void LoadPlayerCombatIcon(bool recolour); /* sub_47A90 */
    static void remove_player_file(Player player);
    static void SavePlayer(String arg_0, Player player); // sub_47DFC
    static bool PlayerFileExists(String fileExt, String player_name); // sub_483AE
    static Player ConvertPoolRadPlayer(PoolRadPlayer bp_var_1C0);
    static void TransferHillsFarCharacter(HillsFarPlayer hf_player, Player player, Player previousSelectPlayer); // sub_48F35
    static void SilentTrainPlayer();
    static Set asc_49280;
    static Array<ClassId> HillsFarClassMap;
    static void import_char01(Player& player, String arg_8);
    static Player ConvertHillsFarPlayer(HillsFarPlayer hf_player, String arg_8);
    static Player load_mob(int monster_id);
    static Player load_mob(int monster_id, bool exit);
    static void load_npc(int monster_id); // sub_4A57D
    static void AssignPlayerIconId(Player player); // sub_4A60A
    static Set save_game_keys; // asc_4A761
    static void loadGameMenu(); // loadGame
    static void loadSaveGame(String file_name);
    static Set unk_4AEA0;
    static Set unk_4AEEF;
    static void SaveGame();
    static void SaveGameToSlot(char inputKey); // goldbox-3ds
    static bool SuspendSave();                 // goldbox-3ds: EXIT GAME, reserved slot J
};


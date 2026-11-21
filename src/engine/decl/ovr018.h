// engine/decl/ovr018.h - declarations for engine/ovr018.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr018.cpp.
// ---------------------------------------------------------------------------
// ovr018.cs
// ---------------------------------------------------------------------------
struct ovr018 {
    static Set unk_4C13D;
    static Set unk_4C15D;
    static void FreePlayer(Player player); // free_player
    static Array<String> menuStrings;
    static constexpr int allow_create = 0;
    static constexpr int allow_drop = 1;
    static constexpr int allow_modify = 2;
    static constexpr int allow_training = 3;
    static constexpr int allow_duelclass = 4;
    static constexpr int allow_view = 5;
    static constexpr int allow_add = 6;
    static constexpr int allow_remove = 7;
    static constexpr int allow_load = 8;
    static constexpr int allow_save = 9;
    static constexpr int allow_begin = 10;
    static constexpr int allow_exit = 11;
    static Array<bool> menuFlags;
    static void startGameMenu();
    static Array<byte> unk_1A1B2; /*seg600:3EA2 */
    static Array2<sbyte> thac0_table; /* seg600:3E3A unk_1A14A */
    static void createPlayer();
    static int con_bonus(ClassId classId);
    static void dropPlayer();
    static void draw_highlight_stat(bool highlighted, byte edited_stat, int name_cursor_pos); /* sub_4E6F2 */
    static void modifyPlayer();
    static void AddPlayer();
    static Player FreeCurrentPlayer(Player player, bool free_icon, bool leave_party_size); // free_players
    static void drawIconEditorIcons(sbyte titleY, sbyte titleX); /* sub_4FB7C */
    static void duplicateCombatIcon(bool recolour, byte destIndex, byte sourceIndex); /* sub_4FC5B */
    static Set unk_4FE94;
    static void icon_builder();
    static Array<sbyte> con_hp_adj;
    static sbyte get_con_hp_adj(Player player);
    static int sub_506BA(Player player);
    class hp_calc_ {
    public:
        hp_calc_(int _dice, int _lvl, int _base, int _mult);
        int dice = 0;
        int lvl_bonus = 0;
        int max_base = 0;
        int max_mult = 0;
    };
    using hp_calc = cs::Ref<hp_calc_>;

    static Array<hp_calc> hp_calc_table;
    static int calc_max_hp(Player player); /* sub_50793 */
    static Array<byte> unk_16B2A; /* seg600:081A */
    static Array<byte> unk_16B32; /* seg600:0822 */
    static Array<byte> classMasks; /* seg600:3EAA unk_1A1BA */
    static byte sub_509E0(byte arg_0, Player player);
    static Array2<int> exp_table;
    static void train_player();
};


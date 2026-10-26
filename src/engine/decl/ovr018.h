// engine/decl/ovr018.h - declarations for engine/ovr018.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr018.cpp.
// ---------------------------------------------------------------------------
// ovr018.cs
// ---------------------------------------------------------------------------
struct ovr018 {
    inline static Set unk_4C13D = New<Set_>(Array<int>{71, 79});
    inline static Set unk_4C15D = New<Set_>(Array<int>{69, 83});
    static void FreePlayer(Player player); // free_player
    inline static Array<String> menuStrings = {
            "Create New Character",
            "Drop Character",
            "Modify Character",
            "Train Character",
            "Human Change Classes",
            "View Character",
            "Add Character to Party",
            "Remove Character from Party",
            "Load Saved Game",
            "Save Current Game",
            "BEGIN Adventuring",
            "Exit to DOS"
        };
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
    inline static Array<bool> menuFlags = {
            true,
            false,
            false,
            false,
            false,
            false,
            true,
            false,
            false,
            false,
            false,
            true
        };
    static void startGameMenu();
    inline static Array<byte> unk_1A1B2 = { 0x02, 0x10, 0x08, 0x40, 0x40, 0x01, 0x04, 0x20 }; /*seg600:3EA2 */
    inline static Array2<sbyte> thac0_table = {
            {40, 40, 40, 40, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C, 0x2C, 0x2E, 0x2E, 0x2E},
            {40, 40, 40, 40, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C, 0x2C, 0x2E, 0x2E, 0x2E},
            {0x27, 40, 40, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33},
            {40, 40, 40, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33},
            {40, 40, 40, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33},
            {0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x29, 0x29, 0x29, 0x29, 0x29, 0x2B, 0x2B},
            {40, 40, 40, 40, 40, 0x29, 0x29, 0x29, 0x29, 0x2C, 0x2C, 0x2C, 0x2C},
            {40, 40, 40, 40, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C, 0x2C, 0x2E, 0x2E, 0x2E} }; /* seg600:3E3A unk_1A14A */
    static void createPlayer();
    static int con_bonus(ClassId classId);
    static void dropPlayer();
    static void draw_highlight_stat(bool highlighted, byte edited_stat, int name_cursor_pos); /* sub_4E6F2 */
    static void modifyPlayer();
    static void AddPlayer();
    static Player FreeCurrentPlayer(Player player, bool free_icon, bool leave_party_size); // free_players
    static void drawIconEditorIcons(sbyte titleY, sbyte titleX); /* sub_4FB7C */
    static void duplicateCombatIcon(bool recolour, byte destIndex, byte sourceIndex); /* sub_4FC5B */
    inline static Set unk_4FE94 = New<Set_>(Array<int>{0, 69});
    static void icon_builder();
    inline static Array<sbyte> con_hp_adj = { 0, 0, 0, -2, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
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

    inline static Array<hp_calc> hp_calc_table = {
            New<hp_calc_>(8, 0, 0x48, 2), // Cleric
            New<hp_calc_>(8, 0, 0x70, 0), // Druid
            New<hp_calc_>(10, 0, 0x5A, 3), // Fighter
            New<hp_calc_>(10, 0, 0x5A, 3), // Paladin
            New<hp_calc_>(8, 1, 0x58, 2), // Ranger
            New<hp_calc_>(4, 0, 0x2c, 1), // Magic User
            New<hp_calc_>(6, 0, 0x3c, 2), // Thief
            New<hp_calc_>(4, 1, 0x48, 0), // Monk
        };
    static int calc_max_hp(Player player); /* sub_50793 */
    inline static Array<byte> unk_16B2A = { 1, 1, 1, 1, 2, 1, 1, 2 }; /* seg600:081A */
    inline static Array<byte> unk_16B32 = { 8, 8, 0xA, 0xA, 8, 4, 6, 4 }; /* seg600:0822 */
    inline static Array<byte> classMasks = { 2, 2, 8, 0x10, 0x20, 1, 4, 4 }; /* seg600:3EAA unk_1A1BA */
    static byte sub_509E0(byte arg_0, Player player);
    inline static Array2<int> exp_table = { /* seg600:4293 unk_1A5A3 */
            /* Cleric */    { 0, 1501, 3001,  6001, 13001, 27501, 55001, 110001, 225001, 450001, -1, -1, -1 },
            /* Druid */     { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
            /* Fighter */   { 0, 2001, 4001,  8001, 18001, 35001, 70001, 125001, 250001, 500001,  750001, 1000001, -1  },
            /* Paladin */   { 0, 2751, 5501, 12001, 24001, 45001, 95001, 175001, 350001, 700001, 1050001, -1, -1 },
            /* Ranger */    { 0, 2251, 4501, 10001, 20001, 40001, 90001, 150001, 225001, 325001,  650001, -1, -1 },
            /* MU */        { 0, 2501, 5001, 10001, 22501, 40001, 60001,  90001, 135001, 250001,  375001, -1, -1 },
            /* Thief */     { 0, 1251, 2501,  5001, 10001, 20001, 42501,  70001, 110001, 160001,  220001, 440001, -1},
            /* Monk */      { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }};
    static void train_player();
};


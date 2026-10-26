// Classes/Gbl.h - port of class gbl from Classes/Gbl.cs (static-only).
// The enums, delegates, MenuColorSet and Point from the top of Gbl.cs live in
// Classes/GblTypes.h (included here).
// All tables are copied verbatim from Gbl.cs.
// PORT-NOTE: these are `inline static` members with dynamic initialisers; they are
// initialised in declaration order before main() (no cross-TU ordering issues:
// nothing here reads another translation unit's statics during initialisation).
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/GblTypes.h"
#include "Classes/Enums.h"
#include "Classes/Affect.h"
#include "Classes/Spells.h"
#include "Classes/ItemData.h"
#include "Classes/Item.h"
#include "Classes/MenuItem.h"
#include "Classes/MoneySet.h"
#include "Classes/Opperation.h"
#include "Classes/Player.h"
#include "Classes/RestTime.h"
#include "Classes/Area1.h"
#include "Classes/Area2.h"
#include "Classes/EclBlock.h"
#include "Classes/GeoBlock.h"
#include "Classes/GasCloud.h"
#include "Classes/DownedPlayerTile.h"
#include "Classes/ClassStatsMin.h"
#include "Classes/Struct_189B4.h"
#include "Classes/Struct_1A35E.h"
#include "Classes/Struct_1ADF6.h"
#include "Classes/Struct_1B2CA.h"
#include "Classes/Struct_1D1BC.h"
#include "Classes/DaxFiles/DaxArray.h"
#include "Classes/DaxFiles/DaxBlock.h"
#include "Classes/Combat/CombatIcon.h"
#include "Classes/Combat/CombatantMap.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;
using namespace Classes::Combat;

struct gbl {

    inline static MenuColorSet defaultMenuColors = New<MenuColorSet_>(15, 10, 13);
    inline static MenuColorSet alertMenuColors = New<MenuColorSet_>(15, 10, 14);

    static constexpr int BackGroundTiles_count = 64; /* 64 is a guess */
    inline static Array<Struct_189B4> BackGroundTiles{ /* unk_189B4 */
        New<Struct_189B4_>( 1 , 0 , 0xFF, 0),
        New<Struct_189B4_>( 0xFF , 1 , 2 , 0 ),
        New<Struct_189B4_>( 0xFF , 1 , 2 , 1 ),
        New<Struct_189B4_>( 0xFF , 1 , 2 , 2 ),
        New<Struct_189B4_>(  0xFF,  1,  2, 3 ),
        New<Struct_189B4_>(  1 , 1 ,  0 ,  4 ),
        New<Struct_189B4_>(  0xFF , 1 ,  2 , 5 ),
        New<Struct_189B4_>( 0xff, 1, 2, 6 ),
        New<Struct_189B4_>( 0xff, 1, 2, 7 ),
        New<Struct_189B4_>( 1, 1, 0, 8 ),
        New<Struct_189B4_>( 0xff, 1, 2, 9 ),
        New<Struct_189B4_>( 1, 1, 0, 0x0A ),
        New<Struct_189B4_>( 0xff, 1, 2, 0x0B ),
        New<Struct_189B4_>( 1, 1, 0, 0x0C ),
        New<Struct_189B4_>( 0xff, 1, 2, 0x0D ),
        New<Struct_189B4_>( 1, 1, 0, 0x0E ),
        New<Struct_189B4_>( 0xff, 1, 2, 0x0F ),
        New<Struct_189B4_>( 1, 1, 0, 0x10 ),
        New<Struct_189B4_>( 0xff, 1, 2, 0x11 ),
        New<Struct_189B4_>( 0xff, 1, 2, 0x12 ),
        New<Struct_189B4_>( 0xff, 1, 2, 0x13 ),
        New<Struct_189B4_>( 0xff, 1, 2, 0x14 ),
        New<Struct_189B4_>( 0xff, 1, 2, 0x15 ),
        New<Struct_189B4_>( 1, 1, 0, 0x16 ),
        New<Struct_189B4_>( 1, 1, 0, 0x17 ),
        New<Struct_189B4_>( 0xff, 1, 2, 0x18 ),
        New<Struct_189B4_>( 2, 2, 0, 0x22 ),
        New<Struct_189B4_>( 1, 1, 0, 0x23 ),
        New<Struct_189B4_>( 1, 1, 0, 0x24 ),
        New<Struct_189B4_>( 1, 1, 0, 0x25 ),
        New<Struct_189B4_>( 1, 1, 0, 0x26 ),
        New<Struct_189B4_>( 1, 1, 0, 0x27 ),
        New<Struct_189B4_>( 0xff, 1, 2, 0 ),
        New<Struct_189B4_>( 0xff, 1, 2, 1 ),
        New<Struct_189B4_>( 0xff, 1, 2, 2 ),
        New<Struct_189B4_>( 0xff, 1, 2, 3 ),
        New<Struct_189B4_>( 0xff, 1, 2, 4 ),
        New<Struct_189B4_>( 1, 1, 0, 5 ),
        New<Struct_189B4_>( 1, 1, 0, 6 ),
        New<Struct_189B4_>( 1, 1, 0, 7 ),
        New<Struct_189B4_>( 1, 1, 0, 8 ),
        New<Struct_189B4_>( 1, 1, 0, 9 ),
        New<Struct_189B4_>( 0xff, 1, 2, 10 ),
        New<Struct_189B4_>( 0xff, 1, 2, 11 ),
        New<Struct_189B4_>( 1, 1, 0, 12 ),
        New<Struct_189B4_>( 1, 1, 0, 13 ),
        New<Struct_189B4_>( 1, 1, 0, 14 ),
        New<Struct_189B4_>( 1, 1, 0, 15 ),
        New<Struct_189B4_>( 2, 1, 0, 16 ),
        New<Struct_189B4_>( 2, 1, 0, 17 ),
        New<Struct_189B4_>( 2, 1, 0, 18 ),
        New<Struct_189B4_>( 2, 1, 0, 19 ),
        New<Struct_189B4_>( 2, 1, 0, 20 ),
        New<Struct_189B4_>( 2, 1, 0, 21 ),
        New<Struct_189B4_>( 1, 1, 0, 22 ),
        New<Struct_189B4_>( 1, 1, 0, 0x17 ),
        New<Struct_189B4_>( 1, 1, 0, 0x18 ),
        New<Struct_189B4_>( 1, 1, 0, 0x19 ),
        New<Struct_189B4_>( 2, 1, 0, 0x1A ),
        New<Struct_189B4_>( 2, 1, 0, 0x1B ),
        New<Struct_189B4_>( 4, 0, 0, 0x1C ),
        New<Struct_189B4_>( 4, 0, 0, 0x1D ),
        New<Struct_189B4_>( 4, 0, 0, 0x1E ),
        New<Struct_189B4_>( 4, 0, 0, 0x1F ),
        New<Struct_189B4_>( 1, 1, 0, 0x20 ),
        New<Struct_189B4_>( 1, 1, 0, 0x21 ),
        New<Struct_189B4_>( 0, 0, 0xff, 0xff ),
        New<Struct_189B4_>( 0xff, 0xff, 0xff, 0xff ),
        New<Struct_189B4_>( 0, 0, 0, 1 ),
        New<Struct_189B4_>( 0xff, 0xff, 0xff, 0xff ),
        New<Struct_189B4_>( 0, 0, 1, 0 ),
        New<Struct_189B4_>( 0xff, 0xff, 0xff, 0xff ),
        New<Struct_189B4_>( 0, 0, 1, 0 ),
        New<Struct_189B4_>( 0, 1, 1, 1 )
    };


    inline static const Array<byte> max_class_hit_dice{ 10, 15, 10, 10, 11, 12, 11, 13 }; // byte_1A1CB seg600:3EBB
    inline static const Array<byte> default_icon_colours{ 1, 2, 3, 4, 6, 7 }; // unk_1A1D3[0] == unk_1A1D2[1];

    inline static const Array<Array<ClassId>> RaceClasses{
        Array<ClassId>(0) , // C#: new ClassId[] /*MonsterType*/{ } (empty array, not null)
        Array<ClassId>/*Dwarf*/{ ClassId::fighter, ClassId::thief, ClassId::mc_f_t},
        Array<ClassId>/*Elf*/{ ClassId::fighter, ClassId::magic_user, ClassId::thief, ClassId::mc_f_mu, ClassId::mc_f_t, ClassId::mc_f_mu_t, ClassId::mc_mu_t},
        Array<ClassId>/*Gnome*/{ ClassId::fighter, ClassId::thief, ClassId::mc_f_t},
        Array<ClassId>/*Half-Elf*/{ ClassId::cleric, ClassId::fighter, ClassId::magic_user, ClassId::thief, ClassId::ranger,ClassId::mc_c_f, ClassId::mc_c_r, ClassId::mc_c_f_m, ClassId::mc_c_mu, ClassId::mc_f_mu, ClassId::mc_f_t, ClassId::mc_f_mu_t, ClassId::mc_mu_t},
        Array<ClassId>/*Halfling*/{ ClassId::fighter, ClassId::thief, ClassId::mc_f_t},
        Array<ClassId>/*Half-Orc*/{ ClassId::cleric, ClassId::fighter, ClassId::thief, ClassId::mc_c_f, ClassId::mc_c_t,ClassId::mc_f_t},
        Array<ClassId>/*Human*/{ ClassId::cleric, ClassId::fighter, ClassId::magic_user, ClassId::thief, ClassId::paladin, ClassId::ranger},
        Array<ClassId>/*Cheaters*/{ ClassId::cleric, ClassId::fighter, ClassId::magic_user, ClassId::thief, ClassId::ranger,ClassId::mc_c_f, ClassId::mc_c_r, ClassId::mc_c_f_m, ClassId::mc_c_mu, ClassId::mc_f_mu, ClassId::mc_f_t, ClassId::mc_f_mu_t, ClassId::mc_mu_t}};


    inline static bool stopVM = false; //byte_1AB08
    inline static bool vmFlag01 = false; // byte_1AB09
    inline static bool restore_player_ptr = false; // byte_1AB0A
    inline static bool byte_1AB0B = false;
    inline static bool byte_1AB0C = false;
    inline static bool filesLoaded = false; // byte_1AB0D
    inline static byte numLoadedMonsters = 0; // byte_1AB0E
    inline static bool byte_1AB14 = false;
    inline static bool shopRedrawMenuItems = false; // byte_1AB16
    inline static Array<int> team_start_x{ 0, 0 }; /* byte_1AD2C */
    inline static Array<int> team_start_y{ 0, 0 }; /* byte_1AD2E */
    inline static Array<int> half_team_count{ 0, 0 }; /* unk_1AD30 */
    inline static Array<int> team_direction{ 0, 0 }; /* byte_1AD32 */
    inline static sbyte byte_1AD34 = 0x1A;
    inline static sbyte byte_1AD35 = 0;
    inline static int dir_0_flags = 0; // byte_1AD36
    inline static int dir_6_flags = 0; // byte_1AD37
    inline static int dir_2_flags = 0; // byte_1AD38
    inline static int dir_4_flags = 0; // byte_1AD39
    inline static sbyte currentTeam = 0; // field_197
    inline static byte current_city = 0;
    inline static byte byte_1AD3D = 0;
    inline static byte byte_1AD44 = 0;
    inline static byte byte_1ADFA = 0;
    inline static byte byte_1AE0A = 0;
    inline static byte byte_1AE1B = 0;
    inline static Array<bool> affects_timed_out = Array<bool>(0x48); /* unk_1AE24 */

    inline static bool reload_ecl_and_pictures = false; // byte_1B2EB
    inline static byte head_block_id = 0; // byte_1B2EE
    inline static byte body_block_id = 0; // byte_1B2EF
    inline static bool party_killed = false; // byte_1B2F0

    inline static bool can_train_no_more = false; // byte_1D8B0
    inline static bool silent_training = false; // byte_1B2F1
    inline static bool DelayBetweenCharacters = false; // byte_1B2F2

    inline static byte byte_1BF12 = 0; // TODO remove or workout what it's was for?

    inline static SoundType soundType = SoundType::None; // byte_1BF14

    inline static bool gameSaved = false; // byte_1C01B
    inline static Array<byte> monoCharData = Array<byte>(8); // byte_1C8C2
    inline static int textXCol = 0; // byte_1C8CA
    inline static int textYCol = 0; // byte_1C8CB
    inline static int sortedCombatantCount = 0; // byte_1D1C0
    inline static Affects current_affect = (Affects)0; // byte_1D2BD
    inline static int damage = 0; // byte_1D2BE
    inline static DamageType damage_flags = (DamageType)0; // byte_1D2BF
    inline static int halfActionsLeft = 0; // byte_1D2C0
    inline static bool resetMovesLeft = false; // byte_1D2C4, reset_byte_1D2C0
    inline static int spell_id = 0; // byte_1D2C1
    inline static int dice_count = 0; // byte_1D2C2
    inline static bool targetInvisible = false; // byte_1D2C5
    inline static bool cureSpell = false; // byte_1D2C6
    inline static bool byte_1D2C7 = false;
    inline static bool byte_1D2C8 = false;
    inline static int attack_roll = 0; // byte_1D2C9

    inline static Array<byte> bytes_1D2C9 = Array<byte>(3); // byte_1D2CA = bytes_1D2C9[1] & byte_1D2CB = bytes_1D2C9[2]
    inline static int monster_morale = 0; // byte_1D2CC
    inline static int sky_colour = 0; // byte_1D534

    inline static bool mapAreaDisplay = false; //byte_1D538, Show Area Map
    inline static int mapPosX = 0; // byte_1D539, 0 map left, + map right
    inline static int mapPosY = 0; // byte_1D53A, 0 map top, +map bottom
    inline static byte mapDirection = 0; // byte_1D53B , 0 N, 2 E, 4 S, 6 W
    inline static byte mapWallType = 0; // byte_1D53C
    inline static byte mapWallRoof = 0; // byte_1D53D

    class SetBlock_ {
    public:
        SetBlock_() { Reset(); }
        SetBlock_(int _setId, int _blockId) { setId = _setId; blockId = _blockId; }
        void Reset() { setId = -1; blockId = -1; }
        int blockId = 0; // byte_1D53A[di] 1*4
        int setId = 0; // byte_1D53C[di] 1*4
    };
    using SetBlock = Ref<SetBlock_>;


    inline static Array<SetBlock> setBlocks = Array<SetBlock>(3);

    inline static DaxArray byte_1D556;
    inline static String lastDaxFile;
    inline static String byte_1D5AB;
    inline static byte lastDaxBlockId = 0; // byte_1D5B4
    inline static byte byte_1D5B5 = 0;
    inline static byte bigpic_block_id = 0; /* byte_1D5BA */
    inline static int menuSelectedWord = 0; // byte_1D5BE
    inline static bool displayInput_specialKeyPressed = false; // byte_1D5BF displayInput
    inline static Point targetPos; // byte_1D883 & byte_1D884
    inline static bool spell_from_item = false; // byte_1D88D
    inline static bool displayPlayerStatusLine18 = false; /* byte_1D8A8 */
    inline static bool can_draw_bigpic = false; // byte_1D8AA
    inline static bool applyItemAffect = false; // byte_1D8AC
    inline static int combat_round = 0; // byte_1D8B7
    inline static int combat_round_no_action_limit = 0; // byte_1D8B8
    static constexpr int combat_round_no_action_value = 15;


    inline static Array<byte> bytes_1D900 = Array<byte>(3); // byte_1D901 & byte_1D902
    inline static int enemyHealthPercentage = 0; /* byte_1D903 */
    inline static bool AutoPCsCastMagic = false; /* byte_1D904 magicOn */
    inline static bool byte_1D90E = false; // byte_1D90E
    inline static bool display_hitpoints_ac = false; /* byte_1D90F */
    inline static bool focusCombatAreaOnPlayer = false; // byte_1D910
    inline static byte sprite_block_id = 0; /* byte_1D92B */
    inline static byte pic_block_id = 0; /* byte_1D92C */
    inline static byte monster_icon_id = 0; // byte_1D92D

    inline static bool byte_1DA70 = false;

    inline static Array<bool> encounter_flags = Array<bool>(2); /* byte_1EE72 */
    inline static bool redrawPartySummary1 = false; // byte_1EE7C
    inline static bool redrawPartySummary2 = false; // byte_1EE7D
    inline static bool redrawBoarder = false; // byte_1EE7E
    inline static int partyAnimatedCount = 0; // byte_1EE81
    inline static bool battleWon = false; // byte_1EE86
    inline static byte EclBlockId = 0;
    inline static int search_flag_bkup = 0; // byte_1EE89
    inline static bool spriteChanged = false; // byte_1EE8C
    inline static bool byte_1EE8D = false;
    inline static bool displayPlayerSprite = false; /* byte_1EE8F */
    inline static bool bottomTextHasBeenCleared = false; // byte_1EE90
    inline static bool byte_1EE91 = false;
    inline static bool positionChanged = false; // byte_1EE92
    inline static bool monstersLoaded = false; // byte_1EE93
    inline static bool byte_1EE94 = false;
    inline static bool byte_1EE95 = false;
    inline static byte byte_1EE96 = 0;
    inline static bool player_not_found = false; // byte_1EE97
    inline static bool byte_1EE98 = false;
    inline static bool gameWon = false; // byte_1EE99


    inline static const Array<short> symbol_set_fix{ 0x0001, 0x002E, 0x0074, 0x00BA, 0x0100 };

    inline static ushort word_1AE0F = 0;
    inline static ushort word_1AE11 = 0;
    inline static ushort word_1AE13 = 0;
    inline static short word_1AE15 = 0;
    inline static ushort word_1AE17 = 0;
    inline static short word_1AE19 = 0;


    inline static ushort vm_run_addr_1 = 0; // word_1B2D3
    inline static ushort SearchLocationAddr = 0; // word_1B2D5 vm_run_addr_2
    inline static ushort PreCampCheckAddr = 0; // word_1B2D7 vm_run_addr_3
    inline static ushort CampInterruptedAddr = 0; // word_1B2D9 vm_run_addr_4
    inline static ushort ecl_initial_entryPoint = 0; // word_1B2DB
    inline static short rest_incounter_count = 0;
    inline static DaxBlock dword_1C8FC; //TODO - overlay dax block, not currently used.
    inline static DaxBlock bigpic_dax; /* word_1D5B6 */
    inline static int menuScreenIndex = 0;
    inline static int displayInputSecondsToWait = 0; // word_1D5C0 & word_1D5C2 - was centiseconds
    inline static char displayInputTimeoutValue = '\0'; // byte_1D5C4

    inline static Dictionary<Spells, spellDelegate2> spellTable = nullptr; // C# default null

    inline static Player lastSelectetSpellTarget; // dword_1D87F

    inline static RestTime timeToRest = New<RestTime_>(); // unk_1D890
    inline static int rest_10_seconds = 0; // word_1D8A6 seg600:7596
    inline static short word_1D914 = 0;
    inline static short word_1D916 = 0;
    inline static short word_1D918 = 0;

    inline static ushort word_1EE76 = 0;
    inline static ushort word_1EE78 = 0;
    inline static ushort word_1EE7A = 0;

    inline static short FIND_result = 0; // word_1EFBC

    inline static Array<Item> scribeScrolls = Array<Item>(0x30); // array 01-0x30; seg600:4C08 unk_1AF18
    inline static byte scribeScrollsCount = 0; // byte_1AFDC

    static constexpr int max_spells = 0x54;
    inline static Array<byte> memorize_spell_id = Array<byte>(max_spells); // unk_1AEC4 seg600:4BB4
    inline static Array<int> memorize_count = Array<int>(max_spells); /* unk_1AE70 */

    inline static Array<Struct_1ADF6> dword_1ADF6;
    inline static List<MenuItem> spell_string_list; // dword_1AE6C
    inline static DaxBlock cursor_bkup; // dword_1C8F4
    inline static DaxBlock dax24x24Set; //dword_1C8F8;
    inline static Item currentScroll; // dword_1D5C6
    inline static spellDelegate SpellCastFunction;
    inline static DaxBlock missile_dax; /* */
    inline static int exp_to_add = 0;

    inline static Stack<ushort> vmCallStack; // dword_1D91A

    inline static DaxBlock cursor; // dword_1EFA0

    inline static Player LastSelectedPlayer; // player_ptr2
    inline static Player SelectedPlayer; // player_ptr

    inline static GameState game_state = (GameState)0; // 1- shop, 5 - combat
    inline static GameState last_game_state = (GameState)0; // byte_1B2E4


    inline static CombatType combat_type = (CombatType)0;
    inline static ushort ecl_offset = 0;


    inline static byte command = 0;
    inline static List<Player> TeamList; // player_next_ptr

    inline static Item item_ptr; // rename current item
    inline static Player spell_target;
    inline static List<Player> spellTargets; /* sp_target */
    // 744Bh[1] == sp_target[0]
    inline static Array<Player> player_array = Array<Player>(256);

    static constexpr int MaxCombatantCount = 0xff; /* stru_1C9CD_count */
    inline static int CombatantCount = 0; // gbl.stru_1C9CD[0].field_3
    inline static Array<CombatantMap> CombatMap; // seg600:66BD stru_1C9CD

    inline static Player tradeWith; //player_ptr01

    inline static byte game_area = 0;
    inline static byte game_area_backup = 0;

    inline static Area1 area_ptr;
    inline static Area2 area2_ptr;

    inline static Struct_1B2CA stru_1B2CA;
    inline static EclBlock ecl_ptr;
    inline static Array2<byte> dax_8x8d1_201;
    inline static WallDefs wallDef = New<WallDefs_>();
    inline static GeoBlock geo_ptr = New<GeoBlock_>();

    inline static Array<CombatIcon> combat_icons;

    inline static Array<DaxBlock> symbol_8x8_set; // seg600:65D0 - seg600:65E3 DaxBlock[5]

    inline static List<Item> items_pointer = nullptr;

    inline static MoneySet pooled_money = New<MoneySet_>();


    /// <summary>
    /// 0 ==, 1 !=, 2 &lt;, 3 &gt;, 4 &lt;=, 5 &gt;=
    /// </summary>
    inline static Array<bool> compare_flags = Array<bool>(6); /*item_find*/

    inline static int game_speed_var = 0;

    inline static bool inDemo = false;
    inline static bool AnimationsOn = true;
    inline static bool PicsOn = true;

    inline static byte current_head_id = 0;
    inline static DaxBlock headX_dax;
    inline static byte current_body_id = 0;
    inline static DaxBlock bodyX_dax;

    inline static bool can_bash_door = false;
    inline static bool can_pick_door = false;
    inline static bool can_knock_door = false;


    inline static int savingThrowRoll = 0;
    inline static bool savingThrowMade = false;
    inline static SaveVerseType saveVerseType = (SaveVerseType)0; //byte_1D2D1

    inline static bool printCommands = false;

    inline static DaxBlock sky_dax_250;
    inline static DaxBlock sky_dax_251;
    inline static DaxBlock sky_dax_252;

    inline static Array<String> unk_1D972{ "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }; // new string[15] { string.Empty x15 }

    static constexpr int cmdOppsLimit = 0x40;
    inline static Array<Opperation> cmd_opps = Array<Opperation>(cmdOppsLimit);

    inline static Array<SpellEntry> spellCastingTable{ /* seg600:37DC asc_19AEC */
        nullptr,
        New<SpellEntry_>(1, SpellClass::Cleric,    1, 6, 0, 6, 0, 10, SpellTargets::WholeParty, DamageOnSave::Normal, SaveVerseType::Spell, Affects::bless, SpellWhen::Both, 10, 1, 0, 0),
        New<SpellEntry_>(2, SpellClass::Cleric,    1, 6, 0, 6, 0, 10, SpellTargets::Combat, DamageOnSave::Normal,  SaveVerseType::Spell, Affects::cursed, SpellWhen::Combat, 10, 3, 1, 0),
        New<SpellEntry_>(3, SpellClass::Cleric,    1, 0, 0, 0, 0, 4, SpellTargets::PartyMember, DamageOnSave::Normal,   SaveVerseType::Spell, Affects::none, SpellWhen::Both, 5, 1, 0, 0),
        New<SpellEntry_>(4, SpellClass::Cleric,    1, -1, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Normal,  SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 5, 2, 1, 0),
        New<SpellEntry_>(5, SpellClass::Cleric,    1, 3, 0, 10, 0, 0, SpellTargets::Self, DamageOnSave::Normal,  SaveVerseType::Spell, Affects::detect_magic, SpellWhen::Both, 1, 0, 0, 0),
        New<SpellEntry_>(6, SpellClass::Cleric,    1, 0, 0, 0, 3, 4, SpellTargets::PartyMember, DamageOnSave::Normal,   SaveVerseType::Spell, Affects::protection_from_evil, SpellWhen::Both, 4, 1, 0, 0),
        New<SpellEntry_>(7, SpellClass::Cleric,    1, 0, 0, 0, 3, 4, SpellTargets::PartyMember, DamageOnSave::Normal,   SaveVerseType::Spell, Affects::protection_from_good, SpellWhen::Both, 4, 1, 0, 0),
        New<SpellEntry_>(8, SpellClass::Cleric,    1, 0, 0, 0, 10, 4, SpellTargets::PartyMember, DamageOnSave::Normal,  SaveVerseType::Spell, Affects::resist_cold, SpellWhen::Both, 10, 0, 0, 0),
        New<SpellEntry_>(9, SpellClass::MagicUser, 1, 0, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 1, 2, 1, 0),
        New<SpellEntry_>(0xa, SpellClass::MagicUser, 1, 12, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::charm_person, SpellWhen::Combat, 1, 4, 1, 0),
        New<SpellEntry_>(0xb, SpellClass::MagicUser, 1, 0, 0, 0, 2, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::detect_magic, SpellWhen::Both, 1, 0, 0, 0),
        New<SpellEntry_>(0xc, SpellClass::MagicUser, 1, 0, 2, 0, 10, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::enlarge, SpellWhen::Both, 1, 0, 0, 0),
        New<SpellEntry_>(0xd, SpellClass::MagicUser, 1, 0, 2, 0, 10, 4, SpellTargets::PartyMember, DamageOnSave::Zero, SaveVerseType::Spell, Affects::reduce, SpellWhen::Both, 1, 0, 1, 0),
        New<SpellEntry_>(0xe, SpellClass::MagicUser, 1, 0, 0, 0, 1, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::friends, SpellWhen::Camp, 1, 0, 0, 0),
        New<SpellEntry_>(0xf, SpellClass::MagicUser, 1, 6, 4, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 1, 4, 1, 0),
        New<SpellEntry_>(0x10, SpellClass::MagicUser, 1, 0, 0, 0, 2, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::protection_from_evil, SpellWhen::Both, 1, 1, 0, 0),
        New<SpellEntry_>(0x11, SpellClass::MagicUser, 1, 0, 0, 0, 2, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::protection_from_good, SpellWhen::Both, 1, 1, 0, 0),
        New<SpellEntry_>(0x12, SpellClass::MagicUser, 1, 0, 0, 0, 2, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::read_magic, SpellWhen::Camp, 10, 0, 0, 0),
        New<SpellEntry_>(0x13, SpellClass::MagicUser, 1, 0, 0, 0, 5, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::shield, SpellWhen::Both, 1, 2, 0, 0),
        New<SpellEntry_>(0x14, SpellClass::MagicUser, 1, -1, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 1, 2, 1, 0),
        New<SpellEntry_>(0x15, SpellClass::MagicUser, 1, 3, 4, 0, 5, 9, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::sleep, SpellWhen::Combat, 1, 2, 1, 1),
        New<SpellEntry_>(0x16, SpellClass::Cleric,    2, 0, 0, 30, 0, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::find_traps, SpellWhen::Camp, 5, 0, 0, 0),
        New<SpellEntry_>(0x17, SpellClass::Cleric,    2, 6, 0, 4, 1, 6, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::paralyze, SpellWhen::Combat, 5, 6, 1, 0),
        New<SpellEntry_>(0x18, SpellClass::Cleric,    2, 0, 0, 0, 10, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::resist_fire, SpellWhen::Both, 5, 1, 0, 0),
        New<SpellEntry_>(0x19, SpellClass::Cleric,    2, 12, 0, 0, 2, 31, SpellTargets::Combat, DamageOnSave::Unknown_3, SaveVerseType::Spell, Affects::silence_15_radius, SpellWhen::Combat, 5, 4, 1, 1), /* 396C - */
        New<SpellEntry_>(0x1a, SpellClass::Cleric,    2, 0, 0, 0, 60, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::slow_poison, SpellWhen::Both, 1, 0, 0, 0),
        New<SpellEntry_>(0x1b, SpellClass::Cleric,    2, 3, 0, 0, 0, 240, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::snake_charm, SpellWhen::Combat, 5, 0, 1, 0),
        New<SpellEntry_>(0x1c, SpellClass::Cleric,    2, 3, 0, 0, 1, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::spiritual_hammer, SpellWhen::Both, 5, 1, 0, 0),
        New<SpellEntry_>(0x1d, SpellClass::MagicUser, 2, 0, 4, 0, 5, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::detect_invisibility, SpellWhen::Both, 2, 1, 0, 0),
        New<SpellEntry_>(0x1e, SpellClass::MagicUser, 2, 0, 0, 0, 0, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::invisibility, SpellWhen::Both, 2, 2, 0, 0),
        New<SpellEntry_>(0x1f, SpellClass::MagicUser, 2, 0, 0, 0, 0, 0, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Camp, 1, 0, 0, 0),
        New<SpellEntry_>(0x20, SpellClass::MagicUser, 2, 0, 0, 0, 2, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::mirror_image, SpellWhen::Both, 2, 3, 0, 0),
        New<SpellEntry_>(0x21, SpellClass::MagicUser, 2, 1, 1, 0, 1, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::ray_of_enfeeblement, SpellWhen::Combat, 2, 2, 1, 0),
        New<SpellEntry_>(0x22, SpellClass::MagicUser, 2, 3, 0, 0, 1, 9, SpellTargets::Combat, DamageOnSave::Unknown_3, SaveVerseType::Poison, Affects::stinking_cloud, SpellWhen::Combat, 2, 5, 1, 1),
        New<SpellEntry_>(0x23, SpellClass::MagicUser, 2, 0, 0, 0, 60, 0, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::strength, SpellWhen::Camp, 10, 0, 0, 0),
        New<SpellEntry_>(0x24, SpellClass::Monster,   7, 4, 0, 0, 0, 8, SpellTargets::WholeParty, DamageOnSave::Zero, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 0, 2, 0, 0),
        New<SpellEntry_>(0x25, SpellClass::Cleric,    3, 0, 0, 0, 0, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 10, 0, 0, 0),
        New<SpellEntry_>(0x26, SpellClass::Cleric,    3, -1, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::blinded, SpellWhen::Combat, 10, 3, 1, 0),
        New<SpellEntry_>(0x27, SpellClass::Cleric,    3, 0, 0, 0, 0, 0, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Camp, 100, 0, 0, 0),
        New<SpellEntry_>(0x28, SpellClass::Cleric,    3, -1, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::cause_disease_1, SpellWhen::Combat, 100, 4, 1, 0),
        New<SpellEntry_>(0x29, SpellClass::Cleric,    3, 6, 0, 0, 0, 9, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 4, 3, 1, 1),
        New<SpellEntry_>(0x2a, SpellClass::Cleric,    3, 0, 0, 0, 1, 0, SpellTargets::WholeParty, DamageOnSave::Normal, SaveVerseType::Spell, Affects::prayer, SpellWhen::Both, 6, 5, 0, 0),
        New<SpellEntry_>(0x2b, SpellClass::Cleric,    3, 0, 0, 0, 0, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 6, 0, 0, 0),
        New<SpellEntry_>(0x2c, SpellClass::Cleric,    3, -1, 0, 0, 10, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::bestow_curse, SpellWhen::Combat, 6, 5, 1, 0),
        New<SpellEntry_>(0x2d, SpellClass::MagicUser, 3, 0, 0, 0, 1, 0, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::blink, SpellWhen::Combat, 1, 2, 0, 0),
        New<SpellEntry_>(0x2e, SpellClass::MagicUser, 3, 12, 0, 0, 1, 9, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 3, 2, 1, 1),
        New<SpellEntry_>(0x2f, SpellClass::MagicUser, 3, 10, 1, 0, 0, 11, SpellTargets::Combat, DamageOnSave::Half, SaveVerseType::Spell, Affects::none,SpellWhen::Combat, 3, 7, 1, 3),
        New<SpellEntry_>(0x30, SpellClass::MagicUser, 3, 6, 0, 3, 1, 10, SpellTargets::WholeParty, DamageOnSave::Normal, SaveVerseType::Spell, Affects::haste, SpellWhen::Both, 3, 3, 0, 0),
        New<SpellEntry_>(0x31, SpellClass::MagicUser, 3, 12, 0, 0, 2, 7, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::paralyze, SpellWhen::Combat, 3, 6, 1, 0),
        New<SpellEntry_>(0x32, SpellClass::MagicUser, 3, 0, 0, 0, 0, 9, SpellTargets::WholeParty, DamageOnSave::Normal, SaveVerseType::Spell, Affects::invisibility, SpellWhen::Both, 3, 1, 0, 0),
        New<SpellEntry_>(0x33, SpellClass::MagicUser, 3, 4, 1, 0, 0, 8, SpellTargets::Combat, DamageOnSave::Half, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 3, 6, 1, 0),
        New<SpellEntry_>(0x34, SpellClass::MagicUser, 3, 0, 0, 0, 2, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::prot_from_evil_10_radius, SpellWhen::Both, 3, 1, 0, 0),
        New<SpellEntry_>(0x35, SpellClass::MagicUser, 3, 0, 0, 0, 2, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::prot_from_good_10_radius, SpellWhen::Both, 3, 2, 0, 0),
        New<SpellEntry_>(0x36, SpellClass::MagicUser, 3, 0, 0, 0, 10, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::prot_from_normal_missiles, SpellWhen::Both, 3, 3, 0, 0),
        New<SpellEntry_>(0x37, SpellClass::MagicUser, 3, 9, 1, 3, 1, 10, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::slow, SpellWhen::Combat, 3, 4, 1, 0),
        New<SpellEntry_>(0x38, SpellClass::Cleric,    7, 0, 0, 0, 0, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 6, 0, 0, 0),
        New<SpellEntry_>(0x39, SpellClass::Monster,   6, 0, 0, 0, 0, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::haste, SpellWhen::Both, 0, 3, 0, 0),
        New<SpellEntry_>(0x3a, SpellClass::Cleric,    4, 0, 0, 0, 0, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 7, 1, 0, 0),
        New<SpellEntry_>(0x3b, SpellClass::Monster,   6, 0, 0, 0, 0, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::strength, SpellWhen::Both, 0, 1, 0, 0),
        New<SpellEntry_>(0x3c, SpellClass::Monster,   6, 4, 4, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Half, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 0, 7, 1, 0),
        New<SpellEntry_>(0x3d, SpellClass::Monster,   6, 6, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Poison, Affects::paralyze, SpellWhen::Combat, 0, 7, 1, 0),
        New<SpellEntry_>(0x3e, SpellClass::Monster,   6, 0, 0, 0, 0, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::haste, SpellWhen::Both, 0, 1, 0, 0),
        New<SpellEntry_>(0x3f, SpellClass::Monster,   6, 0, 0, 0, 0, 7, SpellTargets::WholeParty, DamageOnSave::Normal, SaveVerseType::Spell, Affects::invisible, SpellWhen::Both, 0, 2, 0, 0),
        New<SpellEntry_>(0x40, SpellClass::Monster,   6, 7, 0, 0, 0, 11, SpellTargets::Combat, DamageOnSave::Half, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 0, 7, 1, 3),
        New<SpellEntry_>(0x41, SpellClass::Monster,   6, 12, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 0, 6, 1, 0),
        New<SpellEntry_>(0x42, SpellClass::Cleric,    4, 0, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 7, 5, 1, 0),
        New<SpellEntry_>(0x43, SpellClass::Cleric,    4, 0, 0, 0, 0, 0, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Camp, 7, 0, 0, 0),
        New<SpellEntry_>(0x44, SpellClass::Cleric,    4, 0, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Poison, Affects::none, SpellWhen::Combat, 7, 6, 1, 0),
        New<SpellEntry_>(0x45, SpellClass::Cleric,    4, 3, 0, 0, 10, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::prot_from_evil_10_radius, SpellWhen::Both, 7, 2, 0, 0),
        New<SpellEntry_>(0x46, SpellClass::Cleric,    4, 3, 0, 0, 2, 4, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::sticks_to_snakes, SpellWhen::Combat, 7, 4, 1, 0),
        New<SpellEntry_>(0x47, SpellClass::Cleric,    5, 0, 0, 0, 0, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 8, 2, 0, 0),
        New<SpellEntry_>(0x48, SpellClass::Cleric,    5, -1, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 8, 6, 1, 0),
        New<SpellEntry_>(0x49, SpellClass::Cleric,    5, 0, 0, 0, 1, 0, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::sp_dispel_evil, SpellWhen::Combat, 8, 3, 0, 0),
        New<SpellEntry_>(0x4a, SpellClass::Cleric,    5, 6, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Half, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 8, 6, 1, 0),
        New<SpellEntry_>(0x4b, SpellClass::Cleric,    5, 0, 0, 0, 0, 0, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Camp, 10, 1, 0, 0),
        New<SpellEntry_>(0x4c, SpellClass::Cleric,    5, 3, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 10, 7, 1, 0),
        New<SpellEntry_>(0x4d, SpellClass::Druid,     1, 3, 0, 12, 0, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::detect_magic, SpellWhen::Both, 3, 1, 0, 0),
        New<SpellEntry_>(0x4e, SpellClass::Druid,     1, 8, 0, 10, 0, 11, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::entangle, SpellWhen::Combat, 3, 3, 1, 0),
        New<SpellEntry_>(0x4f, SpellClass::Druid,     1, 8, 0, 0, 4, 5, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::faerie_fire, SpellWhen::Combat, 3, 4, 1, 0),
        New<SpellEntry_>(0x50, SpellClass::Druid,     1, -1, 0, 10, 1, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::invisible_to_animals, SpellWhen::Both, 4, 1, 1, 0),
        New<SpellEntry_>(0x51, SpellClass::MagicUser, 4, 6, 0, 0, 0, 5, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::charm_person, SpellWhen::Combat, 4, 6, 1, 0),
        New<SpellEntry_>(0x52, SpellClass::MagicUser, 4, 12, 0, 2, 1, 11, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::confuse, SpellWhen::Combat, 4, 7, 1, 0),
        New<SpellEntry_>(0x53, SpellClass::MagicUser, 4, 0, 3, 0, 0, 8, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 1, 0, 1, 0),
        New<SpellEntry_>(0x54, SpellClass::MagicUser, 4, 6, 0, 0, 1, 8, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::fear, SpellWhen::Combat, 4, 6, 1, 0),
        New<SpellEntry_>(0x55, SpellClass::MagicUser, 4, 0, 0, 2, 1, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 4, 8, 0, 0),
        New<SpellEntry_>(0x56, SpellClass::MagicUser, 4, 0, 1, 0, 1, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::fumbling, SpellWhen::Combat, 4, 4, 1, 0),
        New<SpellEntry_>(0x57, SpellClass::MagicUser, 4, 0, 1, 0, 0, 10, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 4, 7, 1, 0),
        New<SpellEntry_>(0x58, SpellClass::MagicUser, 4, 0, 1, 0, 1, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::minor_globe_of_invulnerability, SpellWhen::Both, 4, 5, 0, 0),
        New<SpellEntry_>(0x59, SpellClass::MagicUser, 4, 0, 0, 0, 0, 4, SpellTargets::PartyMember, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 4, 0, 0, 0),
        New<SpellEntry_>(0x5a, SpellClass::Monster,   5, 1, 0, 0, 0, 240, SpellTargets::WholeParty, DamageOnSave::Normal, SaveVerseType::Spell, Affects::animate_dead, SpellWhen::Both, 5, 0, 0, 0),
        New<SpellEntry_>(0x5b, SpellClass::MagicUser, 5, 2, 0, 0, 1, 9, SpellTargets::Combat, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 5, 5, 1, 0),
        New<SpellEntry_>(0x5c, SpellClass::MagicUser, 5, 6, 0, 0, 0, 8, SpellTargets::Combat, DamageOnSave::Half, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 5, 6, 1, 0),
        New<SpellEntry_>(0x5d, SpellClass::MagicUser, 5, 16, 0, 0, 0, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::feeblemind, SpellWhen::Combat, 5, 6, 1, 0),
        New<SpellEntry_>(0x5e, SpellClass::MagicUser, 5, 0, 1, 0, 1, 7, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::paralyze, SpellWhen::Combat, 5, 7, 1, 0),
        New<SpellEntry_>(0x5f, SpellClass::Monster,   6, 0, 0, 0, 0, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::prot_drag_breath, SpellWhen::Both, 10, 1, 0, 0),
        New<SpellEntry_>(0x60, SpellClass::Monster,   6, 0, 0, 0, 0, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::resist_paralyze, SpellWhen::Both, 10, 1, 0, 0),
        New<SpellEntry_>(0x61, SpellClass::Monster,   6, 0, 0, 0, 0, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::invisibility, SpellWhen::Both, 0, 1, 0, 0),
        New<SpellEntry_>(0x62, SpellClass::Monster,   6, 3, 0, 0, 0, 11, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 0, 1, 1, 0),
        New<SpellEntry_>(0x63, SpellClass::Monster,   6, 0, 0, 0, 0, 0, SpellTargets::Self, DamageOnSave::Normal, SaveVerseType::Spell, Affects::none, SpellWhen::Both, 0, 1, 0, 0),
        New<SpellEntry_>(0x64, SpellClass::MagicUser, 4, 0, 0, 0, 10, 4, SpellTargets::Combat, DamageOnSave::Zero, SaveVerseType::Spell, Affects::none, SpellWhen::Combat, 4, 4, 1, 0),
        New<SpellEntry_>(0x65, SpellClass::Unknown10, 0, 10, 0, 6, 0, 24, SpellTargets::Combat, DamageOnSave::Unknown_1E, SaveVerseType::Poison, Affects::enlarge, SpellWhen::Camp, 0, 1, 0x28, 0x28) };

    inline static Classes::ItemDataTable ItemDataTable; // unk_1C020 (type qualified: member shares the type's name)

    inline static List<DownedPlayerTile> downedPlayers = nullptr; // unk_1D183

    inline static Struct_1D1BC mapToBackGroundTile; // stru_1D1BC
    static constexpr int Tile_Table = 0x1A;
    static constexpr int Tile_Chair = 0x1B;
    static constexpr int Tile_CloudKill = 0x1C;
    static constexpr int Tile_StinkingCloud = 0x1E;
    static constexpr int Tile_DownPlayer = 0x1F;

    inline static const Array<byte> /*seg600:27D9*/ SmallCloudDirections{ 8, 2, 3, 4 }; // unk_18AE9 used by NoxiousCloud
    inline static const Array<byte> unk_18AEA{ 2, 3, 4, 8 }; /*seg600:27DA*/

    inline static const Array<byte> /* seg600:27DD */ CloudDirections{ 8, 0, 1, 2, 3, 4, 5, 6, 7 }; // unk_18AED used by CloudKill

    inline static List<GasCloud> StinkingCloud = nullptr; // stru_1D885
    inline static List<GasCloud> CloudKillCloud = nullptr; // stru_1D889

    inline static Array<Point> MapDirectionDelta{ Point(0, -1), Point(1, -1), Point(1, 0), Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0), Point(-1, -1), Point(0, 0) };
    inline static const Array<sbyte> /*unk_189A6 seg600:2696*/ MapDirectionXDelta{ 0, 1, 1, 1, 0, -1, -1, -1, 0 }; //TODO remove
    inline static const Array<sbyte> /*unk_189AF seg600:269F*/ MapDirectionYDelta{ -1, -1, 0, 1, 1, 1, 0, -1, 0 }; //TODO remove

    inline static ImportSource import_from = (ImportSource)0;
    inline static bool party_fled = false;

    inline static int friends_count = 0;
    inline static int foe_count = 0;


    inline static Array<ClassStatsMin> class_stats_min{ // unk_1A484
        New<ClassStatsMin_>(6, 6, 9, 0, 0, 0),
        New<ClassStatsMin_>(0, 0, 0xC, 0, 0, 0xF),
        New<ClassStatsMin_>(9, 0, 6, 6, 7, 0),
        New<ClassStatsMin_>(0xC, 9, 0xD, 0, 9, 0x11),
        New<ClassStatsMin_>(0xD, 0xD, 0xE, 0, 0xE, 0),
        New<ClassStatsMin_>(0, 9, 6, 6, 0, 0),
        New<ClassStatsMin_>(6, 6, 0, 9, 0, 0),
        New<ClassStatsMin_>(0xF, 0, 0xF, 0xF, 0xB, 0),
        New<ClassStatsMin_>(9, 0, 9, 0, 0, 0),
        New<ClassStatsMin_>(9, 9, 9, 0, 0, 0),
        New<ClassStatsMin_>(0, 0xD, 0xE, 0, 0xE, 0),
        New<ClassStatsMin_>(0, 9, 9, 0, 0, 0),
        New<ClassStatsMin_>(0, 0, 9, 9, 0, 0),
        New<ClassStatsMin_>(9, 9, 0, 0, 0, 0),
        New<ClassStatsMin_>(9, 0, 0, 9, 0, 0),
        New<ClassStatsMin_>(9, 9, 0, 9, 0, 0),
        New<ClassStatsMin_>(0, 9, 0, 9, 0, 0) };

    inline static String exe_path; // unk_1B21A
    inline static String data_path; // unk_1B26A


private:
    inline static Struct_1A35E monster_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 6, 2, 6),
        New<SubStruct_1A35E_>( 0x0c08, 0xe, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 6, 0),
        New<SubStruct_1A35E_>( 0x502, 6, 3),
        New<SubStruct_1A35E_>( 4, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0)});

    inline static Struct_1A35E dwarf_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0xfa, 2, 0x14),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x28, 5, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x4B, 3, 6)});

    inline static Struct_1A35E elf_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0x28a, 10, 10),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x82, 5, 6),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x96, 5, 6),
        New<SubStruct_1A35E_>( 100, 5, 6)});

    inline static Struct_1A35E gnome_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0x12C, 3, 0xC),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x3C, 5, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 100, 2, 0x0C),
        New<SubStruct_1A35E_>( 0x50, 5, 4)});

    inline static Struct_1A35E halfelf_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0x28, 2, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x16, 3, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x1E, 2,  8),
        New<SubStruct_1A35E_>( 0x16, 3, 8)});

    inline static Struct_1A35E halfling_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 20, 3, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x28, 2, 4)});

    inline static Struct_1A35E halforc_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 20, 1, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0xD, 1, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 20, 2, 4)});

    inline static Struct_1A35E human_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 18, 1, 4),
        New<SubStruct_1A35E_>( 18, 1, 4),
        New<SubStruct_1A35E_>( 15, 1, 4),
        New<SubStruct_1A35E_>( 17, 1, 4),
        New<SubStruct_1A35E_>( 20, 1, 4),
        New<SubStruct_1A35E_>( 0x18, 2, 4),
        New<SubStruct_1A35E_>( 18 , 1, 4)});

public:
    inline static Array<Struct_1A35E> race_ages{ monster_ages, dwarf_ages, elf_ages, gnome_ages, halfelf_ages, halfling_ages, halforc_ages, human_ages }; // unk_1A35E

    inline static Array<byte> unk_1AE0B = Array<byte>(3);


    inline static const Array2<byte> class_alignments{ // unk_1A4EA
        { 9,0,1,2,3,4,5,6,7,8},
        { 5,1,3,4,5,7,0,0,0,0},
        { 9,0,1,2,3,4,5,6,7,8},
        { 1,0,0,0,0,0,0,0,0,0},
        { 3,0,3,6,0,0,0,0,0,0},
        { 9,0,1,2,3,4,5,6,7,8},
        { 7,1,2,3,4,5,7,8,0,0},
        { 9,0,1,2,3,4,5,6,7,8},
        { 9,0,1,2,3,4,5,6,7,8},
        { 9,0,1,2,3,4,5,6,7,8},
        { 3,0,3,6,0,0,0,0,0,0},
        { 9,0,1,2,3,4,5,6,7,8},
        { 9,0,1,2,3,4,5,6,7,8},
        { 9,0,1,2,3,4,5,6,7,8},
        { 7,1,2,3,4,5,7,8,0,0},
        { 7,1,2,3,4,5,7,8,0,0},
        { 7,1,2,3,4,5,7,8,0,0} };


};

}  // namespace Classes

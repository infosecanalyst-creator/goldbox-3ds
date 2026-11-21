// Classes/Gbl.h - port of class gbl from Classes/Gbl.cs (static-only).
// The enums, delegates, MenuColorSet and Point from the top of Gbl.cs live in
// Classes/GblTypes.h (included here).
// All tables are copied verbatim from Gbl.cs.
// PORT-NOTE (GB-08): the non-trivial table initialisers moved to statics_classes.cpp / statics_engine.cpp (one copy of the init code instead of one per translation unit). They are
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

    static MenuColorSet defaultMenuColors;
    static MenuColorSet alertMenuColors;

    static constexpr int BackGroundTiles_count = 64; /* 64 is a guess */
    static Array<Struct_189B4> BackGroundTiles;


    static const Array<byte> max_class_hit_dice; // byte_1A1CB seg600:3EBB
    static const Array<byte> default_icon_colours; // unk_1A1D3[0] == unk_1A1D2[1];

    static const Array<Array<ClassId>> RaceClasses;


    inline static bool stopVM = false; //byte_1AB08
    inline static bool vmFlag01 = false; // byte_1AB09
    inline static bool restore_player_ptr = false; // byte_1AB0A
    inline static bool byte_1AB0B = false;
    inline static bool byte_1AB0C = false;
    inline static bool filesLoaded = false; // byte_1AB0D
    inline static byte numLoadedMonsters = 0; // byte_1AB0E
    inline static bool byte_1AB14 = false;
    inline static bool shopRedrawMenuItems = false; // byte_1AB16
    static Array<int> team_start_x; /* byte_1AD2C */
    static Array<int> team_start_y; /* byte_1AD2E */
    static Array<int> half_team_count; /* unk_1AD30 */
    static Array<int> team_direction; /* byte_1AD32 */
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
    static Array<bool> affects_timed_out; /* unk_1AE24 */

    inline static bool reload_ecl_and_pictures = false; // byte_1B2EB
    inline static byte head_block_id = 0; // byte_1B2EE
    inline static byte body_block_id = 0; // byte_1B2EF
    inline static bool party_killed = false; // byte_1B2F0

    inline static bool can_train_no_more = false; // byte_1D8B0
    inline static bool silent_training = false; // byte_1B2F1
    inline static bool DelayBetweenCharacters = false; // byte_1B2F2

    inline static byte byte_1BF12 = 0; // TODO remove or workout what it's was for?

    static SoundType soundType; // byte_1BF14

    inline static bool gameSaved = false; // byte_1C01B
    static Array<byte> monoCharData; // byte_1C8C2
    inline static int textXCol = 0; // byte_1C8CA
    inline static int textYCol = 0; // byte_1C8CB
    inline static int sortedCombatantCount = 0; // byte_1D1C0
    static Affects current_affect; // byte_1D2BD
    inline static int damage = 0; // byte_1D2BE
    static DamageType damage_flags; // byte_1D2BF
    inline static int halfActionsLeft = 0; // byte_1D2C0
    inline static bool resetMovesLeft = false; // byte_1D2C4, reset_byte_1D2C0
    inline static int spell_id = 0; // byte_1D2C1
    inline static int dice_count = 0; // byte_1D2C2
    inline static bool targetInvisible = false; // byte_1D2C5
    inline static bool cureSpell = false; // byte_1D2C6
    inline static bool byte_1D2C7 = false;
    inline static bool byte_1D2C8 = false;
    inline static int attack_roll = 0; // byte_1D2C9

    static Array<byte> bytes_1D2C9; // byte_1D2CA = bytes_1D2C9[1] & byte_1D2CB = bytes_1D2C9[2]
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


    static Array<SetBlock> setBlocks;

    static DaxArray byte_1D556;
    static String lastDaxFile;
    static String byte_1D5AB;
    inline static byte lastDaxBlockId = 0; // byte_1D5B4
    inline static byte byte_1D5B5 = 0;
    inline static byte bigpic_block_id = 0; /* byte_1D5BA */
    inline static int menuSelectedWord = 0; // byte_1D5BE
    // goldbox-3ds addition: port-only conveniences (clear font, title menu
    // defaults to Play). Set by every host for normal play; stays false in the
    // coab differential tests so those still match byte for byte.
    inline static bool portEnhancements = false;
    inline static bool displayInput_specialKeyPressed = false; // byte_1D5BF displayInput
    static Point targetPos; // byte_1D883 & byte_1D884
    inline static bool spell_from_item = false; // byte_1D88D
    inline static bool displayPlayerStatusLine18 = false; /* byte_1D8A8 */
    inline static bool can_draw_bigpic = false; // byte_1D8AA
    inline static bool applyItemAffect = false; // byte_1D8AC
    inline static int combat_round = 0; // byte_1D8B7
    inline static int combat_round_no_action_limit = 0; // byte_1D8B8
    static constexpr int combat_round_no_action_value = 15;


    static Array<byte> bytes_1D900; // byte_1D901 & byte_1D902
    inline static int enemyHealthPercentage = 0; /* byte_1D903 */
    inline static bool AutoPCsCastMagic = false; /* byte_1D904 magicOn */
    inline static bool byte_1D90E = false; // byte_1D90E
    inline static bool display_hitpoints_ac = false; /* byte_1D90F */
    inline static bool focusCombatAreaOnPlayer = false; // byte_1D910
    inline static byte sprite_block_id = 0; /* byte_1D92B */
    inline static byte pic_block_id = 0; /* byte_1D92C */
    inline static byte monster_icon_id = 0; // byte_1D92D

    inline static bool byte_1DA70 = false;

    static Array<bool> encounter_flags; /* byte_1EE72 */
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


    static const Array<short> symbol_set_fix;

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
    static DaxBlock dword_1C8FC; //TODO - overlay dax block, not currently used.
    static DaxBlock bigpic_dax; /* word_1D5B6 */
    inline static int menuScreenIndex = 0;
    inline static int displayInputSecondsToWait = 0; // word_1D5C0 & word_1D5C2 - was centiseconds
    static char displayInputTimeoutValue; // byte_1D5C4

    static Dictionary<Spells, spellDelegate2> spellTable; // C# default null

    static Player lastSelectetSpellTarget; // dword_1D87F

    static RestTime timeToRest; // unk_1D890
    inline static int rest_10_seconds = 0; // word_1D8A6 seg600:7596
    inline static short word_1D914 = 0;
    inline static short word_1D916 = 0;
    inline static short word_1D918 = 0;

    inline static ushort word_1EE76 = 0;
    inline static ushort word_1EE78 = 0;
    inline static ushort word_1EE7A = 0;

    inline static short FIND_result = 0; // word_1EFBC

    static Array<Item> scribeScrolls; // array 01-0x30; seg600:4C08 unk_1AF18
    inline static byte scribeScrollsCount = 0; // byte_1AFDC

    static constexpr int max_spells = 0x54;
    static Array<byte> memorize_spell_id; // unk_1AEC4 seg600:4BB4
    static Array<int> memorize_count; /* unk_1AE70 */

    static Array<Struct_1ADF6> dword_1ADF6;
    static List<MenuItem> spell_string_list; // dword_1AE6C
    static DaxBlock cursor_bkup; // dword_1C8F4
    static DaxBlock dax24x24Set; //dword_1C8F8;
    static Item currentScroll; // dword_1D5C6
    static spellDelegate SpellCastFunction;
    static DaxBlock missile_dax; /* */
    inline static int exp_to_add = 0;

    static Stack<ushort> vmCallStack; // dword_1D91A

    static DaxBlock cursor; // dword_1EFA0

    static Player LastSelectedPlayer; // player_ptr2
    static Player SelectedPlayer; // player_ptr

    static GameState game_state; // 1- shop, 5 - combat
    static GameState last_game_state; // byte_1B2E4


    static CombatType combat_type;
    inline static ushort ecl_offset = 0;


    inline static byte command = 0;
    static List<Player> TeamList; // player_next_ptr

    static Item item_ptr; // rename current item
    static Player spell_target;
    static List<Player> spellTargets; /* sp_target */
    // 744Bh[1] == sp_target[0]
    static Array<Player> player_array;

    static constexpr int MaxCombatantCount = 0xff; /* stru_1C9CD_count */
    inline static int CombatantCount = 0; // gbl.stru_1C9CD[0].field_3
    static Array<CombatantMap> CombatMap; // seg600:66BD stru_1C9CD

    static Player tradeWith; //player_ptr01

    inline static byte game_area = 0;
    inline static byte game_area_backup = 0;

    static Area1 area_ptr;
    static Area2 area2_ptr;

    static Struct_1B2CA stru_1B2CA;
    static EclBlock ecl_ptr;
    static Array2<byte> dax_8x8d1_201;
    static WallDefs wallDef;
    static GeoBlock geo_ptr;

    static Array<CombatIcon> combat_icons;

    static Array<DaxBlock> symbol_8x8_set; // seg600:65D0 - seg600:65E3 DaxBlock[5]

    static List<Item> items_pointer;

    static MoneySet pooled_money;


    /// <summary>
    /// 0 ==, 1 !=, 2 &lt;, 3 &gt;, 4 &lt;=, 5 &gt;=
    /// </summary>
    static Array<bool> compare_flags; /*item_find*/

    inline static int game_speed_var = 0;

    inline static bool inDemo = false;
    inline static bool AnimationsOn = true;
    inline static bool PicsOn = true;

    inline static byte current_head_id = 0;
    static DaxBlock headX_dax;
    inline static byte current_body_id = 0;
    static DaxBlock bodyX_dax;

    inline static bool can_bash_door = false;
    inline static bool can_pick_door = false;
    inline static bool can_knock_door = false;


    inline static int savingThrowRoll = 0;
    inline static bool savingThrowMade = false;
    static SaveVerseType saveVerseType; //byte_1D2D1

    inline static bool printCommands = false;

    static DaxBlock sky_dax_250;
    static DaxBlock sky_dax_251;
    static DaxBlock sky_dax_252;

    static Array<String> unk_1D972; // new string[15] { string.Empty x15 }

    static constexpr int cmdOppsLimit = 0x40;
    static Array<Opperation> cmd_opps;

    static Array<SpellEntry> spellCastingTable;

    static Classes::ItemDataTable ItemDataTable; // unk_1C020 (type qualified: member shares the type's name)

    static List<DownedPlayerTile> downedPlayers; // unk_1D183

    static Struct_1D1BC mapToBackGroundTile; // stru_1D1BC
    static constexpr int Tile_Table = 0x1A;
    static constexpr int Tile_Chair = 0x1B;
    static constexpr int Tile_CloudKill = 0x1C;
    static constexpr int Tile_StinkingCloud = 0x1E;
    static constexpr int Tile_DownPlayer = 0x1F;

    static const Array<byte> SmallCloudDirections; // unk_18AE9 used by NoxiousCloud
    static const Array<byte> unk_18AEA; /*seg600:27DA*/

    static const Array<byte> CloudDirections; // unk_18AED used by CloudKill

    static List<GasCloud> StinkingCloud; // stru_1D885
    static List<GasCloud> CloudKillCloud; // stru_1D889

    static Array<Point> MapDirectionDelta;
    static const Array<sbyte> MapDirectionXDelta; //TODO remove
    static const Array<sbyte> MapDirectionYDelta; //TODO remove

    static ImportSource import_from;
    inline static bool party_fled = false;

    inline static int friends_count = 0;
    inline static int foe_count = 0;


    static Array<ClassStatsMin> class_stats_min;

    static String exe_path; // unk_1B21A
    static String data_path; // unk_1B26A


private:
    static Struct_1A35E monster_ages;

    static Struct_1A35E dwarf_ages;

    static Struct_1A35E elf_ages;

    static Struct_1A35E gnome_ages;

    static Struct_1A35E halfelf_ages;

    static Struct_1A35E halfling_ages;

    static Struct_1A35E halforc_ages;

    static Struct_1A35E human_ages;

public:
    static Array<Struct_1A35E> race_ages; // unk_1A35E

    static Array<byte> unk_1AE0B;


    static const Array2<byte> class_alignments;


};

}  // namespace Classes

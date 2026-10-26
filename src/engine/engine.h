// engine/engine.h - declarations for every class in coab's `engine` assembly
// (engine/*.cs), translated per docs/TRANSLATION.md. Each engine/ovrNNN.cpp /
// segNNN.cpp defines the methods declared here. Generated from the C# sources
// (member order preserved) and then hand-edited where marked PORT-NOTE.
// See src/engine/ENGINE_NOTES.md.
//
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#pragma once

#include "cs/cs.h"
#include "cs/io.h"
#include "Logging/Logger.h"
#include "Classes/Classes.h"

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>

namespace engine {
using namespace cs;
using namespace cs::enum_ops;
using namespace Classes;
using namespace Classes::Combat;
using namespace Classes::DaxFiles;

// ---------------------------------------------------------------------------
// Array4<T>: C# T[,,,] (only ovr011.unk_1AB1C). Same conventions as cs::Array3:
// reference semantics, default == null, index as a(i, j, k, l).
// PORT-NOTE: engine-local because cs.h has no 4-D array.
// ---------------------------------------------------------------------------
template <class T>
class Array4 {
    std::shared_ptr<std::vector<T>> p;
    int d0 = 0, d1 = 0, d2 = 0, d3 = 0;

public:
    Array4() = default;
    Array4(std::nullptr_t) {}
    Array4(int n0, int n1, int n2, int n3)
        : p(std::make_shared<std::vector<T>>((std::size_t)(n0 * n1 * n2 * n3))), d0(n0), d1(n1), d2(n2), d3(n3) {}
    bool IsNull() const { return !p; }
    friend bool operator==(const Array4& a, std::nullptr_t) { return !a.p; }
    friend bool operator!=(const Array4& a, std::nullptr_t) { return (bool)a.p; }
    int GetLength(int dim) const { return dim == 0 ? d0 : (dim == 1 ? d1 : (dim == 2 ? d2 : d3)); }
    int Length() const { return d0 * d1 * d2 * d3; }
    T& operator()(int i, int j, int k, int l) const {
        if (!p) throw NullReferenceException("Array4");
        if (i < 0 || i >= d0 || j < 0 || j >= d1 || k < 0 || k >= d2 || l < 0 || l >= d3) throw IndexOutOfRangeException();
        return (*p)[(std::size_t)(((i * d1 + j) * d2 + k) * d3 + l)];
    }
};

// ---------------------------------------------------------------------------
// C# `object` appears only as the 2nd parameter of affectDelegate-shaped methods
// (Effect, object, Player) in ovr013/ovr014/ovr023. It must match the parameter
// type of Classes::affectDelegate (Gbl.h). PORT-NOTE: `(Affect)param` ->
// `std::static_pointer_cast<Affect_>(param)`, `(Item)param` likewise; passing an
// Affect/Item/nullptr converts implicitly.
// ---------------------------------------------------------------------------
using object = std::shared_ptr<void>;

// ---------------------------------------------------------------------------
// Engine-defined top-level types (hoisted ahead of the ovr/seg structs).
// ---------------------------------------------------------------------------

// ovr020.cs
enum class SpellLoc : int {
    memory = 0,
    grimoire = 1,
    scroll = 2,
    scrolls = 3,
    choose = 4,
    memorize = 5,
    scribe = 6
};

// ovr024.cs
enum class CheckType : int {
    None = 0,
    Visibility = 1,
    Type_2 = 2,
    Type_3 = 3,
    SpecialAttacks = 4,
    Type_5 = 5,
    PreDamage = 6,
    PlayerRestrained = 7,
    Type_8 = 8,
    MagicResistance = 9,
    Type_10 = 10,
    Type_11 = 11,
    SavingThrow = 12,
    Death = 13,
    Type_14 = 14,
    Type_15 = 15,
    Type_16 = 16,
    Morale = 17,
    Movement = 18,
    Type_19 = 19,
    FireShield = 20,
    Confusion = 21,
    Type_22 = 22,
    Type_23 = 23
};

// VmOpp.cs
class MemLoc_ {
public:
    ushort loc = 0;
    MemLoc_(ushort _loc);
    String ToString() const;
};
using MemLoc = cs::Ref<MemLoc_>;

// ovr003.cs
class CmdItem_ {
public:
    using CmdDelegate = std::function<void()>;
    int size = 0;
    String name;
    CmdDelegate cmd;
    CmdItem_(int Size, String Name, CmdDelegate Cmd);
    void Run();
    String Name();
    void Skip();
};
using CmdItem = cs::Ref<CmdItem_>;

// VmOpp.cs: static class VmLog. `params object[] args` forwarded to Logger -> variadic
// templates, so they must be defined here (bodies are a verbatim translation).
struct VmLog {
    template <class... A>
    static void Write(const String& fmt, const A&... args) {
        if (gbl::printCommands == true) {
            Logging::Logger::DebugWrite(fmt, args...);
        }
    }

    template <class... A>
    static void WriteLine(const String& fmt, const A&... args) {
        if (gbl::printCommands == true) {
            Logging::Logger::Debug(fmt, args...);
        }
    }
};

// ---------------------------------------------------------------------------
// ovr002.cs
// ---------------------------------------------------------------------------
struct ovr002 {
    static void delay_or_key(int seconds);
    static void credits();
    static void title_screen();
};

// ---------------------------------------------------------------------------
// ovr003.cs
// ---------------------------------------------------------------------------
struct ovr003 {
    static void CMD_Exit();
    static void CMD_Goto();
    static void CMD_Gosub();
    static void CMD_Compare(); // sub_2611D
    static void CMD_AddSubDivMulti(); // sub_2619A
    static void CMD_Random(); // sub_2623D
    static void CMD_Save();
    static void CMD_LoadCharacter(); /* sub_262E9 */
    static void CMD_SetupMonster(); /* sub_263C9 */
    static void CMD_LoadMonster(); /* sub_26465 */
    static void CMD_Approach(); // sub_26835
    static void CMD_Picture(); /* sub_26873 */
    static void CMD_InputNumber(); /* sub_2695E */
    static void CMD_InputString(); /* sub_269A4 */
    static void CMD_Print();
    static void CMD_Return();
    static void CMD_CompareAnd(); /* sub_26B0C */
    static void CMD_If();
    static void CMD_NewECL();
    static void CMD_LoadFiles(); /* sub_26C41 */
    static void CMD_AndOr(); /* sub_26DD0 */
    static void CMD_GetTable(); /* sub_26E3F */
    static void CMD_SaveTable(); /* sub_26E9D */
    static void CMD_VertMenu(); /* sub_26EE9 */
    static void CMD_HorizontalMenu();
    static void CMD_ClearMonsters(); /* sub_27240 */
    static void CMD_PartyStrength(); /* sub_272A9 */
    static void setMemoryFour(bool val_d, byte val_c, byte val_b, byte val_a, ushort loc_a, ushort loc_b, ushort loc_c, ushort loc_d); /* sub_273F6 */
    static void CMD_CheckParty(); /* sub_27454 */
    static void CMD_PartySurprise(); /* sub_2767E */
    static void CMD_Surprise(); /* sub_2771E */
    static void CMD_Combat(); // sub_277E4
    static void CMD_OnGotoGoSub(); /* sub_27AE5 */
    static void CMD_Treasure(); /* load_item */
    static void CMD_Rob(); /* sub_27F76*/
    static void CMD_EncounterMenu();
    static void CMD_Parlay(); /* talk_style */
    static void CMD_FindItem(); // sub_28856
    static void CMD_Delay();
    static void CMD_Damage(); /* sub_28958 */
    static void CMD_SpriteOff(); /* sub_28CB6 */
    static void CMD_EclClock(); /* sub_28CDA */
    static void CMD_PrintReturn(); // sub_28D0F
    static void CMD_ClearBox(); // sub_28D38
    static void CMD_Who(); // sub_28D7F
    static void CMD_AddNPC(); // sub_28DCA
    static void CMD_Spell();
    static void CMD_Call();
    static void TryEncamp();
    static void CMD_Program(); //YourHaveWon
    static void CMD_Protection(); // sub_2923F
    static void CMD_Dump(); // sub_29271
    static void CMD_FindSpecial(); // sub_292A5
    static void CMD_DestroyItems(); // sub_292F9
    inline static Dictionary<int, CmdItem> CommandTable;
    static void SetupCommandTable();
    static void SkipNextCommand();
    static void RunEclVm(ushort offset); // sub_29607
    static void sub_29677();
    static void sub_29758();
};

// ---------------------------------------------------------------------------
// ovr004.cs
// ---------------------------------------------------------------------------
struct ovr004 {
    inline static Array<String> codeWheel = {
                                 "CWLNRTESSCEDCSHSISERRRNSHSSTSSNNHSHN",
                                 "LAASRDAIILIDSUGADAEEOEGRLSELIITESOIO",
                                 "LRUNIMMORIIGRRIUPTIIUELIMLHMIXACGRIL",
                                 "Z0LIOHEUVNODSGEOGXYWISIOCRARLRARRHOI",
                                 "AMTELRLUIYNAEOOITOUELRREREUIMADPPFAB",
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
                             };
    static void copy_protection();
};

// ---------------------------------------------------------------------------
// ovr005.cs
// ---------------------------------------------------------------------------
struct ovr005 {
    inline static Array<Affects> disease_types = {  Affects::helpless,  Affects::cause_disease_1,
                                            Affects::weaken, Affects::cause_disease_2,
                                            Affects::animate_dead, Affects::affect_39 };
    inline static Array<String> temple_sl = { "Cure Blindness", "Cure Disease", "Cure Light Wounds", "Cure Serious Wounds", "Cure Critical Wounds", "Heal", "Neutralize Poison", "Raise Dead", "Remove Curse", "Stone to Flesh", "Exit" };
    static bool CastCureAnyway(String text);
    static bool buy_cure(int cost, String cure_name); /* buy_cure */
    static void cure_blindness();
    static void cure_disease();
    static void cure_wounds(int healType);
    static void raise_dead();
    static void cure_poison2();
    static void remove_curse();
    static void stone_to_flesh();
    static void temple_heal();
    static void temple_shop();
};

// ---------------------------------------------------------------------------
// ovr006.cs
// ---------------------------------------------------------------------------
struct ovr006 {
    static int calc_battle_exp();
    static void addExp(int exp_to_add);
    inline static Array<Affects> affects_array = {
                                            Affects::sticks_to_snakes,
                                            Affects::charm_person,
                                            Affects::reduce,
                                            Affects::silence_15_radius,
                                            Affects::spiritual_hammer,
                                            Affects::fumbling,
                                            Affects::confuse,
                                            Affects::affect_in_stinking_cloud,
                                            Affects::snake_charm,
                                            Affects::paralyze,
                                            Affects::sleep,
                                            Affects::clear_movement,
                                            Affects::affect_in_cloud_kill,
                                            Affects::entangle,
                                            Affects::affect_89,
                                            Affects::affect_8b,
                                            Affects::fear,
                                            Affects::owlbear_hug_round_attack,
                                            Affects::helpless
                                        };
    static void CleanupPlayersStateAfterCombat(); // sub_2D556
    static void displayCombatResults(int exp); /* sub_2DABC */
    static void select_treasure(int& index, Item& selectedItem, char& key); /* sub_2DD2B */
    static void take_items_treasure(); /* sub_2DDFC */
    static void take_treasure(bool& items_present, bool& money_present); /* sub_2DF2E */
    static void distributeCombatTreasure(); /* sub_2E0C3 */
    static void DeallocateNonTeamMemebers(); // sub_2E3C7
    static void distributeNpcTreasure(); /*sub_2E50E*/
    static void AfterCombatExpAndTreasure(); // sub_2E7A2
};

// ---------------------------------------------------------------------------
// ovr007.cs
// ---------------------------------------------------------------------------
struct ovr007 {
    static char ShopChooseItem(int& index, Item& selectedItem); // sub_2F04E
    static int ItemsValue(Item item_ptr);
    static bool PlayerAddItem(Item item); /*was overloaded */
    static void shop_buy(); /* sub_2F474 */
    static void CityShop(); // sub_2F6E7
};

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

// ---------------------------------------------------------------------------
// ovr009.cs
// ---------------------------------------------------------------------------
struct ovr009 {
    static void free_combat_stuff(); /* sub_3304B */
    static void MainCombatLoop(); //sub_33100
    static Player FindNextCombatant(); // PORT-NOTE: C# IEnumerable<Player> + yield; see ENGINE_NOTES.md. Returns the next combatant or nullptr. /* sub_331BC */
    static void DoPlayerCombatTurn(Player player); // sub_33281
    static void combat_menu(Player player); /* camp_menu */
    inline static Set unk_33748 = New<Set_>(Array<int>{16, 19, 45, 50, 71, 72, 73, 75, 77, 79, 80, 81});
    inline static Set unk_33768 = New<Set_>(Array<int>{16, 19, 32, 45, 50, 65, 67, 68, 71, 72, 73, 75, 77, 79, 80, 81, 84, 85, 86});
    static void combat_menu(char& arg_0, Player player);
    static bool BattleRoundChecks(); // battle01
    static void sub_33B26(bool& arg_0, char arg_4, Player player);
    static void sub_33F03(bool& arg_0, Player target, Player player);
    static void delay_menu(bool& turnEnded, Player player);
    static void set_gamespeed();
    static void SetPlayerQuickFight(Player player); // sub_3432F
};

// ---------------------------------------------------------------------------
// ovr010.cs
// ---------------------------------------------------------------------------
struct ovr010 {
    static void PlayerQuickFight(Player player); // sub_3504B
    static bool turn_undead(Player player);
    static bool ShouldCastSpellX_sub1(int spell_id, Point pos); // sub_352AF
    static bool ShouldCastSpellX(int minPriority, int spellId, Player attacker); // sub_353B1
    static bool sub_354AA(Player player);
    static bool sub_3560B(Player player);
    inline static Array2<int> data_2B8 = {
            {8, 7, 6, 1, 2, 8}, {8, 1, 2, 7, 6, 7}, {7, 1, 8, 6, 2, 1}, {1, 7, 8, 2, 6, 8}, {8, 7, 6, 5, 4, 8},
            {8, 1, 2, 3, 4, 8}, {8, 4, 6, 2, 8, 6}, {6, 4, 0, 8, 0, 6}, {6, 2, 8, 2, 0, 4}, {4, 0, 0, 2, 6, 2},
            {2, 2, 0, 4, 4, 4} /*, {4, 2, 6, 6}*/ }; /* actual from seg600:02BD - seg600:02F8 */
    static bool CanMove(bool& groundClear, int baseDirecction, int dirStep, Player player); // sub_3573B
    static void moralFailureEscape(Player player); // sub_359D1
    inline static int byte_1AB18 = 0; // byte_1AB18
    inline static int byte_1AB19 = 0; // byte_1AB19
    static bool sub_35DB1(Player player);
    static void TryGuarding(Player player); // sub_361F7
    static bool process_input_in_monsters_turn(Player player); /* sub_36269 */
    static bool FleeCheck_001(Player player); // sub_3637F
    static int CalcItemPowerRating(Item item, Player player); // sub_36535
    static void AI_items_selection(Player player); // sub_36673
};

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

// ---------------------------------------------------------------------------
// ovr013.cs
// ---------------------------------------------------------------------------
struct ovr013 {
    static void ProtectedIf(Affects affect); /* sub_3A019 */
    static void Protected();
    static bool addAffect(ushort time, int data, Affects affect_type, Player player);
    static void sub_3A071(Effect arg_0, object param, Player player);
    static void Bless(Effect add_remove, object param, Player player);
    static void Curse(Effect arg_0, object param, Player player);
    static void SticksToSnakes(Effect arg_0, object param, Player player);
    static void DispelEvil(Effect arg_0, object param, Player player);
    static void BonusVsMonstersX(Effect arg_0, object param, Player player); // sub_3A17A
    static void FaerieFire(Effect arg_0, object param, Player player);
    static void affect_protect_evil(Effect arg_0, object param, Player player); /* sub_3A224 */
    static void affect_protect_good(Effect arg_0, object param, Player player); /* sub_3A259 */
    static void affect_resist_cold(Effect arg_0, object param, Player player); /* sub_3A28E */
    static void affect_charm_person(Effect arg_0, object param, Player player); /* sub_3A2AD */
    static void Suffocates(Effect arg_0, object param, Player player);
    static void AffectPoisonDamage(Effect arg_0, object param, Player player); // sub_3A3BC
    static void AffectShield(Effect arg_0, object param, Player player); /* sub_3A41F */
    static void AffectGnomeVsManSizedGiant(Effect arg_0, object param, Player player); // sub_3A44A
    static void AffectResistFire(Effect add_remove, object param, Player player); /* sub_3A480 */
    static void is_silenced1(Effect arg_0, object param, Player player);
    static void AffectSlowPoison(Effect arg_0, object param, Player player); // sub_3A517
    static void affect_spiritual_hammer(Effect add_remove, object param, Player player); /* sub_3A583 */
    static void sub_3A6C6(Effect arg_0, object param, Player player);
    static void AffectDwarfVsOrc(Effect arg_0, object param, Player player); // sub_3A7E8
    static void MirrorImage(Effect arg_0, object param, Player player);
    static void three_quarters_damage(Effect arg_0, object param, Player player);
    static void StinkingCloud(Effect arg_0, object param, Player player);
    static void sub_3A89E(Effect arg_0, object param, Player player);
    static void AffectBlinded(Effect arg_0, object param, Player player); // sub_3A951
    static void AffectCauseDisease(Effect add_remove, object param, Player player); // sub_3A974
    static void AffectConfuse(Effect arg_0, object arg_2, Player player); // sub_3A9D9
    static void affect_curse(Effect arg_0, object param, Player player); /* sub_3AB6F */
    static void AffectBlink(Effect arg_0, object param, Player player); // has_action_timedout
    static void AffectHaste(Effect arg_0, object param, Player player); // spl_age
    static void StinkingCloudAffect(Effect arg_0, object param, Player player); // sub_3AC1D
    static void AvoidMissleAttack(int percentage, Player player); // sub_3AF06
    static Item get_primary_weapon(Player player); /* sub_3AF77 */
    static void AffectProtNormalMissles(Effect arg_0, object param, Player player); // sub_3AFE0
    static void AffectSlow(Effect arg_0, object param, Player player); //sub_3B01B
    static void weaken(Effect arg_0, object param, Player player);
    static void sub_3B0C2(Effect arg_0, object param, Player player);
    static void AffectDwarfGnomeVsGiants(Effect arg_0, object param, Player player);
    static void sub_3B1A2(Effect arg_0, object param, Player player);
    static void AffectPrayer(Effect arg_0, object param, Player player); // sub_3B1C9
    static void HotFireShield(Effect arg_0, object param, Player player); // sub_3B212
    static void ColdFireShield(Effect arg_0, object param, Player player); // sub_3B243
    static void sub_3B27B(Effect arg_0, object param, Player player); // sub_3B27B
    static void AffectClearMovement(Effect arg_0, object param, Player player); //sub_3B29A
    static void AffectRegenration(Effect arg_0, object param, Player player);
    static void AffectResistWeapons(Effect arg_0, object param, Player player); // sub_3B2D8
    static void AffectFireResist(Effect arg_0, object param, Player player);
    static void AffectHighConRegen(Effect arg_0, object param, Player player); /* sub_3B386 */
    static void AffectMinorGlobeOfInvulnerability(Effect arg_0, object param, Player player); /* sub_3B3CA */
    static void PoisonAttack(int save_bonus, Player player);
    static void AffectPoisonPlus0(Effect arg_0, object param, Player player); // sub_3B520
    static void AffectPoisonPlus4(Effect arg_0, object param, Player player); // sub_3B534
    static void AffectPoisonPlus2(Effect arg_0, object param, Player player); // sub_3B548
    static void ThriKreenParalyze(Effect arg_0, object param, Player player); // sub_3B55C
    static void AffectFeebleMind(Effect arg_0, object param, Player player); // spell_stupid
    static void AffectInvisToAnimals(Effect arg_0, object param, Player player); // sub_3B636
    static void AffectPoisonNeg2(Effect arg_0, object param, Player player); // sub_3B671
    static void AffectInvisible(Effect arg_0, object param, Player player); // sub_3B685
    static void AffectCamouflage(Effect arg_0, object param, Player player); // sub_3B696
    static void ProtDragonsBreath(Effect arg_0, object param, Player player);
    static void AffectDragonSlayer(Effect arg_0, object param, Player player); // sub_3B71A
    static void AffectFrostBrand(Effect arg_0, object param, Player player); // sub_3B772
    static void AffectBerzerk(Effect arg_0, object param, Player player);
    static void sub_3B8D9(Effect arg_0, object param, Player player);
    static void MagicFireAttack_2d10(Effect arg_0, object param, Player player); // sub_3B919
    static void AnkhegAcidAttack(Effect arg_0, object param, Player player); // sub_3B94C
    static void half_damage(Effect arg_0, object param, Player player); /* sub_3B97F */
    static void AffectResistFireAndCold(Effect arg_0, object param, Player player); // sub_3B990
    static void AffectShamblerAbsorbLightning(Effect arg_0, object param, Player player); // sub_3B9E1
    static void sub_3BA14(Effect arg_0, object param, Player player);
    static void AffectDisplace(Effect arg_0, object param, Player player); /*sub_3BA55*/
    static void CloudKillAffect(Effect arg_0, object param, Player player); // sub_3BAB9
    static void half_fire_damage(Effect arg_0, object param, Player arg_6); // sub_3BD98
    static void sub_3BDB2(Effect arg_0, object param, Player arg_6);
    static void sub_3BE06(Effect arg_0, object param, Player player);
    static void con_saving_bonus(Effect arg_0, object param, Player player); /* sub_3BE42 */
    static void AffectRegen3Hp(Effect arg_0, object param, Player player); // sub_3BEB8
    static void sub_3BEE8(Effect arg_0, object param, Player player);
    static void AffectTrollFireOrAcid(Effect arg_0, object param, Player player);
    static void AffectTrollRegenerate(Effect arg_0, object param, Player player); // sp_regenerate
    static void AffectTrollRegen(Effect arg_0, object param, Player player); // sub_3C01E
    static void AffectSalamanderHeatDamage(Effect arg_0, object param, Player player); // sub_3C05D
    static void sub_3C0DA(Effect arg_0, object param, Player player);
    static void ResistMagicPercent(int rollBase); // sub_3C0EE
    static void ResistMagic50Percent(Effect arg_0, object param, Player arg_6); // sub_3C14F
    static void ResistMagic15Percent(Effect arg_0, object param, Player arg_6); // sub_3C15D
    static void AffectElfRisistSleep(Effect arg_0, object param, Player arg_6); // sub_3C16B
    static void AffectProtCharmSleep(Effect arg_0, object param, Player arg_6); // sub_3C18F
    static void ResistParalyze(Effect arg_0, object param, Player arg_6); // sub_3C1A4
    static void AffectImmuneToCold(Effect arg_0, object param, Player arg_6); // sub_3C1B2
    static void sub_3C1C9(Effect arg_0, object param, Player arg_6);
    static void AffectImmuneToFire(Effect arg_0, object param, Player arg_6); // sub_3C1EA
    static void sub_3C201(Effect arg_0, object param, Player arg_6);
    static void AffectProtectionFromElectricity(Effect arg_0, object param, Player player); // sub_3C246
    static void sub_3C260(Effect arg_0, object param, Player player);
    static void half_damage_if_weap_magic(Effect arg_0, object param, Player player); /* sub_3C2BF */
    static void sub_3C2F9(Effect arg_0, object param, Player player); // sub_3C2F9
    static void AffectProtCold(Effect arg_0, object param, Player player); // sub_3C33C
    static void AffectProtNonMagicWeapons(Effect arg_0, object param, Player player); // sub_3C356
    static void sub_3C3A2(Effect arg_0, object param, Player player);
    static void sub_3C3F6(Effect arg_0, object param, Player player);
    static void AffectDracolichParalysis(Effect arg_0, object param, Player player); // spl_paralyze
    static void sub_3C59D(Effect arg_0, object param, Player player);
    static void AffectHalfElfResistance(Effect arg_0, object param, Player player); // sub_3C5D0
    static void sub_3C5F4(Effect arg_0, object param, Player player);
    static void AffectProtMagic(Effect arg_0, object param, Player player); // sub_3C623
    static void sub_3C643(Effect arg_0, object arg_2, Player player);
    static void do_items_affect(Effect remove_affect, object param, Player player); /* sub_3C6D3 */
    static void AffectDracolichA(Effect arg_0, object param, Player player); //sub_3C750
    static void AffectRangerVsGiant(Effect arg_0, object param, Player player); // sub_3C77C
    static void AffectProtElec(Effect arg_0, object param, Player player); //sub_3C7B5
    static void AffectEntangle(Effect arg_0, object param, Player player); // sub_3C7CC
    static void sub_3C7E0(Effect arg_0, object param, Player player); // sub_3C7E0
    static void add_affect_19(Effect arg_0, object param, Player player);
    static void PaladinCastCureRefresh(Effect add_remove, object param, Player player); // sub_3C8EF
    static void AffectFear(Effect add_remove, object param, Player player); /* sub_3C932 */
    static void sub_3C975(Effect arg_0, object arg_2, Player target);
    static void sp_dispel_evil(Effect arg_0, object param, Player player);
    static void empty(Effect arg_0, object param, Player player);
    inline static Dictionary<Affects, affectDelegate> affect_table = nullptr; // null until SetupAffectTables()
    static void SetupAffectTables(); // setup_spells2
    static void CallAffectTable(Effect add_remove, object parameter, Player player, Affects affect); /* sub_630C7 */
};

// ---------------------------------------------------------------------------
// ovr014.cs
// ---------------------------------------------------------------------------
struct ovr014 {
    static void CalculateInitiative(Player player); // sub_3E000
    static int CalcMoves(Player player); // sub_3E124
    static void sub_3E192(int index, Player target, Player attacker);
    enum class AttackType : int {
        Normal = 0,
        Behind = 1,
        Backstab = 2,
        Slay = 3
    };

    static void DisplayAttackMessage(bool attackHits, int attackDamge, int actualDamage, AttackType attack, Player target, Player attacker); /* backstab */
    static void move_step_into_attack(Player target); /* sub_3E65D */
    static void sub_3E748(int direction, Player player);
    static void move_step_away_attack(int direction, Player player); /* sub_3E954 */
    static void flee_battle(Player player);
    static void reclac_attacks(Player player); // sub_3EDD4
    static int ThisRoundActionCount(int halfActionsLeft); // sub_3EF0D
    static bool TrySweepAttack(Player target, Player attacker); // sub_3EF3D
    static bool CanSeeTargetA(Player targetA, Player targetB); //sub_3F143
    inline static Array<sbyte> unk_16679 = { 0,
        17, 17,  0,  0,  1, 17,  0,  0, 32, 32,
        10,  7,  4,  1,  1,  0,  0, -1, -1, -1 }; /*seg600:0369*/
    static void turns_undead(Player player);
    static bool FindLowestE9Target(Player& output, Player player); /* sub_3F433 */
    static bool AttackTarget01(Item item, int arg_8, Player target, Player attacker); // sub_3F4EB
    static void RecalcAttacksReceived(Player target, Player attacker); // sub_3F94D
    static bool AttackTarget(Item rangedWeapon, int attackType, Player target, Player attacker); // sub_3F9DB
    static int RangedDefenseBonus(Player target, Player attacker); /* sub_3FCED */
    static bool find_healing_target(Player& target, Player healer); /* sub_3FDFE */
    inline static Array<Affects> unk_18ADB = { Affects::bless, Affects::snake_charm, Affects::paralyze, Affects::sleep, Affects::helpless }; // seg600:27CB first is filler (off by 1)
    static bool sub_4001C(DownedPlayerTile arg_0, bool canTargetEmptyGround, QuickFight quick_fight, int spellId);
    static bool target(QuickFight quick_fight, int spellId);
    static void spell_menu3(bool& casting_spell, QuickFight quick_fight, int spell_id);
    static bool CanBackStabTarget(Player target, Player attacker); /* sub_408D7 */
    static byte getTargetDirection(Player playerB, Player playerA); /* sub_409BC */
    static void DrawRangedAttack(Item item, Player target, Player attacker); /* sub_40BF1 */
    static void calc_enemy_health_percentage(); /* sub_40E00 */
    static int MaxOppositionMoves(Player player); // sub_40E8F
    static bool can_attack_target(Player target, Player attacker); /* sub_40F1F */
    static char aim_sub_menu(bool showTarget, bool showRange, int maxRange, Player target, Player attacker); /* Aim_menu */
    static bool sub_411D8(DownedPlayerTile arg_0, bool showRange, Player target, Player attacker);
    inline static Set asc_41342 = New<Set_>(Array<int>{0, 69, 84});
    static bool Target(DownedPlayerTile arg_0, bool allowTarget, bool canTargetEmptyGround, bool showRange, int maxRange, Player target, Player player01);
    static Array<SortedCombatant> copy_sorted_players(Player player); /* sub_4188F */
    static Player step_combat_list(bool arg_2, int step, int& list_index, Point& attackerPos, Array<SortedCombatant> sorted_list); /* sub_41932 */
    inline static Set unk_41AE5 = New<Set_>(Array<int>{0, 69});
    inline static Set unk_41B05 = New<Set_>(Array<int>{71, 72, 73, 75, 77, 79, 80, 81});
    static bool aim_menu(DownedPlayerTile arg_0, bool allowTarget, bool canTargetEmptyGround, bool showRange, int maxRange, Player attacker); /* sub_41B25 */
    static bool find_target(bool clear_target, byte arg_2, int max_range, Player player); /* sub_41E44 */
    static void engulfs(Effect arg_0, object param, Player attacker);
    static void LoadMissleIconAndDraw(int icon_id, Player target, Player attacker); //sub_42159
    static bool sub_421C1(bool clear_target, int& range, Player player); // sub_421C1
    static void attack_or_kill(Effect arg_0, object param, Player attacker);
    static void sub_425C6(Effect add_remove, object param, Player player);
    static void AffectOwlbearHugRoundAttack(Effect arg_0, object param, Player player); // sub_426FC
    static void AffectOwlbearHugAttackCheck(Effect arg_0, object param, Player player); // hugs
    static bool god_intervene();
};

// ---------------------------------------------------------------------------
// ovr015.cs
// ---------------------------------------------------------------------------
struct ovr015 {
    static void MapSetDoorUnlocked(int mapDir, int mapY, int mapX); /*sub_43148*/
    static bool AnyPlayerHasSkill(SkillType skill); // any_player_has_skill
    static bool bash_door();
    static bool pick_lock(); /*sub_435B6*/
    static bool TeamMemberHasSpell(Spells spellId);
    static bool RemoveKnockSpell();
    static void TryStepForward(); // sub_43765
    static void MovePartyForward(); /* sub_43813 */
    static char main_3d_world_menu(); /* sub_438DF */
    static void locked_door();
};

// ---------------------------------------------------------------------------
// ovr016.cs
// ---------------------------------------------------------------------------
struct ovr016 {
    static int sub_44032(Player player);
    static void cancel_memorize(Player player);
    static void cancel_scribes(Player player);
    static void cancel_spells();
    static int HowManySpellsPlayerCanLearn(SpellClass spellClass, int spellLevel); //sub_4428E
    static bool sub_443A0(byte learn_type);
    static void cast_spell();
    static bool BuildMemorizeSpellText(); // sub_445D4
    static bool rest_menu();
    static void memorize_spell();
    static void scribe_spell();
    inline static Dictionary<Affects, String> EffectNameMap;
    static void BuildEffectNameMap();
    static void DisplayMagicEffects();
    static bool magic_menu();
    static void MoveCurrentPlayerUp(); // sub_4558D
    static void MoveCurrentPlayerDown(); // sub_456E5
    inline static Array<String> reorderStrings = { "Select Exit", "Place Exit" }; //seg600_04A6
    inline static Set reorderSet = New<Set_>(Array<int>{13,80,83});
    static void reorder_party();
    static void DropPlayer(); // drop_player
    static void game_speed();
    inline static Set AlterSet = New<Set_>(Array<int>{0, 69});
    static void alter_menu();
    static int CalculateInitialHealing(); // sub_45F22
    static void CalculateHealing(int& healingAvailable, int numCureLight, int numCureSerious, int numCureCritical); // sub_45FDD
    static int TotalHitpointsLost(); /* sub_4608F */
    static void CalculateTimeAndSpellNumbers(int& numCureCritical, int& numCureSerious, int& numCureLight); //sub_460ED
    static void DoTeamHealing(int& healingAvailable); //sub_46280
    static bool FixTeam(); // fix_menu
    inline static Set unk_463F4 = New<Set_>(Array<int>{0, 69});
    static bool MakeCamp(); // make_camp
};

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

// ---------------------------------------------------------------------------
// ovr019.cs
// ---------------------------------------------------------------------------
struct ovr019 {
    static void SetPixel(byte colour, ushort row, ushort column);
    static byte GetPixel(ushort row, ushort column);
    static void sub_52068();
    class Struct_1ADFB_ {
    public:
        void Reset();
        byte field_0 = 0;
        byte field_1 = 0;
        byte field_2 = 0;
        byte field_3 = 0;
        byte field_4 = 0;
        byte operator[](int index) const; // C# indexer `internal byte this[int index] { get }`
    };
    using Struct_1ADFB = cs::Ref<Struct_1ADFB_>;

    inline static Array<Struct_1ADFB> unk_1ADFB = Array<Struct_1ADFB>(3);
    static void sub_520B8(Array<byte> arg_0, int arg_4, int arg_6, ushort arg_8, ushort arg_A);
    static void sub_524F7(Array<Struct_1ADF6> arg_2, int arg_6);
    static void sub_5279B(Array<byte> arg_0);
    static void endgame_5285E(byte arg_0, short arg_2, short& arg_4, ushort& arg_8, ushort& arg_C, ushort& arg_10); /* sub_5285E */
    static void endgame_529F4(); /* sub_529F4 */
    static void ShowAnimation(int num_loops, byte block_id, short row_y, short col_x); // sub_52B79
    inline static String aTyranthraxusSp = "Tyranthraxus' spirit coalesces over the slain ";
    inline static String aStormGiant_You = "storm giant. 'You have defeated me. Were it not for ";
    inline static String aTheAmuletOfLyt = "the Amulet of Lythander, I could possess you and rob ";
    inline static String aYouOfYourVicto = "you of your victory. Still I can escape through the pool.";
    inline static String aAsYouReachForT = "As you reach for the Pool of Radiance, he cries ";
    inline static String aOutKeepTheGaun = "out, 'Keep the Gauntlet of Moander away from there, you ";
    inline static String aWillUnleashDan = "will unleash dangerous energies. Stay back!' As the ";
    inline static String aGauntletContac = "gauntlet contacts the pool, it contracts and shatters it.";
    inline static String aIAmTrappedWith = "'I am trapped without escape, you have succeeded ";
    inline static String aWhereArmiesHav = "where armies have not. Gloat while you may, Tyranthraxus ";
    inline static String aIsSlainThisDay = "is slain this day.' Before your eyes he crumbles into ";
    inline static String aNothingness_ = "nothingness.";
    inline static String aYouAreCertainH = "You are certain he is destroyed because your ";
    inline static String aFinalBondFades = "final bond fades away. The Curse of the Azure Bonds ";
    inline static String aHasFinallyBeen = "has finally been lifted from you! You are free at ";
    inline static String aLast = "last!";
    inline static String aTheKnightsOfMy = "The Knights of Myth Drannor rush in, '";
    inline static String aCongratulati_0 = "Congratulations, you have destroyed the Flamed One. ";
    inline static String aWithThePowerOf = "With the power of Elminster, let us take you from ";
    inline static String aThisFoulPlaceT = "this  foul place, to a fine feast.'";
    inline static String aYouAreTeleport = "You are teleported to Shadowdale, where festivities ";
    inline static String aHaveAlreadyBeg = "have already begun. A huge cheer goes up at your arrival. ";
    inline static String aGharriAndNacac = "Gharri and Nacacia, arm in arm, yell congratulations ";
    inline static String aFromTheNearbyS = "from the nearby stands. 'You have won!'";
    static void end_game_text();
};

// ---------------------------------------------------------------------------
// ovr020.cs
// ---------------------------------------------------------------------------
struct ovr020 {
    inline static Array<String> sexString = { "Male", "Female" };
    inline static Array<String> raceString = { "Monster", "Dwarf", "Elf", "Gnome",
                                         "Half-Elf", "Halfling", "Half-Orc", "Human" };
    inline static Array<String> alignmentString = { "Lawful Good", "Lawful Neutral", "Lawful Evil",
                                              "Neutral Good", "True Neutral", "Neutral Evil",
                                              "Chaotic Good", "Chaotic Neutral", "Chaotic Evil" };
    inline static Array<String> classString = { "Cleric", "Druid", "Fighter", "Paladin", "Ranger",
                                          "Magic-User", "Thief", "Monk", "Cleric/Fighter",
                                          "Cleric/Fighter/Magic-User", "Cleric/Ranger",
                                          "Cleric/Magic-User","Cleric/Thief", "Fighter/Magic-User",
                                          "Fighter/Thief", "Fighter/Magic-User/Thief",
                                          "Magic-User/Thief" };
    inline static Array<String> statShortString = { "STR ", "INT ", "WIS ", "DEX ", "CON ", "CHA " };
    inline static Array<String> statusString = { "Okay", "Animated", "tempgone", "Running",
                                         "Unconscious", "Dying", "Dead", "Stoned",
                                         "Gone" };
    inline static Array<String> moneyString = { "Copper", "Silver", "Electrum", "Gold", "Platinum",
                                        "Gems", "Jewelry" };
    static void playerDisplayFull(Player player);
    static void displayMoney();
    static void display_player_stats01();
    static void display_stat(bool highlighted, int stat_index);
    inline static Set asc_54B50 = New<Set_>(Array<int>{73, 83, 84 });
    inline static Set unk_54B03 = New<Set_>(Array<int>{0, 69 });
    static bool viewPlayer();
    static bool CanSellDropTradeItem(Item item); // sub_54EC1
    static void ItemDisplayStats(Item arg_0); /*sub_550A6*/
    inline static Set unk_554EE = New<Set_>(Array<int>{0, 69});
    static void PlayerItemsMenu(bool& arg_0); /*use_item*/
    inline static const Array2<byte> MU_spell_lvl_learn = {
            {1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {1, 1, 0, 0, 0},
            {1, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 1, 0, 1, 0},
            {0, 0, 1, 1, 0},
            {0, 0, 0, 0, 1},
            {0, 1, 0, 0, 1},
            {0, 0, 1, 1, 1},
            {0, 0, 0, 1, 1}  };
    static void calc_items_effects(bool add_item, Item item); /*sub_55B04*/
    enum class Weld : int {
        Ok = 0,
        WrongClass = 1,
        AlreadyUsingX = 2,
        HandsFull = 3
    };

    static void ready_Item(Item item);
    static void trade_item(Item item);
    static void halve_items(Item item);
    static void join_items(Item item); /*sub_56285*/
    static void UseMagicItem(bool& arg_0, Item item); // sub_56478
    static void ShopSellItem(Item item); // sell_Item
    static void IdentifyItem(bool& arg_0, Item item);
    static void tradeCoin();
    static void drop_coin();
    static bool canCarry(Item item, Player player);
    static void scroll_team_list(char input_key);
    static byte spell_menu2(bool& arg_0, int& index, SpellSource arg_8, SpellLoc spl_location);
    static bool CanCastHeal(Player player); /* sub_575F0 */
    static bool CanCastCureDiseases(Player player); /* sub_57655 */
    static void PaladinHeal(Player player);
    inline static Array<Affects> paladinCureableDiseases = { // unk_16B39
            Affects::helpless,
            Affects::cause_disease_1,
            Affects::weaken,
            Affects::cause_disease_2,
            Affects::hot_fire_shield,
            Affects::affect_39 };
    static void PaladinCureDisease(Player player); /* sub_577EC */
};

// ---------------------------------------------------------------------------
// ovr021.cs
// ---------------------------------------------------------------------------
struct ovr021 {
    inline static Array<int> timeScales = { 10, 10, 6, 24, 30, 12, 0x100 }; //word_1A13C
    static void CheckAffectsTimingOut(int timeSlot, int timeSteps); // sub_5801E
    static void NormalizeClock(RestTime arg_0); /* sub_58317 */
    static void clock_583C8(); /* sub_583C8 */
    static void step_game_time(int time_slot, int amount); /* sub_583FA */
    static void rest_time_5849F(int time_index, byte arg_2); /* sub_5849F */
    static String format_time(int value); /* sub_5858A */
    static void display_resting_time(int highlight_time); /* sub_58615 */
    inline static Set unk_58731 = New<Set_>(Array<int>{0, 69, 82 });
    static bool resting_time_menu(); /* sub_58751 */
    static void rest_heal(bool show_text); /* reset_heal */
    static int rest_memorize(bool& findNext, Player player);
    static int rest_scribe(bool& findNext, Player player);
    inline static Array<int> spellLaernTimeout = Array<int>(9); // seg600:758D
    static void CheckForSpellLearning(); // sub_58B4D
    static void sub_58C03(int& arg_0);
    static bool resting(bool interactive_resting); /* reseting */
};

// ---------------------------------------------------------------------------
// ovr022.cs
// ---------------------------------------------------------------------------
struct ovr022 {
    static int get_max_load(Player player);
    static bool willOverload(int item_weight, Player player);
    static bool willOverload(int& weight, int item_weight, Player player);
    static void addPlayerGold(int item_weight);
    static short AskNumberValue(byte fgColor, String prompt, int maxValue); // sub_592AD
    static void trade_money(int money_slot, short num_coins, Player dest, Player source); /* add_object */
    static void poolMoney();
    static int GetPartyCount(); /* sub_595FF */
    static void share_pooled();
    static void DropCoins(int money_slot, int num_coins, Player player); /* sub_59A19 */
    static void PickupCoins(int money_slot, int num_coins, Player player); /* sub_59AA0 */
    static int GetMoneyIndexFromString(String& displayText, String input); // sub_59BAB
    static void TakePoolMoney(); // takeItems
    static void treasureOnGround(bool& items, bool& money);
    static sbyte randomBonus(); // sub_59FCF
    inline static Array2<short> preconfiiguredItems = {
        {0x00B9, 0x00BB, 0x0040, 0x0001, 0x0320, 0x0003, 0x0063, 0x0000},
        {0x00EF, 0x00A7, 0x0040, 0x0001, 0x044C, 0x0001, 0x003b, 0x0000},
        {0x00b9, 0x00a7, 0x0040, 0x0001, 0x0190, 0x0001, 0x0003, 0x0000},
        {0x00AD, 0x00a7, 0x0040, 0x0001, 0x01C2, 0x0001, 0x0030, 0x0000},
        {0x00CE, 0x00A7, 0x0045, 0x0001, 0x2AF8, 0x001E, 0x000F, 0x0000},
        {0x00E2, 0x00A7, 0x0064, 0x000A, 0x3A98, 0x0000, 0x0026, 0x0083},
        {0x009d, 0x00a7, 0x0015, 0x0014, 0x0bb8, 0x0001, 0x0033, 0x0000} }; /*seg600:082E unk_16B3E */
    static Item create_item(ItemType item_type); /* sub_5A007 */
    static bool appraiseGemsJewels();
};

// ---------------------------------------------------------------------------
// ovr023.cs
// ---------------------------------------------------------------------------
struct ovr023 {
    inline static Array<String> SpellNames = { /* AffectNames */
                            "",
                            "Bless",
                            "Curse",
                            "Cure Light Wounds",
                            "Cause Light Wounds",
                            "Detect Magic",
                            "Protection from Evil",
                            "Protection from Good",
                            "Resist Cold",
                            "Burning Hands",
                            "Charm Person",
                            "Detect Magic",
                            "Enlarge",
                            "Reduce",
                            "Friends",
                            "Magic Missile",
                            "Protection From Evil",
                            "Protection From Good",
                            "Read Magic",
                            "Shield",
                            "Shocking Grasp",
                            "Sleep",
                            "Find Traps",
                            "Hold Person",
                            "Resist Fire",
                            "Silence, 15' Radius",
                            "Slow Poison",
                            "Snake Charm",
                            "Spiritual Hammer",
                            "Detect Invisibility",
                            "Invisibility",
                            "Knock",
                            "Mirror Image",
                            "Ray of Enfeeblement",
                            "Stinking Cloud",
                            "Strength",
                            "Animate Dead",
                            "Cure Blindness",
                            "Cause Blindness",
                            "Cure Disease",
                            "Cause Disease",
                            "Dispel Magic",
                            "Prayer",
                            "Remove Curse",
                            "Bestow Curse",
                            "Blink",
                            "Dispel Magic",
                            "Fireball",
                            "Haste",
                            "Hold Person",
                            "Invisibility, 10' Radius",
                            "Lightning Bolt",
                            "Protection From Evil, 10' Radius",
                            "Protection From Good, 10' Radius",
                            "Protection From Normal Missiles",
                            "Slow",
                            "Restoration",
                            "",
                            "Cure Serious Wounds",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "Cause Serious Wounds",
                            "Neutralize Poison",
                            "Poison",
                            "Protection Evil, 10' Radius",
                            "Sticks to Snakes",
                            "Cure Critical Wounds",
                            "Cause Critical Wounds",
                            "Dispel Evil",
                            "Flame Strike",
                            "Raise Dead",
                            "Slay Living",
                            "Detect Magic",
                            "Entangle",
                            "Faerie Fire",
                            "Invisibility to Animals",
                            "Charm Monsters",
                            "Confusion",
                            "Dimension Door",
                            "Fear",
                            "Fire Shield",
                            "Fumble",
                            "Ice Storm",
                            "Minor Globe Of Invulnerability",
                            "Remove Curse",
                            "Animate Dead",
                            "Cloud Kill",
                            "Cone of Cold",
                            "Feeblemind",
                            "Hold Monsters",
                            "",
                            "",
                            "",
                            "",
                            "",
                            "Bestow Curse" };
    inline static Array<String> LevelStrings = {
                            "",
                            "1st Level",
                            "2nd Level",
                            "3rd Level",
                            "4th Level",
                            "5th Level",
                            "6th Level",
                            "7th Level",
                            "8th Level",
                            "9th Level"
                        };
    static bool can_learn_spell(int spell_id, Player player); /* sub_5C01E */
    inline static Set unk_5C1A2 = New<Set_>(Array<int>{1,2,3,4});
    inline static Set asc_5C1D1 = New<Set_>(Array<int>{0, 67, 69, 76, 77, 83});
    inline static Set unk_5C1F1 = New<Set_>(Array<int>{0, 69});
    static byte spell_menu(int& index, SpellSource spellSource);
    static void add_spell_to_list(byte spell_id); /* sub_5C3ED */
    static void add_spell_to_learning_list(int spell_id); /* sub_5C5B9 */
    static void scroll_5C912(bool learning); /* sub_5C912 */
    static void BuildScrollSpellLists(bool showLearning); // sub_5C9F4
    static bool BuildSpellList(SpellLoc spl_location); // sub_5CA74
    static int SpellRange(int spellId); // sub_5CDE5
    static ushort GetSpellAffectTimeout(Spells spellId); // sub_5CE92
    static void DoSpellCastingWork(String text, DamageType damageFlags, int damage, bool call_affect_table, int TargetCount, int spell_id); // sub_5CF7F
    static bool NonCombatSpellCast(QuickFight quick_fight, int spellId); // cast_spell_on
    static void sub_5D2E1(bool showCastingText, QuickFight quick_fight, int spell_id); // sub_5D2E1
    static void sub_5D2E1(bool& arg_0, bool showCastingText, QuickFight quick_fight, int spell_id); // sub_5D2E1
    static void localSteppingPathInit(Point target, Point caster, SteppingPath path); /* sub_5D676 */
    static int find_players_on_path(SteppingPath path, List<int> player_list); /* sub_5D702 */
    inline static Array<Point> unk_16D22 = { Point(-1, 0), Point(0, -1), Point(0, -1), Point(1, 0), Point(1, 0), Point(0, 1), Point(0, 1), Point(-1, 0) };
    inline static Array<Point> unk_16D32 = { Point(1, 0), Point(1, 0), Point(0, 1), Point(0, 1), Point(-1, 0), Point(-1, 0), Point(0, -1), Point(0, -1) };
    static void BuildAreaDamageTargets(int max_range, int playerSize, Point targetPos, Point casterPos); // sub_5D7CF
    static void MultiTargetedSpell(String text, int save_bonus); // sub_5DB24
    static void CastTeamSpell(String text, CombatTeam team); // sub_5DCA0
    static void cleric_bless(); /* is_Blessed */
    static void cleric_curse(); /* is_Cursed */
    static void SpellCureLight(); /* sub_5DDBC */
    static void SpellCauseLight(); /* sub_5DDF8 */
    static void is_affected();
    static void SpellProtectionFromX(); // is_protected
    static void SpellResistCold(); // is_cold_resistant
    static void SpellBuringHands(); // sub_5DEE1
    static void SpellCharm(); // is_charmed
    static void SpellEnlarge(); // is_stronger
    static void SpellReduce(); // has_been_reduced
    static void SpellFriends(); // is_friendly
    static void SpellMagicMissile(); // sub_5E221
    static void SpellShield(); // is_shielded
    static void SpellShockingGrasp(); // sub_5E2B2
    static void SpellSleep(); // falls_asleep
    static int CalcSleepCost(Player target);
    static void SpellHoldX(); // is_held
    static void SpellFireResistant(); // is_fire_resistant
    static void SpellSilence15Radius(); // is_silenced
    static void is_affected2();
    static void SpellSnakeCharm(); // is_charmed2
    static void SpellSpiritualHammer(); // sub_5E681
    static void is_invisible();
    static void SpellKnock();
    static void SpellMirrorImage(); // is_duplicated
    static void SpellRayOfEnfeeblement(); // is_weakened
    static constexpr int StinkingCloudMaxTargets = 4;
    static void SpellStinkingCloud(); //TODO similar to spell_poisonous_cloud
    static void SpellStrength(); // sub_5EC5B
    static void SpellAnimateDead(); // is_animated
    static void SpellCureBlindness(); // can_see
    static void SpellCauseBlindness(); // is_blind
    static bool sub_5F037();
    static void SpellCureDisease(); // sub_5F0DC
    static void SpellCauseDisease(); // is_diseased
    static bool sub_5F126(Player arg_2, int target_count); // sub_5F126
    static void SpellDispelMagic(); // is_affected3
    static void SpellPrayer(); // is_praying
    static void SpellRemoveCurse(); // uncurse
    static void curse();
    static void spell_blinking();
    static void sub_5F782();
    static void RemoveComplimentSpellFirst(String text, CombatTeam combatTeam, Affects affect); //sub_5F87B
    static void cast_haste();
    static void DoElecDamage(int player_index, SaveVerseType bonusType, int damage, Point pos);
    static void DoActualElecDamage(int player_index, SaveVerseType bonusType, int damage, int playerIndex);
    static bool DoElecDamage(bool arg_0, int player_index, SaveVerseType bonusType, int damage, Point pos); // sub_5F986
    static void sub_5FA44(byte arg_0, SaveVerseType bonusType, int damage, byte arg_6);
    static void SpellLightningBolt(); // sub_5FCD9
    static void SpellSlow(); // sub_5FD2E
    static void SpellRestoration(); // cast_restore
    static void cast_speed();
    static void SpellCureSeriousWounds(); // sub_5FF6D
    static void cast_strength();
    static void sub_6003C();
    static void cast_paralyzed();
    static void cast_heal();
    static void cast_invisible();
    static void dam2d4plus2();
    static void SpellCauseSeriousWounds(); // sub_60185
    static void SpellNeutralizePoison(); // cure_poison
    static void SpellPoison(); // sub_602D0
    static void SpellSticksToSnakes(); // cast_flattern
    static void SpellCureCriticalWounds(); // sub_603F0
    static void SpellCauseCriticalWounds(); // sub_60431
    static void SpellDispelEvil(); // is_affected4
    static void SpellFlameStrike(); // sub_604DA
    static void SpellRaiseDead(); // cast_raise
    static void SpellSlayLiving(); //cast_slay
    static void SpellEntangle(); // cast_entangle
    static void SpellFaerieFire(); /* cast_highlisht */
    static void SpellInvisToAnimals(); // cast_invisible2
    static void SpellCharmMonsters(); // cast_charmed
    static void SpellConfusion(); // cast_confuse
    static void SpellDimensionDoor(); // cast_teleport
    static void SpellFear(); /* cast_terror */
    static void SpellFireProtection(); // cast_protection
    static void SpellFumble(); // spell_slow
    static void SpellIceStorm(); // sub_60F0B
    static void SpellMinorGlobeOfInvulnerability(); // sub_60F4E
    static void SpellCloudKill(); // spell_poisonous_cloud // similar to create_noxious_cloud
    static void SpellConeOfCold(); // sub_61550
    static void SpellFeeblemind(); // sub_615F2
    static void sub_616CC();
    static void sub_61727();
    static void cast_heal2();
    static void AffectParalizingGaze(Effect arg_0, object param, Player player); /* spell_stone */
    static void DragonBreathElec(Effect arg_0, object param, Player player); // cast_breath
    static void AffectSpitAcid(Effect arg_0, object param, Player player); // spell_spit_acid
    static void DragonBreathAcid(Effect arg_0, object param, Player attacker); // spell_breathes_acid
    static void DragonBreathFire(Effect arg_0, object param, Player attacker); // spell_breathes_fire
    static void cast_breath_fire(Effect arg_0, object param, Player arg_6);
    static void cast_throw_lightening(Effect arg_0, object param, Player caster); /* cast_throw_lightning */
    static void cast_gaze_paralyze(Effect arg_0, object param, Player arg_6);
    static void remove_spell_from_scroll(int affect, Item item, Player player); /* sub_623FF */
    static void DisplayCaseSpellText(int spellId, String arg_2, Player player); /* cast_a_spell */
    static void setup_spells();
};

// ---------------------------------------------------------------------------
// ovr024.cs
// ---------------------------------------------------------------------------
struct ovr024 {
    static void KillPlayer(String text, Status new_health_status, Player player); // sub_63014
    static void remove_affect(Affect affect, Affects affect_id, Player player);
    inline static Array<Affects> unk_6325A = { Affects::silence_15_radius, Affects::prot_from_evil_10_radius, Affects::prot_from_good_10_radius, Affects::prayer };
    static void calc_affect_effect(Affects affect_type, Player player);
    static void CheckAffectsEffect(Player player, CheckType type); // work_on_00
    static Player sub_63D03(Array<byte> directions, int arraySize, List<GasCloud> list, Point mapPos); // sub_63D03
    static void in_poison_cloud(byte arg_0, Player player);
    static bool CanHitTarget(int bonus, Player target); // sub_641DD
    static bool PC_CanHitTarget(int target_ac, Player target, Player attacker); /* sub_64245 */
    static bool RollSavingThrow(int saveBonus, SaveVerseType saveType, Player player); // do_saving_throw
    static byte roll_dice(int dice_size, int dice_count);
    static int roll_dice_save(int dice_size, int dice_count);
    static void add_affect(bool call_spell_jump_list, int data, ushort minutes, Affects type, Player player);
    static void RemoveFromCombat(String msg, Status health_status, Player player); // sub_644A7
    static void remove_invisibility(Player player);
    static void RemoveCombatAffects(Player player); // sub_645AB
    static void RemoveAttackersAffects(Player player); // sub_6460D
    static bool cure_affect(Affects affectId, Player player); /* is_cured */
    static int encode_strength(int str_00, int str); /* odd_math */
    static void decode_strength(int& str_00, int& str, Affect arg_8); /* sub_646D9 */
    static bool TryEncodeStrength(int& encoded_str, int str_100, int str, Player player); // sub_64728
    static void max_strength(int& str_a, int str_b, int& str_00_a, int str_00_b); /* sub_64771 */
    static int ConHitPointBonus(int classLvl, SkillType class_index, int cons, Player player); // sub_647BE
    static void CalcStatBonuses(Stat stat_index, Player player); // sub_648D9
    static void damage_person(bool change_damage, DamageOnSave arg_2, int damage, Player player);
    static void TryLooseSpell(Player player);
    static void ApplyAttackSpellAffect(String text, bool saved, DamageOnSave can_save, bool call_affect_table, int data, ushort time, Affects affect_id, Player target); // is_unaffected
    static bool heal_player(byte arg_0, int amount_healed, Player player);
    static bool combat_heal(byte arg_0, Player player);
};

// ---------------------------------------------------------------------------
// ovr025.cs
// ---------------------------------------------------------------------------
struct ovr025 {
    static void CalculateAttackValues(Player player); // sub_66023
    static void CalcArmorWeightEffect(Item item, Player player); // sub_6621E
    static void sub_662A6(byte& output, Array<sbyte>& bonus, Item item, Player player);
    static void calc_movement(Player player); /* sub_663C4 */
    static void ItemDisplayNameBuild(bool display_new_name, bool displayReadied, int yCol, int xCol, Item item); /*id_item*/
    static void PartySummary(Player player);
    static void display_AC(int y_offset, int x_offset, Player player);
    static void display_hp(bool hightlighted, int y_pos, int x_pos, Player player);
    static void CombatDisplayPlayerSummary(Player player); /* hitpoint_ac */
    static void reclac_player_values(Player player); /* sub_66C20 */
    static sbyte DexAcBonus(Player player); /* stat_bonus */
    static int DexReactionAdj(Player player);
    static int player_strength_group(Player player); /* playerStrengh */
    static int strengthHitBonus(Player player);
    static sbyte strengthDamBonus(Player player);
    static int max_encumberance(Player player); /* strength_bonus */
    static void lose_item(Item item, Player player);
    static void string_print01(String text);
    static void DisplayPlayerStatusString(bool clearDisplay, int lineY, String text, Player player); /* sub_67788 */
    static void ClearPlayerTextArea(); /* sub_6786F */
    static void displayPlayerName(bool pural, int y_offset, int x_offset, Player player); /*sub_678A2*/
    static void load_missile_dax(bool flipIcon, byte iconOffset, Icon iconAction, int iconIdx); /* sub_67924 */
    static void load_missile_icons(int iconIdx); /* sub_67A59 */
    static void draw_missile_attack(int delay, int frameCount, Point target, Point attacker); /* sub_67AA4 */
    static void MagicAttackDisplay(String text, bool showMagicStars, Player player); // sub_6818A
    static bool FindAffect(Affect& affectFound, Affects affect_type, Player player);
    static void damage_player(int damage, Player player);
    static void DescribeHealing(Player player); /* sub_684F7 */
    static void CountCombatTeamMembers(); // count_teams
    static List<CombatPlayerIndex> BuildNearTargets(int max_range, Player player); /*near_enermy*/
    static int getTargetRange(Player target, Player attacker); /* sub_68708 */
    static void clear_actions(Player player);
    static void guarding(Player player);
    static int spellMaxTargetCount(int spell_id); /* sub_6886F */
    static void LoadPic(); // load_pic
    static String direction(int dir);
    static void display_map_position_time(); // camping_search
    static void RedrawCombatScreen(); // sub_68DC0
    inline static Set unk_68DFA = New<Set_>(Array<int>{13, 27, 69, 83});
    static void selectAPlayer(Player& player, bool showExit, String prompt);
    static bool item_is_ranged_melee(Item item);
    static bool is_weapon_ranged(Player player); /* offset_above_1 */
    static bool is_weapon_ranged_melee(Player player); /* offset_equals_20 */
    static bool GetCurrentAttackItem(Item& found_item, Player player); // sub_6906C
    static bool bandage(bool applyBandage);
};

// ---------------------------------------------------------------------------
// ovr026.cs
// ---------------------------------------------------------------------------
struct ovr026 {
    inline static Array2<byte> ClericSpellLevels = { // unk_1A5CC
            {1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {1, 1, 0, 0, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 1, 1, 0},
            {1, 1, 0, 0, 1},  //seg600:42EE
            {0, 0, 0, 1, 1},  //seg600:42F3
            {1, 0, 1, 0, 0},  //seg600:42F8
            {1, 1, 1, 0, 0},  //seg600:42FD
        }; /*seg600:42BC*/
    inline static Array2<byte> PaladinSpellLevels = { // unk_1A6F5
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {0, 1, 0, 0, 0}
        }; /*seg600:43E5*/
    inline static Array2<byte> unk_1A758 = {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {1, 0, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 1, 0, 0, 0}
        }; /*seg600:4448*/
    static void sub_6A00F(Player player); // sub_6A00F
    static void ReclacClassBonuses(Player player); // sub_6A3C6
    static void calc_cleric_spells(bool ResetSpellLevels, Player player); /* sub_6A686 */
    inline static Array3<byte> SaveThrowValues = { // [8,13,5] class, level, save_type
            {{20, 20, 20, 20, 20}, {10, 13, 14, 16, 15}, {10, 13, 14, 16, 15}, {10, 13, 14, 16, 15}, {9, 12, 13, 15, 14}, {9, 12, 13, 15, 14}, {9, 12, 13, 15, 14}, {7, 10, 11, 13, 12}, {7, 10, 11, 13, 12}, {7, 10, 11, 13, 12}, {6, 9, 10, 12, 11}, {6, 9, 10, 12, 11}, {6, 9, 10, 12, 11}},
            {{20, 20, 20, 20, 20}, {10, 13, 14, 16, 15}, {10, 13, 14, 16, 15}, {10, 13, 14, 16, 15}, {9, 12, 13, 15, 14}, {9, 12, 13, 15, 14}, {9, 12, 13, 15, 14}, {7, 10, 11, 13, 12}, {7, 10, 11, 13, 12}, {7, 10, 11, 13, 12}, {6, 9, 10, 12, 11}, {6, 9, 10, 12, 11}, {6, 9, 10, 12, 11}},
            {{20, 20, 20, 20, 20}, {14, 15, 16, 17, 17}, {14, 15, 16, 17, 17}, {13, 14, 15, 16, 16}, {13, 14, 15, 16, 16}, {11, 12, 13, 13, 14}, {11, 12, 13,13, 14}, {10, 11, 12, 12, 13}, {10, 11, 12, 12, 13}, {8, 9, 10, 9, 11}, {8, 9, 10, 9, 11}, {7, 8, 9, 8, 10}, {7, 8, 9, 8, 10}},
            {{20, 20, 20, 20, 20}, {12, 13, 14, 15, 15}, {12, 13, 14, 15, 15}, {11, 12, 13, 14, 14}, {11, 12, 13, 14, 14}, {9, 9, 11, 11, 12}, {9, 9, 11, 11, 12}, {8, 9, 10, 10, 11}, {9, 9, 10, 10, 11}, {6, 7, 8, 7, 9}, {6, 7, 8, 7, 9}, {5, 6, 7, 6, 8}, {5, 6, 7, 6, 8}},
            {{20, 20, 20, 20, 20}, {14, 15, 16, 17, 17}, {14, 15, 16, 17, 17}, {13, 14, 15, 16, 16}, {13, 14, 15, 16, 16}, {11, 12, 13, 13, 14}, {11, 12, 13, 13, 14}, {10, 11, 12, 12, 13}, {10, 11, 12, 12, 13}, {8, 9, 10, 9, 11}, {8, 9, 10, 9, 11}, {7, 8, 9, 8, 10}, {7, 8, 9, 8, 10}},
            {{20, 20, 20, 20, 20}, {14, 13, 11, 15, 12}, {14, 13, 11, 15, 12}, {14, 13, 11, 15, 12}, {14, 13, 11, 15, 12}, {14, 13, 11, 15, 12}, {13, 11, 9, 13, 10}, {13, 11, 9, 13, 10}, {13, 11, 9, 13, 10}, {13, 11, 9, 13, 10}, {13, 11, 9, 13, 10}, {11, 9, 7, 11, 8}, {11, 9, 7, 11, 8}},
            {{20, 20, 20, 20, 20}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {11, 10, 10, 14, 11}, {11, 10, 10, 14, 11}, {11, 10, 10, 14, 11}, {11, 10, 10, 14, 11}},
            {{20, 20, 20, 20, 20}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {11, 10, 10, 14, 11}, {13, 11, 9, 13, 10}, {11, 9, 7, 11, 8}, {11, 9, 7, 11, 8}}};
    static void reclac_saving_throws(Player player); // sub_6A7FB
    static void SaveVersePoisonBonus(Player player, bool applyBonus);
    inline static Array2<sbyte> unk_1A230 = {
            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 10, 15, 0, 0, 0, -10, -5 },
            { 0, 5, -5, 0, 5, 10, 5, 0, 0 },
            { 0, 0, 5, 10, 5, 5, 10, -15, 0 },
            { 0, 10, 0, 0, 0, 5, 0, 0, 0 },
            { 0, 5, 5, 5, 10, 15, 5, -15, -5 },
            { 0, -5, 5, 5, 0, 0, 5, 5, -10 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, -15, -10, -10, -20, -10, -19, -5, 10 },
            { 0, -15, -5, -5, 0, -5, -10, 0, 0 },
            { 0, 0, 0, -5, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, -5, 0, 0, 0 } }; /*seg600:3F20 */
    inline static Array2<sbyte> unk_1A243 = {
            { 0, 5, 10, 5, 0, 0 },
            { 0, 0, 5, 10, 5, 5 },
            { 5, 10, -15, 0, 10, 0 },
            { 0, 0, 0, 5, 0, 0 },
            { 0, 0, 5, 5, 5, 10 },
            { 10, 15, 5, -15, -5, -5 },
            { -5, 5, 5, 0, 0, 5 },
            { 5, 5, -10, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, -15, -10, -10, -20, -10 },
            { -10, -19, -5, -10, -15, -5 },
            { -5, -5, 0, -5, -10, 0 },
            { 0, 0, 0, 0, -5, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, -5, 0, 0, 0 },
            { 0, 5, 10, 0, 5, 5 },
            { 5, 10, 15, 5, 10, 10 },
            { 10, 15, 20, 10, 12, 12 },
            { 12, 8, 8, 18, 17, 99 },
            { 99, 0, 3, 18, 3, 18 } }; /*seg600:3F33 */
    inline static Array2<byte> base_chance = {
            { 0, 0, 0, 0 , 0 , 0 ,0 , 0, 0 },
            { 0, 0x1E, 0x19, 0x14 ,0x0F, 0x0A, 0x0A, 0x55, 0x00 },
            { 0, 0x23, 0x1D, 0x19, 0x15, 0x0F, 0x0A, 0x56, 0x00 },
            { 0, 0x28, 0x21, 0x1E, 0x1B, 0x14, 0x0F, 0x57, 0x00 },
            { 0, 0x2D, 0x25, 0x23, 0x21, 0x19, 0x0F, 0x58, 0x14 },
            { 0, 0x32, 0x2A, 0x28, 0x28, 0x1F, 0x14, 0x5A, 0x19 },
            { 0, 0x37, 0x2F, 0x2D, 0x25, 0x14, 0x5C, 0x1E, 0x3C },
            { 0, 0x34, 0x32, 0x37, 0x2B, 0x19, 0x5E, 0x23, 0x41},
            { 0, 0x39, 0x37, 0x3E, 0x31, 0x19, 0x60, 0x28, 0x46},
            { 0, 0x3E, 0x3C, 0x46, 0x38, 0x1E, 0x62, 0x2D, 0x50},
            { 0, 0x43, 0x41, 0x4E, 0x3F, 0x1E, 0x63, 0x32, 0x5A},
            { 0, 0x48, 0x46, 0x56, 0x46, 0x23, 0x63, 0x3C, 0x64},
            { 0, 0x64, 0x4D, 0x4B, 0x5E, 0x4D, 0x23, 0x63, 0x41 } }; /*seg600:3EC0 unk_1A1D0 */
    static void reclac_thief_skills(Player player); // sub_6AAEA
    static bool SecondClassAllowed(ClassId _class, Player player); /* sub_6AD3E */
    static void DuelClass(Player player);
    static ClassId HumanCurrentClass_Unknown(Player player); // getFirstSkill
    static byte HumanCurrentClassLevel_Zero(Player player); /* hasAnySkills */
    static bool DualClassExceedLastLevel(Player player); // sub_6B3D1
};

// ---------------------------------------------------------------------------
// ovr027.cs
// ---------------------------------------------------------------------------
struct ovr027 {
    static MenuItem getStringListEntry(List<MenuItem> list, int index);
    inline static Set highlightable_text = New<Set_>(Array<int>{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'});
    class highlight_ {
    public:
        int start = -1;
        int end = -1;
        String ToString() const;
    };
    using highlight = cs::Ref<highlight_>;

    class HighlightSet_ {
    public:
        Array<highlight> highlights;
        static constexpr int length = 20;
        HighlightSet_();
        highlight& operator[](int index); // C# indexer `internal highlight this[int index] { get; set; }`
        void Clear();
    };
    using HighlightSet = cs::Ref<HighlightSet_>;

    static HighlightSet BuildInputKeys(String menuText, int& highlighCount);
    static void display_highlighed_text(int highlighed_word, int highlightFgColor, String text, int xOffset, int fgColor, HighlightSet highlights); /* sub_6C1E9 */
    inline static Set alpha_number_input = New<Set_>(Array<int>{' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}); //unk_6C398
    inline static Set number_input = New<Set_>(Array<int>{'1', '2', '3', '4', '5', '6', '7', '8', '9', '\\'}); //unk_6C3B8
    inline static Array<char> keypad_ctrl_codes = { 'O', 'P', 'Q', 'K', ' ', 'M', 'G', 'H', 'I' };
    static char displayInput(bool useOverlay, byte arg_6, MenuColorSet colors, String displayInputString, String displayExtraString);
    static char displayInput(bool& specialKeyPressed, bool useOverlay, byte accept_ctrlkeys, MenuColorSet colors, String displayInputString, String displayExtraString);
    static void ClearPromptArea(); // redraw_screen
    static void ClearPromptAreaNoUpdate();
    static void sub_6C897(int index, int yEnd, int xEnd, int yStart, int xStart, List<MenuItem> list, int normalColor, int headingColor, int displayFillWidth); // sub_6C897
    static int getBegingOfString(String text);
    static void ListItemHighlighted(int index, List<MenuItem> stringList, int yCol, int xCol, int bgColor);
    static void ListItemNormal(int index, List<MenuItem> list, int yCol, int xCol, int normalColor, int headingColor);
    static int skipHeadings(bool backwardsStep, int index, List<MenuItem> list, int listDisplayHeight); // sub_6CC08
    static void menu_scroll_page(bool backwardsStep, int& index, List<MenuItem> list, int listDisplayHeight, int yEnd, int xEnd, int yStart, int xStart, int normalColor, int headingColor, int displayFillWidth); // sub_6CD38
    static int menu_scroll_in_page(bool backwardsStep, int index, List<MenuItem> list, int listDisplayHeight); // sub_6CDCA
    static char sl_select_item(MenuItem& result_ptr, int& index_ptr, bool& redrawMenuItems, bool showExit, List<MenuItem> stringList, int endY, int endX, int startY, int startX, MenuColorSet colors, String inputString, String extraTextString);
    static char yes_no(MenuColorSet colors, String inputString);
};

// ---------------------------------------------------------------------------
// ovr028.cs
// ---------------------------------------------------------------------------
struct MapCursor { // ovr028
    inline static Array<int> city_map_x = { /* unk_16D5A */
            0x04,0x0C,0x15,0x0B,0x1D,0x14,0x26,0x15,
            0x1E,0x1F,0x19,0x25,0x1C,0x1D,0x03,0x0C,
            0x19,0x1D,0x1D,0x21,0x13,0x10,0x09,0x10,
            0x14,0x15,0x19,0x19,0x1A,0x1F,0x25,0x22, 0x0F };
    inline static Array<int> city_map_y = { /* unk_16D7A */
            0x0F,0x08,0x0B,0x04,0x0A,0x04,0x01,0x02,
            0x0D,0x0F,0x03,0x05,0x02,0x08,0x0C,0x0D,
            0x0A,0x0C,0x09,0x09,0x08,0x06,0x06,0x03,
            0x02,0x02,0x03,0x02,0x03,0x04,0x02,0x01, 0x00 };
    inline static int loc_X = 0; // word_1EF9C
    inline static int loc_Y = 0; // word_1EF9E
    static void SetPosition(int currentCity); //sub_6E005
    static void Draw(); /* sub_6E02E */
    static void Restore(); /* sub_6E05D */
};

// ---------------------------------------------------------------------------
// ovr029.cs
// ---------------------------------------------------------------------------
struct ovr029 {
    inline static Array<int> sky_colours = { /* seg600:0A8A unk_16D9A*/
        0x00, 0x0F, 0x04, 0x0B, 0x0D, 0x02, 0x09, 0x0E, 0x00, 0x0F, 0x04, 0x0B, 0x0D, 0x02 , 0x09, 0x0E};
    static void RedrawView(); /* sub_6F0BA */
};

// ---------------------------------------------------------------------------
// ovr030.cs
// ---------------------------------------------------------------------------
struct ovr030 {
    inline static Array<byte> fadeOldColors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    inline static Array<byte> fadeNewColors = { 12, 12, 12, 12, 4, 5, 6, 7, 12, 12, 10, 12, 12, 12, 14, 12 };
    inline static Array<byte> transparentOldColors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    inline static Array<byte> transparentNewColors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 14, 15 };
    static void DrawMaybeOverlayed(DaxBlock dax_block, bool useOverlay, int rowY, int colX); // sub_7000A
    static void load_pic_final(DaxArray& daxArray, byte masked, byte block_id, String file_name);
    static void DaxArrayFreeDaxBlocks(DaxArray animation);
    static void head_body(byte body_id, byte head_id);
    static void draw_head_and_body(bool draw_body, byte rowY, byte colX); /* sub_706DC */
    static void Show3DSprite(DaxArray arg_0, int sprite_index);
    static void load_bigpic(byte block_id); /* bigpic */
    static void draw_bigpic(); /* sub_7087A */
};

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

// ---------------------------------------------------------------------------
// ovr032.cs
// ---------------------------------------------------------------------------
struct ovr032 {
    class MapReach_ {
    public:
        bool reach = false;
        Point target;
        int range = 0;
        MapReach_(bool r, int ra, Point p);
    };
    using MapReach = cs::Ref<MapReach_>;

    inline static Array3<MapReach> mapReachCache = Array3<MapReach>(Point::MapMaxY * Point::MapMaxX, Point::MapMaxY * Point::MapMaxX, 2);
    static MapReach MapCacheGet(Point p1, Point p2, bool ignoreWalls);
    static void canReachTarget(Point& target, Point attacker);
    static bool canReachTarget(int& range, Point target, Point attacker);
    static MapReach canReachTargetCalc(Struct_1D1BC groundTilesMap, Point outPos, Point attacker); /* sub_733F1 */
    static bool CanSeeCombatant(int direction, Point playerA, Point playerB); /* sub_7354A */
    static List<SortedCombatant> Rebuild_SortedCombatantList(Player player, int max_range, std::function<bool(Player)> filter); /* sub_738D8 */
    static List<SortedCombatant> Rebuild_SortedCombatantList(int size, int max_range, Point pos, std::function<bool(Player)> filter); /* sub_738D8 */
    static byte FindCombatantDirection(Point target, Point attacker);
};

// ---------------------------------------------------------------------------
// ovr033.cs
// ---------------------------------------------------------------------------
struct ovr033 {
    static constexpr int MaxSize = 4;
    inline static Array<Array<Point>> Steps = {
            Array<Point>(0),
            Array<Point>{ Point( 0, 0 ) },
            Array<Point>{ Point( 0, 0 ), Point(  0,  1 ) },
            Array<Point>{ Point( 0, 0 ), Point(  1,  0 ) },
            Array<Point>{ Point( 0, 0 ), Point(  1,  0 ), Point( 0,  1 ), Point(  1,  1 ) }
        };
    static Array<Point> GetSizeBasedMapDeltas(int size); /*sub_7400F*/
    static Array<Point> BuildSizeMap(int size, Point pos);
    static void calculatePlayerScreenPositions(); /* sub_74077 */
    static void Color_0_8_inverse();
    static void Color_0_8_normal();
    static void sub_7416E(Point pos); // sub_7416E
    static void DrawPlayerIconIfOnScreen(int player_index);
    static void RedrawPosition(Point pos); // sub_7431C
    inline static Array2<int> mapToPlayerIndex = Array2<int>(Point::MapMaxY, Point::MapMaxX);
    static void setup_mapToPlayerIndex_and_playerScreen(); /* sub_743E7 */
    static int PlayerIndexAtMapXY(int posY, int posX); /* sub_74505 */
    static void AtMapXY(int& groundTile, int& playerIndex, Point pos);
    static void AtMapXY(int& groundTile, int& playerIndex, int posY, int posX); /* sub_74505 */
    static void RedrawPlayerBackground(int player_index, Point map); /* sub_74572 */
    static void RedrawPlayerBackground(int player_index); /* sub_74572 */
    static bool CoordOnScreen(Point pos); /* sub_74730 */
    static bool PlayerOnScreen(bool AllOnScreen, Player player); // sub_74761
    static bool PlayerOnScreen(bool AllOnScreen, int player_index); // sub_74761
    static bool ScreenMapCheck(int radius, Point pos);
    static void redrawCombatArea(int dir, int radius, Point map); /*sub_749DD*/
    static void draw_74B3F(bool arg_0, Icon iconState, int direction, Player player); /* sub_74B3F */
    static Point PlayerMapPos(Player player); /* sub_74C5A */
    static int PlayerMapSize(Player player); /*sub_74C82*/
    static int GetPlayerIndex(Player player);
    static void getGroundInformation(int& groundTile, int& playerIndex, byte direction, Player player); /* sub_74D04 */
    static void getGroundInformation(bool& isPoisonousCloud, bool& isNoxiousCloud, int& groundTile, int& playerIndex, int direction, Player player); /* sub_74D04 */
    static void CombatantKilled(Player player); // sub_74E6F
    static bool sub_7515A(bool arg_0, Point pos, Player player);
    static void RedrawCombatIfFocusOn(bool draw_cursor, byte radius, Player player); // sub_75356
};

// ---------------------------------------------------------------------------
// ovr034.cs
// ---------------------------------------------------------------------------
struct ovr034 {
    static void Load24x24Set(int cellCount, int destCellOffset, int block_id, String fileName);
    static void DrawIsoTile(int tileIndex, int rowY, int colX); /* sub_760F7 */
    static void ReleaseCombatIcon(int index); // free_icon
    inline static Array<byte> unk_16E30 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }; // seg600:0B20
    inline static Array<byte> unk_16E40 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }; // seg600:0B30
    inline static Array<byte> unk_16E50 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 14, 15 }; // seg600:0B40
    static void chead_cbody_comspr_icon(byte combat_icon_index, int block_id, String fileText);
    static void draw_combat_icon(int iconIndex, Icon iconState, int direction, int tileY, int tileX); /* sub_76504 */
};

// ---------------------------------------------------------------------------
// ovr038.cs
// ---------------------------------------------------------------------------
struct ovr038 {
    static void Load8x8D(int symbolSet, int block_id);
    static void Put8x8Symbol(byte arg_0, bool use_overlay, int symbol_id, int rowY, int colX);
};

// ---------------------------------------------------------------------------
// seg001.cs
// ---------------------------------------------------------------------------
struct seg001 {
    // PORT-NOTE: `internal static System.Threading.Thread EngineThread;` dropped; EngineStop() is hand-ported (platform::EngineStopped + throw cs::EngineAbort).
    using VoidDelegate = std::function<void()>;
    inline static VoidDelegate EngineStoppedCallback;
    static void EngineStop();
    static void __SystemInit(VoidDelegate stoppedCallback);
    static void ConfigGame();
    static void PROGRAM();
    static void InitFirst(); /* sub_39054 */
    static void InitAgain(); /* sub_396E5 */
};

// ---------------------------------------------------------------------------
// seg037.cs
// ---------------------------------------------------------------------------
struct seg037 {
    inline static Array<int> outer_frame_bottom = { 1, 8, 6, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 6, 8, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3 }; /*unk_16EB0*/
    inline static Array<byte> unk_16ED6 = { 4, 3, 0, 6, 1, 1, 1, 1, 8, 1, 1, 4, 1, 1, 2, 1, 4 };
    inline static Array<byte> unk_16F0A = { 0, 7, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 2, 9, 4 };
    inline static Array<int> outer_frame_top = { 0, 6, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 4, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 6, 1, 2 }; /*byte_16E60*/
    inline static Array<byte> x8x8_07 = { 0, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 8, 4,
                                    1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 6, 1,
                                    1, 1, 1, 1, 8, 2 };
    inline static Array<byte> unk_16EE3 = { 1, 2, 1, 4, 1, 1, 1, 1, 1, 1, 8, 4, 1, 1, 3 };
    inline static Array<int> outer_frame_left = { 0, 2, 9, 5, 2, 2, 2, 2, 2, 2, 5, 7, 2, 2, 2, 2, 2, 9, 7, 2, 2, 2, 7, 1 }; /*unk_16EF2*/
    inline static Array<int> outer_frame_right = { 2, 2, 9, 7, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 5, 2 }; /*unk_16F1B*/
    inline static Array<byte> unk_16F31 = { 5, 2, 0, 2, 7, 2, 2, 2, 2, 5, 2, 2, 2, 2, 1 };
    inline static Array<byte> unk_16F3E = { 2, 1, 2, 5, 9, 2, 2, 2, 7, 5, 2, 2, 2, 2, 3 };
    inline static Array<byte> unk_16F4D = { 0, 2, 9, 5, 2, 2, 2, 2, 2, 2, 5, 7, 2, 2, 2, 2, 2, 9, 7, 2, 2, 2, 1 };
    inline static Array<byte> unk_16F64 = { 0, 7, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 5, 2, 2, 2, 2, 2, 2, 5, 2, 4 };
    inline static Array<byte> unk_16F7B = { 2, 2, 9, 7, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2 };
    static void DrawFrame_Outer(); /* draw8x8_01 */
    static void draw8x8_02();
    static void draw8x8_03();
    static void DrawFrame_WildernessMap(); // draw8x8_04
    static void draw8x8_05();
    static void DrawFrame_Combat(); // draw8x8_06
    static void draw8x8_07();
    static void draw8x8_clear_area(TextRegion region);
    static void draw8x8_clear_area(int yEnd, int xEnd, int yStart, int xStart);
};

// ---------------------------------------------------------------------------
// seg040.cs
// ---------------------------------------------------------------------------
struct seg040 {
    static DaxBlock LoadDax(byte mask_colour, byte masked, int block_id, String fileName); // load_dax
    static void OverlayUnbounded(DaxBlock source, int arg_8, int itemIdex, int rowY, int colX);
    static void OverlayBounded(DaxBlock source, int arg_8, int itemIndex, int rowY, int colX); /* sub_E353 */
    static void ega_backup(DaxBlock dax_block, int rowY, int colX); /* ega_01 */
    inline static int color_no_draw = 17;
    inline static int color_re_color_from = 17;
    inline static int color_re_color_to = 17;
    static void draw_clipped_recolor(int from, int to);
    static void draw_clipped_nodraw(int color);
    static void draw_clipped_picture(DaxBlock dax_block, int rowY, int colX, int index, int clipMinX, int clipMaxX, int clipMinY, int clipMaxY);
    static void draw_combat_picture(DaxBlock dax_block, int rowY, int colX, int index);
    static void draw_picture(DaxBlock dax_block, int rowY, int colX, int index);
    static void DrawOverlay();
    static void SetPaletteColor(int color, int index);
    static void DrawColorBlock(int color, int lineCount, int colWidth, int lineY, int colX);
};

// ---------------------------------------------------------------------------
// seg041.cs
// ---------------------------------------------------------------------------
struct seg041 {
    static void DrawRectangle(byte color, int yEnd, int xEnd, int yStart, int xStart);
    static void Load8x8Tiles(); // load_8x8d1_201
    static void display_char01(char ch, int repeatCount, int bgColor, int fgColor, int YCol, int XCol); // display_char01
    static void displaySpaceChar(int yCol, int xCol);
    static void displayString(String str, int bgColor, int fgColor, int yCol, int xCol);
    static int displayStringSlow(String text, int text_index, int text_length, int fgColor); // sub_107DE
    static void text_skip_space(String text, int text_max, int& text_index); /* sub_10854 */
    inline static Array2<int> bounds = {
            { 0x16, 0x26, 0x11, 1 },
            { 0x16, 0x26, 0x15, 1 },
            { 0x15, 0x26, 1, 0x17 } // TextRegion.CombatSummary
        };
    static void press_any_key(String text, bool clearArea, int fgColor, TextRegion region);
    inline static Set puncutation = New<Set_>(Array<int>{33, 44, 45, 46, 58, 59, 63}); // "!,-.:;?" // 33,44,45,46,58,59,63
    static void press_any_key(String text, bool clearArea, int fgColor, int yEnd, int xEnd, int yStart, int xStart);
    static String getUserInputString(byte inputLen, byte bgColor, byte fgColor, String prompt);
    static ushort getUserInputShort(byte bgColor, byte fgColor, String prompt);
    static void DisplayAndPause(String txt, byte fgColor); // displayAndDebug
    static int time01(); // time01
    static void ClearScreen();
    static void DisplayStatusText(byte bgColor, byte fgColor, String text); /* sub_10ECF */
    static void GameDelay();
};

// ---------------------------------------------------------------------------
// seg042.cs
// ---------------------------------------------------------------------------
struct seg042 {
    static void debug_display(String text);
    static void delete_file(String fileString);
    static bool find_and_open_file(File& file_ptr, bool noError, String full_file_name);
    static bool file_find(String filePath);
    inline static Array<char> unk_16FA9 = { ' ', '.', '*', ',', '?', '/', '\\', ':', ';', '|' };
    static String clean_string(String s);
    static bool setupDaxFiles(IO::BinaryReader& fileA, IO::BinaryReader& fileB, short& arg_8, String file_name);
    static void load_decode_dax(Array<byte>& out_data, short& decodeSize, int block_id, String file_name);
    static void set_game_area(byte arg_0);
    static void restore_game_area();
};

// ---------------------------------------------------------------------------
// seg043.cs
// ---------------------------------------------------------------------------
struct seg043 {
    inline static bool in_print_and_exit = false;
    static void print_and_exit();
    static byte GetInputKey();
    static void DumpPlayerAffects();
    static void ToggleCommandDebugging();
    static void clear_keyboard();
    static void clear_one_keypress();
    static void DumpTreasureItems();
    static void TxtDumpPlayer(Player p, int area, int id);
    static void HtmlTableDumpPlayer(Ref<Logging::DebugWriter> dw, Player p, byte area, int id);
    static void DumpMonstersFiltered();
    static void DumpMonstersFilteredSub(Ref<Logging::DebugWriter> dw, std::function<bool(Player)> filter);
    static void DumpMonsters();
};

// ---------------------------------------------------------------------------
// seg044.cs
// ---------------------------------------------------------------------------
struct seg044 {
    static void SetSound(bool On);
    static void SetPicture(bool On);
    static void SetAnimation(bool On);
    static void PlaySound(Sound arg_0); /*sub_120E0*/
    inline static Array<bool> soundLoaded; // PORT-NOTE: was `static System.Media.SoundPlayer[] sounds;` (13 slots); hand-ported against platform::LoadSound/PlaySound/StopAllSounds.
    static void SoundInit();
};

// ---------------------------------------------------------------------------
// seg049.cs
// ---------------------------------------------------------------------------
struct seg049 {
    inline static byte skipReadFlag = 0;
    static void SysDelay(int milliseconds);
    inline static Queue<ushort> keysPressed;
    inline static std::mutex keysPressedLock; // PORT-NOTE: C# `lock (keysPressed)`
    // PORT-NOTE: C# `static Semaphore WaitForKey = new Semaphore(0, 1);` -> binary semaphore built from
    // a mutex + condition variable + count (0 or 1). Hand-ported.
    inline static std::mutex WaitForKeyLock;
    inline static std::condition_variable WaitForKey;
    inline static int WaitForKeyCount = 0;
    static void AddKey(ushort key);
    static ushort int_check_keyPressed();
    static ushort int_get_keyPressed();
    static bool KEYPRESSED();
    static byte READKEY();
    // goldbox-3ds: host shutdown request; the next key wait throws cs::EngineAbort.
    inline static bool abortRequested = false;
    static void RequestAbort();
};

// ---------------------------------------------------------------------------
// seg051.cs
// ---------------------------------------------------------------------------
struct seg051 {
    inline static cs::Random random_number{0}; // PORT-NOTE: C# is null until Randomize(); cs::Random has no default ctor. Spelled cs::Random because seg051::Random(...) methods hide the type name.
    static String Copy(int CopyLen, int StartAt, String InString);
    static byte Random(byte arg_0);
    static int Random(int arg_0);
    static double Random__Real();
    static void Randomize();
    static void Reset(File arg_4);
    static void Rewrite(File arg_2);
    static void Close(File arg_0);
    static int BlockRead(int count, Array<byte> data, File file);
    static void BlockWrite(int arg_4, Array<byte> arg_6, File arg_A);
    static void FillChar(byte fill_byte, int buffer_size, Array<byte> buffer);
};

}  // namespace engine

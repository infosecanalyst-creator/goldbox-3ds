// Generated once by tools/outline_statics.py (kept in the tree, edit freely).
// The non-trivial "inline static" tables used to live in the headers, so every translation unit
// that includes them carried its own copy of their initialisation code (about 5 MB of the 3DS
// code segment). They are defined here instead, once.
#include "Classes/Classes.h"
using namespace cs;
using namespace cs::enum_ops;
using namespace Classes;

// Gbl.h
MenuColorSet Classes::gbl::defaultMenuColors = New<MenuColorSet_>(15, 10, 13);

// Gbl.h
MenuColorSet Classes::gbl::alertMenuColors = New<MenuColorSet_>(15, 10, 14);

// Gbl.h
Array<Struct_189B4> Classes::gbl::BackGroundTiles { /* unk_189B4 */
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

// Gbl.h
const Array<byte> Classes::gbl::max_class_hit_dice { 10, 15, 10, 10, 11, 12, 11, 13 };

// Gbl.h
const Array<byte> Classes::gbl::default_icon_colours { 1, 2, 3, 4, 6, 7 };

// Gbl.h
const Array<Array<ClassId>> Classes::gbl::RaceClasses {
        Array<ClassId>(0) , 
        Array<ClassId>/*Dwarf*/{ ClassId::fighter, ClassId::thief, ClassId::mc_f_t},
        Array<ClassId>/*Elf*/{ ClassId::fighter, ClassId::magic_user, ClassId::thief, ClassId::mc_f_mu, ClassId::mc_f_t, ClassId::mc_f_mu_t, ClassId::mc_mu_t},
        Array<ClassId>/*Gnome*/{ ClassId::fighter, ClassId::thief, ClassId::mc_f_t},
        Array<ClassId>/*Half-Elf*/{ ClassId::cleric, ClassId::fighter, ClassId::magic_user, ClassId::thief, ClassId::ranger,ClassId::mc_c_f, ClassId::mc_c_r, ClassId::mc_c_f_m, ClassId::mc_c_mu, ClassId::mc_f_mu, ClassId::mc_f_t, ClassId::mc_f_mu_t, ClassId::mc_mu_t},
        Array<ClassId>/*Halfling*/{ ClassId::fighter, ClassId::thief, ClassId::mc_f_t},
        Array<ClassId>/*Half-Orc*/{ ClassId::cleric, ClassId::fighter, ClassId::thief, ClassId::mc_c_f, ClassId::mc_c_t,ClassId::mc_f_t},
        Array<ClassId>/*Human*/{ ClassId::cleric, ClassId::fighter, ClassId::magic_user, ClassId::thief, ClassId::paladin, ClassId::ranger},
        Array<ClassId>/*Cheaters*/{ ClassId::cleric, ClassId::fighter, ClassId::magic_user, ClassId::thief, ClassId::ranger,ClassId::mc_c_f, ClassId::mc_c_r, ClassId::mc_c_f_m, ClassId::mc_c_mu, ClassId::mc_f_mu, ClassId::mc_f_t, ClassId::mc_f_mu_t, ClassId::mc_mu_t}};

// Gbl.h
Array<int> Classes::gbl::team_start_x { 0, 0 };

// Gbl.h
Array<int> Classes::gbl::team_start_y { 0, 0 };

// Gbl.h
Array<int> Classes::gbl::half_team_count { 0, 0 };

// Gbl.h
Array<int> Classes::gbl::team_direction { 0, 0 };

// Gbl.h
Array<bool> Classes::gbl::affects_timed_out = Array<bool>(0x48);

// Gbl.h
SoundType Classes::gbl::soundType = SoundType::None;

// Gbl.h
Array<byte> Classes::gbl::monoCharData = Array<byte>(8);

// Gbl.h
Affects Classes::gbl::current_affect = (Affects)0;

// Gbl.h
DamageType Classes::gbl::damage_flags = (DamageType)0;

// Gbl.h
Array<byte> Classes::gbl::bytes_1D2C9 = Array<byte>(3);

// Gbl.h
Array<gbl::SetBlock> Classes::gbl::setBlocks = Array<gbl::SetBlock>(3);

// Gbl.h
DaxArray Classes::gbl::byte_1D556 ;

// Gbl.h
String Classes::gbl::lastDaxFile ;

// Gbl.h
String Classes::gbl::byte_1D5AB ;

// Gbl.h
Point Classes::gbl::targetPos ;

// Gbl.h
Array<byte> Classes::gbl::bytes_1D900 = Array<byte>(3);

// Gbl.h
Array<bool> Classes::gbl::encounter_flags = Array<bool>(2);

// Gbl.h
const Array<short> Classes::gbl::symbol_set_fix { 0x0001, 0x002E, 0x0074, 0x00BA, 0x0100 };

// Gbl.h
DaxBlock Classes::gbl::dword_1C8FC ;

// Gbl.h
DaxBlock Classes::gbl::bigpic_dax ;

// Gbl.h
char Classes::gbl::displayInputTimeoutValue = '\0';

// Gbl.h
Dictionary<Spells, spellDelegate2> Classes::gbl::spellTable = nullptr;

// Gbl.h
Player Classes::gbl::lastSelectetSpellTarget ;

// Gbl.h
RestTime Classes::gbl::timeToRest = New<RestTime_>();

// Gbl.h
Array<Item> Classes::gbl::scribeScrolls = Array<Item>(0x30);

// Gbl.h
Array<byte> Classes::gbl::memorize_spell_id = Array<byte>(max_spells);

// Gbl.h
Array<int> Classes::gbl::memorize_count = Array<int>(max_spells);

// Gbl.h
Array<Struct_1ADF6> Classes::gbl::dword_1ADF6 ;

// Gbl.h
List<MenuItem> Classes::gbl::spell_string_list ;

// Gbl.h
DaxBlock Classes::gbl::cursor_bkup ;

// Gbl.h
DaxBlock Classes::gbl::dax24x24Set ;

// Gbl.h
Item Classes::gbl::currentScroll ;

// Gbl.h
spellDelegate Classes::gbl::SpellCastFunction ;

// Gbl.h
DaxBlock Classes::gbl::missile_dax ;

// Gbl.h
Stack<ushort> Classes::gbl::vmCallStack ;

// Gbl.h
DaxBlock Classes::gbl::cursor ;

// Gbl.h
Player Classes::gbl::LastSelectedPlayer ;

// Gbl.h
Player Classes::gbl::SelectedPlayer ;

// Gbl.h
GameState Classes::gbl::game_state = (GameState)0;

// Gbl.h
GameState Classes::gbl::last_game_state = (GameState)0;

// Gbl.h
CombatType Classes::gbl::combat_type = (CombatType)0;

// Gbl.h
List<Player> Classes::gbl::TeamList ;

// Gbl.h
Item Classes::gbl::item_ptr ;

// Gbl.h
Player Classes::gbl::spell_target ;

// Gbl.h
List<Player> Classes::gbl::spellTargets ;

// Gbl.h
Array<Player> Classes::gbl::player_array = Array<Player>(256);

// Gbl.h
Array<CombatantMap> Classes::gbl::CombatMap ;

// Gbl.h
Player Classes::gbl::tradeWith ;

// Gbl.h
Area1 Classes::gbl::area_ptr ;

// Gbl.h
Area2 Classes::gbl::area2_ptr ;

// Gbl.h
Struct_1B2CA Classes::gbl::stru_1B2CA ;

// Gbl.h
EclBlock Classes::gbl::ecl_ptr ;

// Gbl.h
Array2<byte> Classes::gbl::dax_8x8d1_201 ;

// Gbl.h
WallDefs Classes::gbl::wallDef = New<WallDefs_>();

// Gbl.h
GeoBlock Classes::gbl::geo_ptr = New<GeoBlock_>();

// Gbl.h
Array<CombatIcon> Classes::gbl::combat_icons ;

// Gbl.h
Array<DaxBlock> Classes::gbl::symbol_8x8_set ;

// Gbl.h
List<Item> Classes::gbl::items_pointer = nullptr;

// Gbl.h
MoneySet Classes::gbl::pooled_money = New<MoneySet_>();

// Gbl.h
Array<bool> Classes::gbl::compare_flags = Array<bool>(6);

// Gbl.h
DaxBlock Classes::gbl::headX_dax ;

// Gbl.h
DaxBlock Classes::gbl::bodyX_dax ;

// Gbl.h
SaveVerseType Classes::gbl::saveVerseType = (SaveVerseType)0;

// Gbl.h
DaxBlock Classes::gbl::sky_dax_250 ;

// Gbl.h
DaxBlock Classes::gbl::sky_dax_251 ;

// Gbl.h
DaxBlock Classes::gbl::sky_dax_252 ;

// Gbl.h
Array<String> Classes::gbl::unk_1D972 { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" };

// Gbl.h
Array<Opperation> Classes::gbl::cmd_opps = Array<Opperation>(cmdOppsLimit);

// Gbl.h
Array<SpellEntry> Classes::gbl::spellCastingTable { /* seg600:37DC asc_19AEC */
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

// Gbl.h
Classes::ItemDataTable Classes::gbl::ItemDataTable;

// Gbl.h
List<DownedPlayerTile> Classes::gbl::downedPlayers = nullptr;

// Gbl.h
Struct_1D1BC Classes::gbl::mapToBackGroundTile ;

// Gbl.h
const Array<byte> Classes::gbl::SmallCloudDirections { 8, 2, 3, 4 };

// Gbl.h
const Array<byte> Classes::gbl::unk_18AEA { 2, 3, 4, 8 };

// Gbl.h
const Array<byte> Classes::gbl::CloudDirections { 8, 0, 1, 2, 3, 4, 5, 6, 7 };

// Gbl.h
List<GasCloud> Classes::gbl::StinkingCloud = nullptr;

// Gbl.h
List<GasCloud> Classes::gbl::CloudKillCloud = nullptr;

// Gbl.h
Array<Point> Classes::gbl::MapDirectionDelta { Point(0, -1), Point(1, -1), Point(1, 0), Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0), Point(-1, -1), Point(0, 0) };

// Gbl.h
const Array<sbyte> Classes::gbl::MapDirectionXDelta { 0, 1, 1, 1, 0, -1, -1, -1, 0 };

// Gbl.h
const Array<sbyte> Classes::gbl::MapDirectionYDelta { -1, -1, 0, 1, 1, 1, 0, -1, 0 };

// Gbl.h
ImportSource Classes::gbl::import_from = (ImportSource)0;

// Gbl.h
Array<ClassStatsMin> Classes::gbl::class_stats_min { 
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

// Gbl.h
String Classes::gbl::exe_path ;

// Gbl.h
String Classes::gbl::data_path ;

// Gbl.h
Struct_1A35E Classes::gbl::monster_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 6, 2, 6),
        New<SubStruct_1A35E_>( 0x0c08, 0xe, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 6, 0),
        New<SubStruct_1A35E_>( 0x502, 6, 3),
        New<SubStruct_1A35E_>( 4, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0)});

// Gbl.h
Struct_1A35E Classes::gbl::dwarf_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0xfa, 2, 0x14),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x28, 5, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x4B, 3, 6)});

// Gbl.h
Struct_1A35E Classes::gbl::elf_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0x28a, 10, 10),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x82, 5, 6),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x96, 5, 6),
        New<SubStruct_1A35E_>( 100, 5, 6)});

// Gbl.h
Struct_1A35E Classes::gbl::gnome_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0x12C, 3, 0xC),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x3C, 5, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 100, 2, 0x0C),
        New<SubStruct_1A35E_>( 0x50, 5, 4)});

// Gbl.h
Struct_1A35E Classes::gbl::halfelf_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0x28, 2, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x16, 3, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x1E, 2,  8),
        New<SubStruct_1A35E_>( 0x16, 3, 8)});

// Gbl.h
Struct_1A35E Classes::gbl::halfling_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 20, 3, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0x28, 2, 4)});

// Gbl.h
Struct_1A35E Classes::gbl::halforc_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 20, 1, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0xD, 1, 4),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 0, 0, 0),
        New<SubStruct_1A35E_>( 20, 2, 4)});

// Gbl.h
Struct_1A35E Classes::gbl::human_ages = New<Struct_1A35E_>(Array<SubStruct_1A35E>{
        New<SubStruct_1A35E_>( 18, 1, 4),
        New<SubStruct_1A35E_>( 18, 1, 4),
        New<SubStruct_1A35E_>( 15, 1, 4),
        New<SubStruct_1A35E_>( 17, 1, 4),
        New<SubStruct_1A35E_>( 20, 1, 4),
        New<SubStruct_1A35E_>( 0x18, 2, 4),
        New<SubStruct_1A35E_>( 18 , 1, 4)});

// Gbl.h
Array<Struct_1A35E> Classes::gbl::race_ages { monster_ages, dwarf_ages, elf_ages, gnome_ages, halfelf_ages, halfling_ages, halforc_ages, human_ages };

// Gbl.h
Array<byte> Classes::gbl::unk_1AE0B = Array<byte>(3);

// Gbl.h
const Array2<byte> Classes::gbl::class_alignments { 
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

// Limits.h
Array2<int> Classes::Limits::RaceAgeBrackets { 
        { 9999, 9999, 9999, 9999, 9999}, 
        {   50,  150,  250,  350,  450}, 
        {  175,  550,  875, 1200, 1600}, 
        {   90,  300,  450,  600,  750}, 
        {   40,  100,  175,  250,  325}, 
        { 0x21, 0x44, 0x65, 0x90, 0xC7}, 
        { 0x0F, 0x1E, 0x2D, 0x3C, 0x50}, 
        { 0x14, 0x28, 0x3C, 0x5A, 0x78} };

// Limits.h
Array<int> Classes::Limits::StrAgeEffect { 0, 1, -1, -2, -1 };

// Limits.h
Array<int> Classes::Limits::Str00AgeEffect { 0, 0, 0, 0, 0 };

// Limits.h
Array<int> Classes::Limits::IntAgeEffect { 0, 0, 1, 0, 1 };

// Limits.h
Array<int> Classes::Limits::WisAgeEffect { -1, 1, 1, 1, 1 };

// Limits.h
Array<int> Classes::Limits::DexAgeEffect { 0, 0, 0, -2, -1 };

// Limits.h
Array<int> Classes::Limits::ConAgeEffect { 1, 0, -1, -1, -1 };

// Limits.h
Array<int> Classes::Limits::ChaAgeEffect { 0, 0, 0, 0, 0 };

// Limits.h
Array3<int> Classes::Limits::StrRaceSexMinMax {
        { {0, 5}, {10,  0} },
        { {8, 8}, {18, 17} },
        { {3, 3}, {18, 16} },
        { {6, 6}, {18, 15} },
        { {3, 3}, {18, 17} },
        { {6, 6}, {17, 14} },
        { {6, 6}, {18, 18} },
        { {3, 3}, {18, 18} } };

// Limits.h
Array3<int> Classes::Limits::Str00RaceSexMinMax {
        { {0, 0}, {  5,  5} },
        { {0, 0}, { 99,  0} },
        { {0, 0}, { 75,  0} },
        { {0, 0}, { 50,  0} },
        { {0, 0}, { 90,  0} },
        { {0, 0}, {  0,  0} },
        { {0, 0}, { 99, 75} },
        { {0, 0}, {100, 50} } };

// Limits.h
Array3<int> Classes::Limits::IntRaceSexMinMax {
        { {10, 10}, {15, 15} } ,
        { { 3,  3}, {18, 18} } ,
        { { 8,  8}, {18, 18} } ,
        { { 7,  7}, {18, 18} } ,
        { { 4,  4}, {18, 18} } ,
        { { 6,  6}, {18, 18} } ,
        { { 3,  3}, {17, 17} } ,
        { { 3,  3}, {18, 18} } };

// Limits.h
Array3<int> Classes::Limits::WisRaceSexMinMax {
        { {5, 5}, {10, 10} },
        { {3, 3}, {18, 18} },
        { {3, 3}, {18, 18} },
        { {3, 3}, {18, 18} },
        { {3, 3}, {18, 18} },
        { {3, 3}, {17, 17} },
        { {3, 3}, {14, 14} },
        { {3, 3}, {18, 18} } };

// Limits.h
Array3<int> Classes::Limits::DexRaceSexMinMax {
        { {10, 10}, {15, 15} },
        { { 3,  3}, {17, 17} },
        { { 7,  7}, {19, 19} },
        { { 3,  3}, {18, 18} },
        { { 6,  6}, {18, 18} },
        { { 8,  8}, {18, 18} },
        { { 3,  3}, {17, 17} },
        { { 3,  3}, {18, 18} } };

// Limits.h
Array3<int> Classes::Limits::ConRaceSexMinMax {
        { {20, 20}, {10, 10} },
        { {12, 12}, {19, 19} },
        { { 6,  6}, {18, 18} },
        { { 8,  8}, {18, 18} },
        { { 6,  6}, {18, 18} },
        { {10, 10}, {19, 19} },
        { {13, 13}, {19, 19} },
        { { 3,  3}, {18, 18} } };

// Limits.h
Array3<int> Classes::Limits::ChaRaceSexMinMax {
        { {12, 12}, {12, 12} },
        { { 3,  3}, {16, 16} },
        { { 8,  8}, {18, 18} },
        { { 3,  3}, {18, 18} },
        { { 3,  3}, {18, 18} },
        { { 3,  3}, {18, 18} },
        { { 3,  3}, {12, 12} },
        { { 3,  3}, {18, 18} } };

// Limits.h
Array<int> Classes::Limits::StrClassMin { 6, 0, 9, 12, 13, 0, 6, 15, 9, 9, 0, 0, 0, 9, 9, 9, 0 };

// Limits.h
Array<int> Classes::Limits::IntClassMin { 6, 0, 0, 9, 13, 9, 6, 0, 0, 9, 13, 9, 0, 9, 0, 9, 9 };

// Limits.h
Array<int> Classes::Limits::WisClassMin { 9, 12, 6, 13, 14, 6, 0, 15, 9, 9, 14, 9, 9, 0, 0, 0, 0 };

// Limits.h
Array<int> Classes::Limits::DexClassMin { 0, 0, 6, 0, 0, 6, 9, 15, 0, 0, 0, 0, 9, 0, 9, 9, 9 };

// Limits.h
Array<int> Classes::Limits::ConClassMin { 0, 0, 7, 9, 14, 0, 0, 11, 0, 0, 14, 0, 0, 0, 0, 0, 0 };

// Limits.h
Array<int> Classes::Limits::ChaClassMin { 0, 15, 0, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Limits.h
Array<int> Classes::Limits::Str00ClassMin { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// ItemLibrary.h
String Classes::ItemLibrary::libraryPath ;

// ItemLibrary.h
String Classes::ItemLibrary::libraryFile ;

// ItemLibrary.h
List<Item> Classes::ItemLibrary::library ;

// MoneySet.h
Array<String> Classes::Money::names { "Copper", "Silver", "Electrum", "Gold", "Platinum", "Gems", "Jewelry" };

// MoneySet.h
Array<int> Classes::Money::per_copper { 1, 10, 100, 200, 1000 };

// SteppingPath.h
Array<byte> Classes::SteppingPath_::directions { 7, 0, 1, 6, 8, 2, 5, 4, 3, 8 };

// Player.h
Array<Affects> Classes::Player_::held_affects {Affects::snake_charm, Affects::paralyze, Affects::sleep, Affects::helpless};

// Item.h
Array<String> Classes::Item_::itemNames { "",
        "Battle Axe","Hand Axe","Bardiche","Bec De Corbin","Bill-Guisarme",
        "Bo Stick", "Club","Dagger","Dart","Fauchard",

        "Fauchard-Fork","Flail","Military Fork","Glaive","Glaive-Guisarme",
        "Guisarme","Guisarme-Voulge","Halberd","Lucern Hammer","Hammer",

        "Javelin","Jo Stick","Mace","Morning Star","Partisan",
        "Military Pick","Awl Pike","Quarrel","Ranseur","Scimitar",

        "Spear","Spetum","Quarter Staff","Bastard Sword","Broad Sword",

        "Long Sword","Short Sword","Two-Handed Sword","Trident","Voulge",
        "Composite Long Bow","Composite Short Bow","Long Bow","Short Bow","Heavy Crossbow",

        "Light Crossbow","Sling","Mail","Armor","Leather",
        "Padded","Studded","Ring","Scale","Chain",
        "Splint","Banded","Plate","Shield","Woods",

        "Arrow","","","Potion","Scroll",
        "Ring","Rod","Stave","Wand","Jug",
        "Amulet","Dragon Breath","Bag","Defoliation","Ice Storm",
        "Book","Boots","Hornets Nest","Bracers","Piercing",

        "Brooch","Elfin Chain","Wizardry","ac10", "Dexterity",
        "Fumbling","Chime","Cloak","Crystal","Cube",
        "Cubic","The Dwarves","Decanter","Gloves","Drums",
        "Dust","Thievery","Hat","Flask","Gauntlets",

        "Gem","Girdle","Helm","Horn","Stupidity",
        "Incense","Stone","Ioun Stone", "Javelin","Jewel",
        "Ointment","Pale Blue","Scarlet And","Manual","Incandescent",

        "Deep Red","Pink","Mirror","Necklace","And Green",
        "Blue","Pearl","Powerlessness",
        "Vermin","Pipes","Hole","Dragon Slayer","Robe","Rope",
        "Frost Brand","Berserker","Scarab","Spade","Sphere",
        "Blessed","Talisman","Tome","Trident","Grimoire","Well",
        "Wings","Vial","Lantern","","Flask of Oil",
        "10 ft. Pole","50 ft. Rope","Iron","Thf Prickly Tools",
        "Iron Rations","Standard Rations","Holy Symbol",
        "Holy Water vial","Unholy Water vial","Barding","Dragon",
        "Lightning","Saddle","Staff","Drow","Wagon","+1",
        "+2","+3","+4","+5","of","Vulnerability","Cloak",
        "Displacement","Torches","Oil","Speed","Tapestry",
        "Spine","Copper","Silver","Electrum","Gold","Platinum",
        "Ointment","Keoghtum's","Sheet","Strength","Healing",

        "Holding","Extra","Gaseous Form","Slipperiness",
        "Jewelled","Flying","Treasure Finding","Fear",
        "Disappearance","Statuette","Fungus","Chain","Pendant",
        "Broach","Of Seeking","-1","-2","-3","Lightning Bolt",
        "Fire Resistance","Magic Missiles","Save","Clrc Scroll",
        "MU Scroll","With 1 Spell","With 2 Spells","With 3 Spells",
        "Prot. Scroll","Jewelry","Fine","Huge","Bone","Brass",
        "Key","AC 2","AC 6","AC 4","AC 3","Of Prot.","Paralyzation",
        "Ogre Power","Invisibility","Missiles","Elvenkind",
        "Rotting","Covered","Efreeti","Bottle","Missile Attractor",
        "Of Maglubiyet","Secr Door & Trap Det","Gd Dragon Control",
        "Feather Falling","Giant Strength","Restoring Level(s)",
        "Flame Tongue","Fireballs","Spiritual","Boulder","Diamond",
        "Emerald","Opal","Saphire","Of Tyr","Of Tempus","Of Sune",
        "Wooden","+3 vs Undead","Pass","Cursed"
    };

// DaxCache.h
Dictionary<String, DaxFiles::DaxFileCache> Classes::DaxFiles::DaxCache::fileCache;


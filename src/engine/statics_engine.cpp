// Generated once by tools/outline_statics.py (kept in the tree, edit freely).
// The non-trivial "inline static" tables used to live in the headers, so every translation unit
// that includes them carried its own copy of their initialisation code (about 5 MB of the 3DS
// code segment). They are defined here instead, once.
#include "engine/engine.h"
using namespace cs;
using namespace cs::enum_ops;
using namespace Classes;
using namespace engine;

// ovr003.h
Dictionary<int, CmdItem> ovr003::CommandTable ;

// ovr003.h
volatile int ovr003::eclDepth = 0;

// ovr004.h
Array<String> ovr004::codeWheel = {
                                 "CWLNRTESSCEDCSHSISERRRNSHSSTSSNNHSHN",
                                 "LAASRDAIILIDSUGADAEEOEGRLSELIITESOIO",
                                 "LRUNIMMORIIGRRIUPTIIUELIMLHMIXACGRIL",
                                 "Z0LIOHEUVNODSGEOGXYWISIOCRARLRARRHOI",
                                 "AMTELRLUIYNAEOOITOUELRREREUIMADPPFAB",
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
                             };

// ovr005.h
Array<Affects> ovr005::disease_types = {  Affects::helpless,  Affects::cause_disease_1,
                                            Affects::weaken, Affects::cause_disease_2,
                                            Affects::animate_dead, Affects::affect_39 };

// ovr005.h
Array<String> ovr005::temple_sl = { "Cure Blindness", "Cure Disease", "Cure Light Wounds", "Cure Serious Wounds", "Cure Critical Wounds", "Heal", "Neutralize Poison", "Raise Dead", "Remove Curse", "Stone to Flesh", "Exit" };

// ovr006.h
Array<Affects> ovr006::affects_array = {
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

// ovr008.h
Set ovr008::unk_31673 = New<Set_>(Array<int>{48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90});

// ovr008.h
Set ovr008::validkeys = New<Set_>(Array<int>{48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90});

// ovr009.h
Set ovr009::unk_33748 = New<Set_>(Array<int>{16, 19, 45, 50, 71, 72, 73, 75, 77, 79, 80, 81});

// ovr009.h
Set ovr009::unk_33768 = New<Set_>(Array<int>{16, 19, 32, 45, 50, 65, 67, 68, 71, 72, 73, 75, 77, 79, 80, 81, 84, 85, 86});

// ovr010.h
Array2<int> ovr010::data_2B8 = {
            {8, 7, 6, 1, 2, 8}, {8, 1, 2, 7, 6, 7}, {7, 1, 8, 6, 2, 1}, {1, 7, 8, 2, 6, 8}, {8, 7, 6, 5, 4, 8},
            {8, 1, 2, 3, 4, 8}, {8, 4, 6, 2, 8, 6}, {6, 4, 0, 8, 0, 6}, {6, 2, 8, 2, 0, 4}, {4, 0, 0, 2, 6, 2},
            {2, 2, 0, 4, 4, 4} /*, {4, 2, 6, 6}*/ };

// ovr011.h
Array4<byte> ovr011::unk_1AB1C = Array4<byte>(2, 4, 6, 11);

// ovr011.h
Array<int> ovr011::dir_x_offset = { 0, 1, 0, -1 };

// ovr011.h
Array<int> ovr011::dir_y_offset = { -1, 0, 1, 0 };

// ovr011.h
Array<int> ovr011::CityInfo = { /* unk_16664 seg600:0354 */
            0x00, 0x18, 0x11, 0x15, 0x01, 0x01, 0x60, 0x14, 
            0x08, 0x01, 0x00, 0x21, 0x71, 0x09, 0x06, 0x04, 
            0x01, 0x09, 0x09, 0x08, 0x59, 0x00, 0x11, 0x11, 
            0x00, 0x00, 0x01, 0x11, 0x00, 0x00, 0x20, 0x20, 
            0x0A};

// ovr011.h
Array2<int> ovr011::direction_165EC = { { 8, 4, 6, 2 }, { 8, 6, 4, 0 }, { 8, 0, 6, 2 }, { 8, 2, 0, 4 } };

// ovr011.h
Array2<int> ovr011::direction_165FC = { { 0, 0, 2, 6 }, { 2, 2, 0, 4 }, { 4, 4, 2, 6 }, { 6, 6, 4, 0 } };

// ovr011.h
Array<int> ovr011::HalfDirToIso = { 7, 2, 3, 6 };

// ovr011.h
Array<byte> ovr011::unk_16610 = { 5, 4, 5, 6, 3, 8, 7, 2 };

// ovr011.h
Array<byte> ovr011::unk_16618 = { 3, 2, 2, 3, 0, 2, 5, 3 };

// ovr011.h
Array3<byte> ovr011::unk_16620 = { 
                {{1,0},{1,0},{1,0},{2,9},{3,10},{4,10}}, 
                {{0,2},{0,3},{1,4},{2,5},{3,6},{4,7}}, 
                {{0,6},{0,7},{1,8},{1,0},{1,0},{1,0}}, 
                {{3,6},{4,7},{5,8},{6,9},{7,10},{8,10}}, 
                {{0,6},{0,7},{1,8},{2,9},{3,10},{4,10}}, 
        };

// ovr013.h
Dictionary<Affects, affectDelegate> ovr013::affect_table = nullptr;

// ovr014.h
Array<sbyte> ovr014::unk_16679 = { 0,
        17, 17,  0,  0,  1, 17,  0,  0, 32, 32,
        10,  7,  4,  1,  1,  0,  0, -1, -1, -1 };

// ovr014.h
Array<Affects> ovr014::unk_18ADB = { Affects::bless, Affects::snake_charm, Affects::paralyze, Affects::sleep, Affects::helpless };

// ovr014.h
Set ovr014::asc_41342 = New<Set_>(Array<int>{0, 69, 84});

// ovr014.h
Set ovr014::unk_41AE5 = New<Set_>(Array<int>{0, 69});

// ovr014.h
Set ovr014::unk_41B05 = New<Set_>(Array<int>{71, 72, 73, 75, 77, 79, 80, 81});

// ovr016.h
Dictionary<Affects, String> ovr016::EffectNameMap ;

// ovr016.h
Array<String> ovr016::reorderStrings = { "Select Exit", "Place Exit" };

// ovr016.h
Set ovr016::reorderSet = New<Set_>(Array<int>{13,80,83});

// ovr016.h
Set ovr016::AlterSet = New<Set_>(Array<int>{0, 69});

// ovr016.h
Set ovr016::unk_463F4 = New<Set_>(Array<int>{0, 69});

// ovr017.h
Array<int> ovr017::PlayerNameOffset = { 0, 0, 4 };

// ovr017.h
Array<int> ovr017::NpcFileOffset = { 0xf7, 0x84, 0x13 };

// ovr017.h
Set ovr017::unk_47635 = New<Set_>(Array<int>{0, 5});

// ovr017.h
Set ovr017::asc_49280 = New<Set_>(Array<int>{18, 26, 47, 48, 97, 107, 124});

// ovr017.h
Array<ClassId> ovr017::HillsFarClassMap = {
    ClassId::unknown,    ClassId::thief,      ClassId::fighter,    ClassId::mc_f_t, ClassId::magic_user,
    ClassId::mc_mu_t,    ClassId::mc_f_mu,    ClassId::mc_f_mu_t,  ClassId::cleric, ClassId::mc_c_t,
    ClassId::mc_c_f,     ClassId::unknown,    ClassId::mc_c_mu,    ClassId::unknown, ClassId::mc_c_f_m,
    ClassId::unknown};

// ovr017.h
Set ovr017::save_game_keys = New<Set_>(Array<int>{65, 66, 67, 68, 69, 70, 71, 72, 73, 74});

// ovr017.h
Set ovr017::unk_4AEA0 = New<Set_>(Array<int>{0, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74});

// ovr017.h
Set ovr017::unk_4AEEF = New<Set_>(Array<int>{0, 2, 18});

// ovr018.h
Set ovr018::unk_4C13D = New<Set_>(Array<int>{71, 79});

// ovr018.h
Set ovr018::unk_4C15D = New<Set_>(Array<int>{69, 83});

// ovr018.h
Array<String> ovr018::menuStrings = {
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

// ovr018.h
Array<bool> ovr018::menuFlags = {
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

// ovr018.h
Array<byte> ovr018::unk_1A1B2 = { 0x02, 0x10, 0x08, 0x40, 0x40, 0x01, 0x04, 0x20 };

// ovr018.h
Array2<sbyte> ovr018::thac0_table = {
            {40, 40, 40, 40, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C, 0x2C, 0x2E, 0x2E, 0x2E},
            {40, 40, 40, 40, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C, 0x2C, 0x2E, 0x2E, 0x2E},
            {0x27, 40, 40, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33},
            {40, 40, 40, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33},
            {40, 40, 40, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33},
            {0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x29, 0x29, 0x29, 0x29, 0x29, 0x2B, 0x2B},
            {40, 40, 40, 40, 40, 0x29, 0x29, 0x29, 0x29, 0x2C, 0x2C, 0x2C, 0x2C},
            {40, 40, 40, 40, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C, 0x2C, 0x2E, 0x2E, 0x2E} };

// ovr018.h
Set ovr018::unk_4FE94 = New<Set_>(Array<int>{0, 69});

// ovr018.h
Array<sbyte> ovr018::con_hp_adj = { 0, 0, 0, -2, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

// ovr018.h
Array<ovr018::hp_calc> ovr018::hp_calc_table = {
            New<hp_calc_>(8, 0, 0x48, 2), 
            New<hp_calc_>(8, 0, 0x70, 0), 
            New<hp_calc_>(10, 0, 0x5A, 3), 
            New<hp_calc_>(10, 0, 0x5A, 3), 
            New<hp_calc_>(8, 1, 0x58, 2), 
            New<hp_calc_>(4, 0, 0x2c, 1), 
            New<hp_calc_>(6, 0, 0x3c, 2), 
            New<hp_calc_>(4, 1, 0x48, 0), 
        };

// ovr018.h
Array<byte> ovr018::unk_16B2A = { 1, 1, 1, 1, 2, 1, 1, 2 };

// ovr018.h
Array<byte> ovr018::unk_16B32 = { 8, 8, 0xA, 0xA, 8, 4, 6, 4 };

// ovr018.h
Array<byte> ovr018::classMasks = { 2, 2, 8, 0x10, 0x20, 1, 4, 4 };

// ovr018.h
Array2<int> ovr018::exp_table = { /* seg600:4293 unk_1A5A3 */
            /* Cleric */    { 0, 1501, 3001,  6001, 13001, 27501, 55001, 110001, 225001, 450001, -1, -1, -1 },
            /* Druid */     { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
            /* Fighter */   { 0, 2001, 4001,  8001, 18001, 35001, 70001, 125001, 250001, 500001,  750001, 1000001, -1  },
            /* Paladin */   { 0, 2751, 5501, 12001, 24001, 45001, 95001, 175001, 350001, 700001, 1050001, -1, -1 },
            /* Ranger */    { 0, 2251, 4501, 10001, 20001, 40001, 90001, 150001, 225001, 325001,  650001, -1, -1 },
            /* MU */        { 0, 2501, 5001, 10001, 22501, 40001, 60001,  90001, 135001, 250001,  375001, -1, -1 },
            /* Thief */     { 0, 1251, 2501,  5001, 10001, 20001, 42501,  70001, 110001, 160001,  220001, 440001, -1},
            /* Monk */      { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }};

// ovr019.h
Array<ovr019::Struct_1ADFB> ovr019::unk_1ADFB = Array<ovr019::Struct_1ADFB>(3);

// ovr019.h
String ovr019::aTyranthraxusSp = "Tyranthraxus' spirit coalesces over the slain ";

// ovr019.h
String ovr019::aStormGiant_You = "storm giant. 'You have defeated me. Were it not for ";

// ovr019.h
String ovr019::aTheAmuletOfLyt = "the Amulet of Lythander, I could possess you and rob ";

// ovr019.h
String ovr019::aYouOfYourVicto = "you of your victory. Still I can escape through the pool.";

// ovr019.h
String ovr019::aAsYouReachForT = "As you reach for the Pool of Radiance, he cries ";

// ovr019.h
String ovr019::aOutKeepTheGaun = "out, 'Keep the Gauntlet of Moander away from there, you ";

// ovr019.h
String ovr019::aWillUnleashDan = "will unleash dangerous energies. Stay back!' As the ";

// ovr019.h
String ovr019::aGauntletContac = "gauntlet contacts the pool, it contracts and shatters it.";

// ovr019.h
String ovr019::aIAmTrappedWith = "'I am trapped without escape, you have succeeded ";

// ovr019.h
String ovr019::aWhereArmiesHav = "where armies have not. Gloat while you may, Tyranthraxus ";

// ovr019.h
String ovr019::aIsSlainThisDay = "is slain this day.' Before your eyes he crumbles into ";

// ovr019.h
String ovr019::aNothingness_ = "nothingness.";

// ovr019.h
String ovr019::aYouAreCertainH = "You are certain he is destroyed because your ";

// ovr019.h
String ovr019::aFinalBondFades = "final bond fades away. The Curse of the Azure Bonds ";

// ovr019.h
String ovr019::aHasFinallyBeen = "has finally been lifted from you! You are free at ";

// ovr019.h
String ovr019::aLast = "last!";

// ovr019.h
String ovr019::aTheKnightsOfMy = "The Knights of Myth Drannor rush in, '";

// ovr019.h
String ovr019::aCongratulati_0 = "Congratulations, you have destroyed the Flamed One. ";

// ovr019.h
String ovr019::aWithThePowerOf = "With the power of Elminster, let us take you from ";

// ovr019.h
String ovr019::aThisFoulPlaceT = "this  foul place, to a fine feast.'";

// ovr019.h
String ovr019::aYouAreTeleport = "You are teleported to Shadowdale, where festivities ";

// ovr019.h
String ovr019::aHaveAlreadyBeg = "have already begun. A huge cheer goes up at your arrival. ";

// ovr019.h
String ovr019::aGharriAndNacac = "Gharri and Nacacia, arm in arm, yell congratulations ";

// ovr019.h
String ovr019::aFromTheNearbyS = "from the nearby stands. 'You have won!'";

// ovr020.h
Array<String> ovr020::sexString = { "Male", "Female" };

// ovr020.h
Array<String> ovr020::raceString = { "Monster", "Dwarf", "Elf", "Gnome",
                                         "Half-Elf", "Halfling", "Half-Orc", "Human" };

// ovr020.h
Array<String> ovr020::alignmentString = { "Lawful Good", "Lawful Neutral", "Lawful Evil",
                                              "Neutral Good", "True Neutral", "Neutral Evil",
                                              "Chaotic Good", "Chaotic Neutral", "Chaotic Evil" };

// ovr020.h
Array<String> ovr020::classString = { "Cleric", "Druid", "Fighter", "Paladin", "Ranger",
                                          "Magic-User", "Thief", "Monk", "Cleric/Fighter",
                                          "Cleric/Fighter/Magic-User", "Cleric/Ranger",
                                          "Cleric/Magic-User","Cleric/Thief", "Fighter/Magic-User",
                                          "Fighter/Thief", "Fighter/Magic-User/Thief",
                                          "Magic-User/Thief" };

// ovr020.h
Array<String> ovr020::statShortString = { "STR ", "INT ", "WIS ", "DEX ", "CON ", "CHA " };

// ovr020.h
Array<String> ovr020::statusString = { "Okay", "Animated", "tempgone", "Running",
                                         "Unconscious", "Dying", "Dead", "Stoned",
                                         "Gone" };

// ovr020.h
Array<String> ovr020::moneyString = { "Copper", "Silver", "Electrum", "Gold", "Platinum",
                                        "Gems", "Jewelry" };

// ovr020.h
Set ovr020::asc_54B50 = New<Set_>(Array<int>{73, 83, 84 });

// ovr020.h
Set ovr020::unk_54B03 = New<Set_>(Array<int>{0, 69 });

// ovr020.h
Set ovr020::unk_554EE = New<Set_>(Array<int>{0, 69});

// ovr020.h
const Array2<byte> ovr020::MU_spell_lvl_learn = {
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

// ovr020.h
Array<Affects> ovr020::paladinCureableDiseases = { 
            Affects::helpless,
            Affects::cause_disease_1,
            Affects::weaken,
            Affects::cause_disease_2,
            Affects::hot_fire_shield,
            Affects::affect_39 };

// ovr021.h
Array<int> ovr021::timeScales = { 10, 10, 6, 24, 30, 12, 0x100 };

// ovr021.h
Set ovr021::unk_58731 = New<Set_>(Array<int>{0, 69, 82 });

// ovr021.h
Array<int> ovr021::spellLaernTimeout = Array<int>(9);

// ovr022.h
Array2<short> ovr022::preconfiiguredItems = {
        {0x00B9, 0x00BB, 0x0040, 0x0001, 0x0320, 0x0003, 0x0063, 0x0000},
        {0x00EF, 0x00A7, 0x0040, 0x0001, 0x044C, 0x0001, 0x003b, 0x0000},
        {0x00b9, 0x00a7, 0x0040, 0x0001, 0x0190, 0x0001, 0x0003, 0x0000},
        {0x00AD, 0x00a7, 0x0040, 0x0001, 0x01C2, 0x0001, 0x0030, 0x0000},
        {0x00CE, 0x00A7, 0x0045, 0x0001, 0x2AF8, 0x001E, 0x000F, 0x0000},
        {0x00E2, 0x00A7, 0x0064, 0x000A, 0x3A98, 0x0000, 0x0026, 0x0083},
        {0x009d, 0x00a7, 0x0015, 0x0014, 0x0bb8, 0x0001, 0x0033, 0x0000} };

// ovr023.h
Array<String> ovr023::SpellNames = { /* AffectNames */
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

// ovr023.h
Array<String> ovr023::LevelStrings = {
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

// ovr023.h
Set ovr023::unk_5C1A2 = New<Set_>(Array<int>{1,2,3,4});

// ovr023.h
Set ovr023::asc_5C1D1 = New<Set_>(Array<int>{0, 67, 69, 76, 77, 83});

// ovr023.h
Set ovr023::unk_5C1F1 = New<Set_>(Array<int>{0, 69});

// ovr023.h
Array<Point> ovr023::unk_16D22 = { Point(-1, 0), Point(0, -1), Point(0, -1), Point(1, 0), Point(1, 0), Point(0, 1), Point(0, 1), Point(-1, 0) };

// ovr023.h
Array<Point> ovr023::unk_16D32 = { Point(1, 0), Point(1, 0), Point(0, 1), Point(0, 1), Point(-1, 0), Point(-1, 0), Point(0, -1), Point(0, -1) };

// ovr024.h
Array<Affects> ovr024::unk_6325A = { Affects::silence_15_radius, Affects::prot_from_evil_10_radius, Affects::prot_from_good_10_radius, Affects::prayer };

// ovr025.h
Set ovr025::unk_68DFA = New<Set_>(Array<int>{13, 27, 69, 83});

// ovr026.h
Array2<byte> ovr026::ClericSpellLevels = { 
            {1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {1, 1, 0, 0, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 1, 1, 0},
            {1, 1, 0, 0, 1},  
            {0, 0, 0, 1, 1},  
            {1, 0, 1, 0, 0},  
            {1, 1, 1, 0, 0},  
        };

// ovr026.h
Array2<byte> ovr026::PaladinSpellLevels = { 
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
        };

// ovr026.h
Array2<byte> ovr026::unk_1A758 = {
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
        };

// ovr026.h
Array3<byte> ovr026::SaveThrowValues = { 
            {{20, 20, 20, 20, 20}, {10, 13, 14, 16, 15}, {10, 13, 14, 16, 15}, {10, 13, 14, 16, 15}, {9, 12, 13, 15, 14}, {9, 12, 13, 15, 14}, {9, 12, 13, 15, 14}, {7, 10, 11, 13, 12}, {7, 10, 11, 13, 12}, {7, 10, 11, 13, 12}, {6, 9, 10, 12, 11}, {6, 9, 10, 12, 11}, {6, 9, 10, 12, 11}},
            {{20, 20, 20, 20, 20}, {10, 13, 14, 16, 15}, {10, 13, 14, 16, 15}, {10, 13, 14, 16, 15}, {9, 12, 13, 15, 14}, {9, 12, 13, 15, 14}, {9, 12, 13, 15, 14}, {7, 10, 11, 13, 12}, {7, 10, 11, 13, 12}, {7, 10, 11, 13, 12}, {6, 9, 10, 12, 11}, {6, 9, 10, 12, 11}, {6, 9, 10, 12, 11}},
            {{20, 20, 20, 20, 20}, {14, 15, 16, 17, 17}, {14, 15, 16, 17, 17}, {13, 14, 15, 16, 16}, {13, 14, 15, 16, 16}, {11, 12, 13, 13, 14}, {11, 12, 13,13, 14}, {10, 11, 12, 12, 13}, {10, 11, 12, 12, 13}, {8, 9, 10, 9, 11}, {8, 9, 10, 9, 11}, {7, 8, 9, 8, 10}, {7, 8, 9, 8, 10}},
            {{20, 20, 20, 20, 20}, {12, 13, 14, 15, 15}, {12, 13, 14, 15, 15}, {11, 12, 13, 14, 14}, {11, 12, 13, 14, 14}, {9, 9, 11, 11, 12}, {9, 9, 11, 11, 12}, {8, 9, 10, 10, 11}, {9, 9, 10, 10, 11}, {6, 7, 8, 7, 9}, {6, 7, 8, 7, 9}, {5, 6, 7, 6, 8}, {5, 6, 7, 6, 8}},
            {{20, 20, 20, 20, 20}, {14, 15, 16, 17, 17}, {14, 15, 16, 17, 17}, {13, 14, 15, 16, 16}, {13, 14, 15, 16, 16}, {11, 12, 13, 13, 14}, {11, 12, 13, 13, 14}, {10, 11, 12, 12, 13}, {10, 11, 12, 12, 13}, {8, 9, 10, 9, 11}, {8, 9, 10, 9, 11}, {7, 8, 9, 8, 10}, {7, 8, 9, 8, 10}},
            {{20, 20, 20, 20, 20}, {14, 13, 11, 15, 12}, {14, 13, 11, 15, 12}, {14, 13, 11, 15, 12}, {14, 13, 11, 15, 12}, {14, 13, 11, 15, 12}, {13, 11, 9, 13, 10}, {13, 11, 9, 13, 10}, {13, 11, 9, 13, 10}, {13, 11, 9, 13, 10}, {13, 11, 9, 13, 10}, {11, 9, 7, 11, 8}, {11, 9, 7, 11, 8}},
            {{20, 20, 20, 20, 20}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {11, 10, 10, 14, 11}, {11, 10, 10, 14, 11}, {11, 10, 10, 14, 11}, {11, 10, 10, 14, 11}},
            {{20, 20, 20, 20, 20}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {13, 12, 14, 16, 15}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {12, 11, 12, 15, 13}, {11, 10, 10, 14, 11}, {13, 11, 9, 13, 10}, {11, 9, 7, 11, 8}, {11, 9, 7, 11, 8}}};

// ovr026.h
Array2<sbyte> ovr026::unk_1A230 = {
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
            { 0, 0, 0, 0, 0, -5, 0, 0, 0 } };

// ovr026.h
Array2<sbyte> ovr026::unk_1A243 = {
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
            { 99, 0, 3, 18, 3, 18 } };

// ovr026.h
Array2<byte> ovr026::base_chance = {
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
            { 0, 0x64, 0x4D, 0x4B, 0x5E, 0x4D, 0x23, 0x63, 0x41 } };

// ovr027.h
Set ovr027::highlightable_text = New<Set_>(Array<int>{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'});

// ovr027.h
Set ovr027::alpha_number_input = New<Set_>(Array<int>{' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'});

// ovr027.h
Set ovr027::number_input = New<Set_>(Array<int>{'1', '2', '3', '4', '5', '6', '7', '8', '9', '\\'});

// ovr027.h
Array<char> ovr027::keypad_ctrl_codes = { 'O', 'P', 'Q', 'K', ' ', 'M', 'G', 'H', 'I' };

// ovr028.h
Array<int> MapCursor::city_map_x = { /* unk_16D5A */
            0x04,0x0C,0x15,0x0B,0x1D,0x14,0x26,0x15,
            0x1E,0x1F,0x19,0x25,0x1C,0x1D,0x03,0x0C,
            0x19,0x1D,0x1D,0x21,0x13,0x10,0x09,0x10,
            0x14,0x15,0x19,0x19,0x1A,0x1F,0x25,0x22, 0x0F };

// ovr028.h
Array<int> MapCursor::city_map_y = { /* unk_16D7A */
            0x0F,0x08,0x0B,0x04,0x0A,0x04,0x01,0x02,
            0x0D,0x0F,0x03,0x05,0x02,0x08,0x0C,0x0D,
            0x0A,0x0C,0x09,0x09,0x08,0x06,0x06,0x03,
            0x02,0x02,0x03,0x02,0x03,0x04,0x02,0x01, 0x00 };

// ovr029.h
Array<int> ovr029::sky_colours = { /* seg600:0A8A unk_16D9A*/
        0x00, 0x0F, 0x04, 0x0B, 0x0D, 0x02, 0x09, 0x0E, 0x00, 0x0F, 0x04, 0x0B, 0x0D, 0x02 , 0x09, 0x0E};

// ovr030.h
Array<byte> ovr030::fadeOldColors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

// ovr030.h
Array<byte> ovr030::fadeNewColors = { 12, 12, 12, 12, 4, 5, 6, 7, 12, 12, 10, 12, 12, 12, 14, 12 };

// ovr030.h
Array<byte> ovr030::transparentOldColors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

// ovr030.h
Array<byte> ovr030::transparentNewColors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 14, 15 };

// ovr031.h
Array<byte> ovr031::idxOffset = { 0, 2, 6, 10, 22, 38, 54, 110, 132, 154, 1 };

// ovr031.h
Array<int> ovr031::colCount = { 1, 1, 1, 3, 2, 2, 7, 2, 2, 1 };

// ovr031.h
Array<int> ovr031::rowCount = { 2, 4, 4, 4, 8, 8, 8, 11, 11, 2 };

// ovr032.h
ovr032::ReachCache ovr032::mapReachCache ;

// ovr033.h
Array<Array<Point>> ovr033::Steps = {
            Array<Point>(0),
            Array<Point>{ Point( 0, 0 ) },
            Array<Point>{ Point( 0, 0 ), Point(  0,  1 ) },
            Array<Point>{ Point( 0, 0 ), Point(  1,  0 ) },
            Array<Point>{ Point( 0, 0 ), Point(  1,  0 ), Point( 0,  1 ), Point(  1,  1 ) }
        };

// ovr033.h
Array2<int> ovr033::mapToPlayerIndex = Array2<int>(Point::MapMaxY, Point::MapMaxX);

// ovr034.h
Array<byte> ovr034::unk_16E30 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

// ovr034.h
Array<byte> ovr034::unk_16E40 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

// ovr034.h
Array<byte> ovr034::unk_16E50 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 14, 15 };

// seg001.h
seg001::VoidDelegate seg001::EngineStoppedCallback ;

// seg037.h
Array<int> seg037::outer_frame_bottom = { 1, 8, 6, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 6, 8, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3 };

// seg037.h
Array<byte> seg037::unk_16ED6 = { 4, 3, 0, 6, 1, 1, 1, 1, 8, 1, 1, 4, 1, 1, 2, 1, 4 };

// seg037.h
Array<byte> seg037::unk_16F0A = { 0, 7, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 2, 9, 4 };

// seg037.h
Array<int> seg037::outer_frame_top = { 0, 6, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 4, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 6, 1, 2 };

// seg037.h
Array<byte> seg037::x8x8_07 = { 0, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 8, 4,
                                    1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 6, 1,
                                    1, 1, 1, 1, 8, 2 };

// seg037.h
Array<byte> seg037::unk_16EE3 = { 1, 2, 1, 4, 1, 1, 1, 1, 1, 1, 8, 4, 1, 1, 3 };

// seg037.h
Array<int> seg037::outer_frame_left = { 0, 2, 9, 5, 2, 2, 2, 2, 2, 2, 5, 7, 2, 2, 2, 2, 2, 9, 7, 2, 2, 2, 7, 1 };

// seg037.h
Array<int> seg037::outer_frame_right = { 2, 2, 9, 7, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 5, 2 };

// seg037.h
Array<byte> seg037::unk_16F31 = { 5, 2, 0, 2, 7, 2, 2, 2, 2, 5, 2, 2, 2, 2, 1 };

// seg037.h
Array<byte> seg037::unk_16F3E = { 2, 1, 2, 5, 9, 2, 2, 2, 7, 5, 2, 2, 2, 2, 3 };

// seg037.h
Array<byte> seg037::unk_16F4D = { 0, 2, 9, 5, 2, 2, 2, 2, 2, 2, 5, 7, 2, 2, 2, 2, 2, 9, 7, 2, 2, 2, 1 };

// seg037.h
Array<byte> seg037::unk_16F64 = { 0, 7, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 5, 2, 2, 2, 2, 2, 2, 5, 2, 4 };

// seg037.h
Array<byte> seg037::unk_16F7B = { 2, 2, 9, 7, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2 };

// seg041.h
Array2<int> seg041::bounds = {
            { 0x16, 0x26, 0x11, 1 },
            { 0x16, 0x26, 0x15, 1 },
            { 0x15, 0x26, 1, 0x17 } 
        };

// seg041.h
Set seg041::puncutation = New<Set_>(Array<int>{33, 44, 45, 46, 58, 59, 63});

// seg042.h
Array<char> seg042::unk_16FA9 = { ' ', '.', '*', ',', '?', '/', '\\', ':', ';', '|' };

// seg044.h
Array<bool> seg044::soundLoaded ;

// seg049.h
Queue<ushort> seg049::keysPressed ;

// seg049.h
cs::Mutex seg049::keysPressedLock ;

// seg049.h
cs::Mutex seg049::WaitForKeyLock ;

// seg049.h
cs::CondVar seg049::WaitForKey ;

// seg049.h
volatile int seg049::suspendSave = 0;

// seg051.h
cs::Random seg051::random_number {0};


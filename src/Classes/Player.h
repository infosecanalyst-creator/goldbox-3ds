// Classes/Player.h - port of Classes/Player.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/DataIO.h"
#include "Classes/GblTypes.h"
#include "Classes/Enums.h"
#include "Classes/Affect.h"
#include "Classes/ItemData.h"
#include "Classes/Spells.h"
#include "Classes/Limits.h"

#include <vector>

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

struct StatValue : public IDataIO {
private:
    Array3<int> raceSexMinMax;
    Array<int> classMin;
    Array<int> ageEffects;

public:
    StatValue() = default;  // C# struct default ctor: everything zero / null
    StatValue(Array3<int> _raceSexMinMax, Array<int> _classMin, Array<int> _ageEffects);

    int cur = 0;
    int full = 0;

    void Load(int val);

    void Assign(StatValue sv);

    void Inc();

    void Dec();

    void EnforceRaceSexLimits(int race, int sex);

    void EnforceClassLimits(int _class);

    void AgeEffects(int race, int age);

    void Write(Array<byte> data, int offset) override;

    void Read(Array<byte> data, int offset) override;

    String ToString() const;
    // PORT-NOTE: implicit conversion so cs::Format("{0}", sv) works (see Point in GblTypes.h).
    operator String() const { return ToString(); }
};
// cs::ToString(statValue) / Format("{0}", statValue) support for the value struct.
inline String ToString(const StatValue& sv) { return sv.ToString(); }

class PlayerStats_ : public IDataIO {
public:
    StatValue Str = StatValue(Limits::StrRaceSexMinMax, Limits::StrClassMin, Limits::StrAgeEffect);
    StatValue Str00 = StatValue(Limits::Str00RaceSexMinMax, Limits::Str00ClassMin, Limits::Str00AgeEffect);
    StatValue Con = StatValue(Limits::ConRaceSexMinMax, Limits::ConClassMin, Limits::ConAgeEffect);
    StatValue Dex = StatValue(Limits::DexRaceSexMinMax, Limits::DexClassMin, Limits::DexAgeEffect);
    StatValue Int = StatValue(Limits::IntRaceSexMinMax, Limits::IntClassMin, Limits::IntAgeEffect);
    StatValue Wis = StatValue(Limits::WisRaceSexMinMax, Limits::WisClassMin, Limits::WisAgeEffect);
    StatValue Cha = StatValue(Limits::ChaRaceSexMinMax, Limits::ChaClassMin, Limits::ChaAgeEffect);

    // explicit interface implementation IDataIO.Write / IDataIO.Read
    void Write(Array<byte> data, int offset) override;
    void Read(Array<byte> data, int offset) override;

    void Assign(PlayerStats ps);

    void Dec(int idx);

    void Inc(int idx);

    // C# get-only indexer `StatValue this[int idx]` returns a copy of the struct.
    StatValue operator[](int idx) const;

    String ToString() const;
};

struct ClassLevels {
};

class ActiveItems_ {
    static constexpr int ItemSlots = 13;
    Array<Item> itemArray = Array<Item>(ItemSlots); // 0x151[]

public:
    byte PrimaryWeaponHandCount();

    byte SecondaryWeaponHandCount();

    // C# get/set properties -> Item& accessors (read and write through them).
    Item& primaryWeapon() { return itemArray[0]; }  // field_151 // 0x151
    Item& secondaryWeapon() { return itemArray[1]; } // field_155 // 0x155
    Item& armor() { return itemArray[2]; }           // field_159 // 0x159
    Item& field_15D() { return itemArray[3]; }       // 0x15D
    Item& field_161() { return itemArray[4]; }       // 0x161
    Item& field_165() { return itemArray[5]; }       // 0x165
    Item& field_169() { return itemArray[6]; }       // 0x169
    Item& field_16D() { return itemArray[7]; }       // 0x16D
    Item& field_171() { return itemArray[8]; }       // 0x171
    Item& Item_ptr_01() { return itemArray[9]; }     // 0x175
    Item& Item_ptr_02() { return itemArray[10]; }    // 0x179
    Item& arrows() { return itemArray[11]; }         // 0x17d
    Item& quarrels() { return itemArray[12]; }       // 0x181

    Item& operator[](ItemSlot slot) { return itemArray[(int)slot]; }

    void Reset();

    void UndreadyAll(int classFlags);
};

struct Control {
    static constexpr byte PC_Base = 0;
    static constexpr byte PC_Mask = 0x7F;
    static constexpr byte NPC_Base = 0x80;
    static constexpr byte NPC_Berzerk = 0xB2;
    static constexpr byte PC_Berzerk = 0xB3;
};

/// <summary>
/// Summary description for Player.
/// </summary>
class Player_ {
public:
    String name; // [DataOffset(0x00, DataType.PString, 15)]

    //[DataOffset(0x10, DataType.Cust1Array, 6)]
    //public StatValue[] stats;

    PlayerStats stats2; // [DataOffset(0x10, DataType.CustSaveLoad, 0x12)]

    SpellList spellList; // ox1e was spell_list

    byte spell_to_learn_count = 0; // [DataOffset(0x72, DataType.Byte)] // 0x72;
    sbyte thac0 = 0; // [DataOffset(0x73, DataType.SByte)] // 0x73; field_73

    Race race = (Race)0; // [DataOffset(0x74, DataType.IByte)] // 0x74;

    ClassId _class = (ClassId)0; // [DataOffset(0x75, DataType.IByte)] // 0x75;
    short age = 0; // [DataOffset(0x76, DataType.SWord)] // 0x76;

    byte hit_point_max = 0; // [DataOffset(0x78, DataType.Byte)] // 0x78;

    Array<byte> spellBook = Array<byte>(100); // [DataOffset(0x79, DataType.ByteArray, 100)] //78[di]; // 1- 100 or 0x79 - 0xDC
    bool KnowsSpell(Spells spell) { return spellBook[(int)spell - 1] != 0; }
    void LearnSpell(Spells spell) { spellBook[(int)spell - 1] = 1; }

    byte attackLevel = 0; // [DataOffset(0xdd, DataType.Byte)] // 0xdd; field_DD
    byte field_DE = 0; // [DataOffset(0xde, DataType.Byte)] // 0xde;
    Array<byte> saveVerse = Array<byte>(5); // [DataOffset(0xdf, DataType.ByteArray, 5)] // 0xdf; field_DF

    byte base_movement = 0; // [DataOffset(0xe4, DataType.Byte)] // 0xe4;
    byte HitDice = 0; // [DataOffset(0xe5, DataType.Byte)] // 0xe5; HitDice?
    byte multiclassLevel = 0; // [DataOffset(0xe6, DataType.Byte)] // 0xe6;
    byte lost_lvls = 0; // [DataOffset(0xe7, DataType.Byte)] // 0xe7;
    byte lost_hp = 0; // [DataOffset(0xe8, DataType.Byte)] // 0xe8;
    byte field_E9 = 0; // [DataOffset(0xe9, DataType.Byte)] // 0xe9;
    Array<byte> thief_skills = Array<byte>(8); // [DataOffset(0xeA, DataType.ByteArray, 8)] // 0xeA; [] was 1 offset @ 0xe9, pick_pockets, open_locks, find_remove_traps, move_silently, hide_in_shadows, hear_noise, climb_walls, read_languages
    List<Affect> affects = nullptr; // f2 - affect_ptr

    byte field_F6 = 0; // [DataOffset(0xf6, DataType.Byte)] // 0xf6; field_F6
    byte control_morale = 0; // [DataOffset(0xf7, DataType.Byte)] // 0xf7; field_F7
    byte npcTreasureShareCount = 0; // [DataOffset(0xf8, DataType.Byte)] // 0xf8;
    byte field_F9 = 0; // [DataOffset(0xf9, DataType.Byte)] // 0xf9
    byte field_FA = 0; // [DataOffset(0xfa, DataType.Byte)] // 0xfa;

    MoneySet Money; // [DataOffset(0xfb, DataType.ShortArray, 7)]

    Array<byte> ClassLevel = Array<byte>(8); // [DataOffset(0x109, DataType.ByteArray, 8)] /* Skill_A_lvl */

    // C# get/set properties -> byte& accessors (read and write through them).
    byte& cleric_lvl() { return ClassLevel[0]; }     // 0x109;
    byte& druid_lvl() { return ClassLevel[1]; }      // 0x10a;
    byte& fighter_lvl() { return ClassLevel[2]; }    // 0x10b;
    byte& paladin_lvl() { return ClassLevel[3]; }    // 0x10c;
    byte& ranger_lvl() { return ClassLevel[4]; }     // 0x10d;
    byte& magic_user_lvl() { return ClassLevel[5]; } // 0x10e;
    byte& thief_lvl() { return ClassLevel[6]; }      // 0x10f;
    byte& monk_lvl() { return ClassLevel[7]; }       // 0x110;

    Array<byte> ClassLevelsOld = Array<byte>(8); // [DataOffset(0x111, DataType.ByteArray, 8)]

    byte& cleric_old_lvl() { return ClassLevelsOld[0]; }     // 0x111;
    //public byte druid_old_lvl // 0x112;
    byte& fighter_old_lvl() { return ClassLevelsOld[2]; }    // 0x113;
    byte& paladin_old_lvl() { return ClassLevelsOld[3]; }    // 0x114;
    byte& ranger_old_lvl() { return ClassLevelsOld[4]; }     // 0x115;
    byte& magic_user_old_lvl() { return ClassLevelsOld[5]; } // 0x116;
    byte& thief_old_lvl() { return ClassLevelsOld[6]; }      // 0x117;
    //public byte monk_old_level // 0x118;

    int SkillLevel(SkillType skill);

    byte sex = 0; // [DataOffset(0x119, DataType.Byte)] // 0x119;
    MonsterType monsterType = (MonsterType)0; // [DataOffset(0x11a, DataType.IByte)] // 0x11a;
    byte alignment = 0; // [DataOffset(0x11b, DataType.Byte)] // 0x11b;
    /// <summary>
    /// half-attacks count
    /// </summary>
    byte attacksCount = 0; // [DataOffset(0x11c, DataType.Byte)] // 0x11c;
    byte baseHalfMoves = 0; // [DataOffset(0x11d, DataType.Byte)] // 0x11d;
    byte attack1_DiceCountBase = 0; // [DataOffset(0x11e, DataType.Byte)] // 0x11e;
    byte attack2_DiceCountBase = 0; // [DataOffset(0x11f, DataType.Byte)] // 0x11f;
    byte attack1_DiceSizeBase = 0; // [DataOffset(0x120, DataType.Byte)] // 0x120;
    byte attack2_DiceSizeBase = 0; // [DataOffset(0x121, DataType.Byte)] // 0x121;
    byte attack1_DamageBonusBase = 0; // [DataOffset(0x122, DataType.Byte)] // 0x122;
    byte attack2_DamageBonusBase = 0; // [DataOffset(0x123, DataType.Byte)] // 0x123;
    byte base_ac = 0; // [DataOffset(0x124, DataType.Byte)] // 0x124;
    byte field_125 = 0; // [DataOffset(0x125, DataType.Byte)] // 0x125;
    byte mod_id = 0; // [DataOffset(0x126, DataType.Byte)] // 0x126; field_126
    int exp = 0; // [DataOffset(0x127, DataType.Int)] // 0x127
    byte classFlags = 0; // [DataOffset(0x12b, DataType.Byte)] // 0x12b;
    byte hit_point_rolled = 0; // [DataOffset(0x12c, DataType.Byte)] // 0x12c;

    //[DataOffset(0x12d, DataType.ByteArray,15)]
    Array2<byte> spellCastCount = Array2<byte>(3, 5); // 0x12d - field_12D

    short field_13C = 0; // [DataOffset(0x13c, DataType.SWord)] // 0x13c
    byte field_13E = 0; // [DataOffset(0x13e, DataType.Byte)] // 0x13e;
    byte field_13F = 0; // [DataOffset(0x13f, DataType.Byte)] // 0x13f;
    byte field_140 = 0; // [DataOffset(0x140, DataType.Byte)] // 0x140;
    byte head_icon = 0; // [DataOffset(0x141, DataType.Byte)] // 0x141;
    byte weapon_icon = 0; // [DataOffset(0x142, DataType.Byte)] // 0x142;
    byte icon_id = 0; // [DataOffset(0x143, DataType.Byte)] // 0x143;
    byte icon_size = 0; // [DataOffset(0x144, DataType.Byte)] // 0x144; field_144  1 small 2 normal
    Array<byte> icon_colours = Array<byte>(6); // [DataOffset(0x145, DataType.ByteArray, 6)] // 0x145 = field_144[1] // byte[6]
    byte field_14B = 0; // [DataOffset(0x14b, DataType.Byte)] // 0x14b;

    //[DataOffset(0x14c, DataType.Byte)]
    //public byte field_14C; // 0x14c; // items.Count
    static constexpr int MaxItems = 16;

    List<Item> items = nullptr; // 0x14d
    //public Item itemsPtr; // 0x14d

    ActiveItems activeItems = New<ActiveItems_>();

    byte weaponsHandsUsed = 0; // [DataOffset(0x185, DataType.Byte)] // 0x185;
    sbyte field_186 = 0; // [DataOffset(0x186, DataType.SByte)] // 0x186;
    short weight = 0; // [DataOffset(0x187, DataType.SWord)] // 0x187

    Action actions; // 0x18d
    byte paladinCuresLeft = 0; // [DataOffset(0x191, DataType.Byte)] // 0x191 field_191
    byte field_192 = 0; // [DataOffset(0x192, DataType.Byte)] // 0x192
    byte field_193 = 0; // [DataOffset(0x193, DataType.Byte)] // 0x193
    byte field_194 = 0; // [DataOffset(0x194, DataType.Byte)] // 0x194
    Status health_status = (Status)0; // [DataOffset(0x195, DataType.IByte)] // 0x195
    bool in_combat = false; // [DataOffset(0x196, DataType.Bool)] // 0x196
    CombatTeam combat_team = (CombatTeam)0; // [DataOffset(0x197, DataType.IByte)] // 0x197 0 - our team, 1 - enemy
    QuickFight quick_fight = (QuickFight)0; // [DataOffset(0x198, DataType.IByte)] // 0x198
    int hitBonus = 0; // [DataOffset(0x199, DataType.IByte)] // 0x199 field_199
    byte ac = 0; // [DataOffset(0x19a, DataType.Byte)] // 0x19a
    int DisplayAc() const { return 0x3C - ac; }

    byte ac_behind = 0; // [DataOffset(0x19b, DataType.Byte)] // 0x19b field_19B

    byte AttacksLeft(int index);

    void AttacksLeftSet(int index, byte value);

    void AttacksLeftDec(int index);

    byte attack1_AttacksLeft = 0; // [DataOffset(0x19c, DataType.Byte)] // 0x19c - field_19C
    byte attack2_AttacksLeft = 0; // [DataOffset(0x19d, DataType.Byte)] // 0x19d - field_19D

    byte attackDiceCount(int index);
    byte attack1_DiceCount = 0; // [DataOffset(0x19E, DataType.Byte)] // 0x19e field_19E
    byte attack2_DiceCount = 0; // [DataOffset(0x19F, DataType.Byte)] // 0x19f

    byte attackDiceSize(int index);
    byte attack1_DiceSize = 0; // [DataOffset(0x1A0, DataType.Byte)] // 0x1a0 field_1A0
    byte attack2_DiceSize = 0; // [DataOffset(0x1A1, DataType.Byte)] // 0x1a1

    byte attackDamageBonus(int index);
    sbyte attack1_DamageBonus = 0; // [DataOffset(0x1a2, DataType.SByte)] // 0x1a2
    byte attack2_DamageBonus = 0; // [DataOffset(0x1a3, DataType.Byte)] // 0x1a3

    byte hit_point_current = 0; // [DataOffset(0x1a4, DataType.Byte)] // 0x1a4

    byte movement = 0; // [DataOffsetAttribute(0x1A5, DataType.Byte)] // 0x1a5 initiative

    static constexpr int StructSize = 0x1A6;

    Player_();

    Player_(Array<byte> data, int offset);

private:
    void Init();

public:
    Player ShallowClone();

    Array<byte> ToByteArray();

    String ToString() const;

    bool CanDuelClass();

private:
    int DualClassExceedsPreviousLevel(); // sub_6B3D1

    int DuelClassCurrentLevel();

public:
    CombatTeam OppositeTeam();

    bool HasAffect(Affects type);

    Affect GetAffect(Affects type);

private:
    inline static Array<Affects> held_affects{Affects::snake_charm, Affects::paralyze, Affects::sleep, Affects::helpless};

public:
    bool IsHeld();

    void RemoveWeight(int amount);

    void AddWeight(int amount);

    // DataIO field table ([DataOffset] fields in declaration order); defined in Player.cpp.
    static const std::vector<FieldDesc>& DataFields();
};

}  // namespace Classes

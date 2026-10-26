// Classes/Player.cpp - port of Classes/Player.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

// ---------------------------------------------------------------------------
// StatValue
// ---------------------------------------------------------------------------

StatValue::StatValue(Array3<int> _raceSexMinMax, Array<int> _classMin, Array<int> _ageEffects)
{
    raceSexMinMax = _raceSexMinMax;
    classMin = _classMin;
    ageEffects = _ageEffects;
    cur = full = 0;
}

void StatValue::Load(int val)
{
    full = cur = val;
}

void StatValue::Assign(StatValue sv)
{
    full = sv.full;
    cur = sv.cur;
}

void StatValue::Inc()
{
    full += 1;
    cur += 1;
}

void StatValue::Dec()
{
    full -= 1;
    cur -= 1;
}

void StatValue::EnforceRaceSexLimits(int race, int sex)
{
    if (raceSexMinMax != nullptr)
    {
        full = Math::Min(raceSexMinMax(race, 1, sex), full);
        full = Math::Max(raceSexMinMax(race, 0, sex), full);
    }
    cur = full;
}

void StatValue::EnforceClassLimits(int _class)
{
    if (classMin != nullptr)
    {
        full = Math::Max(classMin[_class], full);
    }
    cur = full;
}

void StatValue::AgeEffects(int race, int age)
{
    for (int i = 0; i < 5; i++)
    {
        if (Limits::RaceAgeBrackets(race, i) < age)
        {
            full += ageEffects[i];
        }
    }
}

void StatValue::Write(Array<byte> data, int offset)
{
    data[offset + 0] = (byte)cur;
    data[offset + 1] = (byte)full;
}

void StatValue::Read(Array<byte> data, int offset)
{
    // enforce values in valid range
    cur = Math::Min((int)data[offset + 0], 25);
    full = Math::Min((int)data[offset + 1], 25);
}

String StatValue::ToString() const
{
    return Format("{0}/{1}", cur, full);
}

// ---------------------------------------------------------------------------
// PlayerStats
// ---------------------------------------------------------------------------

void PlayerStats_::Write(Array<byte> data, int offset)
{
    Str.Write(data, offset + 0x00);
    Int.Write(data, offset + 0x02);
    Wis.Write(data, offset + 0x04);
    Dex.Write(data, offset + 0x06);
    Con.Write(data, offset + 0x08);
    Cha.Write(data, offset + 0x0a);
    Str00.Write(data, offset + 0x0c);
}

void PlayerStats_::Read(Array<byte> data, int offset)
{
    Str.Read(data, offset + 0x00);
    Int.Read(data, offset + 0x02);
    Wis.Read(data, offset + 0x04);
    Dex.Read(data, offset + 0x06);
    Con.Read(data, offset + 0x08);
    Cha.Read(data, offset + 0x0a);
    Str00.Read(data, offset + 0x0c);
}

void PlayerStats_::Assign(PlayerStats ps)
{
    Str.Assign(ps->Str);
    Int.Assign(ps->Int);
    Wis.Assign(ps->Wis);
    Dex.Assign(ps->Dex);
    Con.Assign(ps->Con);
    Cha.Assign(ps->Cha);
    Str00.Assign(ps->Str00);
}

void PlayerStats_::Dec(int idx)
{
    switch (idx)
    {
        case 0: Str.Dec(); break;
        case 1: Int.Dec(); break;
        case 2: Wis.Dec(); break;
        case 3: Dex.Dec(); break;
        case 4: Con.Dec(); break;
        case 5: Cha.Dec(); break;
        default: throw IndexOutOfRangeException(Format("idx {0} not in [0-5]", idx));
    }
}

void PlayerStats_::Inc(int idx)
{
    switch (idx)
    {
        case 0: Str.Inc(); break;
        case 1: Int.Inc(); break;
        case 2: Wis.Inc(); break;
        case 3: Dex.Inc(); break;
        case 4: Con.Inc(); break;
        case 5: Cha.Inc(); break;
        default: throw IndexOutOfRangeException(Format("idx {0} not in [0-5]", idx));
    }
}

StatValue PlayerStats_::operator[](int idx) const
{
    switch (idx)
    {
        case 0: return Str;
        case 1: return Int;
        case 2: return Wis;
        case 3: return Dex;
        case 4: return Con;
        case 5: return Cha;
        default: throw IndexOutOfRangeException(Format("idx {0} not in [0-5]", idx));
    }
}

String PlayerStats_::ToString() const
{
    return Format("S:{0} ({6}),I:{1},W:{2},C:{3},D:{4},Ch:{5}", Str, Int, Wis, Con, Dex, Cha, Str00);
}

// ---------------------------------------------------------------------------
// ActiveItems
// ---------------------------------------------------------------------------

byte ActiveItems_::PrimaryWeaponHandCount()
{
    if (primaryWeapon() == nullptr) return 0;

    return primaryWeapon()->HandsCount();
}

byte ActiveItems_::SecondaryWeaponHandCount()
{
    if (secondaryWeapon() == nullptr) return 0;

    return secondaryWeapon()->HandsCount();
}

void ActiveItems_::Reset()
{
    for (int slot = 0; slot < ItemSlots; slot++)
    {
        itemArray[slot] = nullptr;
    }
}

void ActiveItems_::UndreadyAll(int classFlags)
{
    for (int item_slot = 0; item_slot < ItemSlots; item_slot++)
    {
        if (itemArray[item_slot] != nullptr &&
            (gbl::ItemDataTable[itemArray[item_slot]->type]->classFlags & classFlags) == 0 &&
            itemArray[item_slot]->cursed == false)
        {
            itemArray[item_slot]->readied = false;
        }
    }
}

// ---------------------------------------------------------------------------
// Player
// ---------------------------------------------------------------------------

int Player_::SkillLevel(SkillType skill)
{
    return ClassLevel[(int)skill] + (ClassLevelsOld[(int)skill] * DualClassExceedsPreviousLevel());
}

byte Player_::AttacksLeft(int index)
{
    switch (index)
    {
        case 1:
            return attack1_AttacksLeft;
        case 2:
            return attack2_AttacksLeft;
        default:
            throw NotImplementedException();
    }
}

void Player_::AttacksLeftSet(int index, byte value)
{
    switch (index)
    {
        case 1:
            attack1_AttacksLeft = value;
            break;
        case 2:
            attack2_AttacksLeft = value;
            break;
        default:
            throw NotImplementedException();
    }
}

void Player_::AttacksLeftDec(int index)
{
    switch (index)
    {
        case 1:
            attack1_AttacksLeft -= 1;
            break;
        case 2:
            attack2_AttacksLeft -= 1;
            break;
        default:
            throw NotImplementedException();
    }
}

byte Player_::attackDiceCount(int index)
{
    switch (index)
    {
        case 1:
            return attack1_DiceCount;
        case 2:
            return attack2_DiceCount;
        default:
            throw NotImplementedException();
    }
}

byte Player_::attackDiceSize(int index)
{
    switch (index)
    {
        case 1:
            return attack1_DiceSize;
        case 2:
            return attack2_DiceSize;
        default:
            throw NotImplementedException();
    }
}

byte Player_::attackDamageBonus(int index)
{
    switch (index)
    {
        case 1:
            return (byte)attack1_DamageBonus;
        case 2:
            return attack2_DamageBonus;
        default:
            throw NotImplementedException();
    }
}

Player_::Player_()
{
    Init();
}

Player_::Player_(Array<byte> data, int offset)
{
    Init();

    DataIO::ReadObject(this, data, offset);

    spellList->Load(data, offset + 0x1e);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            // PORT-NOTE: coab ignores `offset` and indexes with (i * i), not (i * 5); kept as is.
            spellCastCount(i, j) = data[0x12d + j + (i * i)];
        }
    }
}

void Player_::Init()
{
    spellCastCount = Array2<byte>(3, 5);
    //stats = new StatValue[6];
    stats2 = New<PlayerStats_>();

    name = String::Empty;
    items = List<Item>();
    affects = List<Affect>();

    actions = nullptr;
    Money = New<MoneySet_>();
    spellList = New<SpellList_>();
}

Player Player_::ShallowClone()
{
    // MemberwiseClone: the implicit copy constructor copies every member;
    // Ref/Array/List members alias, exactly as in C#.
    Player p = New<Player_>(*this);
    // PORT-NOTE: p.stats2 aliases this.stats2 after MemberwiseClone, so this Assign
    // is a self-assignment in coab too; kept as is.
    p->stats2->Assign(this->stats2);
    return p;
}

Array<byte> Player_::ToByteArray()
{
    Array<byte> data = Array<byte>(StructSize);

    DataIO::WriteObject(this, data);

    spellList->Save(data, 0x1e);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            data[0x12d + j + (i * i)] = spellCastCount(i, j);
        }
    }

    return data;
}

String Player_::ToString() const
{
    return name;
}

bool Player_::CanDuelClass()
{
    if (race != Race::human)
    {
        return false;
    }

    for (ClassId index = ClassId::cleric; index <= ClassId::monk; index++)
    {
        if (ClassLevelsOld[(int)index] > 0)
        {
            return false;
        }
    }

    return true;
}

int Player_::DualClassExceedsPreviousLevel() // sub_6B3D1
{
    if (DuelClassCurrentLevel() > multiclassLevel)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Player_::DuelClassCurrentLevel()
{
    if (race != Race::human)
    {
        return 0;
    }

    int loop_var = 0;

    while (loop_var < 7 &&
        ClassLevel[loop_var] == 0)
    {
        loop_var++;
    }

    return ClassLevel[loop_var];
}

CombatTeam Player_::OppositeTeam()
{
    return (combat_team == CombatTeam::Ours) ? CombatTeam::Enemy : CombatTeam::Ours;
}

bool Player_::HasAffect(Affects type)
{
    return affects.Exists([&](const Affect& aff) { return aff->type == type; });
}

Affect Player_::GetAffect(Affects type)
{
    return affects.Find([&](const Affect& aff) { return aff->type == type; });
}

bool Player_::IsHeld()
{
    return Array<Affects>::Exists(held_affects, [&](const Affects& affect) { return HasAffect(affect); });
}

void Player_::RemoveWeight(int amount)
{
    weight -= (short)amount;
}

void Player_::AddWeight(int amount)
{
    weight += (short)amount;
}

// ---------------------------------------------------------------------------
// DataIO field table: every [DataOffset] field of Player, in declaration order.
// ---------------------------------------------------------------------------
const std::vector<FieldDesc>& Player_::DataFields()
{
    static const std::vector<FieldDesc> fields = {
        DATAIO_FIELD(Player_, name, 0x00, DataType::PString, 15),
        DATAIO_FIELD(Player_, stats2, 0x10, DataType::CustSaveLoad, 0x12),
        DATAIO_FIELD_DS(Player_, spell_to_learn_count, 0x72, DataType::Byte),
        DATAIO_FIELD_DS(Player_, thac0, 0x73, DataType::SByte),
        DATAIO_FIELD_DS(Player_, race, 0x74, DataType::IByte),
        DATAIO_FIELD_DS(Player_, _class, 0x75, DataType::IByte),
        DATAIO_FIELD_DS(Player_, age, 0x76, DataType::SWord),
        DATAIO_FIELD_DS(Player_, hit_point_max, 0x78, DataType::Byte),
        DATAIO_FIELD(Player_, spellBook, 0x79, DataType::ByteArray, 100),
        DATAIO_FIELD_DS(Player_, attackLevel, 0xdd, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_DE, 0xde, DataType::Byte),
        DATAIO_FIELD(Player_, saveVerse, 0xdf, DataType::ByteArray, 5),
        DATAIO_FIELD_DS(Player_, base_movement, 0xe4, DataType::Byte),
        DATAIO_FIELD_DS(Player_, HitDice, 0xe5, DataType::Byte),
        DATAIO_FIELD_DS(Player_, multiclassLevel, 0xe6, DataType::Byte),
        DATAIO_FIELD_DS(Player_, lost_lvls, 0xe7, DataType::Byte),
        DATAIO_FIELD_DS(Player_, lost_hp, 0xe8, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_E9, 0xe9, DataType::Byte),
        DATAIO_FIELD(Player_, thief_skills, 0xeA, DataType::ByteArray, 8),
        DATAIO_FIELD_DS(Player_, field_F6, 0xf6, DataType::Byte),
        DATAIO_FIELD_DS(Player_, control_morale, 0xf7, DataType::Byte),
        DATAIO_FIELD_DS(Player_, npcTreasureShareCount, 0xf8, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_F9, 0xf9, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_FA, 0xfa, DataType::Byte),
        DATAIO_FIELD(Player_, Money, 0xfb, DataType::ShortArray, 7),
        DATAIO_FIELD(Player_, ClassLevel, 0x109, DataType::ByteArray, 8),
        DATAIO_FIELD(Player_, ClassLevelsOld, 0x111, DataType::ByteArray, 8),
        DATAIO_FIELD_DS(Player_, sex, 0x119, DataType::Byte),
        DATAIO_FIELD_DS(Player_, monsterType, 0x11a, DataType::IByte),
        DATAIO_FIELD_DS(Player_, alignment, 0x11b, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attacksCount, 0x11c, DataType::Byte),
        DATAIO_FIELD_DS(Player_, baseHalfMoves, 0x11d, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack1_DiceCountBase, 0x11e, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack2_DiceCountBase, 0x11f, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack1_DiceSizeBase, 0x120, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack2_DiceSizeBase, 0x121, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack1_DamageBonusBase, 0x122, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack2_DamageBonusBase, 0x123, DataType::Byte),
        DATAIO_FIELD_DS(Player_, base_ac, 0x124, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_125, 0x125, DataType::Byte),
        DATAIO_FIELD_DS(Player_, mod_id, 0x126, DataType::Byte),
        DATAIO_FIELD_DS(Player_, exp, 0x127, DataType::Int),
        DATAIO_FIELD_DS(Player_, classFlags, 0x12b, DataType::Byte),
        DATAIO_FIELD_DS(Player_, hit_point_rolled, 0x12c, DataType::Byte),
        //[DataOffset(0x12d, DataType.ByteArray,15)] spellCastCount
        DATAIO_FIELD_DS(Player_, field_13C, 0x13c, DataType::SWord),
        DATAIO_FIELD_DS(Player_, field_13E, 0x13e, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_13F, 0x13f, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_140, 0x140, DataType::Byte),
        DATAIO_FIELD_DS(Player_, head_icon, 0x141, DataType::Byte),
        DATAIO_FIELD_DS(Player_, weapon_icon, 0x142, DataType::Byte),
        DATAIO_FIELD_DS(Player_, icon_id, 0x143, DataType::Byte),
        DATAIO_FIELD_DS(Player_, icon_size, 0x144, DataType::Byte),
        DATAIO_FIELD(Player_, icon_colours, 0x145, DataType::ByteArray, 6),
        DATAIO_FIELD_DS(Player_, field_14B, 0x14b, DataType::Byte),
        //[DataOffset(0x14c, DataType.Byte)] field_14C
        DATAIO_FIELD_DS(Player_, weaponsHandsUsed, 0x185, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_186, 0x186, DataType::SByte),
        DATAIO_FIELD_DS(Player_, weight, 0x187, DataType::SWord),
        DATAIO_FIELD_DS(Player_, paladinCuresLeft, 0x191, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_192, 0x192, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_193, 0x193, DataType::Byte),
        DATAIO_FIELD_DS(Player_, field_194, 0x194, DataType::Byte),
        DATAIO_FIELD_DS(Player_, health_status, 0x195, DataType::IByte),
        DATAIO_FIELD_DS(Player_, in_combat, 0x196, DataType::Bool),
        DATAIO_FIELD_DS(Player_, combat_team, 0x197, DataType::IByte),
        DATAIO_FIELD_DS(Player_, quick_fight, 0x198, DataType::IByte),
        DATAIO_FIELD_DS(Player_, hitBonus, 0x199, DataType::IByte),
        DATAIO_FIELD_DS(Player_, ac, 0x19a, DataType::Byte),
        DATAIO_FIELD_DS(Player_, ac_behind, 0x19b, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack1_AttacksLeft, 0x19c, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack2_AttacksLeft, 0x19d, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack1_DiceCount, 0x19E, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack2_DiceCount, 0x19F, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack1_DiceSize, 0x1A0, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack2_DiceSize, 0x1A1, DataType::Byte),
        DATAIO_FIELD_DS(Player_, attack1_DamageBonus, 0x1a2, DataType::SByte),
        DATAIO_FIELD_DS(Player_, attack2_DamageBonus, 0x1a3, DataType::Byte),
        DATAIO_FIELD_DS(Player_, hit_point_current, 0x1a4, DataType::Byte),
        DATAIO_FIELD_DS(Player_, movement, 0x1A5, DataType::Byte),
    };
    return fields;
}

}  // namespace Classes

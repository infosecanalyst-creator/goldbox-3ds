// Classes/Item.cpp - port of Classes/Item.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

byte Item_::field_2EArray(int index)
{
    switch (index)
    {
        case 1: return (byte)namenum1;
        case 2: return (byte)namenum2;
        case 3: return (byte)namenum3;
        default: throw NotSupportedException();
    }
}

byte Item_::HandsCount()
{
    return gbl::ItemDataTable[type]->handsCount;
}

bool Item_::ScrollLearning(int i, int spell)
{
    return ((int)getAffect(i) > 0x7F && ((int)getAffect(i) & 0x7F) == spell);
}

bool Item_::IsScroll()
{
    return (gbl::ItemDataTable[type]->item_slot >= ItemSlot::slot_11 && gbl::ItemDataTable[type]->item_slot <= ItemSlot::slot_13);
}

Affects Item_::getAffect(int i)
{
    switch (i)
    {
        case 1:
            return affect_1;
        case 2:
            return affect_2;
        case 3:
            return affect_3;
        default:
            throw ArgumentOutOfRangeException();
    }
}

void Item_::setAffect(int i, Affects value)
{
    switch (i)
    {
        case 1:
            affect_1 = value;
            break;
        case 2:
            affect_2 = value;
            break;
        case 3:
            affect_3 = value;
            break;
        default:
            throw ArgumentOutOfRangeException();
    }
}

Item_::Item_()
{
    Clear();
}

Item_::Item_(Affects _affect_3, Affects _affect_2, Affects _affect_1, short __value, byte _count,
    short _weight, bool _cursed, byte _name_flags, bool _readied, byte _plus_save, sbyte _plus, byte _namenum3,
    byte _namenum2, byte _namenum1, ItemType _type, bool AddToLibrary)
{
    name = String::Empty;
    type = _type;
    namenum1 = _namenum1;
    namenum2 = _namenum2;
    namenum3 = _namenum3;
    plus = _plus;
    plus_save = _plus_save;
    readied = _readied;
    hidden_names_flag = _name_flags;
    cursed = _cursed;
    weight = _weight;
    count = _count;
    _value = __value;
    affect_1 = _affect_1;
    affect_2 = _affect_2;
    affect_3 = _affect_3;

    if (AddToLibrary)
    {
        // PORT-NOTE: shared_from_this() is unusable in a constructor; pass a
        // non-owning Ref (ItemLibrary.Add only shallow-clones it).
        ItemLibrary::Add(Item(std::shared_ptr<Item_>(std::shared_ptr<Item_>(), this)));
    }
}

Item_::Item_(Array<byte> data, int offset)
{
    name = Sys::ArrayToString(data, offset, 0x2a);

    type = (ItemType)data[offset + 0x2e];
    namenum1 = data[offset + 0x2f];
    namenum2 = data[offset + 0x30];
    namenum3 = data[offset + 0x31];
    plus = (sbyte)data[offset + 0x32];
    plus_save = data[offset + 0x33];
    readied = (data[offset + 0x34] != 0);
    hidden_names_flag = data[offset + 0x35];
    cursed = (data[offset + 0x36] != 0);

    weight = Sys::ArrayToShort(data, offset + 0x37);
    count = data[offset + 0x39];
    _value = Sys::ArrayToShort(data, offset + 0x3a);
    affect_1 = (Affects)data[offset + 0x3C];
    affect_2 = (Affects)data[offset + 0x3D];
    affect_3 = (Affects)data[offset + 0x3E];

    // PORT-NOTE: non-owning Ref to `this` (see the other constructor).
    ItemLibrary::Add(Item(std::shared_ptr<Item_>(std::shared_ptr<Item_>(), this)));
    //AddItemsText(string.Format("{0},{1},{2},{3},{4}", type, namenum1, namenum2, namenum3, GenerateName(0)));
}

Item Item_::ShallowClone()
{
    // MemberwiseClone: implicit copy constructor (no library Add, as in C#).
    Item i = New<Item_>(*this);
    return i;
}

void Item_::Clear()
{
    name = String::Empty;

    type = (ItemType)0;
    namenum1 = 0;
    namenum2 = 0;
    namenum3 = 0;
    plus = 0;
    plus_save = 0;
    readied = false;
    hidden_names_flag = 0;
    cursed = false;
    weight = 0;
    count = 0;
    _value = 0;
    affect_1 = (Affects)0;
    affect_2 = (Affects)0;
    affect_3 = (Affects)0;
}

Array<byte> Item_::ToByteArray()
{
    Array<byte> data = Array<byte>(StructSize);

    Sys::StringToArray(data, 0, 0x2a, name);

    data[0x2e] = (byte)type;
    data[0x2f] = (byte)namenum1;
    data[0x30] = (byte)namenum2;
    data[0x31] = (byte)namenum3;
    data[0x32] = (byte)plus;
    data[0x33] = plus_save;
    data[0x34] = readied ? (byte)1 : (byte)0;
    data[0x35] = hidden_names_flag;
    data[0x36] = cursed ? (byte)1 : (byte)0;
    Sys::ShortToArray(weight, data, 0x37);
    data[0x39] = (byte)count;
    Sys::ShortToArray(_value, data, 0x3a);
    data[0x3C] = (byte)affect_1;
    data[0x3D] = (byte)affect_2;
    data[0x3E] = (byte)affect_3;

    return data;
}

String Item_::GenerateName(int hidden_names_flag)
{
    int display_flags = 0;
    display_flags |= (namenum1 != 0 && (hidden_names_flag & 0x4) == 0) ? 0x1 : 0;
    display_flags |= (namenum2 != 0 && (hidden_names_flag & 0x2) == 0) ? 0x2 : 0;
    display_flags |= (namenum3 != 0 && (hidden_names_flag & 0x1) == 0) ? 0x4 : 0;

    bool pural_added = false;
    String name = "";

    for (int var_1 = 3; var_1 >= 1; var_1--)
    {
        if (((display_flags >> (var_1 - 1)) & 1) > 0)
        {
            name += itemNames[field_2EArray(var_1)];

            if (count < 2 ||
                pural_added == true)
            {
                name += " ";
            }
            else if ((1 << (var_1 - 1) == display_flags) ||
                    (var_1 == 1 && display_flags > 4 && type != ItemType::FlaskOfOil) ||
                    (var_1 == 2 && (display_flags & 1) == 0) ||
                    (var_1 == 3 && type == ItemType::FlaskOfOil) ||
                    (namenum3 != 0x87 && (type == ItemType::Arrow || type == ItemType::Quarrel || type == ItemType::Dart) && namenum3 != 0xb1))
            {
                name += "s ";
                pural_added = true;
            }
            else
            {
                name += " ";
            }
        }
    }

    return name.Trim();
}

bool Item_::Equals(const Item& obj) const
{
    Item rhs = obj;
    return rhs != nullptr &&
        rhs->type == type &&
        rhs->namenum1 == namenum1 &&
        rhs->namenum2 == namenum2 &&
        rhs->namenum3 == namenum3 &&
        rhs->plus == plus &&
        rhs->plus_save == plus_save &&
        rhs->readied == readied &&
        rhs->hidden_names_flag == hidden_names_flag &&
        rhs->cursed == cursed &&
        rhs->weight == weight &&
        rhs->count == count &&
        rhs->_value == _value &&
        rhs->affect_1 == affect_1 &&
        rhs->affect_2 == affect_2 &&
        rhs->affect_3 == affect_3
        ;
}

String Item_::ToString() const
{
    // PORT-NOTE: GenerateName is non-const in the header; C# ToString calls it.
    return const_cast<Item_*>(this)->GenerateName(0);
}

bool Item_::IsRanged()
{
    return gbl::ItemDataTable[type]->range > 1;
}

}  // namespace Classes

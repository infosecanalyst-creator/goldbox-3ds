// Classes/Item.h - port of Classes/Item.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/Affect.h"
#include "Classes/ItemData.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Item.
/// </summary>
// [Serializable]
class Item_ : public std::enable_shared_from_this<Item_> {
public:
    String name; // 0x00

    ItemType type = (ItemType)0; // 0x2e;
    byte field_2EArray(int index);
    int namenum1 = 0;
    int namenum2 = 0;
    int namenum3 = 0;

    int plus = 0; // 0x32
    byte plus_save = 0; // 0x33
    bool readied = false; // 0x34
    byte hidden_names_flag = 0; // 0x35
    bool cursed = false; // 0x36
    short weight = 0; // 0x37
    int count = 0;   // 0x39
    short _value = 0; // 0x3A "seams like value is in electrum, as value is doubled.";
    Affects affect_1 = (Affects)0; // 0x3C
    Affects affect_2 = (Affects)0; // 0x3D
    Affects affect_3 = (Affects)0; // 0x3E

    byte HandsCount();

    bool ScrollLearning(int i, int spell);

    bool IsScroll();

    Affects getAffect(int i);
    void setAffect(int i, Affects value);

    static constexpr int StructSize = 0x3F;

    Item_();

    // PORT-NOTE: when AddToLibrary is true the C# constructor passes `this` to
    // ItemLibrary.Add; shared_from_this() is not usable inside a constructor, so the
    // body must wrap `this` without ownership (ItemLibrary.Add only shallow-clones it).
    Item_(Affects _affect_3, Affects _affect_2, Affects _affect_1, short __value, byte _count,
          short _weight, bool _cursed, byte _name_flags, bool _readied, byte _plus_save, sbyte _plus, byte _namenum3,
          byte _namenum2, byte _namenum1, ItemType _type, bool AddToLibrary);

    // PORT-NOTE: also calls ItemLibrary.Add(this) - see above.
    Item_(Array<byte> data, int offset);

    // MemberwiseClone
    Item ShallowClone();

    void Clear();

    Array<byte> ToByteArray();

private:
    static Array<String> itemNames;

public:
    String GenerateName(int hidden_names_flag);

    // C# `override bool Equals(object obj)` (value equality, used by
    // List<Item>.Contains in ItemLibrary). Ref == stays pointer identity.
    bool Equals(const Item& obj) const;
    static bool Equals(const Item& a, const Item& b) { return a != nullptr && a->Equals(b); }

    String ToString() const;

    bool IsRanged();
};

}  // namespace Classes

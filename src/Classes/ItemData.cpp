// Classes/ItemData.cpp - port of Classes/ItemData.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

ItemDataTable_::ItemDataTable_(String fileName)
{
    String filePath = IO::Path::Combine(gbl::exe_path, fileName);

    IO::FileStream stream = IO::File::Open(filePath, IO::FileMode::Open, IO::FileAccess::Read);

    stream->Seek(2, IO::SeekOrigin::Begin);
    Array<byte> data = Array<byte>(0x810);
    stream->Read(data, 0, 0x810);

    table = Array<ItemData>(0x81);
    for (int i = 0; i < 0x81; i++)
    {
        table[i] = New<ItemData_>(data, i * 0x10);
    }

    stream->Close();
}

/// <summary>
/// Summary description for Struct_1C020.
/// </summary>
ItemData_::ItemData_(Array<byte> data, int offset)
{
    item_slot = (ItemSlot)data[offset + 0];
    handsCount = data[offset + 1];
    diceCountLarge = data[offset + 2];
    diceSizeLarge = data[offset + 3];
    bonusLarge = (sbyte)data[offset + 4];
    numberAttacks = data[offset + 5];
    field_6 = data[offset + 6];
    field_7 = data[offset + 7];
    field_8 = data[offset + 8];
    diceCountNormal = data[offset + 9];
    diceSizeNormal = data[offset + 0xa];
    bonusNormal = (sbyte)data[offset + 0xb];
    range = data[offset + 0xc];
    classFlags = data[offset + 0xd];
    field_E = (ItemDataFlags)data[offset + 0xe];
    field_F = data[offset + 0xf];
}

String ItemData_::ToString() const
{
    // PORT-NOTE: C# prints item_slot and field_E by enum name; here they print as numbers.
    return Format("{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}",
           item_slot, handsCount, diceCountLarge, diceSizeLarge, bonusLarge, numberAttacks, field_6, field_7, field_8,
           diceCountNormal, diceSizeNormal, bonusNormal, range, classFlags, field_E, field_F);
}

}  // namespace Classes

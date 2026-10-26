// Classes/Area1.h - port of Classes/Area1.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/DataIO.h"

#include <vector>

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Area1.
/// </summary>
class Area1_ {
    static constexpr int Area1Size = 0x800;

public:
    Area1_();
    Area1_(Array<byte> data, int offset);

private:
    void constructorInit();

public:
    void Clear();

protected:
    Array<byte> origData;

public:
    byte field_186 = 0; // [DataOffset(0x186, DataType.Byte)]
    byte field_188 = 0; // [DataOffset(0x188, DataType.Byte)]
    byte current_3DMap_block_id = 0; // [DataOffset(0x18A, DataType.Byte)] // field_18A

    // PORT-NOTE: coab tags field_18C with offset 0x18E (same as time_minutes_ones); kept as-is.
    ushort field_18C = 0; // [DataOffset(0x18E, DataType.Word)]
    ushort time_minutes_ones = 0; // [DataOffset(0x18E, DataType.Word)] // field_18E
    ushort time_minutes_tens = 0; // [DataOffset(0x190, DataType.Word)] // field_190
    ushort time_hour = 0; // [DataOffset(0x192, DataType.Word)] // field_192
    ushort time_day = 0; // [DataOffset(0x194, DataType.Word)] // field_194
    ushort time_year = 0; // [DataOffset(0x196, DataType.Word)] // field_196
    ushort field_198 = 0; // [DataOffset(0x198, DataType.Word)]

    short field_1CA = 0; // [DataOffset(0x1CA, DataType.SWord)]
    short inDungeon = 0; // [DataOffset(0x1CC, DataType.SWord)]
    short field_1CE = 0; // [DataOffset(0x1CE, DataType.SWord)]
    short field_1D0 = 0; // [DataOffset(0x1D0, DataType.SWord)]

    short lastXPos = 0; // [DataOffset(0x1E0, DataType.SWord)]
    short lastYPos = 0; // [DataOffset(0x1E2, DataType.SWord)]
    ushort LastEclBlockId = 0; // [DataOffset(0x1E4, DataType.Word)] // field_1E4
    short block_area_view = 0; // [DataOffset(0x1F6, DataType.SWord)] // field_1F6
    byte game_speed = 0; // [DataOffset(0x1F8, DataType.Byte)]
    ushort outdoor_sky_colour = 0; // [DataOffset(0x1FA, DataType.Word)]
    ushort indoor_sky_colour = 0; // [DataOffset(0x1FC, DataType.Word)]
    byte pics_on = 0; // [DataOffset(0x1FE, DataType.Byte)] //field_1FE;

    bool can_cast_spells = false; // [DataOffset(0x1FF, DataType.Bool)]

    Array<short> field_200; // [DataOffset(0x200, DataType.ShortArray, 33)] // 1-32


    ushort field_244 = 0; // [DataOffset(0x244, DataType.Word)]

    ushort field_24E = 0; // [DataOffset(0x24E, DataType.Word)]
    ushort field_250 = 0; // [DataOffset(0x250, DataType.Word)]
    ushort field_252 = 0; // [DataOffset(0x252, DataType.Word)]
    ushort field_254 = 0; // [DataOffset(0x254, DataType.Word)]
    ushort field_256 = 0; // [DataOffset(0x256, DataType.Word)]
    ushort field_258 = 0; // [DataOffset(0x258, DataType.Word)]
    ushort field_25A = 0; // [DataOffset(0x25A, DataType.Word)]
    ushort field_25C = 0; // [DataOffset(0x25C, DataType.Word)]
    ushort field_25E = 0; // [DataOffset(0x25E, DataType.Word)]
    ushort field_260 = 0; // [DataOffset(0x260, DataType.Word)]

    ushort field_26A = 0; // [DataOffset(0x26A, DataType.Word)]

    ushort field_296 = 0; // [DataOffset(0x296, DataType.Word)]
    ushort field_298 = 0; // [DataOffset(0x298, DataType.Word)]
    ushort field_29A = 0; // [DataOffset(0x29A, DataType.Word)]
    ushort field_2B2 = 0; // [DataOffset(0x2B2, DataType.Word)]
    ushort field_2B4 = 0; // [DataOffset(0x2B4, DataType.Word)]
    ushort field_2B6 = 0; // [DataOffset(0x2B6, DataType.Word)]

    ushort field_2C0 = 0; // [DataOffset(0x2C0, DataType.Word)]
    ushort field_2CA = 0; // [DataOffset(0x2CA, DataType.Word)]


    byte field_336 = 0; // [DataOffset(0x336, DataType.Byte)]
    byte field_338 = 0; // [DataOffset(0x338, DataType.Byte)]
    byte field_33A = 0; // [DataOffset(0x33A, DataType.Byte)]
    ushort field_33C = 0; // [DataOffset(0x33C, DataType.Word)]

    byte field_340 = 0; // [DataOffset(0x340, DataType.Byte)]
    byte current_city = 0; // [DataOffset(0x342, DataType.Byte)] //field_342
    byte field_344 = 0; // [DataOffset(0x344, DataType.Byte)]
    byte field_346 = 0; // [DataOffset(0x346, DataType.Byte)]
    byte field_348 = 0; // [DataOffset(0x348, DataType.Byte)]

    ushort field_3C2 = 0; // [DataOffset(0x3C2, DataType.Word)]
    ushort field_3CA = 0; // [DataOffset(0x3CA, DataType.Word)]
    ushort field_3CC = 0; // [DataOffset(0x3CC, DataType.Word)]

    ushort field_3D4 = 0; // [DataOffset(0x3D4, DataType.Word)]
    ushort field_3D6 = 0; // [DataOffset(0x3D6, DataType.Word)]
    ushort field_3D8 = 0; // [DataOffset(0x3D8, DataType.Word)]
    ushort field_3DA = 0; // [DataOffset(0x3DA, DataType.Word)]
    ushort field_3DC = 0; // [DataOffset(0x3DC, DataType.Word)]
    ushort field_3DE = 0; // [DataOffset(0x3DE, DataType.Word)]

    ushort field_3E0 = 0; // [DataOffset(0x3E0, DataType.Word)]
    ushort field_3E2 = 0; // [DataOffset(0x3E2, DataType.Word)]
    ushort field_3E4 = 0; // [DataOffset(0x3E4, DataType.Word)]
    ushort field_3E6 = 0; // [DataOffset(0x3E6, DataType.Word)]
    ushort field_3E8 = 0; // [DataOffset(0x3E8, DataType.Word)]

    byte field_3FA = 0; // [DataOffset(0x3FA, DataType.Byte)]
    ushort field_3FC = 0; // [DataOffset(0x3FC, DataType.Word)]
    short picture_fade = 0; // [DataOffset(0x3FE, DataType.SWord)] //field_3FE

    ushort field_596 = 0; // [DataOffset(0x596, DataType.Word)]



    void field_6A00_Set(int index, ushort value);
    ushort field_6A00_Get(int index);

    Array<byte> ToByteArray();

    void RestField200Values();

    // DataIO field table ([DataOffset] fields in declaration order); defined in Area1.cpp.
    static const std::vector<FieldDesc>& DataFields();
};

}  // namespace Classes

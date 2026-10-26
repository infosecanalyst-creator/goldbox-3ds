// Classes/GeoBlock.cpp - port of Classes/GeoBlock.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

void GeoBlock_::LoadData(Array<byte> _data)
{
    data = Array<byte>(0x400);
    Array<byte>::Copy(_data, 2, data, 0, 0x400);

    maps = Array2<MapInfo>(16, 16);

    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            maps(y, x) = New<MapInfo_>(data, x, y);
        }
    }
}

WallDefs_::WallDefs_()
{
    blocks[0] = New<WallDefBlock_>();
    blocks[1] = New<WallDefBlock_>();
    blocks[2] = New<WallDefBlock_>();
}

void WallDefs_::LoadData(int baseSet, Array<byte> _data)
{
    int offset = 0;
    for (int i = 0; i < (_data.Length() / 780); i++)
    {
        blocks[baseSet + i - 1]->LoadData(_data, offset);

        offset += 780;
    }
}

void WallDefs_::BlockOffset(int set, int offset)
{
    blocks[set - 1]->Offset(offset);
}

void WallDefBlock_::LoadData(Array<byte> _data, int offset)
{
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 156; x++)
        {
            data(y, x) = _data[offset++];
        }
    }
}

int WallDefBlock_::Id(int y, int x)
{
    return data(y, x);
}

void WallDefBlock_::Offset(int off)
{
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 156; x++)
        {
            if (data(y, x) >= 0x2D)
            {
                data(y, x) += (byte)off;
            }
        }
    }
}

MapInfo_::MapInfo_(Array<byte> data, int map_x, int map_y)
{
    int map_y_x16 = map_y << 4;

    wall_type_dir_0 = (byte)((data[map_x + map_y_x16] >> 4) & 0x0f);
    wall_type_dir_2 = (byte)((data[map_x + map_y_x16]) & 0x0f);
    wall_type_dir_4 = (byte)((data[0x100 + map_x + map_y_x16] >> 4) & 0x0f);
    wall_type_dir_6 = (byte)((data[0x100 + map_x + map_y_x16]) & 0x0f);

    x2 = data[0x200 + map_y_x16 + map_x];

    byte b = data[0x300 + map_y_x16 + map_x];

    x3_dir_6 = (byte)((b >> 6) & 3);
    x3_dir_4 = (byte)((b >> 4) & 3);
    x3_dir_2 = (byte)((b >> 2) & 3);
    x3_dir_0 = (byte)(b & 3);
}

}  // namespace Classes

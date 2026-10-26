// Classes/GeoBlock.h - port of Classes/GeoBlock.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class GeoBlock_ {
    Array<byte> data;

public:
    Array2<MapInfo> maps;

    void LoadData(Array<byte> _data);
};

class WallDefs_ {
    static constexpr int maxBlocks = 3;

public:
    Array<WallDefBlock> blocks = Array<WallDefBlock>(maxBlocks);

    WallDefs_();

    void LoadData(int baseSet, Array<byte> _data);

    void BlockOffset(int set, int offset);
};

class WallDefBlock_ {
    Array2<byte> data = Array2<byte>(5, 156);

public:
    void LoadData(Array<byte> _data, int offset);

    int Id(int y, int x);

    void Offset(int off);
};

class MapInfo_ {
public:
    byte wall_type_dir_0 = 0;
    byte wall_type_dir_2 = 0;
    byte wall_type_dir_4 = 0;
    byte wall_type_dir_6 = 0;

    byte x2 = 0;

    byte x3_dir_0 = 0;
    byte x3_dir_2 = 0;
    byte x3_dir_4 = 0;
    byte x3_dir_6 = 0;

    // C# `internal` constructor (public here: New<MapInfo_> needs access).
    MapInfo_(Array<byte> data, int map_x, int map_y);
};

}  // namespace Classes

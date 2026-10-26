// Classes/Struct_1D1BC.h - port of Classes/Struct_1D1BC.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/GblTypes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Struct_1D1BC.
/// </summary>
class Struct_1D1BC_ {
    static constexpr int dataSize = 1250; // 0x4E2

public:
    Struct_1D1BC_();

    void SetField_7(int value);

    Point mapScreenTopLeft;
    bool drawTargetCursor = false; // field_4, was byte
    int size = 0; // field_5
    bool ignoreWalls = false; // field_6, was byte

private:
    Array<int> field_7;

public:
    // C# get/set indexers this[Point pos] and this[int x, int y].
    int& operator[](Point pos) {
        int index = (pos.y * 50) + pos.x;
        return field_7[index];
    }
    // PORT-NOTE: C# `this[int x, int y]` -> operator()(x, y). The C# getter uses
    // x + (y * 50) and the setter x + (y * 0x32): identical, so one int& suffices.
    int& operator()(int x, int y) {
        int index = x + (y * 50);
        return field_7[index];
    }
    // Same as operator()(x, y), callable through the Ref:
    // C# `gbl.mapToBackGroundTile[x, y]` -> `gbl.mapToBackGroundTile->at(x, y)`.
    int& at(int x, int y) { return (*this)(x, y); }
};

}  // namespace Classes

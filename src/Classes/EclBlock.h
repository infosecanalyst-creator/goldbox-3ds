// Classes/EclBlock.h - port of Classes/EclBlock.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for EclBlock.
/// </summary>
class EclBlock_ {
public:
    static constexpr ushort ecl_struct_size = 0x1e00;

private:
    Array<byte> data;

public:
    EclBlock_();

    EclBlock_(Array<byte> _data, int offset);

    void Clear();

    // C# get/set indexer `byte this[int index]`: simulates the 16 bit memory space
    // (loc = index & 0xFFFF). Returns a reference so `ecl[i] = v` works.
    byte& operator[](int index) {
        // simulate the 16 bit memory space.
        int loc = index & 0xFFFF;
        return data[loc];
    }

    void SetData(Array<byte> dataArray, int dataOffset, int dataLength);

    Array<byte> ToByteArray();
};

}  // namespace Classes

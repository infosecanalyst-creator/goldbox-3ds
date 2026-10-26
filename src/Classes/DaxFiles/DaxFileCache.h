// Classes/DaxFiles/DaxFileCache.h - port of Classes/DaxFiles/DaxFileCache.cs
// (C# `internal class`; public here.)
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
namespace DaxFiles {
using namespace cs;
using namespace cs::enum_ops;

class DaxFileCache_ {
    Dictionary<int, Array<byte>> entries;

public:
    DaxFileCache_(String filename);

private:
    void LoadFile(String filename);

    void Decode(int decodeSize, int dataLength, Array<byte> output_ptr, Array<byte> input_ptr);

public:
    Array<byte> GetData(int block_id);
};

}  // namespace DaxFiles
}  // namespace Classes

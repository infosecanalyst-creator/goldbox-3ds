// Classes/DaxFiles/DaxHeaderEntry.h - port of Classes/DaxFiles/DaxHeaderEntry.cs
// (C# `internal class` with internal fields; public here.)
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
namespace DaxFiles {
using namespace cs;
using namespace cs::enum_ops;

class DaxHeaderEntry_ {
public:
    int id = 0;
    int offset = 0;
    int rawSize = 0; // decodeSize
    int compSize = 0; // dataLength
};

}  // namespace DaxFiles
}  // namespace Classes

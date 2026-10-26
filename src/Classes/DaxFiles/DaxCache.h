// Classes/DaxFiles/DaxCache.h - port of Classes/DaxFiles/DaxCache.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/DaxFiles/DaxFileCache.h"

namespace Classes {
namespace DaxFiles {
using namespace cs;
using namespace cs::enum_ops;

struct DaxCache {
private:
    inline static Dictionary<String, DaxFileCache> fileCache;

public:
    static Array<byte> LoadDax(String file_name, int block_id);
};

}  // namespace DaxFiles
}  // namespace Classes

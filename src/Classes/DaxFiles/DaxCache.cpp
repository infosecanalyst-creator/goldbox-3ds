// Classes/DaxFiles/DaxCache.cpp - port of Classes/DaxFiles/DaxCache.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {
namespace DaxFiles {

Array<byte> DaxCache::LoadDax(String file_name, int block_id)
{
    DaxFileCache dfc;

    file_name = file_name.ToLower();

    if (!fileCache.TryGetValue(file_name, dfc))
    {
        dfc = New<DaxFileCache_>(file_name);
        fileCache.Add(file_name, dfc);
    }

    return dfc->GetData(block_id);
}

}  // namespace DaxFiles
}  // namespace Classes

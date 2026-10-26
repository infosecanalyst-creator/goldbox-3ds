// Classes/ItemLibrary.h - port of Classes/ItemLibrary.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

// PORT-NOTE: the C# version persists the library with .NET BinaryFormatter to
// %APPDATA%/CotAB/ItemLibrary.dat. Not portable; Read/Write bodies are stubbed.
struct ItemLibrary {
private:
    inline static String libraryPath;  // Path.Combine(Environment.GetFolderPath(SpecialFolder.ApplicationData), "CotAB") - set in the .cpp
    inline static String libraryFile;  // Path.Combine(libraryPath, "ItemLibrary.dat")

    inline static List<Item> library;

public:
    static void Add(Item item);

    static void Read();

    static void Write();
};

}  // namespace Classes

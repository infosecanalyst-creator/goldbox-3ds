// Classes/ItemLibrary.cpp - port of Classes/ItemLibrary.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

// PORT-NOTE: libraryPath / libraryFile (Path.Combine(ApplicationData, "CotAB") and
// ".../ItemLibrary.dat") are left empty: only the stubbed Read/Write used them.

void ItemLibrary::Add(Item item)
{
    Item i = item->ShallowClone();
    i->readied = false;
    i->hidden_names_flag = 0;
    i->name = i->GenerateName(0);
    // C# List<Item>.Contains uses the overridden Item.Equals (value equality).
    if (library.Exists([&](const Item& x) { return Item_::Equals(x, i); }) == false)
    {
        library.Add(i);
        Write();
    }
}

void ItemLibrary::Read()
{
    // PORT-NOTE: the C# version deserialises List<Item> from libraryFile with .NET
    // BinaryFormatter, which is not portable. Stubbed: the library stays as it is
    // (empty at start-up).
}

void ItemLibrary::Write()
{
    // PORT-NOTE: the C# version creates libraryPath and serialises the library to
    // libraryFile with .NET BinaryFormatter. Stubbed to a no-op (nothing persisted).
}

}  // namespace Classes

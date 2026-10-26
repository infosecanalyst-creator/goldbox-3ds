// Classes/MenuItem.h - port of Classes/MenuItem.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class MenuItem_ {
public:
    // C# auto-properties { get; set; } / { get; protected set; } -> plain public fields
    String Text;  // 0x00;
    bool Heading = false; // 0x29;
    Classes::Item Item;

    MenuItem_();

    MenuItem_(String text);

    MenuItem_(String text, bool heading);

    MenuItem_(String text, Classes::Item item);
};

}  // namespace Classes

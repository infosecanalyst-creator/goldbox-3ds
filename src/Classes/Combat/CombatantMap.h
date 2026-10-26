// Classes/Combat/CombatantMap.h - port of Classes/Combat/CombatantMap.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/GblTypes.h"

namespace Classes {
namespace Combat {
using namespace cs;
using namespace cs::enum_ops;

class CombatantMap_ { // Struct_1C9CD
public:
    Point pos;
    int size = 0;

    Point screenPos;

    String ToString() const;
};

}  // namespace Combat
}  // namespace Classes

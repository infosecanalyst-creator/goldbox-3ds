// Classes/Action.cpp - port of Classes/Action.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

Action_::Action_()
{
    target = nullptr;
}

void Action_::Clear()
{
    delay = 0;
    spell_id = 0;
    guarding = false;
    move = 0;
}

}  // namespace Classes

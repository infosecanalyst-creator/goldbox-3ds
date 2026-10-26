// Classes/Action.h - port of Classes/Action.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Action.
/// </summary>
class Action_ {
public:
    int spell_id = 0; // 0x00
    bool can_cast = false; // 0x01
    bool can_use = false;  // 0x02
    int delay = 0;   // 0x03
    int attackIdx = 0;  // 0x04
    byte maxSweapTargets = 0;  // 0x05
    int move = 0;     // 0x06
    bool guarding = false; // 0x07
    bool field_8 = false;  // 0x08
    int direction = 0;  // 0x09 field_9
    Player target; // 0x0A 4 bytes
    int bleeding = 0; // 0x0E
    byte AttacksReceived = 0;  // 0x0F
    bool fleeing = false; // 0x10
    bool hasTurnedUndead = false; // 0x11
    int directionChanges = 0; // 0x12
    bool nonTeamMember = false; // 0x13 field_13
    bool moral_failure = false; // 0x14
    int field_15 = 0; // 0x15

    Action_();

    void Clear();
};

}  // namespace Classes

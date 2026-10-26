// Classes/SteppingPath.h - port of Classes/SteppingPath.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/GblTypes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class SteppingPath_ {
public:
    // 0x17 or 23 long
    Point attacker;      // 00
    Point target;        // 04
private:
    int delta_count = 0;     // 08
public:
    int diff_x = 0;          // 0a
    int diff_y = 0;          // 0a
    Point current;       // 0e
private:
    int sign_x = 0;          // 12
    int sign_y = 0;          // 14
public:
    byte steps = 0;          // 16
    byte direction = 0;      // 17

    void Clear();

    /// <summary>
    /// Return if step was taken
    /// </summary>
    bool Step(); /* sub_7324C */

private:
    inline static Array<byte> directions{ 7, 0, 1, 6, 8, 2, 5, 4, 3, 8 };

public:
    void CalculateDeltas(); /* sub_731A5 */
};

}  // namespace Classes

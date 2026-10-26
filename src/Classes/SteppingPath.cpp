// Classes/SteppingPath.cpp - port of Classes/SteppingPath.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

void SteppingPath_::Clear()
{
    attacker.x = 0;
    attacker.y = 0;
    target.x = 0;
    target.y = 0;
    delta_count = 0;
    diff_x = 0;
    diff_y = 0;
    current.x = 0;
    current.y = 0;
    sign_x = 0;
    sign_y = 0;
    steps = 0;
    direction = 0;
}

/// <summary>
/// Return if step was taken
/// </summary>
bool SteppingPath_::Step() /* sub_7324C */
{
    bool stepMade = false;
    int index_x = 1;
    int index_y = 1;

    if (diff_x >= diff_y)
    {
        if (current.x != target.x)
        {
            current.x += sign_x;
            delta_count += diff_y * 2;
            steps += 2;

            index_x = sign_x + 1;

            if (delta_count >= diff_x)
            {
                current.y += sign_y;
                delta_count -= diff_x * 2;
                steps += 1;

                index_y = sign_y + 1;
            }

            stepMade = true;
        }
    }
    else if (current.y != target.y)
    {
        current.y += sign_y;
        delta_count += diff_x * 2;
        steps += 2;

        index_y = sign_y + 1;

        if (delta_count >= diff_y)
        {
            current.x += sign_x;
            delta_count -= diff_y * 2;
            steps += 1;

            index_x = sign_x + 1;
        }

        stepMade = true;
    }

    direction = directions[(index_y * 3) + index_x];

    return stepMade;
}

void SteppingPath_::CalculateDeltas() /* sub_731A5 */
{
    current = attacker;

    auto tmp = target - attacker;
    diff_x = Math::Abs(tmp.x);
    diff_y = Math::Abs(tmp.y);

    sign_x = Math::Sign(tmp.x);
    sign_y = Math::Sign(tmp.y);

    delta_count = 0;
    steps = 0;
}

}  // namespace Classes

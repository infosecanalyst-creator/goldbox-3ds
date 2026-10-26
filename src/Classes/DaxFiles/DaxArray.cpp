// Classes/DaxFiles/DaxArray.cpp - port of Classes/DaxFiles/DaxArray.cs (namespace Classes)
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

DaxArray_::DaxArray_()
{
    numFrames = 0;
    curFrame = 0;
    frames = Array<AnimationFrame>(8);
    frames[0] = New<AnimationFrame_>(); // 1D558 - seg600:7248
    frames[1] = New<AnimationFrame_>(); // 1D560 - seg600:7250
    frames[2] = New<AnimationFrame_>(); // 1D568 - seg600:7258
    frames[3] = New<AnimationFrame_>(); // 1D570 - seg600:7260
    frames[4] = New<AnimationFrame_>(); // 1D578 - seg600:7268
    frames[5] = New<AnimationFrame_>(); // 1D580 - seg600:7270
    frames[6] = New<AnimationFrame_>(); // 1D588 - seg600:7278
    frames[7] = New<AnimationFrame_>(); // 1D590 - seg600:7280
}

void DaxArray_::NextFrame()
{
    curFrame++;

    if (curFrame > numFrames)
    {
        curFrame = 1;
    }
}

DaxBlock DaxArray_::CurrentPicture()
{
    return frames[curFrame - 1]->picture;
}

/// <summary>
/// Tenth of a second
/// </summary>
int DaxArray_::CurrentDelay()
{
    return frames[curFrame - 1]->delay;
}

}  // namespace Classes

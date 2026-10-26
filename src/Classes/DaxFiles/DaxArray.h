// Classes/DaxFiles/DaxArray.h - port of Classes/DaxFiles/DaxArray.cs (namespace Classes)
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class AnimationFrame_ {
public:
    int delay = 0; // field_0 seg600:7248 == 7240[1] & 7242[1]
    DaxBlock picture; // field_4 seg600:724C == 7244[1] & 7246[1]
};

class DaxArray_ {
public:
    int numFrames = 0;
    int curFrame = 0;
    Array<AnimationFrame> frames;

    DaxArray_();

    void NextFrame();

    DaxBlock CurrentPicture();

    /// <summary>
    /// Tenth of a second
    /// </summary>
    int CurrentDelay();
};

}  // namespace Classes

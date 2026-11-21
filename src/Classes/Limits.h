// Classes/Limits.h - port of Classes/Limits.cs
// NOTE: the tables are plain `public static` (not readonly) in C#, so they are
// non-const here too. Data copied verbatim from Limits.cs.
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/Enums.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

struct Limits {
    static Array2<int> RaceAgeBrackets; // human

    static Array<int> StrAgeEffect;
    static Array<int> Str00AgeEffect;
    static Array<int> IntAgeEffect;
    static Array<int> WisAgeEffect;
    static Array<int> DexAgeEffect;
    static Array<int> ConAgeEffect;
    static Array<int> ChaAgeEffect;


    static Array3<int> StrRaceSexMinMax;

    static Array3<int> Str00RaceSexMinMax;

    static Array3<int> IntRaceSexMinMax;

    static Array3<int> WisRaceSexMinMax;

    static Array3<int> DexRaceSexMinMax;

    static Array3<int> ConRaceSexMinMax;

    static Array3<int> ChaRaceSexMinMax;

    static Array<int> StrClassMin;
    static Array<int> IntClassMin;
    static Array<int> WisClassMin;
    static Array<int> DexClassMin;
    static Array<int> ConClassMin;
    static Array<int> ChaClassMin;
    static Array<int> Str00ClassMin;

    static bool RaceClassLimit(int class_lvl, Player player, ClassId _class);

    static bool RaceStatLevelRestricted(ClassId _class, Player player); // sub_69138
};

}  // namespace Classes

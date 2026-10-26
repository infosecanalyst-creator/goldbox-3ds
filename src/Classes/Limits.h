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
    inline static Array2<int> RaceAgeBrackets{ //unk_1A434
        { 9999, 9999, 9999, 9999, 9999}, // monster, made-up
        {   50,  150,  250,  350,  450}, // dwarf
        {  175,  550,  875, 1200, 1600}, //elf
        {   90,  300,  450,  600,  750}, // gnome
        {   40,  100,  175,  250,  325}, // half elf
        { 0x21, 0x44, 0x65, 0x90, 0xC7}, // halfling
        { 0x0F, 0x1E, 0x2D, 0x3C, 0x50}, // half orc
        { 0x14, 0x28, 0x3C, 0x5A, 0x78} }; // human

    inline static Array<int> StrAgeEffect{ 0, 1, -1, -2, -1 };
    inline static Array<int> Str00AgeEffect{ 0, 0, 0, 0, 0 };
    inline static Array<int> IntAgeEffect{ 0, 0, 1, 0, 1 };
    inline static Array<int> WisAgeEffect{ -1, 1, 1, 1, 1 };
    inline static Array<int> DexAgeEffect{ 0, 0, 0, -2, -1 };
    inline static Array<int> ConAgeEffect{ 1, 0, -1, -1, -1 };
    inline static Array<int> ChaAgeEffect{ 0, 0, 0, 0, 0 };


    inline static Array3<int> StrRaceSexMinMax{
        { {0, 5}, {10,  0} },
        { {8, 8}, {18, 17} },
        { {3, 3}, {18, 16} },
        { {6, 6}, {18, 15} },
        { {3, 3}, {18, 17} },
        { {6, 6}, {17, 14} },
        { {6, 6}, {18, 18} },
        { {3, 3}, {18, 18} } };

    inline static Array3<int> Str00RaceSexMinMax{
        { {0, 0}, {  5,  5} },
        { {0, 0}, { 99,  0} },
        { {0, 0}, { 75,  0} },
        { {0, 0}, { 50,  0} },
        { {0, 0}, { 90,  0} },
        { {0, 0}, {  0,  0} },
        { {0, 0}, { 99, 75} },
        { {0, 0}, {100, 50} } };

    inline static Array3<int> IntRaceSexMinMax{
        { {10, 10}, {15, 15} } ,
        { { 3,  3}, {18, 18} } ,
        { { 8,  8}, {18, 18} } ,
        { { 7,  7}, {18, 18} } ,
        { { 4,  4}, {18, 18} } ,
        { { 6,  6}, {18, 18} } ,
        { { 3,  3}, {17, 17} } ,
        { { 3,  3}, {18, 18} } };

    inline static Array3<int> WisRaceSexMinMax{
        { {5, 5}, {10, 10} },
        { {3, 3}, {18, 18} },
        { {3, 3}, {18, 18} },
        { {3, 3}, {18, 18} },
        { {3, 3}, {18, 18} },
        { {3, 3}, {17, 17} },
        { {3, 3}, {14, 14} },
        { {3, 3}, {18, 18} } };

    inline static Array3<int> DexRaceSexMinMax{
        { {10, 10}, {15, 15} },
        { { 3,  3}, {17, 17} },
        { { 7,  7}, {19, 19} },
        { { 3,  3}, {18, 18} },
        { { 6,  6}, {18, 18} },
        { { 8,  8}, {18, 18} },
        { { 3,  3}, {17, 17} },
        { { 3,  3}, {18, 18} } };

    inline static Array3<int> ConRaceSexMinMax{
        { {20, 20}, {10, 10} },
        { {12, 12}, {19, 19} },
        { { 6,  6}, {18, 18} },
        { { 8,  8}, {18, 18} },
        { { 6,  6}, {18, 18} },
        { {10, 10}, {19, 19} },
        { {13, 13}, {19, 19} },
        { { 3,  3}, {18, 18} } };

    inline static Array3<int> ChaRaceSexMinMax{
        { {12, 12}, {12, 12} },
        { { 3,  3}, {16, 16} },
        { { 8,  8}, {18, 18} },
        { { 3,  3}, {18, 18} },
        { { 3,  3}, {18, 18} },
        { { 3,  3}, {18, 18} },
        { { 3,  3}, {12, 12} },
        { { 3,  3}, {18, 18} } };

    inline static Array<int> StrClassMin{ 6, 0, 9, 12, 13, 0, 6, 15, 9, 9, 0, 0, 0, 9, 9, 9, 0 };
    inline static Array<int> IntClassMin{ 6, 0, 0, 9, 13, 9, 6, 0, 0, 9, 13, 9, 0, 9, 0, 9, 9 };
    inline static Array<int> WisClassMin{ 9, 12, 6, 13, 14, 6, 0, 15, 9, 9, 14, 9, 9, 0, 0, 0, 0 };
    inline static Array<int> DexClassMin{ 0, 0, 6, 0, 0, 6, 9, 15, 0, 0, 0, 0, 9, 0, 9, 9, 9 };
    inline static Array<int> ConClassMin{ 0, 0, 7, 9, 14, 0, 0, 11, 0, 0, 14, 0, 0, 0, 0, 0, 0 };
    inline static Array<int> ChaClassMin{ 0, 15, 0, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    inline static Array<int> Str00ClassMin{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static bool RaceClassLimit(int class_lvl, Player player, ClassId _class);

    static bool RaceStatLevelRestricted(ClassId _class, Player player); // sub_69138
};

}  // namespace Classes

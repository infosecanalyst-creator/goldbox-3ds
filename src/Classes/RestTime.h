// Classes/RestTime.h - port of Classes/RestTime.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class RestTime_ {
public:
    int field_0 = 0;
    int field_2 = 0;
    int field_4 = 0;
    int field_6 = 0;
    int field_8 = 0;
    int field_A = 0;
    int field_C = 0;

    RestTime_();

    RestTime_(RestTime old);

    RestTime_(int f0, int f2, int f4, int f6, int f8, int fA, int fC);

    void Clear();

    // C# get/set indexer `int this[int index]` (switch over field_0..field_C, throws
    // NotSupportedException otherwise). Returns a reference so `t[i] -= 1` works.
    int& operator[](int index);
};

}  // namespace Classes

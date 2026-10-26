// Classes/RestTime.cpp - port of Classes/RestTime.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

RestTime_::RestTime_()
{
}

RestTime_::RestTime_(RestTime old)
{
    field_0 = old->field_0;
    field_2 = old->field_2;
    field_4 = old->field_4;
    field_6 = old->field_6;
    field_8 = old->field_8;
    field_A = old->field_A;
    field_C = old->field_C;
}

RestTime_::RestTime_(int f0, int f2, int f4, int f6, int f8, int fA, int fC)
{
    field_0 = f0;
    field_2 = f2;
    field_4 = f4;
    field_6 = f6;
    field_8 = f8;
    field_A = fA;
    field_C = fC;
}

void RestTime_::Clear()
{
    field_0 = 0;
    field_2 = 0;
    field_4 = 0;
    field_6 = 0;
    field_8 = 0;
    field_A = 0;
    field_C = 0;
}

// C# get/set indexer: both accessors use the same switch, so one int& serves both.
int& RestTime_::operator[](int index)
{
    switch (index)
    {
        case 0:
            return field_0;
        case 1:
            return field_2;
        case 2:
            return field_4;
        case 3:
            return field_6;
        case 4:
            return field_8;
        case 5:
            return field_A;
        case 6:
            return field_C;
        default:
            throw NotSupportedException("NotSupportedException");
    }
}

}  // namespace Classes

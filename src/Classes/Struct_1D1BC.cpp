// Classes/Struct_1D1BC.cpp - port of Classes/Struct_1D1BC.cs
// (the indexers are defined inline in Struct_1D1BC.h)
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

Struct_1D1BC_::Struct_1D1BC_()
{
    field_7 = Array<int>(dataSize);
}

void Struct_1D1BC_::SetField_7(int value)
{
    for (int i = 0; i < dataSize; i++)
    {
        field_7[i] = value;
    }
}

}  // namespace Classes

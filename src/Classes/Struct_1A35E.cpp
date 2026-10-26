// Classes/Struct_1A35E.cpp - port of Classes/Struct_1A35E.cs
// (the indexers are defined inline in Struct_1A35E.h)
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

SubStruct_1A35E_::SubStruct_1A35E_(short f0, byte f2, byte f3)
{
    base_age = f0;
    dice_count = f2;
    dice_size = f3;
}

Struct_1A35E_::Struct_1A35E_()
{
    field_00 = Array<SubStruct_1A35E>(7);
}

Struct_1A35E_::Struct_1A35E_(Array<SubStruct_1A35E> values)
{
    field_00 = values;
}

}  // namespace Classes

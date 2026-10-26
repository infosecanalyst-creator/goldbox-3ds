// Classes/Struct_1ADF6.cpp - port of Classes/Struct_1ADF6.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

byte Struct_1ADF6_::byteArray_11(int index)
{
    switch (index)
    {
        case 0:
            return field_11;
        case 1:
            return field_12;
        case 2:
            return field_13;
        case 3:
            return field_14;
        case 4:
            return field_15;
        case 5:
            return field_16;
        default:
            throw ApplicationException("ApplicationException");
    }
}

}  // namespace Classes

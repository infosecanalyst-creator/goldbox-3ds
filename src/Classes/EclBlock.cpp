// Classes/EclBlock.cpp - port of Classes/EclBlock.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

EclBlock_::EclBlock_()
{
    data = Array<byte>(ecl_struct_size);
}

EclBlock_::EclBlock_(Array<byte> _data, int offset)
{
    data = Array<byte>(ecl_struct_size);

    Array<byte>::Copy(_data, data, ecl_struct_size);
}

void EclBlock_::Clear()
{
    Array<byte>::Clear(data, 0, ecl_struct_size);
}

// byte this[int index] { get; set; } -> inline operator[] in EclBlock.h
// (the get/set bodies had only commented-out Console.WriteLine traces:
//  //System.Console.WriteLine("     EclBlock[get] loc: {0,4:X} value {1,2:X}", loc, value);
//  //System.Console.WriteLine("     EclBlock[set] loc: {0,4:X} value: {1,4:X}", loc, value);)

void EclBlock_::SetData(Array<byte> dataArray, int dataOffset, int dataLength)
{
    Array<byte>::Copy(dataArray, dataOffset, this->data, 0, dataLength);
}

Array<byte> EclBlock_::ToByteArray()
{
    return data.Clone();
}

}  // namespace Classes

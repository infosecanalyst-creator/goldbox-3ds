// Classes/Struct_1B2CA.cpp - port of Classes/Struct_1B2CA.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

// 0x400
Struct_1B2CA_::Struct_1B2CA_()
{
    Clear();
}

Struct_1B2CA_::Struct_1B2CA_(Array<byte> data, int offset)
{
    Array<byte>::Copy(data, offset, m_data, 0, 0x400);
}

void Struct_1B2CA_::Clear()
{
    Array<byte>::Clear(m_data, 0, 0x400);
}

Array<byte> Struct_1B2CA_::ToByteArray()
{
    return m_data.Clone();
}

// indexer get
ushort Struct_1B2CA_::Get(int index)
{
    // simulate the 16 bit memory space.
    index &= 0xFFFF;

    return Sys::ArrayToUshort(m_data, index);
}

// indexer set
void Struct_1B2CA_::Set(int index, ushort value)
{
    index &= 0xFFFF;
    Sys::ShortToArray((short)value, m_data, index);
}

}  // namespace Classes

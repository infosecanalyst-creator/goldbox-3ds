// Classes/Affect.cpp - port of Classes/Affect.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Affect.
/// </summary>
Affect_::Affect_(Affects _type, ushort _minutes, byte _affect_data, bool _call_spell_jump_list)
{
    type = _type;
    minutes = _minutes;
    affect_data = _affect_data;
    callAffectTable = _call_spell_jump_list;
}

Affect_::Affect_(Array<byte> data, int offset)
{
    type = (Affects)data[offset + 0x0];
    minutes = Sys::ArrayToUshort(data, offset + 0x1);
    affect_data = data[offset + 0x3];
    callAffectTable = (data[offset + 0x4] != 0);
}

Affect Affect_::ShallowClone()
{
    // MemberwiseClone: every field is a value type, so a copy-construct is exact.
    Affect a = New<Affect_>(*this);
    return a;
}

Array<byte> Affect_::ToByteArray()
{
    Array<byte> data = Array<byte>(StructSize);

    DataIO::WriteObject(this, data);

    return data;
}

const std::vector<FieldDesc>& Affect_::DataFields()
{
    static const std::vector<FieldDesc> fields = {
        DATAIO_FIELD_DS(Affect_, type, 0x00, DataType::IByte),
        DATAIO_FIELD_DS(Affect_, minutes, 0x01, DataType::Word),
        DATAIO_FIELD_DS(Affect_, affect_data, 0x03, DataType::Byte),
        DATAIO_FIELD_DS(Affect_, callAffectTable, 0x04, DataType::Bool),
    };
    return fields;
}

}  // namespace Classes

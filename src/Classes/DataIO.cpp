// Classes/DataIO.cpp - port of Classes/DataIO.cs
//
// PORT-NOTE: C# reflects over [DataOffset] fields (Type.GetFields() +
// Attribute.GetCustomAttributes). Here each class supplies a FieldDesc table
// (C::DataFields(), see DataIO.h) in declaration order, and each FieldDesc plays
// the role of both the FieldInfo (`fInfo`) and the DataOffsetAttribute (`attr`).
#include "Classes/Classes.h"
#include "Logging/Logger.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

namespace {
// PORT-NOTE: C# casts like `(byte[])o` throw InvalidCastException when the field's
// real type does not match the attribute's DataType. We check the FieldKind and
// throw the same way instead of reinterpreting memory.
void CheckKind(const FieldDesc& fInfo, FieldKind expected)
{
    if (fInfo.kind != expected) throw Exception(String("InvalidCastException: ") + fInfo.name);
}

void CheckScalar(const FieldDesc& fInfo)
{
    if (fInfo.getInt == nullptr || fInfo.setInt == nullptr) throw Exception(String("InvalidCastException: ") + fInfo.name);
}
}  // namespace

// ---------------------------------------------------------------------------
// DataOffsetAttribute
// ---------------------------------------------------------------------------

// The constructor is called when the attribute is set.
DataOffsetAttribute_::DataOffsetAttribute_(int offset, DataType type)
{
    this->offset = offset;
    this->type = type;
    this->size = DefaultSize(type);
}

DataOffsetAttribute_::DataOffsetAttribute_(int offset, DataType type, int size)
{
    this->offset = offset;
    this->type = type;
    this->size = size;
}

int DataOffsetAttribute_::DefaultSize(DataType type)
{
    switch (type)
    {
        case DataType::Byte:
            return 1;
        case DataType::SByte:
            return 1;
        case DataType::IByte:
            return 1;
        case DataType::Bool:
            return 1;
        case DataType::Word:
            return 2;
        case DataType::SWord:
            return 2;
        case DataType::Int:
            return 4;
        default:
            throw NotImplementedException();
    }
}

// ---------------------------------------------------------------------------
// DataIO
// ---------------------------------------------------------------------------

ushort DataIO::GetObjectUShort(void* obj, const std::vector<FieldDesc>& fields, const String& objName,
                               Array<byte> data, int location)
{
    // Iterate through all the fields of the class.
    for (const FieldDesc& fInfo : fields)
    {
        const FieldDesc& doAttr = fInfo;
        if (doAttr.offset == location)
        {
            Logging::Logger::Debug("GetObjectUShort {0}.{1}", objName, fInfo.name);
            return GetObjectUshortValue(obj, fInfo, doAttr);
        }
    }

    ushort val = Sys::ArrayToUshort(data, location);
    Logging::Logger::Debug("GetObjectUShort {0} at {1,4:X} val: {2:X}", objName, location, val);
    return val;
}

void DataIO::SetObjectUShort(void* obj, const std::vector<FieldDesc>& fields, const String& objName,
                             Array<byte> data, int location, ushort value)
{
    // Iterate through all the fields of the class.
    for (const FieldDesc& fInfo : fields)
    {
        const FieldDesc& doAttr = fInfo;
        if (doAttr.offset == location)
        {
            Logging::Logger::Debug("SetObjectUShort {0}.{1}", objName, fInfo.name);
            SetObjectUshortValue(obj, fInfo, doAttr, value);
            return;
        }
    }

    Sys::ShortToArray((short)value, data, location);
    Logging::Logger::Debug("SetObjectUShort {0} at {1,4:X} val: {2:X}", objName, location, value);
}

ushort DataIO::GetObjectUshortValue(void* obj, const FieldDesc& fInfo, const FieldDesc& attr)
{
    switch (attr.type)
    {
        case DataType::SByte:
            CheckScalar(fInfo);
            return (ushort)(sbyte)fInfo.getInt(obj);
        case DataType::Byte:
            CheckScalar(fInfo);
            return (ushort)(byte)fInfo.getInt(obj);
        case DataType::SWord:
            CheckScalar(fInfo);
            return (ushort)(short)fInfo.getInt(obj);
        case DataType::Word:
            CheckScalar(fInfo);
            return (ushort)fInfo.getInt(obj);
        case DataType::Bool:
            CheckScalar(fInfo);
            return (fInfo.getInt(obj) != 0) ? (ushort)1 : (ushort)0;
        default:
            throw NotImplementedException();
            //return 0;
    }
}

void DataIO::SetObjectUshortValue(void* obj, const FieldDesc& fInfo, const FieldDesc& attr, ushort value)
{
    //object o = fInfo.GetValue(obj);
    switch (attr.type)
    {
        case DataType::SByte:
            CheckScalar(fInfo);
            fInfo.setInt(obj, (sbyte)value);
            break;
        case DataType::SWord:
            CheckScalar(fInfo);
            fInfo.setInt(obj, (short)value);
            break;
        case DataType::Word:
            CheckScalar(fInfo);
            fInfo.setInt(obj, value);
            break;
        case DataType::Byte:
            CheckScalar(fInfo);
            fInfo.setInt(obj, (byte)value);
            break;
        case DataType::Bool:
            CheckScalar(fInfo);
            fInfo.setInt(obj, value != 0);
            break;


        default:
            throw NotImplementedException();
    }
}

void DataIO::ReadObject(void* obj, const std::vector<FieldDesc>& fields, Array<byte> data, int offset)
{
    // Iterate through all the fields of the class.
    for (const FieldDesc& fInfo : fields)
    {
        // Iterate through all the Attributes for each Field.
        // (one table entry == one field carrying a DataOffset attribute)
        const FieldDesc& doAttr = fInfo;

        readData(obj, fInfo, doAttr, data, offset);
    }
}

void DataIO::WriteObject(void* obj, const std::vector<FieldDesc>& fields, Array<byte> data)
{
    // Iterate through all the fields of the class.
    for (const FieldDesc& fInfo : fields)
    {
        // Iterate through all the Attributes for each Field.
        // (one table entry == one field carrying a DataOffset attribute)
        const FieldDesc& doAttr = fInfo;

        writeData(obj, fInfo, doAttr, data);
    }
}

void DataIO::writeData(void* obj, const FieldDesc& fInfo, const FieldDesc& attr, Array<byte> data)
{
    IDataIO* idata = (fInfo.asDataIO != nullptr) ? fInfo.asDataIO(obj) : nullptr;

    if (idata != nullptr)
    {
        idata->Write(data, attr.offset);
    }
    else
    {
        switch (attr.type)
        {
            case DataType::SByte:
                CheckScalar(fInfo);
                data[attr.offset] = (byte)(sbyte)fInfo.getInt(obj);
                break;

            case DataType::IByte:
                CheckScalar(fInfo);
                data[attr.offset] = (byte)(int)fInfo.getInt(obj);
                break;

            case DataType::Byte:
                CheckScalar(fInfo);
                data[attr.offset] = (byte)fInfo.getInt(obj);
                break;

            case DataType::SWord:
                {
                    CheckScalar(fInfo);
                    short s = (short)fInfo.getInt(obj);
                    data[attr.offset] = (byte)(s & 0xff);
                    data[attr.offset + 1] = (byte)((s >> 8) & 0xff);
                }
                break;

            case DataType::Word:
                {
                    CheckScalar(fInfo);
                    ushort s = (ushort)fInfo.getInt(obj);
                    data[attr.offset] = (byte)(s & 0xff);
                    data[attr.offset + 1] = (byte)((s >> 8) & 0xff);
                }
                break;

            case DataType::Int:
                {
                    CheckScalar(fInfo);
                    int i = (int)fInfo.getInt(obj);
                    int offset = attr.offset;
                    data[offset++] = (byte)(i & 0xff);
                    data[offset++] = (byte)((i >> 8) & 0xff);
                    data[offset++] = (byte)((i >> 16) & 0xff);
                    data[offset++] = (byte)((i >> 24) & 0xff);
                }
                break;

            case DataType::Bool:
                CheckScalar(fInfo);
                data[attr.offset] = (fInfo.getInt(obj) != 0) ? (byte)1 : (byte)0;
                break;

            case DataType::ByteArray:
                {
                    CheckKind(fInfo, FieldKind::ByteArray);
                    Array<byte> values = fInfo.As<Array<byte>>(obj);

                    Array<byte>::Copy(values, 0, data, attr.offset, attr.size);
                }
                break;

            case DataType::ShortArray:
                {
                    CheckKind(fInfo, FieldKind::ShortArray);
                    Array<short> values = fInfo.As<Array<short>>(obj);
                    int offset = attr.offset;
                    for (int i = 0; i < attr.size; i++)
                    {
                        short s = values[i];
                        data[offset++] = (byte)(s & 0xff);
                        data[offset++] = (byte)((s >> 8) & 0xff);
                    }
                }
                break;

            case DataType::WordArray:
                {
                    CheckKind(fInfo, FieldKind::UShortArray);
                    Array<ushort> values = fInfo.As<Array<ushort>>(obj);
                    int offset = attr.offset;
                    for (int i = 0; i < attr.size; i++)
                    {
                        ushort s = values[i];
                        data[offset++] = (byte)(s & 0xff);
                        data[offset++] = (byte)((s >> 8) & 0xff);
                    }
                }
                break;

            case DataType::PString:
                {
                    CheckKind(fInfo, FieldKind::String);
                    byte b = (byte)attr.size;
                    String s = fInfo.As<String>(obj);
                    if (s.Length() < b)
                    {
                        b = (byte)s.Length();
                    }

                    data[attr.offset] = b;
                    for (int i = 0; i < b; i++)
                    {
                        data[attr.offset + i + 1] = (byte)s[i];
                    }
                }
                break;

            case DataType::Cust1Array:
                {
                    CheckKind(fInfo, FieldKind::StatValueArray);
                    int i = attr.offset;
                    Array<StatValue> sv = fInfo.As<Array<StatValue>>(obj);
                    for (StatValue v : sv)
                    {
                        data[i++] = (byte)v.cur;
                        data[i++] = (byte)v.full;
                    }
                }
                break;

            default:
                throw NotImplementedException();
        }
    }
}

void DataIO::readData(void* obj, const FieldDesc& fInfo, const FieldDesc& attr, Array<byte> data, int offset)
{
    offset += attr.offset;
    IDataIO* idata = (fInfo.asDataIO != nullptr) ? fInfo.asDataIO(obj) : nullptr;

    if (idata != nullptr)
    {
        idata->Read(data, offset);
    }
    else
    {
        switch (attr.type)
        {
            case DataType::SByte:
                CheckScalar(fInfo);
                fInfo.setInt(obj, (sbyte)data[offset]);
                break;

            case DataType::IByte:
                CheckScalar(fInfo);
                fInfo.setInt(obj, data[offset]);
                break;

            case DataType::Byte:
                CheckScalar(fInfo);
                fInfo.setInt(obj, data[offset]);
                break;

            case DataType::SWord:
                {
                    CheckScalar(fInfo);
                    short s = (short)(data[offset] + (data[offset + 1] << 8));
                    fInfo.setInt(obj, s);
                }
                break;

            case DataType::Word:
                {
                    CheckScalar(fInfo);
                    ushort w = (ushort)(data[offset] + (data[offset + 1] << 8));
                    fInfo.setInt(obj, w);
                }
                break;

            case DataType::Int:
                {
                    CheckScalar(fInfo);
                    int i = data[offset] + (data[offset + 1] << 8) + (data[offset + 2] << 16) + (data[offset + 3] << 24);
                    fInfo.setInt(obj, i);
                }
                break;

            case DataType::Bool:
                CheckScalar(fInfo);
                fInfo.setInt(obj, (data[offset] != 0));
                break;

            case DataType::ByteArray:
                {
                    CheckKind(fInfo, FieldKind::ByteArray);
                    Array<byte> a = Array<byte>(attr.size);

                    Array<byte>::Copy(data, offset, a, 0, attr.size);
                    fInfo.As<Array<byte>>(obj) = a;
                }
                break;

            case DataType::ShortArray:
                {
                    CheckKind(fInfo, FieldKind::ShortArray);
                    Array<short> a = Array<short>(attr.size);
                    for (int i = 0; i < attr.size; i++)
                    {
                        a[i] = (short)(data[offset + (i * 2)] + (data[offset + 1 + (i * 2)] << 8));
                    }
                    fInfo.As<Array<short>>(obj) = a;
                }
                break;

            case DataType::WordArray:
                {
                    CheckKind(fInfo, FieldKind::UShortArray);
                    Array<ushort> a = Array<ushort>(attr.size);
                    for (int i = 0; i < attr.size; i++)
                    {
                        a[i] = (ushort)(data[offset + (i * 2)] + (data[offset + 1 + (i * 2)] << 8));
                    }
                    fInfo.As<Array<ushort>>(obj) = a;
                }
                break;

            case DataType::PString:
                {
                    CheckKind(fInfo, FieldKind::String);
                    fInfo.As<String>(obj) = Sys::ArrayToString(data, offset, attr.size);
                }
                break;

            case DataType::Cust1Array:
                {
                    CheckKind(fInfo, FieldKind::StatValueArray);
                    Array<StatValue> sv = Array<StatValue>(attr.size);
                    for (int i = 0; i < attr.size; i++)
                    {
                        sv[i].cur = data[offset++];
                        sv[i].full = data[offset++];
                    }
                    fInfo.As<Array<StatValue>>(obj) = sv;
                }
                break;

            default:
                throw NotImplementedException();
        }
    }
}

}  // namespace Classes

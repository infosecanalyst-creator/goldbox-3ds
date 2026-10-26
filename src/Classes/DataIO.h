// Classes/DataIO.h - port of Classes/DataIO.cs.
//
// C# DataIO reflects over public fields tagged [DataOffset(offset, DataType.X[, size])].
// C++ has no reflection, so every class that carries such fields (Player, Affect,
// Area1, Area2) declares
//
//     static const std::vector<FieldDesc>& DataFields();
//
// and defines it in its .cpp as a function-local static table built with
// DATAIO_FIELD, listing the tagged fields IN DECLARATION ORDER (= C# GetFields()
// order, which ReadObject/WriteObject/GetObjectUShort/SetObjectUShort rely on):
//
//     const std::vector<FieldDesc>& Affect_::DataFields() {
//         static const std::vector<FieldDesc> fields = {
//             DATAIO_FIELD(Affect_, type, 0x00, DataType::IByte, 1),
//             DATAIO_FIELD(Affect_, minutes, 0x01, DataType::Word, 2),
//             ...
//         };
//         return fields;
//     }
//
// The size argument is the attribute's explicit size, or DataOffsetDefaultSize(type)
// when the C# attribute had none (DATAIO_FIELD_DS does that for you).
//
// The DataIO functions take the object as `C* obj` (call them as
// `DataIO::ReadObject(this, data, offset)`), look up C::DataFields() and iterate.
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

#include <type_traits>
#include <vector>

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

enum class DataType : int {
    PString,
    Byte,
    SByte,
    IByte, //a byte that needs to be cast to int first.
    Word,
    SWord,
    Int,
    SInt,
    Bool,
    ByteArray,
    ShortArray,
    WordArray,
    Cust1Array,
    CustSaveLoad,
};

struct IDataIO {
    virtual ~IDataIO() = default;
    virtual void Write(Array<byte> data, int offset) = 0;
    virtual void Read(Array<byte> data, int offset) = 0;
};

class DataOffsetAttribute_ {
public:
    // The constructor is called when the attribute is set.
    DataOffsetAttribute_(int offset, DataType type);
    DataOffsetAttribute_(int offset, DataType type, int size);

private:
    int DefaultSize(DataType type);

    // Keep a variable internally ...
protected:
    int offset = 0;

public:
    int Offset() const { return offset; }

protected:
    int size = 0;

public:
    int Size() const { return size; }

protected:
    DataType type = (DataType)0;

public:
    DataType Type() const { return type; }
};

// Same switch as DataOffsetAttribute.DefaultSize (throws NotImplementedException
// for types that need an explicit size).
inline int DataOffsetDefaultSize(DataType type) {
    switch (type) {
        case DataType::Byte: return 1;
        case DataType::SByte: return 1;
        case DataType::IByte: return 1;
        case DataType::Bool: return 1;
        case DataType::Word: return 2;
        case DataType::SWord: return 2;
        case DataType::Int: return 4;
        default: throw NotImplementedException();
    }
}

// ---------------------------------------------------------------------------
// Field descriptor (replacement for FieldInfo + DataOffsetAttribute).
// ---------------------------------------------------------------------------

// What the C++ member actually is (the attribute's DataType says how it is
// stored; the kind says how to get at it).
enum class FieldKind : int {
    Byte,           // byte
    SByte,          // sbyte
    Short,          // short
    UShort,         // ushort
    Int,            // int
    Bool,           // bool
    Enum,           // any enum (getInt/setInt go through the underlying type)
    String,         // String
    ByteArray,      // Array<byte>
    ShortArray,     // Array<short>
    UShortArray,    // Array<ushort>
    StatValueArray, // Array<StatValue>
    DataIORef,      // Ref<X_> where X_ implements IDataIO (PlayerStats, MoneySet)
    Other,
};

struct FieldDesc {
    const char* name;   // fInfo.Name
    int offset;         // attr.Offset
    DataType type;      // attr.Type
    int size;           // attr.Size
    FieldKind kind;

    // Address of the member inside `obj` (obj is the C++ object, e.g. Player_*).
    void* (*addr)(void* obj);
    // Scalar kinds (Byte..Enum): read the value widened to int64 (sign-preserving
    // for signed types / signed enums), and store a value with a C-style cast to
    // the field type (= what FieldInfo.SetValue ends up doing). nullptr otherwise.
    int64_t (*getInt)(const void* obj);
    void (*setInt)(void* obj, int64_t value);
    // C# `fInfo.GetValue(obj) as IDataIO`: non-null only for DataIORef fields
    // whose reference is currently non-null.
    IDataIO* (*asDataIO)(void* obj);

    // Typed access for the non-scalar kinds: desc.As<Array<byte>>(obj) = a;
    template <class T>
    T& As(void* obj) const { return *static_cast<T*>(addr(obj)); }
};

namespace dataio_detail {

template <class M>
struct MemberTraits;
template <class C, class T>
struct MemberTraits<T C::*> {
    using Class = C;
    using Type = T;
};

template <class T>
struct IsRef : std::false_type {};
template <class T>
struct IsRef<Ref<T>> : std::true_type {};

template <class T>
constexpr FieldKind KindOf() {
    if constexpr (std::is_same<T, bool>::value) return FieldKind::Bool;
    else if constexpr (std::is_same<T, byte>::value) return FieldKind::Byte;
    else if constexpr (std::is_same<T, sbyte>::value) return FieldKind::SByte;
    else if constexpr (std::is_same<T, short>::value) return FieldKind::Short;
    else if constexpr (std::is_same<T, ushort>::value) return FieldKind::UShort;
    else if constexpr (std::is_same<T, int>::value) return FieldKind::Int;
    else if constexpr (std::is_enum<T>::value) return FieldKind::Enum;
    else if constexpr (std::is_same<T, String>::value) return FieldKind::String;
    else if constexpr (std::is_same<T, Array<byte>>::value) return FieldKind::ByteArray;
    else if constexpr (std::is_same<T, Array<short>>::value) return FieldKind::ShortArray;
    else if constexpr (std::is_same<T, Array<ushort>>::value) return FieldKind::UShortArray;
    else if constexpr (std::is_same<T, Array<StatValue>>::value) return FieldKind::StatValueArray;
    else if constexpr (IsRef<T>::value) {
        if constexpr (std::is_base_of<IDataIO, typename T::element_type>::value) return FieldKind::DataIORef;
        else return FieldKind::Other;
    } else return FieldKind::Other;
}

template <auto M>
void* Addr(void* obj) {
    using C = typename MemberTraits<decltype(M)>::Class;
    return &(static_cast<C*>(obj)->*M);
}

template <auto M>
int64_t GetInt(const void* obj) {
    using C = typename MemberTraits<decltype(M)>::Class;
    using T = typename MemberTraits<decltype(M)>::Type;
    const T& v = static_cast<const C*>(obj)->*M;
    if constexpr (std::is_enum<T>::value) return (int64_t)(std::underlying_type_t<T>)v;
    else return (int64_t)v;
}

template <auto M>
void SetInt(void* obj, int64_t value) {
    using C = typename MemberTraits<decltype(M)>::Class;
    using T = typename MemberTraits<decltype(M)>::Type;
    if constexpr (std::is_same<T, bool>::value) static_cast<C*>(obj)->*M = (value != 0);
    else if constexpr (std::is_enum<T>::value) static_cast<C*>(obj)->*M = (T)(std::underlying_type_t<T>)value;
    else static_cast<C*>(obj)->*M = (T)value;
}

template <auto M>
IDataIO* AsDataIO(void* obj) {
    using C = typename MemberTraits<decltype(M)>::Class;
    return static_cast<IDataIO*>((static_cast<C*>(obj)->*M).get());
}

template <auto M>
FieldDesc Make(const char* name, int offset, DataType type, int size) {
    using T = typename MemberTraits<decltype(M)>::Type;
    constexpr FieldKind kind = KindOf<T>();
    constexpr bool scalar = kind == FieldKind::Byte || kind == FieldKind::SByte || kind == FieldKind::Short ||
                            kind == FieldKind::UShort || kind == FieldKind::Int || kind == FieldKind::Bool ||
                            kind == FieldKind::Enum;
    FieldDesc d{name, offset, type, size, kind, &Addr<M>, nullptr, nullptr, nullptr};
    if constexpr (scalar) {
        d.getInt = &GetInt<M>;
        d.setInt = &SetInt<M>;
    }
    if constexpr (kind == FieldKind::DataIORef) d.asDataIO = &AsDataIO<M>;
    return d;
}

// What C# prints for `obj` in the Logger.Debug lines (obj.ToString()):
// Player overrides ToString(); the others print their full type name.
template <class C>
struct TypeName;
template <> struct TypeName<Player_> { static constexpr const char* value = "Classes.Player"; };
template <> struct TypeName<Affect_> { static constexpr const char* value = "Classes.Affect"; };
template <> struct TypeName<Area1_> { static constexpr const char* value = "Classes.Area1"; };
template <> struct TypeName<Area2_> { static constexpr const char* value = "Classes.Area2"; };

template <class C, class = void>
struct HasToString : std::false_type {};
template <class C>
struct HasToString<C, std::void_t<decltype(std::declval<const C&>().ToString())>> : std::true_type {};

template <class C>
String ObjName(const C* obj) {
    if constexpr (HasToString<C>::value) return obj->ToString();
    else return TypeName<C>::value;
}

}  // namespace dataio_detail

// DATAIO_FIELD(Player_, name, 0x00, DataType::PString, 15)
#define DATAIO_FIELD(Class_, member, offset, type, size) \
    ::Classes::dataio_detail::Make<&Class_::member>(#member, (offset), (type), (size))
// Attribute without explicit size: [DataOffset(0x72, DataType.Byte)]
#define DATAIO_FIELD_DS(Class_, member, offset, type) \
    ::Classes::dataio_detail::Make<&Class_::member>(#member, (offset), (type), ::Classes::DataOffsetDefaultSize(type))

// ---------------------------------------------------------------------------
// DataIO (static-only). The `void*` + table overloads hold the logic (bodies in
// DataIO.cpp); the template overloads are what translated code calls.
// ---------------------------------------------------------------------------
struct DataIO {
    static ushort GetObjectUShort(void* obj, const std::vector<FieldDesc>& fields, const String& objName,
                                  Array<byte> data, int location);
    static void SetObjectUShort(void* obj, const std::vector<FieldDesc>& fields, const String& objName,
                                Array<byte> data, int location, ushort value);
    static void ReadObject(void* obj, const std::vector<FieldDesc>& fields, Array<byte> data, int offset);
    static void WriteObject(void* obj, const std::vector<FieldDesc>& fields, Array<byte> data);

    template <class C>
    static ushort GetObjectUShort(C* obj, Array<byte> data, int location) {
        return GetObjectUShort((void*)obj, C::DataFields(), dataio_detail::ObjName(obj), data, location);
    }
    template <class C>
    static void SetObjectUShort(C* obj, Array<byte> data, int location, ushort value) {
        SetObjectUShort((void*)obj, C::DataFields(), dataio_detail::ObjName(obj), data, location, value);
    }
    template <class C>
    static void ReadObject(C* obj, Array<byte> data, int offset) {
        ReadObject((void*)obj, C::DataFields(), data, offset);
    }
    template <class C>
    static void WriteObject(C* obj, Array<byte> data) {
        WriteObject((void*)obj, C::DataFields(), data);
    }

private:
    static ushort GetObjectUshortValue(void* obj, const FieldDesc& fInfo, const FieldDesc& attr);
    static void SetObjectUshortValue(void* obj, const FieldDesc& fInfo, const FieldDesc& attr, ushort value);
    static void writeData(void* obj, const FieldDesc& fInfo, const FieldDesc& attr, Array<byte> data);
    static void readData(void* obj, const FieldDesc& fInfo, const FieldDesc& attr, Array<byte> data, int offset);
};

}  // namespace Classes

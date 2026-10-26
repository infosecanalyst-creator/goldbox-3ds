// Classes/Struct_1B2CA.h - port of Classes/Struct_1B2CA.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

// 0x400
class Struct_1B2CA_ {
    Array<byte> m_data = Array<byte>(0x400);

public:
    Struct_1B2CA_();

    Struct_1B2CA_(Array<byte> data, int offset);

    void Clear();

    Array<byte> ToByteArray();

    // C# get/set indexer `ushort this[int index]` over a little-endian byte array:
    // no addressable ushort exists, so the indexer returns a proxy that reads with
    // Get() and writes with Set() (supports `x = s[i]`, `s[i] = v`, `s[i] != 0`).
    class Indexer {
        Struct_1B2CA_* owner;
        int index;

    public:
        Indexer(Struct_1B2CA_* o, int i) : owner(o), index(i) {}
        operator ushort() const { return owner->Get(index); }
        Indexer& operator=(ushort value) { owner->Set(index, value); return *this; }
        Indexer& operator=(const Indexer& other) { owner->Set(index, (ushort)other); return *this; }
    };
    Indexer operator[](int index) { return Indexer(this, index); }

    ushort Get(int index);              // indexer get
    void Set(int index, ushort value);  // indexer set
};

}  // namespace Classes

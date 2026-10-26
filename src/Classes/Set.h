// Classes/Set.h - port of Classes/Set.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Set.
/// </summary>
class Set_ {
    Array<byte> bits;
    static constexpr uint arrayLen = 0x20;
    static constexpr int shift = 3;
    static constexpr int mask = 7;

public:
    Set_();

    Set_(ushort arg_0, Array<byte> arg_2);

    // C#: Set(params int[] toset). PORT-NOTE: call sites write
    // New<Set_>(Array<int>{48, 49, ...}).
    explicit Set_(Array<int> toset);

    void DumpSet();

    void Clear();

    // PORT-NOTE: C# `static Set operator +(Set lhs, byte rhs)` mutates lhs and returns it.
    // Not used by the engine; provided as a named static.
    static Set Add(Set lhs, byte rhs);

    void SetRange(byte arg_0, byte arg_2);

private:
    void SetBit(int i);

public:
    bool MemberOf(int bit);
};

}  // namespace Classes

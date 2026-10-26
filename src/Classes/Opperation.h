// Classes/Opperation.h - port of Classes/Opperation.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

using VmGetMemoryValue = std::function<ushort(ushort loc)>;

class Opperation_ {
    bool codeSet = false;
    bool lowSet = false;
    bool highSet = false;

    byte code = 0;
    byte low = 0;
    byte high = 0;

    ushort word = 0;

public:
    VmGetMemoryValue getMemoryValue;

    void Clear();

    // C# property Code { get; set; } (set throws InvalidOperationException if already set,
    // get throws if not set). Per TRANSLATION.md: setter -> set_Code; getter -> Code().
    void set_Code(byte value);
    byte Code() const;

    // C# get-only property Word
    ushort Word() const;

    void set_Low(byte value);
    byte Low() const;

    void set_High(byte value);
    byte High() const;

    ushort GetCmdValue();
};

}  // namespace Classes

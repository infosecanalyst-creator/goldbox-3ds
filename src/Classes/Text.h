// Classes/Text.h - port of Classes/Text.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "cs/io.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for Text.
/// </summary>
class Text_ {
public:
    ushort field_2 = 0;

    byte field_30 = 0;

    // PORT-NOTE: C# `object field_80` (never used) -> shared_ptr<void>.
    std::shared_ptr<void> field_80;

    IO::TextReader reader;
    IO::TextWriter writer;

    Text_();

    enum class AssignType : int {
        Read,
        Write
    };

private:
    AssignType _type = (AssignType)0;

public:
    void Assign(String s, AssignType type);
};

}  // namespace Classes

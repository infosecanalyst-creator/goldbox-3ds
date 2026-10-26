// Classes/Text.cpp - port of Classes/Text.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

Text_::Text_()
{
    reader = New<IO::TextReader_>(stdin);   // System.Console.In
    writer = New<IO::TextWriter_>(stdout);  // System.Console.Out
}

void Text_::Assign(String s, AssignType type)
{
    _type = type;

    if (s != String::Empty)
    {
        if (type == AssignType::Read)
        {
            reader = New<IO::StreamReader_>(s);
        }
        else
        {
            writer = New<IO::StreamWriter_>(s);
        }
    }

    field_2 = 0xD7B0;
}

}  // namespace Classes

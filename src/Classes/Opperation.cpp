// Classes/Opperation.cpp - port of Classes/Opperation.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

void Opperation_::Clear()
{
    codeSet = false;
    lowSet = false;
    highSet = false;
}

// property Code { set; get; }
void Opperation_::set_Code(byte value)
{
    if (codeSet == false)
    {
        code = value;
        codeSet = true;
    }
    else throw InvalidOperationException("InvalidOperationException");
}

byte Opperation_::Code() const
{
    if (codeSet)
        return code;
    else throw InvalidOperationException("InvalidOperationException");
}

// property Word { get; }
ushort Opperation_::Word() const
{
    if (highSet)
        return word;
    else throw InvalidOperationException("InvalidOperationException");
}

// property Low { set; get; }
void Opperation_::set_Low(byte value)
{
    if (lowSet == false)
    {
        low = value;
        lowSet = true;
    }
    else throw InvalidOperationException("InvalidOperationException");
}

byte Opperation_::Low() const
{
    if (lowSet)
        return low;
    else throw InvalidOperationException("InvalidOperationException");
}

// property High { set; get; }
void Opperation_::set_High(byte value)
{
    if (highSet == false)
    {
        high = value;
        highSet = true;
        word = (ushort)(low + (high << 8));
    }
    else throw InvalidOperationException("InvalidOperationException");
}

byte Opperation_::High() const
{
    if (highSet)
        return high;
    else throw InvalidOperationException("InvalidOperationException");
}

ushort Opperation_::GetCmdValue()
{
    if (codeSet)
    {
        //System.Console.WriteLine("  GetCmdValue: code: {0:X}", code);

        switch (code)
        {
            case 0x00:
                if (lowSet)
                    return low;
                else throw InvalidOperationException("InvalidOperationException");

            case 0x01:
            case 0x03:
            case 0x80:
                if (highSet)
                    return getMemoryValue(word);
                else throw InvalidOperationException("InvalidOperationException");

            case 0x02:
            case 0x81:
                if (highSet)
                    return word;
                else throw InvalidOperationException("InvalidOperationException");

            default:
                throw InvalidOperationException("InvalidOperationException");
            //return 0;
        }
    }
    else throw InvalidOperationException("InvalidOperationException");
}

}  // namespace Classes

// Classes/Set.cpp - port of Classes/Set.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

Set_::Set_()
{
    bits = Array<byte>(arrayLen);
}

Set_::Set_(ushort arg_0, Array<byte> arg_2)
{
    bits = Array<byte>(arrayLen);

    int indA = arg_0 >> 8;
    int indB = arg_0 & 0x00ff;

    for (int i = 0; i < indA; i++)
    {
        bits[i] = 0;
    }

    for (int i = 0; i < indB; i++)
    {
        bits[i + indA] = arg_2[i];
    }

    for (int i = indA + indB; i < 20; i++)
    {
        bits[i] = 0;
    }

    DumpSet();
    int z = 0;
}

Set_::Set_(Array<int> toset)
{
    bits = Array<byte>(arrayLen);

    for (auto b : toset)
    {
        SetBit(b);
    }
}

void Set_::DumpSet()
{
    String sb;

    for (int i = 0; i < 256; i++)
    {
        if (MemberOf(i))
        {
            sb += Format("{0}, ", i);
        }
    }
    auto s = sb;
}

void Set_::Clear()
{
    for (int i = 0; i < (int)arrayLen; i++)
    {
        bits[i] = 0;
    }
}

Set Set_::Add(Set lhs, byte rhs)
{
    lhs->bits[rhs >> shift] |= (byte)(1 << (rhs & mask));

    return lhs;
}

void Set_::SetRange(byte arg_0, byte arg_2)
{
    //public static void Set__operator+=( byte arg_0, byte arg_2, int arg_4 )
    //Set::operator+=(Byte,Byte)
    //original left the Set object on the stack.

    // PORT-NOTE: byte loop variable: if arg_0 == 255 this never terminates, in C# too.
    for (byte i = arg_2; i <= arg_0; i++)
    {
        bits[i >> shift] |= (byte)(1 << (i & mask));
    }
}

void Set_::SetBit(int i)
{
    bits[i >> shift] |= (byte)(1 << (i & mask));
}

bool Set_::MemberOf(int bit)
{
    int lhs = bits[bit >> shift];
    int rhs = 1 << (bit & mask);

    return ((lhs & rhs) != 0);
}

}  // namespace Classes

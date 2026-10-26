// Classes/Sys.cpp - port of Classes/Sys.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

int Sys::ArrayToInt(Array<byte> data, int offset)
{
    int i = data[offset + 0] + (data[offset + 1] << 8) + (data[offset + 2] << 16) + (data[offset + 3] << 24);
    return i;
}

uint Sys::ArrayToUint(Array<byte> data, int offset)
{
    uint i = (uint)(data[offset + 0] + (data[offset + 1] << 8) + (data[offset + 2] << 16) + (data[offset + 3] << 24));
    return i;
}

short Sys::ArrayToShort(Array<byte> data, int offset)
{
    short i = (short)(data[offset + 0] + (data[offset + 1] << 8));
    return i;
}

ushort Sys::ArrayToUshort(Array<byte> data, int offset)
{
    ushort i = (ushort)(data[offset + 0] + (data[offset + 1] << 8));
    return i;
}

void Sys::ShortToArray(short value, Array<byte> data, int offset)
{
    data[offset + 0] = (byte)(value & 0x00ff);
    data[offset + 1] = (byte)((value >> 8) & 0x00ff);
}

/// <summary>
/// Converts a Pascal string array to a C# string.
/// </summary>
String Sys::ArrayToString(Array<byte> data, int offset, int maxLen)
{
    int len = data[offset];
    len = Math::Min(len, maxLen);

    String sb; // StringBuilder sb = new StringBuilder(len);

    for (int i = 1; i <= len; i++)
    {
        // PORT-NOTE: C# char is 16-bit unsigned, so (char)byte is never negative and
        // `c > 0` only skips NULs. C++ char may be signed: test the byte instead.
        byte b = data[i + offset];
        char c = (char)b;
        if (b > 0)
        {
            sb.push_back(c);
        }
    }

    return sb;
}

/// <summary>
/// Converts a C# string to a Pascal string array
/// </summary>
void Sys::StringToArray(Array<byte> data, int offset, int length, String input)
{
    data[offset] = (byte)length;
    for (int i = 1; i <= length; i++)
    {
        if (i <= input.Length())
        {
            data[offset + i] = (byte)input[i - 1];
        }
        else
        {
            data[offset + i] = 0;
        }
    }
}

String Sys::ArrayToString(Array<char> data, int offset, int length)
{
    // PORT-NOTE: C# char is unsigned 16-bit; read the length char as unsigned.
    length = Math::Min((int)(unsigned char)data[offset], length);

    String sb; // StringBuilder sb = new StringBuilder(length);
    for (int i = 1; i < length; i++)
    {
        char c = data[i + offset];
        sb.push_back(c);
    }

    return sb;
}

/// <summary>
/// returns array of strings based on an array of Pascal strings.
/// </summary>
/// <param name="data"></param>
/// <param name="offset"></param>
/// <param name="length"></param>
/// <returns></returns>
Array<String> Sys::ArrayToStrings(Array<byte> data, int offset, int length, int stringWidth)
{
    List<String> strs = List<String>();

    for (int i = 0; i < length; i += stringWidth)
    {
        strs.Add(Sys::ArrayToString(data, i + offset, stringWidth));
    }

    return strs.ToArray();
}

int Sys::WrapMinMax(int val, int min, int max)
{
    if (val > max)
    {
        val = min;
    }
    else if (val < min)
    {
        val = max;
    }
    return val;
}

}  // namespace Classes

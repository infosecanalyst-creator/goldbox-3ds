// Classes/Sys.h - port of Classes/Sys.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

struct Sys {
private:
    Sys() = delete;

public:
    static int ArrayToInt(Array<byte> data, int offset);

    static uint ArrayToUint(Array<byte> data, int offset);

    static short ArrayToShort(Array<byte> data, int offset);

    static ushort ArrayToUshort(Array<byte> data, int offset);

    static void ShortToArray(short value, Array<byte> data, int offset);

    /// <summary>
    /// Converts a Pascal string array to a C# string.
    /// </summary>
    static String ArrayToString(Array<byte> data, int offset, int maxLen);

    /// <summary>
    /// Converts a C# string to a Pascal string array
    /// </summary>
    static void StringToArray(Array<byte> data, int offset, int length, String input);

    static String ArrayToString(Array<char> data, int offset, int length);

    /// <summary>
    /// returns array of strings based on an array of Pascal strings.
    /// </summary>
    static Array<String> ArrayToStrings(Array<byte> data, int offset, int length, int stringWidth);

    static int WrapMinMax(int val, int min, int max);
};

}  // namespace Classes

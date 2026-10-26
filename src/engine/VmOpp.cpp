// engine/VmOpp.cpp - translation of coab engine/VmOpp.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

// static class VmLog: Write / WriteLine are variadic templates defined inline in
// engine/engine.h (C# `params object[] args`).

MemLoc_::MemLoc_(ushort _loc)
{
    loc = _loc;
}

String MemLoc_::ToString() const
{
    return Format("0x{0:X}", loc);
}

}  // namespace engine

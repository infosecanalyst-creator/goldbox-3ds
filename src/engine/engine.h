// engine/engine.h - declarations for every class in coab's `engine` assembly
// (engine/*.cs), translated per docs/TRANSLATION.md. Each engine/ovrNNN.cpp /
// segNNN.cpp defines the methods declared here. Generated from the C# sources
// (member order preserved) and then hand-edited where marked PORT-NOTE.
// See src/engine/ENGINE_NOTES.md.
//
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#pragma once

#include "cs/cs.h"
#include "cs/io.h"
#include "Logging/Logger.h"
#include "Classes/Classes.h"

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace engine {
using namespace cs;
using namespace cs::enum_ops;
using namespace Classes;
using namespace Classes::Combat;
using namespace Classes::DaxFiles;

// ---------------------------------------------------------------------------
// Array4<T>: C# T[,,,] (only ovr011.unk_1AB1C). Same conventions as cs::Array3:
// reference semantics, default == null, index as a(i, j, k, l).
// PORT-NOTE: engine-local because cs.h has no 4-D array.
// ---------------------------------------------------------------------------
template <class T>
class Array4 {
    std::shared_ptr<std::vector<T>> p;
    int d0 = 0, d1 = 0, d2 = 0, d3 = 0;

public:
    Array4() = default;
    Array4(std::nullptr_t) {}
    Array4(int n0, int n1, int n2, int n3)
        : p(std::make_shared<std::vector<T>>((std::size_t)(n0 * n1 * n2 * n3))), d0(n0), d1(n1), d2(n2), d3(n3) {}
    bool IsNull() const { return !p; }
    friend bool operator==(const Array4& a, std::nullptr_t) { return !a.p; }
    friend bool operator!=(const Array4& a, std::nullptr_t) { return (bool)a.p; }
    int GetLength(int dim) const { return dim == 0 ? d0 : (dim == 1 ? d1 : (dim == 2 ? d2 : d3)); }
    int Length() const { return d0 * d1 * d2 * d3; }
    T& operator()(int i, int j, int k, int l) const {
        if (!p) throw NullReferenceException("Array4");
        if (i < 0 || i >= d0 || j < 0 || j >= d1 || k < 0 || k >= d2 || l < 0 || l >= d3) throw IndexOutOfRangeException();
        return (*p)[(std::size_t)(((i * d1 + j) * d2 + k) * d3 + l)];
    }
};

// ---------------------------------------------------------------------------
// C# `object` appears only as the 2nd parameter of affectDelegate-shaped methods
// (Effect, object, Player) in ovr013/ovr014/ovr023. It must match the parameter
// type of Classes::affectDelegate (Gbl.h). PORT-NOTE: `(Affect)param` ->
// `std::static_pointer_cast<Affect_>(param)`, `(Item)param` likewise; passing an
// Affect/Item/nullptr converts implicitly.
// ---------------------------------------------------------------------------
using object = std::shared_ptr<void>;

// ---------------------------------------------------------------------------
// Engine-defined top-level types (hoisted ahead of the ovr/seg structs).
// ---------------------------------------------------------------------------

// ovr020.cs
enum class SpellLoc : int {
    memory = 0,
    grimoire = 1,
    scroll = 2,
    scrolls = 3,
    choose = 4,
    memorize = 5,
    scribe = 6
};

// ovr024.cs
enum class CheckType : int {
    None = 0,
    Visibility = 1,
    Type_2 = 2,
    Type_3 = 3,
    SpecialAttacks = 4,
    Type_5 = 5,
    PreDamage = 6,
    PlayerRestrained = 7,
    Type_8 = 8,
    MagicResistance = 9,
    Type_10 = 10,
    Type_11 = 11,
    SavingThrow = 12,
    Death = 13,
    Type_14 = 14,
    Type_15 = 15,
    Type_16 = 16,
    Morale = 17,
    Movement = 18,
    Type_19 = 19,
    FireShield = 20,
    Confusion = 21,
    Type_22 = 22,
    Type_23 = 23
};

// VmOpp.cs
class MemLoc_ {
public:
    ushort loc = 0;
    MemLoc_(ushort _loc);
    String ToString() const;
};
using MemLoc = cs::Ref<MemLoc_>;

// ovr003.cs
class CmdItem_ {
public:
    using CmdDelegate = std::function<void()>;
    int size = 0;
    String name;
    CmdDelegate cmd;
    CmdItem_(int Size, String Name, CmdDelegate Cmd);
    void Run();
    String Name();
    void Skip();
};
using CmdItem = cs::Ref<CmdItem_>;

// VmOpp.cs: static class VmLog. `params object[] args` forwarded to Logger -> variadic
// templates, so they must be defined here (bodies are a verbatim translation).
struct VmLog {
    template <class... A>
    static void Write(const String& fmt, const A&... args) {
        if (gbl::printCommands == true) {
            Logging::Logger::DebugWrite(fmt, args...);
        }
    }

    template <class... A>
    static void WriteLine(const String& fmt, const A&... args) {
        if (gbl::printCommands == true) {
            Logging::Logger::Debug(fmt, args...);
        }
    }
};

#include "engine/decl/ovr002.h"
#include "engine/decl/ovr003.h"
#include "engine/decl/ovr004.h"
#include "engine/decl/ovr005.h"
#include "engine/decl/ovr006.h"
#include "engine/decl/ovr007.h"
#include "engine/decl/ovr008.h"
#include "engine/decl/ovr009.h"
#include "engine/decl/ovr010.h"
#include "engine/decl/ovr011.h"
#include "engine/decl/ovr013.h"
#include "engine/decl/ovr014.h"
#include "engine/decl/ovr015.h"
#include "engine/decl/ovr016.h"
#include "engine/decl/ovr017.h"
#include "engine/decl/ovr018.h"
#include "engine/decl/ovr019.h"
#include "engine/decl/ovr020.h"
#include "engine/decl/ovr021.h"
#include "engine/decl/ovr022.h"
#include "engine/decl/ovr023.h"
#include "engine/decl/ovr024.h"
#include "engine/decl/ovr025.h"
#include "engine/decl/ovr026.h"
#include "engine/decl/ovr027.h"
#include "engine/decl/ovr028.h"
#include "engine/decl/ovr029.h"
#include "engine/decl/ovr030.h"
#include "engine/decl/ovr031.h"
#include "engine/decl/ovr032.h"
#include "engine/decl/ovr033.h"
#include "engine/decl/ovr034.h"
#include "engine/decl/ovr038.h"
#include "engine/decl/seg001.h"
#include "engine/decl/seg037.h"
#include "engine/decl/seg040.h"
#include "engine/decl/seg041.h"
#include "engine/decl/seg042.h"
#include "engine/decl/seg043.h"
#include "engine/decl/seg044.h"
#include "engine/decl/seg049.h"
#include "engine/decl/seg051.h"

}  // namespace engine

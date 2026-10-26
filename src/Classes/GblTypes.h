// Classes/GblTypes.h - the free-standing types declared at the top of Classes/Gbl.cs
// (enums, delegates, MenuColorSet, Point). Split out of Gbl.h so other headers
// can use them without pulling in class gbl (which itself needs nearly every
// other Classes header for its static initialisers). Gbl.h includes this file.
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

#include <functional>

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

enum class QuickFight : int {
    False = 0,
    True = 1
};

enum class Effect : int {
    Add = 0,
    Remove = 1
};

enum class ImportSource : int {
    Curse = 0,
    Pool = 1,
    Hillsfar = 2
};

enum class GameState : int {
    StartGameMenu = 0,
    Shop = 1,
    Camping = 2,
    WildernessMap = 3,
    DungeonMap = 4,
    Combat = 5,
    AfterCombat = 6,
    EndGame = 7
};

enum class CombatType : int {
    normal = 0,
    duel = 1
};

enum class Sound : int {
    sound_FF = -1,
    sound_0 = 0,
    sound_1 = 1,
    sound_2 = 2,
    sound_3 = 3,
    sound_4 = 4,
    sound_5 = 5,
    sound_6 = 6,
    sound_attackHeld = 7,
    sound_8 = 8,
    sound_9 = 9,
    sound_a = 0xa,
    sound_b = 0xb,
    sound_c = 0xc,
    sound_d = 0xd,
    sound_e = 0xe,
    sound_f = 0xf
};

using spellDelegate = std::function<bool(QuickFight quick_fight, int spellId)>;
using spellDelegate2 = std::function<void()>;
// PORT-NOTE: C# `object affect`. Every caller passes null, an Affect or an Item
// (ovr013 casts it back with (Affect)param / (Item)param), so it is a
// shared_ptr<void> here: pass an Affect/Item (converts implicitly) or nullptr,
// and recover with std::static_pointer_cast<Affect_>(param) / <Item_>.
using affectDelegate = std::function<void(Effect arg_0, std::shared_ptr<void> affect, Player player)>;

// [Flags]
enum class DamageType : int {
    Fire = 0x01,
    Cold = 0x02,
    Electricity = 0x04,
    Magic = 0x08,
    Acid = 0x10,
    DragonBreath = 0x20,
    Unknown40 = 0x40
};

enum class DamageOnSave : int {
    Normal = 0,
    Zero = 1,
    Half = 2,
    Unknown_3 = 3,
    Unknown_1E = 0x1e
};

enum class SoundType : int {
    PC,
    None
};

class MenuColorSet_ {
public:
    int highlight = 0;
    int foreground = 0;
    int prompt = 0;

    MenuColorSet_(int h, int f, int p) { highlight = h; foreground = f; prompt = p; }
};

struct Point {
    int x = 0;
    int y = 0;

    static constexpr int MapMaxX = 50;
    static constexpr int MapMaxY = 25;
    static constexpr int MapMinX = 0;
    static constexpr int MapMinY = 0;

    static constexpr int ScreenMaxX = 6;
    static constexpr int ScreenMaxY = 6;
    static constexpr int ScreenHalfX = ScreenMaxX / 2;
    static constexpr int ScreenHalfY = ScreenMaxY / 2;
    static const Point ScreenCenter;  // = new Point(ScreenHalfX, ScreenHalfY)  (defined below)

    Point() = default;  // C# struct default ctor: zero-initialised
    Point(int _x, int _y) : x(_x), y(_y) {}
    // C# `Point(Point old)` copy constructor == the implicit C++ copy constructor.

    friend Point operator+(Point a, Point b) { return Point(a.x + b.x, a.y + b.y); }
    friend Point operator-(Point a, Point b) { return Point(a.x - b.x, a.y - b.y); }
    Point& operator+=(Point b) { x += b.x; y += b.y; return *this; }
    Point& operator-=(Point b) { x -= b.x; y -= b.y; return *this; }
    friend Point operator*(Point a, int b) { return Point(a.x * b, a.y * b); }
    friend Point operator/(Point a, int b) { return Point(a.x / b, a.y / b); }
    friend bool operator==(Point a, Point b) { return a.x == b.x && a.y == b.y; }
    friend bool operator!=(Point a, Point b) { return a.x != b.x || a.y != b.y; }

    void MapBoundaryTrunc();
    bool MapInBounds() const;

    String ToString() const;
    // PORT-NOTE: implicit conversion so cs::Format("{0}", pt) and String("x") + pt work.
    // cs::Format calls the qualified cs::ToString, which cannot see overloads declared
    // after cs.h; the conversion lets it resolve to cs::ToString(const String&).
    operator String() const { return ToString(); }
};
inline const Point Point::ScreenCenter = Point(Point::ScreenHalfX, Point::ScreenHalfY);

// cs::ToString(point) / Format("{0}", point) support for the value struct.
inline String ToString(const Point& p) { return p.ToString(); }

}  // namespace Classes

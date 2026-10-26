# Translating coab (C#) to C++

goldbox-3ds starts as a **faithful, line-for-line translation** of Simeon Pilgrim's
[coab](https://github.com/simeonpilgrim/coab) (Curse of the Azure Bonds, C#) into C++17.
Faithful means: same names, same control flow, same arithmetic, same comments
(keep the `// sub_xxxxx` / `// 0x151` address comments — they map back to the DOS
EXE). No refactoring, no "improvements", no reordering. Bugs in coab stay bugs for
now; mark anything suspicious with `// PORT-NOTE:` and move on. We will diff the
C++ build against coab with identical RNG seeds and inputs, so any behavioural
drift is a defect.

The compatibility runtime is `src/cs/cs.h` (namespace `cs`). Read it first.

## Files and namespaces

| C#                                   | C++                                                         |
|--------------------------------------|-------------------------------------------------------------|
| `Classes/Foo.cs`                     | `src/Classes/Foo.h` + `src/Classes/Foo.cpp`                 |
| `Classes/Combat/Foo.cs`              | `src/Classes/Combat/Foo.h` + `.cpp`                         |
| `engine/ovr025.cs`                   | `src/engine/ovr025.cpp` (declarations live in `src/engine/engine.h`) |
| `namespace Classes`                  | `namespace Classes`                                         |
| `namespace Classes.Combat`           | `namespace Classes { namespace Combat {` (same for `DaxFiles`) |
| `namespace engine`                   | `namespace engine`                                          |
| `namespace Logging` (Logger)         | `namespace Logging` — hand-written in `src/Logging/Logger.h`|
| `using Classes;` / `using X.Y;`      | `using namespace Classes;` / `using namespace X::Y;`        |

Every translated file starts with:

```cpp
#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;
```

`src/Classes/Classes.h` includes every Classes header; `src/engine/engine.h`
declares every engine class. Engine `.cpp` files include just `engine/engine.h`.
`src/Classes/fwd.h` forward-declares every Classes type and its alias (below), so
Classes headers can refer to each other without include cycles.

## Types

| C#                         | C++                                                    |
|----------------------------|--------------------------------------------------------|
| `int`, `short`, `bool`, `char`, `double` | same                                     |
| `byte`, `sbyte`, `ushort`, `uint` | same names (`cs::byte` etc.)                    |
| `long`                     | `int64_t` (**never** C++ `long`: 32-bit on ARM/Windows) |
| `string`                   | `String` (`cs::String`, a `std::string` with C# members) |
| `object`                   | avoid; use the concrete type or `std::shared_ptr<void>` + PORT-NOTE |
| `T[]`                      | `Array<T>` — reference semantics, default = null       |
| `T[,]`, `T[,,]`            | `Array2<T>`, `Array3<T>`; index as `a(i, j)`, `a(i, j, k)` |
| `T[][]`                    | `Array<Array<T>>`                                      |
| `List<T>`                  | `List<T>` — reference semantics, **default = empty list** |
| `Dictionary<K,V>`, `Queue<T>`, `Stack<T>` | same names in `cs`                      |
| `Predicate<T>`, `Func<..>`, `Action<..>`, delegate types | `std::function<...>`; `public delegate bool spellDelegate(QuickFight q, int id);` → `using spellDelegate = std::function<bool(QuickFight, int)>;` |
| `IEnumerable<T>` + `yield return` | return a `List<T>` built eagerly                |
| `int.MaxValue` etc.        | `INT32_MAX`, `INT16_MAX`, `UINT16_MAX`, `UINT8_MAX` ... |

### Reference types (classes) — the most important rule

Every **instantiable** C# class `Foo` becomes a C++ class `Foo_` plus an alias:

```cpp
class Player_;                       // in fwd.h
using Player = cs::Ref<Player_>;     // in fwd.h
class Player_ { public: ... };       // in Player.h
```

So C# declarations translate verbatim: `Player player` stays `Player player`,
`List<Item>` stays `List<Item>`. Then:

| C#                              | C++                                             |
|---------------------------------|-------------------------------------------------|
| `new Player()` / `new Player(a)`| `New<Player_>()` / `New<Player_>(a)`            |
| `player.hitBonus`               | `player->hitBonus`                              |
| `null`                          | `nullptr`                                       |
| `x == null`, `x != null`        | `x == nullptr`, `x != nullptr`                  |
| `a == b` (two references)       | `a == b` (pointer identity, same as C#)         |
| `obj[i]` (class with indexer)   | `obj[i]` (Ref forwards `[]`)                    |
| `Player.StaticThing`            | `Player_::StaticThing`                          |
| `class A : B`                   | `class A_ : public B_`                          |
| `x is Foo` / `x as Foo`         | `Is<Foo_>(x)` / `As<Foo_>(x)`                   |
| `this` passed/stored as a reference | class derives `std::enable_shared_from_this<Foo_>`; use `Foo(shared_from_this())` |
| `virtual`/`override`/`abstract` | `virtual` / `override` / `= 0`                  |
| interface `IDataIO`             | abstract struct with pure virtuals + virtual dtor |
| `IComparable<T>.CompareTo`      | member `int CompareTo(const T& other) const`    |
| `obj.ToString()` (class)        | member `String ToString() const`; call as `cs::ToString(obj)` |

Classes that only have **static** members (all of `engine/*`, `gbl`, `Limits`,
`Sys`, `Cheats`, `Display`, `ItemLibrary`, `DaxCache`, `Text`, ...) are **not**
aliased: they become `struct Foo { static ...; };` and are used as `Foo::member`.

### Value types (structs)

Only `StatValue`, `ClassLevels`, `Point` are C# structs. They become plain C++
structs with value semantics, members accessed with `.`, no alias, no `New`.
A C# struct's implicit default constructor zero-initialises: give every field a
default member initializer (`int x = 0;`).

### Enums

`enum Race { elf, ... }` → `enum class Race : int { elf, ... };` keeping the C#
underlying type (`enum ItemDataFlags : byte` → `: byte`). Values are used as
`Race::elf`. `cs::enum_ops` supplies `| & ^ ~`, `enum + int`, `enum - enum`,
`++`/`--`, so C# enum arithmetic compiles unchanged. Casts stay casts:
`(int)race`, `(Race)3`. Keep every enumerator and explicit value exactly.
`Enum.GetValues(typeof(X))` → write the explicit list.

## Members

* **Fields**: same name, same type. Give every field the C# default
  (`= 0`, `= false`, `= nullptr` is implicit for Ref, `= ""` implicit for String).
  Field initializers (`List<Item> items = new List<Item>();`) → `List<Item> items;`
  (List default is already a new empty list) or `List<Item> items = List<Item>{...};`.
* **Static fields**: `static int x = 5;` → `inline static int x = 5;` inside the
  class/struct in the header. Static constructors (`static Foo() {...}`) → a
  private `static bool StaticInit();` called from an `inline static bool
  staticInit_ = StaticInit();` member, or call it explicitly from first use;
  add a PORT-NOTE.
* **const**: `const int X = 3;` → `static constexpr int X = 3;`.
  `static readonly int[] X = {...}` → `inline static const Array<int> X{...};`
* **Methods**: same names, same parameter order. Overloads stay overloads.
  `ref`/`out` parameters → `T&`; drop `ref`/`out` at call sites.
  `params T[] args` → `Array<T> args` (call sites wrap with `Array<T>{...}`)
  except `string.Format`-style, which use `cs::Format` directly.
  Default parameter values stay default values.
* **Properties** — there are very few; translate exactly as listed:

| Property (class)                                   | C++ form                                       |
|----------------------------------------------------|------------------------------------------------|
| ActiveItems: `primaryWeapon secondaryWeapon armor field_15D field_161 field_165 field_169 field_16D field_171 Item_ptr_01 Item_ptr_02 arrows quarrels` | `Item& name()` returning `itemArray[n]`. Read **and** write through `name()`: `p->activeItems->arrows() = nullptr;` |
| ActiveItems indexer `this[ItemSlot]`               | `Item& operator[](ItemSlot slot)`              |
| MoneySet `Copper Electrum Silver Gold Platinum Gems Jewels` (get only) | `int Copper() const` ...       |
| Opperation `Code`, `Low`, `High`                   | getters `Code()`, `Low()`, `High()` + setters `set_Code(byte)`, `set_Low`, `set_High` |
| Player `cleric_lvl` … `monk_lvl`, `*_old_lvl`      | `byte& name()` — read and write through `name()` |
| EclBlock / RestTime / ItemDataTable indexers       | `obj[i]` returns a reference (`byte&`/`int&`/`ItemData&`) |
| Struct_1D1BC indexer `[x, y]` / `[Point]`          | `obj->at(x, y)` / `obj[pt]` (both return `int&`) |
| Struct_1B2CA `ushort` indexer                      | `obj[i]` returns a proxy (get/set; no compound assignment) |
| ClassStatsMin, PlayerStats, Struct_1A35E indexers  | get-only, return by value |
| Opperation `Word` (get only)                       | `ushort Word() const`                          |
| Player `DisplayAc` (get only)                      | `int DisplayAc() const`                        |
| DataOffsetAttribute `Offset Size Type`             | getters `Offset()` etc.                        |
| Display `UpdateCallback` (static set)              | `static void set_UpdateCallback(VoidDeledate)` |
| MenuItem `Text Heading Item` (auto `{get;set;}`)   | plain public fields                            |

  Any other property you find: auto-property → plain field; computed → method
  `Name()` (+ `set_Name(v)`); record it in this table.
* **Indexers** `this[...]` → `operator[]` returning a reference where there is a setter.
* **Operator overloads** on classes: translate as C++ operators **on the `_` class**
  and, if C# code compares two references with an overloaded `==`, call a named
  function instead (`Item_::Equals(a, b)`) — `Ref ==` is pointer identity.

## Expressions and statements

| C#                                   | C++                                                  |
|--------------------------------------|------------------------------------------------------|
| `foreach (var x in list)`            | `for (auto x : list)` (handles copy like C# refs)    |
| `foreach (Item item in items)`       | `for (Item item : items)`                            |
| `list.Count` / `array.Length` / `str.Length` | `list.Count()` / `array.Length()` / `str.Length()` |
| `arr.GetLength(0)`                   | `arr.GetLength(0)`                                   |
| `p => p.x == 1`                      | `[&](const auto& p) { return p->x == 1; }`           |
| `delegate(...) { ... }`              | lambda                                               |
| `string.Format("{0} {1,4:X}", a, b)` | `Format("{0} {1,4:X}", a, b)`                        |
| `x.ToString()` / `x.ToString("X2")`  | `ToString(x)` / `ToString(x, "X2")`                  |
| `"abc" + n` (string + non-string)    | **`String("abc") + ToString(n)`** — C++ `"abc" + n` is pointer arithmetic! Every `+` that involves a string must have a `String` on the left or be wrapped in `ToString`. |
| `s1 + s2` where both are `String`    | unchanged                                            |
| `s.Substring(a, b)`, `s.IndexOf(c)`, `s.ToUpper()`, `s.Trim()`, `s.PadRight(n)` ... | same (members of `cs::String`) |
| `s[i]`                               | `s[i]` (returns `char`; bounds-checked)              |
| `string.Empty`, `""`, `null` string  | `""`                                                 |
| `char.ToUpper(c)`                    | `Char::ToUpper(c)`                                   |
| `Math.Min/Max/Abs/Sign/Sin/Cos/PI`   | `Math::Min` ... `Math::PI`                           |
| `Array.Copy(a, i, b, j, n)`          | `Array<T>::Copy(a, i, b, j, n)`                      |
| `Array.Clear(a, i, n)`               | `Array<T>::Clear(a, i, n)`                           |
| `(byte[])x.Clone()`                  | `x.Clone()`                                          |
| `new byte[n]` / `new int[] {1,2}`    | `Array<byte>(n)` / `Array<int>{1, 2}`                |
| `new int[2, 3]` / `{ {..},{..} }`    | `Array2<int>(2, 3)` / `Array2<int>{ {..}, {..} }`    |
| `new List<T>()`                      | `List<T>()`                                          |
| `new List<T>(other)`                 | `List<T>::New(other)`                                |
| `unchecked(expr)`                    | `(expr)` (we build with `-fwrapv`)                   |
| `checked`                            | not used                                             |
| `lock (x) { ... }`                   | `{ std::lock_guard<std::mutex> g(xMutex); ... }`     |
| `throw new Exception("m")`           | `throw Exception("m")` (any `cs::` exception type)   |
| `catch (ApplicationException)`       | `catch (const ApplicationException&)`                |
| `catch (Exception e)`                | `catch (const Exception& e)`                         |
| `using (var x = ...) { }`            | scope + explicit Close()/dtor                        |
| `goto case X;`                       | duplicate the target block or use a label; PORT-NOTE |
| `switch` on enum/int                 | unchanged; add `break;` exactly where C# has it      |
| `Console.WriteLine(...)`             | `Logging::Logger::Console(Format(...))`              |
| `Logger.Debug(fmt, ...)` etc.        | `Logging::Logger::Debug(fmt, ...)` (same overloads)  |
| `System.IO.Path.Combine`, `File.Exists`, `File.Open`, `Directory.GetFiles` | `#include "cs/io.h"`: `IO::Path::Combine`, `IO::File::Exists`, `IO::File::Open`, `IO::Directory::GetFiles` (note `File` alone is coab's own `Classes::File`) |
| `new FileStream(p, mode, access, share)` / `FileStream fs` | `New<IO::FileStream_>(p, IO::FileMode::Open, IO::FileAccess::Read)` / `IO::FileStream fs`; `fs.Seek(n, SeekOrigin.Begin)` → `fs->Seek(n, IO::SeekOrigin::Begin)` |
| `new BinaryReader(fs)` / `BinaryReader r` / `r.BaseStream` | `New<IO::BinaryReader_>(fs)` / `IO::BinaryReader r` / `r->BaseStream` |
| `Config.GetSavePath()` (Logging) | `Logging::Config::GetSavePath()` |
| `DateTime.Now`, `.Ticks`, `.AddSeconds(s)`, `.Subtract(t).TotalMilliseconds` | `DateTime::Now()`, `.Ticks`, `.AddSeconds(s)`, `.Subtract(t).TotalMilliseconds()` (`cs/io.h`) |
| `System.Threading.Thread.Sleep(ms)`  | `platform::Sleep(ms)`                                |
| `seg051.Random` / `new Random(...)`  | `cs::Random` is bit-exact with .NET's seeded Random  |

### Integer semantics

C# and C++ agree on two's-complement wraparound for the casts coab uses
(`(byte)x`, `(sbyte)x`, `(short)x`), integer division truncation and `%` sign.
Keep every cast exactly as written. Remember C# promotes `byte + byte` to `int`
just like C++. Compound assignments like `b += 5` on a `byte` narrow implicitly in
both languages. Shifts: C# masks the shift count (`x << 33` == `x << 1` for int);
coab never relies on this, but PORT-NOTE anything that might.

## Things that are NOT translated mechanically

* `Classes/Display.cs`, `engine/seg044.cs` (sound), `engine/seg049.cs` (keyboard),
  `engine/seg001.cs` thread/abort plumbing, `Logging/`, `Main/` (WinForms UI) —
  hand-ported against `src/platform/platform.h`.
* `Classes/DataIO.cs` uses reflection over `[DataOffset(...)]` attributes.
  Replacement: each class that carries `[DataOffset]` fields gets a static field
  table built with the `DATAIO_FIELD` macro from `Classes/DataIO.h`; the
  `DataIO` functions iterate that table instead of reflecting. Field order and
  every offset/type/size must match the C# attributes exactly.
* `ItemLibrary` (.NET binary serialization of a user item library) — stubbed;
  PORT-NOTE.

## Checking your work

* Each `.cpp` must compile with
  `g++ -std=c++17 -fsyntax-only -fwrapv -Wall -Isrc src/engine/ovrNNN.cpp`.
* Do not edit another translator's files except `engine/engine.h` declarations
  for your own file and the property table above.

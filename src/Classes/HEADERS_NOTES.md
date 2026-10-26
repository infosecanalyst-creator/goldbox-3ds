# Classes headers: judgment calls

## Class kinds
- **Instantiable** (`X_` + `using X = Ref<X_>`): Action, Affect, Area1, Area2, ClassStatsMin, SortedCombatant, DataOffsetAttribute, DownedPlayerTile, EclBlock, File, GasCloud, MenuColorSet, gbl::SetBlock (nested in gbl), GeoBlock, WallDefs, WallDefBlock, MapInfo, HillsFarPlayer, Item, ItemDataTable, ItemData, MenuItem, MoneySet, Opperation, PlayerStats, ActiveItems, Player, PoolRadPlayer, RestTime, Set, SpellItem, SpellList, SpellEntry, SteppingPath, Struct_189B4, SubStruct_1A35E, Struct_1A35E, Struct_1ADF6, Struct_1B2CA, Struct_1D1BC, Text, AnimationFrame, DaxArray, DaxBlock, Combat::CombatIcon, Combat::CombatPlayerIndex, Combat::CombatantMap, DaxFiles::DaxFileCache, DaxFiles::DaxHeaderEntry.
- **Static-only** (`struct X`): Cheats, DataIO, Display, gbl, ItemLibrary, Limits, Money, Control, Sys, DaxFiles::DaxCache.
- **Value structs**: Point, StatValue, ClassLevels. **Interfaces**: IDataIO, IOSDisplay.

## Layout
- The free types at the top of Gbl.cs (enums, delegates, MenuColorSet, Point) live in `GblTypes.h`, because `gbl`'s initialisers need nearly every header, and Spells.h and Player.h need those types. `Gbl.h` includes `GblTypes.h`.
- DaxArray.cs and DaxBlock.cs are in `namespace Classes` (not `Classes::DaxFiles`), as they are in C#.

## Properties and indexers
- ActiveItems slots → `Item& name()`. ActiveItems `this[ItemSlot]` → `Item& operator[]`.
- Player `cleric_lvl` … `monk_lvl` and `*_old_lvl` (get/set) → `byte& name()`. Engine assigns them, e.g. `player.paladin_lvl = 1` → `p->paladin_lvl() = 1`. **Add to the TRANSLATION.md table.**
- Player `DisplayAc` → `int DisplayAc() const`. MoneySet coin properties → `int Copper() const` etc.
- Opperation `Code/Low/High` have **both** get and set in C#, and the engine reads `Code`: `set_Code(v)` plus a `Code() const` getter (same for Low and High). `Word() const`.
- DataOffsetAttribute → `Offset() Size() Type()`. Display `UpdateCallback` → `set_UpdateCallback`. MenuItem `Text Heading Item` → plain fields (field `Item` has type `Classes::Item`).
- Indexers returning a reference: EclBlock `byte&` (index & 0xFFFF), RestTime `int&` (switch; body in .cpp), ItemDataTable `ItemData&`, Struct_1D1BC `int& operator[](Point)`.
- Struct_1D1BC `this[int x, int y]` → `int& operator()(x, y)`, plus `at(x, y)`, because `Ref` only forwards `[]`. Engine: `gbl::mapToBackGroundTile->at(x, y)`.
- Struct_1B2CA `ushort this[int]` reads and writes little-endian bytes, so there is no ushort to return a reference to. It returns a proxy `Indexer` (converts to ushort, assignable), backed by `Get(i)` and `Set(i, v)`. Compound assignment is not supported (the engine doesn't use it).
- Get-only indexers return by value: ClassStatsMin `byte`, PlayerStats `StatValue` (a copy, as in C#), Struct_1A35E `[int]` and `[ClassId]`.

## Operators
- `MoneySet + MoneySet` → `static MoneySet_::Add(a, b)`, plus free `operator+` and `operator+=` on `MoneySet` refs. `+=` rebinds to a new object, as C# does.
- `Set + byte` (unused) → `static Set_::Add`. The `params int[]` constructor is `explicit Set_(Array<int>)`.
- `Item.Equals(object)` → `bool Equals(const Item&) const` plus `static Item_::Equals(a, b)`. List<Item>.Contains in ItemLibrary must use `Exists` with Equals.
- Point operators are friends. Point and StatValue get `operator String()` so `cs::Format("{0}", pt)` resolves; the qualified `cs::ToString` cannot see overloads declared later.
- SortedCombatant → `int CompareTo(const SortedCombatant&) const`.

## Delegates and `object`
- `affectDelegate(Effect, object, Player)`: `object` → `std::shared_ptr<void>`. Callers pass an Affect, an Item or nullptr; recover with `std::static_pointer_cast<Affect_>`. `Text.field_80` (object, unused) is also `shared_ptr<void>`.
- `VmGetMemoryValue`, `spellDelegate`, `spellDelegate2`, `Display::VoidDeledate` → `std::function`.

## Statics and initialisation
- `Display` static ctor → `static bool StaticInit()` + `inline static bool staticInit_`.
- `gbl` table data is copied verbatim; every table was number-checked against the C# by script. Private `*_ages` stay private. `RaceClasses[0]` is an empty array, not null.
- `gbl::ItemDataTable` is declared with type `Classes::ItemDataTable`, because the member name shadows the type.
- `Item_` derives `enable_shared_from_this`. Its constructors call `ItemLibrary.Add(this)`; `shared_from_this` is unusable there. ItemLibrary.Add only shallow-clones, so the body can wrap `this` in a non-owning Ref or clone first.
- Area1 `field_18C` is tagged offset 0x18E, the same as `time_minutes_ones`. That is a coab bug, kept, with a PORT-NOTE.
- `Area2.isDuel` has its attribute commented out in C#, so it is not in the DataIO table.

## DataIO
- `FieldDesc` holds name, offset, DataType, size, `FieldKind`, `addr`, `getInt`/`setInt` (scalars and enums), `asDataIO` (Ref to an IDataIO implementer) and typed `As<T>()`.
- Tables are built with `DATAIO_FIELD(Class_, member, off, DataType::X, size)`, or `DATAIO_FIELD_DS` when the attribute has no explicit size.
- `DataIO::ReadObject(this, data, off)` and the other three entry points are templates over `C*`. They call `C::DataFields()` and pass `obj.ToString()`, or the C# type name, to the Logger lines.

## cs.h addition
- An `Array<bool>` specialization, backed by `deque<bool>`, appended at the end of cs.h. The generic version cannot return `bool&` from `vector<bool>`.

# engine/ translation notes (phase 1: declarations)

`src/engine/engine.h` declares every class in `coab/engine/*.cs`. It was generated
from the C# sources by a script (member order and `// sub_xxxx` / `seg600:` comments
preserved), then hand-edited at the spots marked `PORT-NOTE`. Each `ovrNNN.cpp` /
`segNNN.cpp` includes only `engine/engine.h` and defines its struct's methods, e.g.
`void ovr025::CalculateAttackValues(Player player) { ... }`.

## Layout

* `ovr002`..`seg051` (and `MapCursor` from ovr028.cs, which has no `ovr028` class):
  `struct X { ... };` with every method `static` and every member public.
  Static fields are `inline static` with their C# initializers (all tables copied
  verbatim). `const int` becomes `static constexpr int`. `readonly` becomes `inline static const`
  (only `ovr020::MU_spell_lvl_learn`).
* No engine class has a static constructor, so there is no `StaticInit()`.
* Top-level engine types are placed first, before the ovr/seg structs:
  `SpellLoc` (ovr020.cs) and `CheckType` (ovr024.cs) as `enum class : int`;
  `MemLoc_`/`MemLoc` (VmOpp.cs); `CmdItem_`/`CmdItem` with nested
  `CmdItem_::CmdDelegate = std::function<void()>` (ovr003.cs); `VmLog`.
* Nested types stay nested in their struct:
  `ovr011::tri_state`, `ovr014::AttackType`, `ovr020::Weld` (enum class : int);
  `ovr018::hp_calc_`/`ovr018::hp_calc`, `ovr019::Struct_1ADFB_`/`Struct_1ADFB`,
  `ovr027::highlight_`/`highlight`, `ovr027::HighlightSet_`/`HighlightSet`,
  `ovr032::MapReach_`/`MapReach`; `seg001::VoidDelegate = std::function<void()>`.
  Define their members like this: `ovr018::hp_calc_::hp_calc_(int _dice, ...)`,
  `String ovr027::highlight_::ToString() const`.
* Instantiable classes follow the `X_` + `using X = cs::Ref<X_>` rule
  (`new HighlightSet()` becomes `New<HighlightSet_>()`). Constructors are
  declared, not defined. The .cpp of the owning file defines them.

## Things a .cpp translator must know

* **No identifier renames were needed.** No engine identifier is a C++ keyword or
  common macro. (`exp`, `index`, `time`, `max` as parameters/locals just shadow libc names.)
* **`object`**: `engine.h` declares `using object = std::shared_ptr<void>;`. It is used only
  in the `(Effect, object, Player)` affect-handler methods (ovr013, ovr014, ovr023,
  117+5+8 of them) and `ovr013::CallAffectTable`. This **must equal** the second
  parameter type of `Classes::affectDelegate` (Gbl.h). If the Classes header picks
  a different type, change this one alias. Cast with
  `Affect affect = std::static_pointer_cast<Affect_>(param);` (or `Item_` in
  `do_items_affect`). Passing `affect`/`item`/`nullptr` converts implicitly.
* **`VmLog::Write/WriteLine`** (`params object[] args`) are variadic templates
  defined inline in engine.h (bodies translated verbatim). Call them like the C#:
  `VmLog::WriteLine("CMD_Random: Max: {0} Loc: {1}", rand_max, New<MemLoc_>(loc));`.
  `MemLoc_::ToString` (VmOpp.cpp, not yet written) must be defined somewhere: put
  `MemLoc_` / `VmLog` bodies in `src/engine/VmOpp.cpp`.
* **`ovr009::FindNextCombatant`**: C# returns `IEnumerable<Player>` built with `yield`.
  The rule "build the List eagerly" is **wrong here**, because each step rolls dice and
  depends on `DoPlayerCombatTurn` having changed `actions.delay`. An eager build
  would loop forever or desync the RNG. It is declared `static Player FindNextCombatant();`.
  It runs one pass of the do/while body and returns `output_player`, which is
  `nullptr` at the end. `MainCombatLoop` becomes
  `for (Player player = FindNextCombatant(); player != nullptr; player = FindNextCombatant()) { DoPlayerCombatTurn(player); }`.
  The RNG call order matches the C# exactly.
* **`seg051::random_number`** is `cs::Random random_number{0}` (value, not null):
  `cs::Random` has no default constructor. `Randomize()` →
  `random_number = cs::Random(Determinism::SeedFromTicks(DateTime::Now().Ticks));`.
  Always write `cs::Random` inside seg051: its `Random(...)` methods hide the type.
* **`Array4<T>`** (defined in engine.h, same API as `Array3`, index `a(i,j,k,l)`) exists
  only for `ovr011::unk_1AB1C` (`byte[,,,]`).
* **Indexers**: `ovr019::Struct_1ADFB_::operator[](int) const` returns `byte` (get only,
  throws `NotSupportedException` on default). `ovr027::HighlightSet_::operator[](int)`
  returns `highlight&` (get/set). `Ref` forwards `[]`, so `highlighSet[index]->start` works.
* **`Predicate<Player>`** becomes `std::function<bool(Player)>` (ovr032, seg043).
* **`DebugWriter`** (seg043) is `Ref<Logging::DebugWriter>`. Logging::DebugWriter is a plain
  class with no `_` alias, so create one with `Ref<Logging::DebugWriter>(std::make_shared<Logging::DebugWriter>(filename))`.
  Its destructor closes it.
* **`System.IO.BinaryReader`** (seg042 `setupDaxFiles`) is `IO::BinaryReader&` for the out params.
* Default-null static fields: `ovr013::affect_table = nullptr` (built in
  `SetupAffectTables`), `ovr019::unk_1ADFB` = 3 null refs (as in C#),
  `ovr032::mapReachCache` = `Array3<MapReach>(1250, 1250, 2)` of null refs.
  PORT-NOTE: that is ~3.1M `shared_ptr`s (about 50 MB on 64-bit, 25 MB on 3DS). It is faithful,
  but a memory problem on the 3DS that we will need to revisit.
* `List<T>`/`Dictionary<K,V>` statics default-construct to empty (matching the C#
  `new ...()` initializers). `Set` statics are `New<Set_>(Array<int>{...})` (C#
  `params int[]`). Char literals like `'0'` stay as-is (they promote to int).

## Hand-ported files (declarations kept, names unchanged)

* **seg049**: `keysPressed` stays `Queue<ushort>`. `lock (keysPressed)` becomes
  `std::lock_guard<std::mutex> g(keysPressedLock)`. `Semaphore WaitForKey(0,1)` becomes
  `WaitForKeyLock` (mutex) + `WaitForKey` (condition_variable) + `WaitForKeyCount`
  (0/1): `Release()` sets the count to 1 and notifies; `WaitOne()` waits for count==1 and sets it to 0.
  Blocking waits should throw `cs::EngineAbort` on shutdown (see platform.h).
* **seg044**: `sounds` (SoundPlayer[13]) becomes `inline static Array<bool> soundLoaded`.
  `SoundInit` calls `platform::LoadSound(id, "missle")` etc. and sets the slot to true. `PlaySound` calls
  `platform::PlaySound(sampleId)` / `platform::StopAllSounds()`.
* **seg001**: `EngineThread` is dropped (comment left in place). `EngineStop()` →
  `EngineStoppedCallback(); throw EngineAbort{};` in place of `Thread.Abort()`.
  `ConfigGame`'s `Directory.GetCurrentDirectory()` needs a platform choice.

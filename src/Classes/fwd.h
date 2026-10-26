// Classes/fwd.h - forward declarations of every type in coab's Classes assembly.
// Instantiable C# classes X -> `class X_; using X = cs::Ref<X_>;`
// Static-only C# classes  -> `struct X;`
// C# value structs        -> `struct X;` (Point, StatValue, ClassLevels)
// Enums are declared opaque here (same underlying type as the full definition).
#pragma once

#include "cs/cs.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

// ---- enums (full definitions live in the header named after the .cs file) ----
enum class Affects : int;        // Affect.h
enum class DataType : int;       // DataIO.h
enum class TextRegion : int;     // Display.h
enum class Status : int;         // Enums.h
enum class MonsterType : int;    // Enums.h
enum class Stat : int;           // Enums.h
enum class Race : int;           // Enums.h
enum class SkillType : int;      // Enums.h
enum class ClassId : int;        // Enums.h
enum class CombatTeam : int;     // Enums.h
enum class QuickFight : int;     // Gbl.h
enum class Effect : int;         // Gbl.h
enum class ImportSource : int;   // Gbl.h
enum class GameState : int;      // Gbl.h
enum class CombatType : int;     // Gbl.h
enum class Sound : int;          // Gbl.h
enum class DamageType : int;     // Gbl.h
enum class DamageOnSave : int;   // Gbl.h
enum class SoundType : int;      // Gbl.h
enum class ItemDataFlags : byte; // ItemData.h
enum class ItemSlot : int;       // ItemData.h
enum class ItemType : int;       // ItemData.h
enum class SaveVerseType : int;  // Spells.h
enum class SpellWhen : int;      // Spells.h
enum class SpellTargets : int;   // Spells.h
enum class SpellSource : int;    // Spells.h
enum class SpellClass : int;     // Spells.h
enum class Spells : int;         // Spells.h

// ---- interfaces (abstract structs) ----
struct IDataIO;    // DataIO.h
struct IOSDisplay; // Display.h

// ---- value structs ----
struct Point;       // Gbl.h
struct StatValue;   // Player.h
struct ClassLevels; // Player.h

// ---- static-only classes ----
struct Cheats;      // Cheats.h
struct DataIO;      // DataIO.h
struct Display;     // Display.h
struct gbl;         // Gbl.h
struct ItemLibrary; // ItemLibrary.h
struct Limits;      // Limits.h
struct Money;       // MoneySet.h
struct Control;     // Player.h
struct Sys;         // Sys.h

// ---- instantiable classes ----
class Action_;              using Action = Ref<Action_>;                           // Action.h
class Affect_;              using Affect = Ref<Affect_>;                           // Affect.h
class Area1_;               using Area1 = Ref<Area1_>;                             // Area1.h
class Area2_;               using Area2 = Ref<Area2_>;                             // Area2.h
class ClassStatsMin_;       using ClassStatsMin = Ref<ClassStatsMin_>;             // ClassStatsMin.h
class SortedCombatant_;     using SortedCombatant = Ref<SortedCombatant_>;         // Combat.h
class DataOffsetAttribute_; using DataOffsetAttribute = Ref<DataOffsetAttribute_>; // DataIO.h
class DownedPlayerTile_;    using DownedPlayerTile = Ref<DownedPlayerTile_>;       // DownedPlayerTile.h
class EclBlock_;            using EclBlock = Ref<EclBlock_>;                       // EclBlock.h
class File_;                using File = Ref<File_>;                               // File.h
class GasCloud_;            using GasCloud = Ref<GasCloud_>;                       // GasCloud.h
class MenuColorSet_;        using MenuColorSet = Ref<MenuColorSet_>;               // Gbl.h
// gbl::SetBlock_ / gbl::SetBlock are nested in struct gbl (Gbl.h); a nested
// class cannot be forward-declared outside its enclosing class.
class GeoBlock_;            using GeoBlock = Ref<GeoBlock_>;                       // GeoBlock.h
class WallDefs_;            using WallDefs = Ref<WallDefs_>;                       // GeoBlock.h
class WallDefBlock_;        using WallDefBlock = Ref<WallDefBlock_>;               // GeoBlock.h
class MapInfo_;             using MapInfo = Ref<MapInfo_>;                         // GeoBlock.h
class HillsFarPlayer_;      using HillsFarPlayer = Ref<HillsFarPlayer_>;           // HillsFarPlayer.h
class Item_;                using Item = Ref<Item_>;                               // Item.h
class ItemDataTable_;       using ItemDataTable = Ref<ItemDataTable_>;             // ItemData.h
class ItemData_;            using ItemData = Ref<ItemData_>;                       // ItemData.h
class MenuItem_;            using MenuItem = Ref<MenuItem_>;                       // MenuItem.h
class MoneySet_;            using MoneySet = Ref<MoneySet_>;                       // MoneySet.h
class Opperation_;          using Opperation = Ref<Opperation_>;                   // Opperation.h
class PlayerStats_;         using PlayerStats = Ref<PlayerStats_>;                 // Player.h
class ActiveItems_;         using ActiveItems = Ref<ActiveItems_>;                 // Player.h
class Player_;              using Player = Ref<Player_>;                           // Player.h
class PoolRadPlayer_;       using PoolRadPlayer = Ref<PoolRadPlayer_>;             // PoolRadPlayer.h
class RestTime_;            using RestTime = Ref<RestTime_>;                       // RestTime.h
class Set_;                 using Set = Ref<Set_>;                                 // Set.h
class SpellItem_;           using SpellItem = Ref<SpellItem_>;                     // SpellList.h
class SpellList_;           using SpellList = Ref<SpellList_>;                     // SpellList.h
class SpellEntry_;          using SpellEntry = Ref<SpellEntry_>;                   // Spells.h
class SteppingPath_;        using SteppingPath = Ref<SteppingPath_>;               // SteppingPath.h
class Struct_189B4_;        using Struct_189B4 = Ref<Struct_189B4_>;               // Struct_189B4.h
class SubStruct_1A35E_;     using SubStruct_1A35E = Ref<SubStruct_1A35E_>;         // Struct_1A35E.h
class Struct_1A35E_;        using Struct_1A35E = Ref<Struct_1A35E_>;               // Struct_1A35E.h
class Struct_1ADF6_;        using Struct_1ADF6 = Ref<Struct_1ADF6_>;               // Struct_1ADF6.h
class Struct_1B2CA_;        using Struct_1B2CA = Ref<Struct_1B2CA_>;               // Struct_1B2CA.h
class Struct_1D1BC_;        using Struct_1D1BC = Ref<Struct_1D1BC_>;               // Struct_1D1BC.h
class Text_;                using Text = Ref<Text_>;                               // Text.h
// DaxFiles/*.cs that are in namespace Classes (not Classes.DaxFiles):
class AnimationFrame_;      using AnimationFrame = Ref<AnimationFrame_>;           // DaxFiles/DaxArray.h
class DaxArray_;            using DaxArray = Ref<DaxArray_>;                       // DaxFiles/DaxArray.h
class DaxBlock_;            using DaxBlock = Ref<DaxBlock_>;                       // DaxFiles/DaxBlock.h

namespace Combat {
enum class Icon : int;      // Combat/CombatIcon.h

class CombatIcon_;          using CombatIcon = Ref<CombatIcon_>;                   // Combat/CombatIcon.h
class CombatPlayerIndex_;   using CombatPlayerIndex = Ref<CombatPlayerIndex_>;     // Combat/CombatPlayerIndex.h
class CombatantMap_;        using CombatantMap = Ref<CombatantMap_>;               // Combat/CombatantMap.h
}  // namespace Combat

namespace DaxFiles {
struct DaxCache;            // DaxFiles/DaxCache.h (static-only)
class DaxFileCache_;        using DaxFileCache = Ref<DaxFileCache_>;               // DaxFiles/DaxFileCache.h
class DaxHeaderEntry_;      using DaxHeaderEntry = Ref<DaxHeaderEntry_>;           // DaxFiles/DaxHeaderEntry.h
}  // namespace DaxFiles

}  // namespace Classes

// Classes/Spells.cpp - port of Classes/Spells.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

SpellEntry_::SpellEntry_(int spell_idx, SpellClass _spellClass, sbyte _spellLevel,
    int _fixedRange, int _perLvlRange,
    int _fixedDuration, byte _perLvlDuration,
    byte f6, SpellTargets _targets,
    DamageOnSave _damageOnSave, SaveVerseType _saveVerse,
    Affects _affectId, SpellWhen _whenCast,
    int _castingDelay, int _priority, byte fe, byte ff)
{
    spellClass = _spellClass;
    spellLevel = _spellLevel;
    fixedRange = _fixedRange;
    perLvlRange = _perLvlRange;
    fixedDuration = _fixedDuration;
    perLvlDuration = _perLvlDuration;
    field_6 = f6;
    targetType = _targets;
    damageOnSave = _damageOnSave;
    saveVerse = _saveVerse;
    affect_id = _affectId;
    whenCast = _whenCast;
    castingDelay = _castingDelay;
    priority = _priority;
    field_E = fe;
    field_F = ff;
    spellIdx = spell_idx;
}

}  // namespace Classes

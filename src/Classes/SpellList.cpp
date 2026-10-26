// Classes/SpellList.cpp - port of Classes/SpellList.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

void SpellList_::Clear()
{
    spells.Clear();
}

void SpellList_::ClearSpell(int spellId)
{
    SpellItem found = nullptr;

    for (auto sp : spells)
    {
        if (sp->Id == spellId)
        {
            found = sp;
            break;
        }
    }

    spells.Remove(found);
}

// PORT-NOTE: the three C# iterators (yield return) are evaluated eagerly into a
// List<int>; the snapshot is taken when called, not when enumerated.
List<int> SpellList_::IdList()
{
    List<int> result;
    for (auto sp : spells)
    {
        result.Add(sp->Id);
    }
    return result;
}

List<int> SpellList_::LearntList()
{
    List<int> result;
    for (auto sp : spells)
    {
        if (sp->Learning == false)
        {
            result.Add(sp->Id);
        }
    }
    return result;
}

List<int> SpellList_::LearningList()
{
    List<int> result;
    for (auto sp : spells)
    {
        if (sp->Learning)
        {
            result.Add(sp->Id);
        }
    }
    return result;
}

void SpellList_::AddLearn(int id)
{
    spells.Add(New<SpellItem_>(id, true));
}

void SpellList_::AddLearnt(int id)
{
    spells.Add(New<SpellItem_>(id & 0x7F, id > 0x7f));
}

void SpellList_::MarkLearnt(int id)
{
    auto spell = spells.Find([&](const SpellItem& sp) { return sp->Id == id && sp->Learning == true; });

    if (spell != nullptr)
    {
        spell->Learning = false;
    }
}

bool SpellList_::HasSpells()
{
    return spells.Count() > 0;
}

bool SpellList_::HasSpell(int id)
{
    return spells.Exists([&](const SpellItem& sp) { return sp->Id == id; });
}

void SpellList_::CancelLearning()
{
    spells.RemoveAll([&](const SpellItem& sp) { return sp->Learning == true; });
}

void SpellList_::Load(Array<byte> data, int offset)
{
    for (int i = 0; i < SpellListSize; i++)
    {
        if (data[offset + i] > 0)
        {
            AddLearnt(data[offset + i]);
        }
    }
}

void SpellList_::Save(Array<byte> data, int offset)
{
    for (int i = 0; i < SpellListSize; i++)
    {
        data[offset + i] = 0;
    }

    int idx = SpellListSize - 1;
    for (auto sp : spells)
    {
        if (sp->Learning == false)
        {
            data[offset + idx] = (byte)sp->Id;
            idx -= 1;
        }
    }
}

}  // namespace Classes

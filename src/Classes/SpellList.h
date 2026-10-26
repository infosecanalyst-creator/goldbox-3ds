// Classes/SpellList.h - port of Classes/SpellList.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class SpellItem_ {
public:
    int Id = 0;
    bool Learning = false;

    SpellItem_(int id) { Id = id; Learning = false; }
    SpellItem_(int id, bool learning) { Id = id; Learning = learning; }
};

class SpellList_ {
public:
    static constexpr int SpellListSize = 84;

private:
    List<SpellItem> spells;

public:
    void Clear();

    void ClearSpell(int spellId);

    // IEnumerable<int> + yield return -> List<int> built eagerly
    List<int> IdList();

    List<int> LearntList();

    List<int> LearningList();

    void AddLearn(int id);

    void AddLearnt(int id);

    void MarkLearnt(int id);

    bool HasSpells();

    bool HasSpell(int id);

    void CancelLearning();

    void Load(Array<byte> data, int offset);

    void Save(Array<byte> data, int offset);
};

}  // namespace Classes

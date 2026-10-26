// Classes/MoneySet.h - port of Classes/MoneySet.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "Classes/DataIO.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

struct Money {
    static constexpr int Copper = 0;
    static constexpr int Silver = 1;
    static constexpr int Electrum = 2;
    static constexpr int Gold = 3;
    static constexpr int Platinum = 4;
    static constexpr int Gems = 5;
    static constexpr int Jewelry = 6;

    inline static Array<String> names{ "Copper", "Silver", "Electrum", "Gold", "Platinum", "Gems", "Jewelry" };

    inline static Array<int> per_copper{ 1, 10, 100, 200, 1000 };
};

class MoneySet_ : public IDataIO {
    Array<int> money = Array<int>(7);

public:
    // overload operator +
    // PORT-NOTE: C# `static MoneySet operator +(MoneySet a, MoneySet b)` returns a
    // new MoneySet; on Refs use `gbl.pooled_money = MoneySet_::Add(gbl.pooled_money, player->Money)`
    // (C# `x += y` on a class rebinds x to a new object).
    static MoneySet Add(MoneySet a, MoneySet b);

    void ClearAll();

    void ClearCoins();

    int GetExpWorth();

    int GetGoldWorth();

    void AddCoins(int coinType, int count);

    void SetCoins(int coinType, int count);

    void SubtractGoldWorth(int gold);

    int GetCoins(int coinType);

    bool AnyMoney();

    bool ScaleAll(double scale);

    // C# get-only properties
    int Copper() const { return money[Money::Copper]; }
    int Electrum() const { return money[Money::Electrum]; }
    int Silver() const { return money[Money::Silver]; }
    int Gold() const { return money[Money::Gold]; }
    int Platinum() const { return money[Money::Platinum]; }
    int Gems() const { return money[Money::Gems]; }
    int Jewels() const { return money[Money::Jewelry]; }

    // explicit interface implementation IDataIO.Write / IDataIO.Read
    void Write(Array<byte> data, int offset) override;
    void Read(Array<byte> data, int offset) override;
};

// C# `a + b` / `a += b` on MoneySet references.
inline MoneySet operator+(const MoneySet& a, const MoneySet& b) { return MoneySet_::Add(a, b); }
inline MoneySet& operator+=(MoneySet& a, const MoneySet& b) { a = MoneySet_::Add(a, b); return a; }

}  // namespace Classes

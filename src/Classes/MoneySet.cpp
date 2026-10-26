// Classes/MoneySet.cpp - port of Classes/MoneySet.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

// overload operator +
MoneySet MoneySet_::Add(MoneySet a, MoneySet b)
{
    auto c = New<MoneySet_>();

    for (int coin = Money::Copper; coin <= Money::Jewelry; coin++)
    {
        c->money[coin] = a->money[coin] + b->money[coin];
    }

    return c;
}

void MoneySet_::ClearAll()
{
    for (int coin = Money::Copper; coin <= Money::Jewelry; coin++)
    {
        money[coin] = 0;
    }
}

void MoneySet_::ClearCoins()
{
    for (int coin = Money::Copper; coin <= Money::Platinum; coin++)
    {
        money[coin] = 0;
    }
}

int MoneySet_::GetExpWorth()
{
    int total = GetGoldWorth();

    total += money[Money::Gems] * 250;
    total += money[Money::Jewelry] * 2200;

    return total;
}

int MoneySet_::GetGoldWorth()
{
    int copperValue = 0;
    for (int coin = Money::Copper; coin <= Money::Platinum; coin++)
    {
        copperValue += money[coin] * Money::per_copper[coin];
    }

    return copperValue / Money::per_copper[Money::Gold];
}

void MoneySet_::AddCoins(int coinType, int count)
{
    money[coinType] += count;
}

void MoneySet_::SetCoins(int coinType, int count)
{
    money[coinType] = count;
}

void MoneySet_::SubtractGoldWorth(int gold)
{
    int coppers = gold * Money::per_copper[Money::Gold];

    int coin = Money::Copper;

    while (coppers > 0)
    {
        int sub_coins = (coppers / Money::per_copper[coin]) + 1;

        if (money[coin] < sub_coins)
        {
            sub_coins = money[coin];
        }

        coppers -= Money::per_copper[coin] * sub_coins;
        money[coin] -= sub_coins;

        coin++;
    }

    if (coppers < 0)
    {
        coppers = Math::Abs(coppers);
        coin = Money::Platinum;

        while (coppers > 0)
        {
            int add_coins = coppers / Money::per_copper[coin];
            coppers -= Money::per_copper[coin] * add_coins;

            money[coin] += add_coins;
            coin--;
        }
    }
}

int MoneySet_::GetCoins(int coinType)
{
    return money[coinType];
}

bool MoneySet_::AnyMoney()
{
    for (int coin = Money::Copper; coin <= Money::Jewelry; coin++)
    {
        if (money[coin] > 0)
        {
            return true;
        }
    }

    return false;
}

bool MoneySet_::ScaleAll(double scale)
{
    bool didScale = false;
    for (int coin = Money::Copper; coin <= Money::Platinum; coin++)
    {
        didScale = didScale || (money[coin] > 0);
        money[coin] = (int)(money[coin] * scale);
    }

    return didScale;
}

void MoneySet_::Write(Array<byte> data, int offset)
{
    Sys::ShortToArray((short)money[0], data, offset + 0);
    Sys::ShortToArray((short)money[1], data, offset + 2);
    Sys::ShortToArray((short)money[2], data, offset + 4);
    Sys::ShortToArray((short)money[3], data, offset + 6);
    Sys::ShortToArray((short)money[4], data, offset + 8);
    Sys::ShortToArray((short)money[5], data, offset + 10);
    Sys::ShortToArray((short)money[6], data, offset + 12);
}

void MoneySet_::Read(Array<byte> data, int offset)
{
    money[0] = Sys::ArrayToShort(data, offset + 0);
    money[1] = Sys::ArrayToShort(data, offset + 2);
    money[2] = Sys::ArrayToShort(data, offset + 4);
    money[3] = Sys::ArrayToShort(data, offset + 6);
    money[4] = Sys::ArrayToShort(data, offset + 8);
    money[5] = Sys::ArrayToShort(data, offset + 10);
    money[6] = Sys::ArrayToShort(data, offset + 12);
}

}  // namespace Classes

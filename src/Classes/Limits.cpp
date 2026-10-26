// Classes/Limits.cpp - port of Classes/Limits.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

bool Limits::RaceClassLimit(int class_lvl, Player player, ClassId _class)
{
    bool race_limited = false;

    switch (player->race)
    {
        case Race::dwarf:
            if (_class == ClassId::fighter)
            {
                if (class_lvl == 9 ||
                    (class_lvl == 8 && player->stats2->Str.full == 17) ||
                    (class_lvl == 7 && player->stats2->Str.full < 17))
                {
                    race_limited = true;
                }
            }
            break;

        case Race::elf:
            if (_class == ClassId::fighter)
            {
                if (class_lvl == 7 ||
                    (class_lvl == 6 && player->stats2->Str.full == 17) ||
                    (class_lvl == 5 && player->stats2->Str.full < 17))
                {
                    race_limited = true;
                }
            }

            if (_class == ClassId::magic_user)
            {
                if (class_lvl == 11 ||
                    (class_lvl == 9 && player->stats2->Int.full < 17) ||
                    (class_lvl == 10 && player->stats2->Int.full == 17))
                {
                    race_limited = true;
                }
            }
            break;

        case Race::gnome:
            if (_class == ClassId::fighter)
            {
                if (class_lvl == 6 ||
                    (class_lvl == 5 && player->stats2->Str.full < 18))
                {
                    race_limited = true;
                }
            }
            break;

        case Race::half_elf:
            if (_class == ClassId::cleric && class_lvl == 5)
            {
                race_limited = true;
            }
            else
            {
                if (_class == ClassId::fighter || _class == ClassId::ranger)
                {
                    if (class_lvl == 8 ||
                        (class_lvl == 7 && player->stats2->Str.full == 17) ||
                        (class_lvl == 6 && player->stats2->Str.full < 17))
                    {
                        race_limited = true;
                    }
                }

                if (_class == ClassId::magic_user)
                {
                    if (class_lvl == 8 ||
                        (class_lvl == 7 && player->stats2->Str.full == 17) ||
                        (class_lvl == 6 && player->stats2->Str.full < 17))
                    {
                        race_limited = true;
                    }
                }
            }
            break;

        case Race::halfling:
            if (_class == ClassId::fighter)
            {
                if (class_lvl == 6 ||
                    (class_lvl == 5 && player->stats2->Str.full == 17) ||
                    (class_lvl == 4 && player->stats2->Str.full < 17))
                {
                    race_limited = true;
                }
            }

            break;

    }

    if (Cheats::no_race_level_limits)
    {
        race_limited = false;
    }

    return race_limited;
}

bool Limits::RaceStatLevelRestricted(ClassId _class, Player player) // sub_69138
{
    bool race_limited = false;

    int class_lvl = player->ClassLevel[(int)_class];

    if (class_lvl > 0)
    {
        switch (player->race)
        {
            case Race::dwarf:
                if (_class == ClassId::fighter)
                {
                    if ((class_lvl == 8 && player->stats2->Str.full == 17) ||
                        (class_lvl == 7 && player->stats2->Str.full < 17))
                    {
                        race_limited = true;
                    }
                }
                break;

            case Race::elf:
                if (_class == ClassId::fighter)
                {
                    if ((class_lvl == 7) ||
                        (class_lvl == 6 && player->stats2->Str.full == 17) ||
                        (class_lvl == 5 && player->stats2->Str.full < 17))
                    {
                        race_limited = true;
                    }
                }
                break;

            case Race::gnome:
                if (_class == ClassId::fighter)
                {
                    if ((class_lvl == 6) ||
                        (class_lvl == 5 && player->stats2->Str.full < 18))
                    {
                        race_limited = true;
                    }
                }
                break;

            case Race::half_elf:
                if (_class == ClassId::cleric &&
                    class_lvl == 5)
                {
                    race_limited = true;
                }
                else if (_class == ClassId::fighter)
                {
                    if (class_lvl == 8 ||
                        (class_lvl == 7 && player->stats2->Str.full == 17) ||
                        (class_lvl == 6 && player->stats2->Str.full < 17))
                    {
                        race_limited = true;
                    }
                }
                break;

            case Race::halfling:
                if (_class == ClassId::fighter)
                {
                    if ((class_lvl == 6) ||
                        (class_lvl == 5 && player->stats2->Str.full == 17) ||
                        (class_lvl == 4 && player->stats2->Str.full < 17))
                    {
                        race_limited = true;
                    }
                }
                break;
        }
    }

    if (Cheats::no_race_level_limits)
    {
        race_limited = false;
    }

    return race_limited;
}

}  // namespace Classes

// engine/seg043.cpp - translation of coab engine/seg043.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

using namespace Logging;

void seg043::print_and_exit()
{
    if (in_print_and_exit == false)
    {
        in_print_and_exit = true;

        seg044::PlaySound(Sound::sound_FF);

        Logger::Close();

        ItemLibrary::Write();

        seg001::EngineStop();
    }
}


byte seg043::GetInputKey()
{
    byte key;

    if (gbl::inDemo == true)
    {
        if (seg049::KEYPRESSED() == true)
        {
            key = seg049::READKEY();
            if (gbl::portEnhancements)  // goldbox-3ds: any key ends the demo
            {
                while (seg049::KEYPRESSED() == true) seg049::READKEY();
                throw DemoAbort{};
            }
        }
        else
        {
            key = 0;
        }
    }
    else
    {
        key = seg049::READKEY();
    }

    if (key == 0x13)
    {
        seg044::PlaySound(Sound::sound_0);
    }

    if (Cheats::allow_keyboard_exit && key == 3)
    {
        print_and_exit();
    }

    if (key != 0)
    {
        while (seg049::KEYPRESSED() == true)
        {
            key = seg049::READKEY();
        }
    }

    return key;
}

void seg043::DumpPlayerAffects()
{
    for (Player player : gbl::TeamList)
    {
        for (Affect affect : player->affects)
        {
            Logger::Debug("who: {0}  sp#: {1} - {2}", player->name, (int)affect->type, affect->type);
        }
    }
}

void seg043::ToggleCommandDebugging()
{
    gbl::printCommands = !gbl::printCommands;

    if (gbl::printCommands == true)
    {
        Logger::Debug(DateTime::Now().ToString());
    }
}

void seg043::clear_keyboard()
{
    while (seg049::KEYPRESSED() == true)
    {
        GetInputKey();
    }
}


void seg043::clear_one_keypress()
{
    if (seg049::KEYPRESSED() == true)
    {
        GetInputKey();
    }
}


void seg043::DumpTreasureItems()
{
    //for (int i = 0; i < 0x81; i++)
    //{
    //    //var older = new System.Collections.Generic.List<string>();

    //    //for (int j = 0; j < 1000; j++)
    //    //{
    //    Item it = ovr022.create_item(i);
    //    it.name = it.GenerateName(0);
    //    Player pl = new Player();
    //    pl.field_151 = it;
    //    bool ranged = ovr025.is_weapon_ranged(pl);
    //    bool rangedMelee = ovr025.is_weapon_ranged_melee(pl);

    //    //if (older.Contains(name) == false)
    //    //{
    //    Logging.Logger.Debug("Id: {0} {1} Ranged: {2} Ranged-Melee: {3}", i, it.name, ranged, rangedMelee);
    //    //older.Add(name);
    //    //}
    //    //}
    //}
}

void seg043::TxtDumpPlayer(Player p, int area, int id)
{
    String str100 = p->stats2->Str.full == 18 ? Format("({0})", p->stats2->Str00.full) : String("");
    Logger::Debug("Area {0} Id {1} {2} exp: {3} hp: {4} ac: {5} thac0: {6}", area, id, p->name, p->exp, p->hit_point_max, p->DisplayAc(), 0x3c - p->hitBonus);
    Logger::Debug("   S: {0}{1} D: {2} C: {3} I: {4} W: {5} Ch: {6}", p->stats2->Str.full, str100, p->stats2->Dex.full, p->stats2->Con.full, p->stats2->Int.full, p->stats2->Wis.full, p->stats2->Cha.full);
    Logger::Debug("   Lvls: {0} {1} {2} {3} {4} {5} {6} {7}", p->ClassLevel[0], p->ClassLevel[1], p->ClassLevel[2], p->ClassLevel[3], p->ClassLevel[4], p->ClassLevel[5], p->ClassLevel[6], p->ClassLevel[7]);
    if (p->activeItems->primaryWeapon() != nullptr)
        Logger::Debug("   Weapon: {0}", p->activeItems->primaryWeapon()->GenerateName(0));
    if (p->activeItems->armor() != nullptr)
        Logger::Debug("   Armor: {0}", p->activeItems->armor()->GenerateName(0));

    Logger::Debug("   Damage: {0}d{1}{2}{3}", p->attack1_DiceCount, p->attack1_DiceSize,
        p->attack1_DamageBonus > 0 ? String("+") : String(""), p->attack1_DamageBonus != 0 ? ToString(p->attack1_DamageBonus) : String(""));

    for (int sp : p->spellList->IdList())
    {
        Logger::Debug("   Spell: {0}", ovr023::SpellNames[sp]);
    }

    for (auto af : p->affects)
    {
        Logger::Debug("   Affect: {0}", af->type);
    }
}


// PORT-NOTE: Logging::DebugWriter::Write/WriteLine take a finished string (no
// params object[]), so each `dw.Write(fmt, args)` becomes `dw->Write(Format(fmt, args))`.
void seg043::HtmlTableDumpPlayer(Ref<Logging::DebugWriter> dw, Player p, byte area, int id)
{
    dw->Write("<tr>");
    dw->Write(Format("<td>{0}</td>", area));
    dw->Write(Format("<td>{0}</td>", id));
    dw->Write(Format("<td nowrap=\"nowrap\">{0}</td>", p->name));
    dw->Write(Format("<td>{0}</td>", p->exp));
    dw->Write(Format("<td>{0}</td>", p->hit_point_max));
    dw->Write(Format("<td>{0}</td>", 0x3c - p->ac));
    dw->Write(Format("<td>{0}</td>", 0x3c - p->hitBonus));
    String str100 = p->stats2->Str.full == 18 ? Format("({0})", p->stats2->Str00.full) : String("");
    dw->Write(Format("<td>{0}{1}</td>", p->stats2->Str.full, str100));
    dw->Write(Format("<td>{0}</td>", p->stats2->Dex.full));
    dw->Write(Format("<td>{0}</td>", p->stats2->Con.full));
    dw->Write(Format("<td>{0}</td>", p->stats2->Int.full));
    dw->Write(Format("<td>{0}</td>", p->stats2->Wis.full));
    dw->Write(Format("<td>{0}</td>", p->stats2->Cha.full));
    dw->Write(Format("<td>{0}</td>", p->ClassLevel[0]));
    dw->Write(Format("<td>{0}</td>", p->ClassLevel[1]));
    dw->Write(Format("<td>{0}</td>", p->ClassLevel[2]));
    dw->Write(Format("<td>{0}</td>", p->ClassLevel[3]));
    dw->Write(Format("<td>{0}</td>", p->ClassLevel[4]));
    dw->Write(Format("<td>{0}</td>", p->ClassLevel[5]));
    dw->Write(Format("<td>{0}</td>", p->ClassLevel[6]));
    dw->Write(Format("<td>{0}</td>", p->ClassLevel[7]));

    dw->Write(Format("<td nowrap=\"nowrap\">{0}</td>", p->activeItems->primaryWeapon() != nullptr ? p->activeItems->primaryWeapon()->GenerateName(0) : String("")));
    dw->Write(Format("<td nowrap=\"nowrap\">{0}</td>", p->activeItems->armor() != nullptr ? p->activeItems->armor()->GenerateName(0) : String("")));
    dw->Write(Format("<td nowrap=\"nowrap\">{0}d{1}{2}{3}</td>", p->attack1_DiceCount, p->attack1_DiceSize,
        p->attack1_DamageBonus > 0 ? String("+") : String(""), p->attack1_DamageBonus != 0 ? ToString(p->attack1_DamageBonus) : String("")));

    int last = 0;
    int count = 0;
    String sb; // PORT-NOTE: System.Text.StringBuilder -> String

    for (int sp : p->spellList->IdList())
    {
        if (sp != last)
        {
            if (last != 0)
            {
                sb += ovr023::SpellNames[last];
                if (count > 1)
                {
                    sb += Format(" ({0})", count);
                }
                sb += ", ";
            }
            last = sp;
            count = 1;
        }
        else
        {
            count += 1;
        }
    }

    if (last != 0)
    {
        sb += ovr023::SpellNames[last];
        if (count > 1)
        {
            sb += Format(" ({0})", count);
        }
    }

    dw->Write(Format("<td nowrap=\"nowrap\">{0}</td>", sb));


    sb = String();
    for (auto af : p->affects)
    {
        sb += ToString(af->type);
        sb += ", ";
    }
    if (sb.Length() > 0)
    {
        sb = sb.Remove(sb.Length() - 2, 2);
    }

    dw->Write(Format("<td nowrap=\"nowrap\">{0}</td>", sb));
    dw->WriteLine("</tr>");
}

void seg043::DumpMonstersFiltered()
{
    auto bkupArea = gbl::game_area;

    String filename = IO::Path::Combine(Logger::GetPath(), "MonsterFiltered.txt");
    if (IO::File::Exists(filename))
    {
        IO::File::Delete(filename);
    }
    Ref<Logging::DebugWriter> dw = Ref<Logging::DebugWriter>(std::make_shared<Logging::DebugWriter>(filename));

    dw->WriteLine("GnomeVsManSizedGiant");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->field_14B & 2) != 0; });
    dw->WriteLine("");

    dw->WriteLine("monsterType == 1");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::type_1); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == giant");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::giant); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == dragon");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::dragon); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == animated_dead");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::animated_dead); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == 9");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::type_9); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == fire");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::fire); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == 10");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::troll); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == 12");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::type_12); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == snake");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::snake); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == plant");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::plant); });
    dw->WriteLine("");

    dw->WriteLine("monsterType == animal");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType == MonsterType::animal); });
    dw->WriteLine("");

    dw->WriteLine("monsterType > type_1");
    DumpMonstersFilteredSub(dw, [&](Player p) { return (p->monsterType > MonsterType::type_1); });
    dw->WriteLine("");


    dw->Close();

    gbl::game_area = bkupArea;
}

void seg043::DumpMonstersFilteredSub(Ref<Logging::DebugWriter> dw, std::function<bool(Player)> filter)
{
    for (byte area = 1; area <= 6; area++)
    {
        gbl::game_area = area;
        for (int id = 0; id < 256; id++)
        {
            Player p = ovr017::load_mob(id, false);
            if (p != nullptr)
            {
                ovr025::reclac_player_values(p);

                if (filter(p))
                {
                    dw->WriteLine(p->ToString());
                }
            }
        }
    }
}

void seg043::DumpMonsters()
{
    DumpMonstersFiltered();

    auto bkupArea = gbl::game_area;

    String filename = IO::Path::Combine(Logger::GetPath(), "Monster.html");
    if (IO::File::Exists(filename))
    {
        IO::File::Delete(filename);
    }
    Ref<Logging::DebugWriter> dw = Ref<Logging::DebugWriter>(std::make_shared<Logging::DebugWriter>(filename));

    dw->WriteLine("<html><body><table><tbody>");

    for (byte area = 1; area <= 6; area++)
    {
        gbl::game_area = area;
        for (int id = 0; id < 256; id++)
        {
            Player p = ovr017::load_mob(id, false);
            if (p != nullptr)
            {
                ovr025::reclac_player_values(p);

                //TxtDumpPlayer(p, area, id);
                HtmlTableDumpPlayer(dw, p, area, id);
            }
        }
    }

    dw->WriteLine("</tbody></table></body></html>");
    dw->Close();

    gbl::game_area = bkupArea;
}

}  // namespace engine

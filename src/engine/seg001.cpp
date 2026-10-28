// engine/seg001.cpp - translation of coab engine/seg001.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

// PORT-NOTE: `internal static System.Threading.Thread EngineThread;` dropped (see decl/seg001.h).

void seg001::EngineStop()
{
    EngineStoppedCallback();
    // PORT-NOTE: was `EngineThread.Abort();` - unwind the engine thread with an exception instead.
    throw EngineAbort();
}

void seg001::__SystemInit(VoidDelegate stoppedCallback)
{
    // PORT-NOTE: `EngineThread = System.Threading.Thread.CurrentThread;` dropped.
    EngineStoppedCallback = stoppedCallback;

    ConfigGame();
}

void seg001::ConfigGame()
{
    gbl::exe_path = IO::Directory::GetCurrentDirectory();

    seg044::SoundInit();
}

void seg001::PROGRAM()
{
    /* Memory Init - Start */
    gbl::CombatMap = Array<CombatantMap>(gbl::MaxCombatantCount + 1); /* God damm 1-n arrays */
    for (int i = 0; i <= gbl::MaxCombatantCount; i++)
    {
        gbl::CombatMap[i] = New<CombatantMap_>();
    }
    /* Memory Init - End */

    ovr003::SetupCommandTable();

    InitFirst();

    ItemLibrary::Read();

    seg044::PlaySound(Sound::sound_0);

    //Logging.Logger.Debug("Field_6 & 0x0F == 0");
    //foreach (var s in gbl.spellCastingTable )
    //{
    //    if (s != null && (s.field_6 & 0x0f) == 0)
    //    {
    //        Logging.Logger.Debug("{0} {1}", s.spellIdx, (Spells)s.spellIdx);
    //    }
    //}
    //Logging.Logger.Debug("");
    //Logging.Logger.Debug("Field_6 & 0x0F == 5");
    //foreach (var s in gbl.spellCastingTable)
    //{
    //    if (s != null && (s.field_6 & 0x0f) == 5)
    //    {
    //        Logging.Logger.Debug("{0} {1}", s.spellIdx, (Spells)s.spellIdx);
    //    }
    //}
    //Logging.Logger.Debug("");
    //Logging.Logger.Debug("Field_6 & 0x0F == 15");
    //foreach (var s in gbl.spellCastingTable)
    //{
    //    if (s != null && (s.field_6 & 0x0f) == 15)
    //    {
    //        Logging.Logger.Debug("{0} {1}", s.spellIdx, (Spells)s.spellIdx);
    //    }
    //}
    //Logging.Logger.Debug("");
    //Logging.Logger.Debug("Field_6 & 0x0F >= 8 <= 14");
    //foreach (var s in gbl.spellCastingTable)
    //{
    //    if (s != null)
    //    {
    //        int v = s.field_6 & 0x0f;
    //        if (v >= 8 && v <= 14)
    //        {
    //            Logging.Logger.Debug("{0} {1}", s.spellIdx, (Spells)s.spellIdx);

    //        }
    //    }
    //}
    //Logging.Logger.Debug("");
    //Logging.Logger.Debug("Field_6 & 0x0F otherwise");
    //foreach (var s in gbl.spellCastingTable)
    //{
    //    if (s != null)
    //    {
    //        int v = s.field_6 & 0x0f;
    //        if (v >= 1 && v <= 7 && v != 5)
    //        {
    //            Logging.Logger.Debug("{0} {1} {2}", s.spellIdx, (Spells)s.spellIdx, (v & 3)+1);
    //        }
    //    }
    //}
    //Logging.Logger.Debug("");



    if (Cheats::skip_title_screen == false)
    {
        ovr002::title_screen();
    }

    gbl::displayInputSecondsToWait = 30;
    gbl::displayInputTimeoutValue = 'D';
    if (gbl::portEnhancements) gbl::menuSelectedWord = 0;  // goldbox-3ds: A/Enter = Play, not Demo

    char inputKey = ovr027::displayInput(false, 0, gbl::defaultMenuColors, "Play Demo", "Curse of the Azure Bonds v1.3 ");

    gbl::displayInputSecondsToWait = 0;
    gbl::displayInputTimeoutValue = '\0';

    if (inputKey == 'D')
    {
        gbl::inDemo = true;
    }

    if (Cheats::skip_copy_protection == false &&
        gbl::inDemo == false)
    {
        ovr004::copy_protection();
    }

    while (true)
    {
        if (gbl::inDemo == true)
        {
            gbl::game_area = 1;
            gbl::game_speed_var = 9;
        }
        else
        {
            gbl::game_area = 2;
        }

        if (gbl::inDemo == false)
        {
            ovr018::startGameMenu();
        }

        ovr003::sub_29758();

        InitAgain();

        if (gbl::inDemo == true)
        {
            ovr002::title_screen();
            seg043::clear_keyboard();

            gbl::displayInputSecondsToWait = 10;
            gbl::displayInputTimeoutValue = 'D';
            if (gbl::portEnhancements) gbl::menuSelectedWord = 0;  // goldbox-3ds: A/Enter = Play

            inputKey = ovr027::displayInput(false, 0, gbl::defaultMenuColors, "Play Demo", "Curse of the Azure Bonds v1.3 ");

            gbl::displayInputSecondsToWait = 0;
            gbl::displayInputTimeoutValue = '\0';

            gbl::inDemo = (inputKey == 'D');

            if (Cheats::skip_copy_protection == false &&
                gbl::inDemo == false)
            {
                ovr004::copy_protection();
            }

            seg044::PlaySound(Sound::sound_0);
        }
    }
}

void seg001::InitFirst() /* sub_39054 */
{
    seg051::Randomize();

    gbl::area_ptr = New<Area1_>();
    gbl::area2_ptr = New<Area2_>();
    gbl::stru_1B2CA = New<Struct_1B2CA_>();
    gbl::ecl_ptr = New<EclBlock_>();
    gbl::dax_8x8d1_201 = Array2<byte>(177, 8);
    gbl::geo_ptr->LoadData(Array<byte>(0x402));

    ovr016::BuildEffectNameMap();

    for (int i = 0; i < gbl::cmdOppsLimit; i++)
    {
        gbl::cmd_opps[i] = New<Opperation_>();
        gbl::cmd_opps[i]->getMemoryValue = ovr008::vm_GetMemoryValue;
    }

    gbl::cursor_bkup = New<DaxBlock_>(0, 1, 1, 8);
    gbl::cursor = New<DaxBlock_>(0, 1, 1, 8);

    seg051::FillChar(0xf, gbl::cursor->bpp, gbl::cursor->data);

    gbl::symbol_8x8_set = Array<DaxBlock>(5);
    gbl::symbol_8x8_set[0] = nullptr;
    gbl::symbol_8x8_set[1] = nullptr;
    gbl::symbol_8x8_set[2] = nullptr;
    gbl::symbol_8x8_set[3] = nullptr;
    gbl::symbol_8x8_set[4] = nullptr;

    gbl::dax24x24Set = nullptr;
    gbl::dword_1C8FC = nullptr;

    gbl::dax24x24Set = New<DaxBlock_>(0, 0x30, 3, 0x18);

    gbl::area_ptr->Clear();

    gbl::area_ptr->inDungeon = 1;
    gbl::area_ptr->LastEclBlockId = 0;

    gbl::area2_ptr->Clear();

    gbl::stru_1B2CA->Clear();
    gbl::ecl_ptr->Clear();


    gbl::combat_icons = Array<CombatIcon>(26);
    for (int i = 0; i < 26; i++)
    {
        gbl::combat_icons[i] = New<CombatIcon_>();
    }

    gbl::byte_1AD44 = 2;
    gbl::current_head_id = 0xff;
    gbl::current_body_id = 0xff;
    gbl::headX_dax = nullptr;
    gbl::bodyX_dax = nullptr;

    gbl::byte_1D556 = New<DaxArray_>();

    gbl::bigpic_dax = nullptr;
    gbl::items_pointer = List<Item>();

    gbl::mapPosX = 0;
    gbl::mapPosY = 0;
    gbl::mapDirection = 0;
    gbl::mapWallType = 0;
    gbl::mapWallRoof = 0;

    gbl::mapPosX = 7;
    gbl::mapPosY = 0x0D;
    gbl::mapDirection = 0;

    gbl::can_bash_door = true;
    gbl::can_pick_door = true;
    gbl::can_knock_door = true;

    gbl::byte_1AD44 = 3;

    gbl::setBlocks[0] = New<gbl::SetBlock_>(1, 0);
    gbl::setBlocks[1] = New<gbl::SetBlock_>();
    gbl::setBlocks[2] = New<gbl::SetBlock_>();

    //gbl.AnimationsOn = true;
    //gbl.PicsOn = true;
    gbl::DelayBetweenCharacters = true;
    gbl::reload_ecl_and_pictures = false;
    gbl::rest_incounter_count = 0;

    gbl::TeamList.Clear();
    gbl::SelectedPlayer = nullptr;

    gbl::ecl_offset = 0x8000;
    gbl::game_speed_var = 4;
    gbl::inDemo = false;
    gbl::game_area = 1;
    gbl::game_area_backup = 1;
    gbl::mapAreaDisplay = false;
    gbl::area2_ptr->party_size = 0;
    gbl::menuScreenIndex = 1;
    gbl::combat_type = CombatType::normal;
    gbl::displayPlayerStatusLine18 = false;
    gbl::search_flag_bkup = 0;
    gbl::spriteChanged = false;
    gbl::party_killed = false;
    gbl::byte_1BF12 = 1;
    gbl::displayPlayerSprite = false;
    gbl::lastDaxFile = String::Empty;
    gbl::byte_1D5AB = String::Empty;
    gbl::lastDaxBlockId = 0x0FF;
    gbl::byte_1D5B5 = 0x0FF;
    gbl::gameSaved = false;
    gbl::byte_1EE95 = false;
    gbl::focusCombatAreaOnPlayer = true;
    gbl::bigpic_block_id = 0x0FF;
    gbl::silent_training = false;
    gbl::menuSelectedWord = 1;
    gbl::game_state = GameState::DungeonMap;
    gbl::last_game_state = (GameState)0;
    gbl::applyItemAffect = false;
    gbl::sky_dax_250 = nullptr;
    gbl::sky_dax_251 = nullptr;
    gbl::sky_dax_252 = nullptr;
    gbl::gameWon = false;
    seg041::Load8x8Tiles();
    ovr027::ClearPromptArea();
    seg041::displayString("Loading...Please Wait", 0, 10, 0x18, 0);

    ovr038::Load8x8D(4, 0xca);
    ovr038::Load8x8D(0, 0xcb);

    for (gbl::byte_1AD44 = 0; gbl::byte_1AD44 <= 0x0b; gbl::byte_1AD44++)
    {
        ovr034::chead_cbody_comspr_icon((byte)(gbl::byte_1AD44 + 0x0D), gbl::byte_1AD44, "COMSPR");
    }

    ovr034::chead_cbody_comspr_icon(0x19, 0x19, "COMSPR");

    gbl::sky_dax_250 = seg040::LoadDax(13, 1, 250, "SKY");
    gbl::sky_dax_251 = seg040::LoadDax(13, 1, 251, "SKY");
    gbl::sky_dax_252 = seg040::LoadDax(13, 1, 252, "SKY");

    gbl::ItemDataTable = New<ItemDataTable_>("ITEMS");

    ovr023::setup_spells();
    ovr013::SetupAffectTables();
}


void seg001::InitAgain() /* sub_396E5 */
{
    gbl::area_ptr->Clear();
    gbl::area_ptr->inDungeon = 1;
    gbl::area_ptr->LastEclBlockId = 0;
    gbl::area2_ptr->Clear();
    gbl::stru_1B2CA->Clear();
    gbl::ecl_ptr->Clear();

    gbl::mapPosX = 0;
    gbl::mapPosY = 0;
    gbl::mapDirection = 0;
    gbl::mapWallType = 0;
    gbl::mapWallRoof = 0;

    gbl::mapPosX = 7;
    gbl::mapPosY = 0x0D;
    gbl::mapDirection = 2;

    gbl::can_bash_door = true;
    gbl::can_pick_door = true;
    gbl::can_knock_door = true;

    gbl::byte_1AD44 = 3;

    gbl::setBlocks[0]->blockId = 0;
    gbl::setBlocks[0]->setId = 1;
    gbl::setBlocks[1]->Reset();
    gbl::setBlocks[2]->Reset();

    gbl::DelayBetweenCharacters = true;
    gbl::reload_ecl_and_pictures = false;
    gbl::rest_incounter_count = 0;

    gbl::TeamList.Clear();
    gbl::SelectedPlayer = nullptr;

    gbl::ecl_offset = 0x8000;
    gbl::game_speed_var = 4;
    gbl::game_area = 1;
    gbl::game_area_backup = 1;
    gbl::mapAreaDisplay = false;
    gbl::area2_ptr->party_size = 0;
    gbl::menuScreenIndex = 1;
    gbl::combat_type = CombatType::normal;
    gbl::displayPlayerStatusLine18 = false;
    gbl::search_flag_bkup = 0;
    gbl::spriteChanged = false;
    gbl::party_killed = false;
    gbl::byte_1BF12 = 1;
    gbl::displayPlayerSprite = false;
    gbl::lastDaxFile = String::Empty;
    gbl::byte_1D5AB = String::Empty;
    gbl::lastDaxBlockId = 0x0FF;
    gbl::byte_1D5B5 = 0x0FF;
    gbl::gameSaved = false;
    gbl::byte_1EE95 = false;
    gbl::focusCombatAreaOnPlayer = true;
    gbl::bigpic_block_id = 0x0FF;
    gbl::silent_training = false;
    ovr027::ClearPromptArea();
    gbl::menuSelectedWord = 1;
    gbl::game_state = GameState::DungeonMap;
    gbl::last_game_state = (GameState)0;
    gbl::applyItemAffect = false;
    gbl::gameWon = false;
}

}  // namespace engine

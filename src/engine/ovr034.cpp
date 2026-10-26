// engine/ovr034.cpp - translation of coab engine/ovr034.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

void ovr034::Load24x24Set(int cellCount, int destCellOffset, int block_id, String fileName)
{
    if (destCellOffset > 0x30)
    {
        Logging::Logger::LogAndExit("Start range error in Load24x24Set. {0}", destCellOffset);
    }

    DaxBlock tmp_block = seg040::LoadDax(0, 0, block_id, fileName);

    int dateLength = cellCount * tmp_block->bpp;
    int destByteOffset = destCellOffset * tmp_block->bpp;

    if (gbl::dax24x24Set != nullptr)
    {
        Array<byte>::Copy(tmp_block->data, 0, gbl::dax24x24Set->data, destByteOffset, dateLength);
    }

    seg043::clear_keyboard();
}


void ovr034::DrawIsoTile(int tileIndex, int rowY, int colX) /* sub_760F7 */
{
    if (tileIndex > 0x7f)
    {
        seg040::OverlayUnbounded(gbl::dword_1C8FC, tileIndex, tileIndex & 0x7F, rowY, colX);
    }
    else
    {
        seg040::OverlayUnbounded(gbl::dax24x24Set, 0, tileIndex, rowY, colX);
    }
}


void ovr034::ReleaseCombatIcon(int index) // free_icon
{
    gbl::combat_icons[index]->Release();
}

void ovr034::chead_cbody_comspr_icon(byte combat_icon_index, int block_id, String fileText)
{
    String file_text = fileText;

    String sub = seg051::Copy(5, 0, file_text);
    if (sub == "CHEAD" ||
        sub == "CBODY")
    {
        if (Char::ToUpper(file_text[file_text.Length() - 1]) == 'T')
        {
            block_id += 0x40;
        }

        file_text = seg051::Copy(file_text.Length() - 1, 0, file_text);

        gbl::combat_icons[combat_icon_index]->LoadIcons(0, 1, file_text, block_id, block_id + 0x80);
    }
    else if (file_text == "COMSPR" || file_text == "ICON")
    {
        gbl::combat_icons[combat_icon_index]->LoadIcons(0, 1, file_text, block_id, block_id + 0x80);

        if (file_text == "ICON")
        {
            gbl::combat_icons[combat_icon_index]->Recolor(false, unk_16E50, unk_16E30);
        }
    }
    else
    {
        file_text += ToString(gbl::game_area);

        gbl::combat_icons[combat_icon_index]->LoadIcons(0, 1, file_text, block_id, block_id + 0x80);
        gbl::combat_icons[combat_icon_index]->Recolor(false, unk_16E40, unk_16E30);
    }

    seg043::clear_keyboard();
}


void ovr034::draw_combat_icon(int iconIndex, Icon iconState, int direction, int tileY, int tileX) /* sub_76504 */
{
    DaxBlock icon = gbl::combat_icons[iconIndex]->GetIcon(iconState, direction);

    if (icon != nullptr)
    {
        seg040::draw_combat_picture(icon, (tileY * 3) + 1, (tileX * 3) + 1, 0);
    }
}

}  // namespace engine

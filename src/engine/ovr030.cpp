// engine/ovr030.cpp - translation of coab engine/ovr030.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

void ovr030::DrawMaybeOverlayed(DaxBlock dax_block, bool useOverlay, int rowY, int colX)// sub_7000A
{
    if (dax_block != nullptr)
    {
        if (gbl::area_ptr->picture_fade > 0 || useOverlay == true)
        {
            if (gbl::area_ptr->picture_fade > 0)
            {
                dax_block->Recolor(true, fadeNewColors, fadeOldColors);
            }

            seg040::OverlayBounded(dax_block, 0, 0, rowY - 1, colX - 1);
            seg040::DrawOverlay();
        }
        else
        {
            seg040::draw_picture(dax_block, rowY, colX, 0);
        }
    }
}


void ovr030::load_pic_final(DaxArray& daxArray, byte masked, byte block_id, String file_name)
{
    if (file_name != gbl::lastDaxFile ||
        block_id != gbl::lastDaxBlockId)
    {
        if (block_id != 0xff)
        {
            if (gbl::AnimationsOn)
            {
                ovr027::ClearPromptAreaNoUpdate();
                seg041::displayString("Loading...Please Wait", 0, 10, 0x18, 0);
            }

            DaxArrayFreeDaxBlocks(daxArray);

            gbl::lastDaxFile = file_name;
            gbl::lastDaxBlockId = block_id;

            bool is_pic_or_final = (file_name == "PIC" || file_name == "FINAL");

            short uncompressed_size;
            Array<byte> uncompressed_data;

            seg042::load_decode_dax(uncompressed_data, uncompressed_size, block_id, file_name + ToString(gbl::game_area) + ".dax");

            if (uncompressed_size == 0)
            {
                seg041::DisplayAndPause("PIC not found", 14);
            }
            else
            {
                int src_offset = 0;

                daxArray->numFrames = uncompressed_data[src_offset];
                src_offset++;
                daxArray->curFrame = 1;

                byte frames_count = 0; // kind of pointless...

                if (gbl::AnimationsOn == false && is_pic_or_final == true)
                {
                    daxArray->numFrames = 1;
                }

                Array<byte> first_frame_ega_layout = nullptr;

                for (int frame = 0; frame < daxArray->numFrames; frame++)
                {
                    daxArray->frames[frame]->delay = Sys::ArrayToInt(uncompressed_data, src_offset);
                    src_offset += 4;

                    short height = Sys::ArrayToShort(uncompressed_data, src_offset);
                    src_offset += 2;

                    short width = Sys::ArrayToShort(uncompressed_data, src_offset);
                    src_offset += 2;

                    frames_count++;

                    daxArray->frames[frame]->picture = New<DaxBlock_>(masked, 1, width, height);

                    DaxBlock dax_block = daxArray->frames[frame]->picture;

                    dax_block->x_pos = Sys::ArrayToShort(uncompressed_data, src_offset);
                    src_offset += 2;

                    dax_block->y_pos = Sys::ArrayToShort(uncompressed_data, src_offset);
                    src_offset += 3;

                    Array<byte>::Copy(uncompressed_data, src_offset, dax_block->field_9, 0, 8);
                    src_offset += 8;

                    int ega_encoded_size = (daxArray->frames[frame]->picture->bpp / 2) - 1;

                    if (is_pic_or_final == true)
                    {
                        if (frame == 0)
                        {
                            first_frame_ega_layout = Array<byte>(ega_encoded_size + 1);

                            Array<byte>::Copy(uncompressed_data, src_offset, first_frame_ega_layout, 0, ega_encoded_size + 1);
                        }
                        else
                        {
                            for (int i = 0; i < ega_encoded_size; i++)
                            {
                                byte b = first_frame_ega_layout[i];
                                uncompressed_data[src_offset + i] ^= b;
                            }
                        }
                    }

                    daxArray->frames[frame]->picture->DaxToPicture(0, masked, src_offset, uncompressed_data);

                    if ((masked & 1) > 0)
                    {
                        daxArray->frames[frame]->picture->Recolor(false, transparentNewColors, transparentOldColors);
                    }

                    src_offset += ega_encoded_size + 1;
                }

                daxArray->numFrames = frames_count; // also pointless

                uncompressed_data = nullptr;
                seg043::clear_keyboard();

                if (gbl::AnimationsOn)
                {
                    ovr027::ClearPromptAreaNoUpdate();
                }
            }
        }
    }
}


void ovr030::DaxArrayFreeDaxBlocks(DaxArray animation)
{
    for (int index = 0; index < animation->numFrames; index++)
    {
        animation->frames[index]->picture = nullptr;
        animation->frames[index]->delay = 0;
    }

    animation->numFrames = 0;
    animation->curFrame = 0;

    gbl::lastDaxFile = String::Empty;
    gbl::lastDaxBlockId = 0x0FF;
}


void ovr030::head_body(byte body_id, byte head_id)
{
    String text = ToString(gbl::game_area);

    if (head_id != 0xff &&
        (gbl::current_head_id == 0xff || gbl::current_head_id != head_id))
    {
        gbl::headX_dax = seg040::LoadDax(0, 0, head_id, String("HEAD") + text);

        if (gbl::headX_dax == nullptr)
        {
            seg041::DisplayAndPause("head not found", 14);
        }

        gbl::current_head_id = head_id;
    }

    if (body_id != 0xff &&
        (gbl::current_body_id == 0xff || gbl::current_body_id != body_id))
    {
        gbl::bodyX_dax = seg040::LoadDax(0, 0, body_id, String("BODY") + text);
        if (gbl::bodyX_dax == nullptr)
        {
            seg041::DisplayAndPause("body not found", 14);
        }

        gbl::current_body_id = body_id;
    }

    seg043::clear_keyboard();
}


void ovr030::draw_head_and_body(bool draw_body, byte rowY, byte colX) /* sub_706DC */
{
    if (draw_body == true)
    {
        DrawMaybeOverlayed(gbl::headX_dax, false, rowY, colX);
        DrawMaybeOverlayed(gbl::bodyX_dax, false, rowY + 5, colX);
    }
    else
    {
        DrawMaybeOverlayed(gbl::headX_dax, false, rowY, colX);
    }
}


void ovr030::Show3DSprite(DaxArray arg_0, int sprite_index)
{
    if (sprite_index < 1 || sprite_index > 3)
    {
        Logging::Logger::LogAndExit("Illegal range in Show3DSprite. {0}", sprite_index);
    }

    if (arg_0->frames[sprite_index - 1]->picture != nullptr)
    {
        DaxBlock block = arg_0->frames[sprite_index - 1]->picture;
        seg040::OverlayBounded(arg_0->frames[sprite_index - 1]->picture, 1, 0, block->y_pos + 3 - 1, block->x_pos + 3 - 1);
        seg040::DrawOverlay();
    }
}


void ovr030::load_bigpic(byte block_id) /* bigpic */
{
    DaxArrayFreeDaxBlocks(gbl::byte_1D556);

    if (gbl::bigpic_block_id != block_id)
    {
        gbl::bigpic_dax = seg040::LoadDax(0, 0, block_id, String("bigpic") + ToString(gbl::game_area));
        gbl::bigpic_block_id = block_id;
    }
}


void ovr030::draw_bigpic() /* sub_7087A */
{
    seg037::DrawFrame_WildernessMap();
    seg040::draw_picture(gbl::bigpic_dax, 1, 1, 0);
}

}  // namespace engine

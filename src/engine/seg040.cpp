#include "engine/engine.h"

namespace engine {

DaxBlock seg040::LoadDax(byte mask_colour, byte masked, int block_id, String fileName) // load_dax
{
    short pic_size;
    Array<byte> pic_data;
    seg042::load_decode_dax(pic_data, pic_size, block_id, fileName + ".dax");

    if (pic_size != 0)
    {
        return New<DaxBlock_>(pic_data, masked, mask_colour);
    }

    return nullptr;
}



void seg040::OverlayUnbounded(DaxBlock source, int arg_8, int itemIdex, int rowY, int colX)
{
    draw_combat_picture(source, rowY + 1, colX + 1, itemIdex);
}


void seg040::OverlayBounded(DaxBlock source, int arg_8, int itemIndex, int rowY, int colX) /* sub_E353 */
{
    draw_combat_picture(source, rowY + 1, colX + 1, itemIndex);
}


void seg040::ega_backup(DaxBlock dax_block, int rowY, int colX) /* ega_01 */
{
    if (dax_block != nullptr)
    {
        int offset = 0;

        int minY = rowY * 8;
        int maxY = minY + dax_block->height;

        int minX = colX * 8;
        int maxX = minX + (dax_block->width * 8);

        for (int pixY = minY; pixY < maxY; pixY++)
        {
            for (int pixX = minX; pixX < maxX; pixX++)
            {
                dax_block->data[offset] = Display::GetPixel(pixX, pixY);
                offset++;
            }
        }
    }
}

void seg040::draw_clipped_recolor(int from, int to)
{
    color_re_color_from = from;
    color_re_color_to = to;
}

void seg040::draw_clipped_nodraw(int color)
{
    color_no_draw = color;
}

void seg040::draw_clipped_picture(DaxBlock dax_block, int rowY, int colX, int index,
    int clipMinX, int clipMaxX, int clipMinY, int clipMaxY)
{
    if (dax_block != nullptr)
    {
        int offset = index * dax_block->bpp;

        int minY = rowY * 8;
        int maxY = minY + dax_block->height;

        int minX = colX * 8;
        int maxX = minX + (dax_block->width * 8);

        for (int pixY = minY; pixY < maxY; pixY++)
        {
            for (int pixX = minX; pixX < maxX; pixX++)
            {
                if (pixX >= clipMinX && pixX < clipMaxX &&
                    pixY >= clipMinY && pixY < clipMaxY)
                {
                    byte color = dax_block->data[offset];

                    if (color == color_no_draw)
                    { }
                    else if (color == color_re_color_from)
                    {
                        Display::SetPixel3(pixX, pixY, color_re_color_to);
                    }
                    else
                    {
                        Display::SetPixel3(pixX, pixY, color);
                    }
                }

                offset++;
            }
        }

        Display::Update();
    }
}

void seg040::draw_combat_picture(DaxBlock dax_block, int rowY, int colX, int index)
{
    draw_clipped_picture(dax_block, rowY, colX, index, 8, 176, 8, 176);
}

void seg040::draw_picture(DaxBlock dax_block, int rowY, int colX, int index)
{
    draw_clipped_picture(dax_block, rowY, colX, index, 0, 320, 0, 200);
}

void seg040::DrawOverlay()
{
    //TODO this might be useful when we move to OpenGL.
}

void seg040::SetPaletteColor(int color, int index)
{
    int newColor = color;

    //if (color >= 8)
    //{
    //  newColor += 8;
    //}

    Display::SetEgaPalette(index, newColor);
}


void seg040::DrawColorBlock(int color, int lineCount, int colWidth, int lineY, int colX)
{
    int minY = lineY + 8;
    int maxY = minY + lineCount;

    int minX = (colX * 8) + 8;
    int maxX = minX + (colWidth * 8);

    for (int pixY = minY; pixY < maxY; pixY++)
    {
        for (int pixX = minX; pixX < maxX; pixX++)
        {
            if (pixX >= 0 && pixX < 320 && pixY >= 0 && pixY < 200)
            {
                Display::SetPixel3(pixX, pixY, color);
            }
        }
    }
}

}  // namespace engine

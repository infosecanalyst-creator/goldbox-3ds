// Classes/DaxFiles/DaxBlock.cpp - port of Classes/DaxFiles/DaxBlock.cs (namespace Classes)
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

// static Random random_number: initialised in DaxBlock.h, seeded through
// Determinism::SeedFromTicks(DateTime::Now().Ticks) (PORT-NOTE there).

DaxBlock_::DaxBlock_(int masked, int _item_count, int _width, int _height)
{
    height = _height;
    width = _width;
    bpp = height * width * 8;
    item_count = _item_count;
    int ram_size = item_count * bpp;

    data = Array<byte>(ram_size);
    field_9 = Array<byte>(8);
}

DaxBlock_::DaxBlock_(Array<byte> pic_data, int masked, int mask_color)
{
    height = Sys::ArrayToShort(pic_data, 0);
    width = Sys::ArrayToShort(pic_data, 2);
    bpp = height * width * 8;
    int x_pos = Sys::ArrayToShort(pic_data, 4);
    int y_pos = Sys::ArrayToShort(pic_data, 6);
    item_count = pic_data[8];

    field_9 = Array<byte>(8);
    Array<byte>::Copy(pic_data, 9, field_9, 0, 8);

    int ram_size = item_count * bpp;
    data = Array<byte>(ram_size);

    int pic_data_offset = 17;
    DaxToPicture(mask_color, masked, pic_data_offset, pic_data);
}

void DaxBlock_::FlipIconLeftToRight()
{
    Array<byte> t_data = Array<byte>(data.Length());

    int t_width = width * 8;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < t_width; x++)
        {
            int di = (y * t_width) + x;
            int si = (y * t_width) + (t_width - x) - 1;

            t_data[di] = data[si];
        }
    }

    Array<byte>::Copy(t_data, data, data.Length());
}

void DaxBlock_::Recolor(bool useRandom, Array<byte> newColors, Array<byte> oldColors)
{
    for (int colorIdx = 0; colorIdx < 16; colorIdx++)
    {
        if (oldColors[colorIdx] != newColors[colorIdx])
        {
            int offset = 0;

            for (int posY = 0; posY < height; posY++)
            {
                for (int posX = 0; posX < (width * 8); posX++)
                {
                    if (data[offset] == oldColors[colorIdx] &&
                        (useRandom == false || ((random_number.Next() % 4) == 0)))
                    {
                        data[offset] = newColors[colorIdx];
                    }

                    offset += 1;
                }
            }
        }
    }
}

void DaxBlock_::MergeIcons(DaxBlock srcIcon) /* icon_xx, could be implemented using alpha-blending */
{
    for (int i = 0; i < srcIcon->bpp; i++)
    {
        byte a = data[i];
        byte b = srcIcon->data[i];

        if (a == 16 && b == 16)
        {
            data[i] = 16;
        }
        else if (a == 16)
        {
            data[i] = b;
        }
        else if (b == 16)
        {
            data[i] = a;
        }
        else
        {
            //TODO - not sure about this... more likely there should be a presedant, not just blending on the color code..
            data[i] = (byte)(a | b);
        }
    }
}

void DaxBlock_::DaxToPicture(int mask_colour, int masked, int block_offset, Array<byte> t_data)
{
    int dest_offset = 0;

    for (int loop1_var = 1; loop1_var <= item_count; loop1_var++)
    {
        for (int loop2_var = 0; loop2_var < height; loop2_var++)
        {
            for (int loop3_var = 0; loop3_var < (width * 4); loop3_var++)
            {
                byte c = t_data[block_offset];

                SetMaskedColor(dest_offset, c >> 4, masked, mask_colour);

                dest_offset += 1;

                SetMaskedColor(dest_offset, c & 0x0F, masked, mask_colour);

                dest_offset += 1;
                block_offset += 1;
            }
        }
    }
}

void DaxBlock_::SetMaskedColor(int offset, int color, int masked, int mask_color)
{
    if (masked == 1 && color == mask_color)
    {
        data[offset] = 16;
    }
    else
    {
        data[offset] = (byte)color;
    }
}

}  // namespace Classes

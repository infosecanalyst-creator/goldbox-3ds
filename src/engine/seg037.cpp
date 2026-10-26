// engine/seg037.cpp - translation of coab engine/seg037.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

void seg037::DrawFrame_Outer() /* draw8x8_01 */
{
    Display::UpdateStop();

    draw8x8_clear_area(0x16, 0x26, 1, 1);

    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, outer_frame_top[col_x] + 0x11e, 0, col_x);
    }

    for (int row_y = 0; row_y < 0x17; row_y++)
    {
        ovr038::Put8x8Symbol(0, false, outer_frame_left[row_y] + 0x11e, row_y, 0);
        ovr038::Put8x8Symbol(0, false, outer_frame_right[row_y] + 0x11e, row_y, 0x27);
    }

    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, outer_frame_bottom[col_x] + 0x11e, 0x17, col_x);
    }

    Display::UpdateStart();
}


void seg037::draw8x8_02()
{
    Display::UpdateStop();

    DrawFrame_Outer();

    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, x8x8_07[col_x] + 0x11E, 3, col_x);
        ovr038::Put8x8Symbol(0, false, x8x8_07[col_x] + 0x11E, 8, col_x);
    }

    Display::UpdateStart();
}


void seg037::draw8x8_03()
{
    Display::UpdateStop();

    DrawFrame_Outer();

    for (int colX = 0; colX <= 0x27; colX++)
    {
        ovr038::Put8x8Symbol(0, false, x8x8_07[colX] + 0x11E, 0x10, colX);
    }

    for (int rowY = 0; rowY <= 0x10; rowY++)
    {
        ovr038::Put8x8Symbol(0, false, unk_16F0A[rowY] + 0x11E, rowY, 0x10);
    }

    for (int col_x = 2; col_x <= 14; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, unk_16ED6[col_x] + 0x114, 2, col_x);
        ovr038::Put8x8Symbol(0, false, unk_16EE3[col_x] + 0x114, 14, col_x);
    }

    for (int row_y = 2; row_y <= 14; row_y++)
    {
        ovr038::Put8x8Symbol(0, false, unk_16F31[row_y] + 0x114, row_y, 2);
        ovr038::Put8x8Symbol(0, false, unk_16F3E[row_y] + 0x114, row_y, 14);
    }

    Display::UpdateStart();
}


void seg037::DrawFrame_WildernessMap() // draw8x8_04
{
    Display::UpdateStop();

    DrawFrame_Outer();

    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, x8x8_07[col_x] + 0x11E, 0x10, col_x);
    }

    Display::UpdateStart();
}


void seg037::draw8x8_05()
{
    Display::UpdateStop();

    draw8x8_clear_area(0x10, 0x26, 1, 1);

    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, outer_frame_top[col_x] + 0x11E, 0, col_x);
    }

    for (int row_y = 0; row_y <= 0x17; row_y++)
    {
        ovr038::Put8x8Symbol(0, false, outer_frame_left[row_y] + 0x11E, row_y, 0);
        ovr038::Put8x8Symbol(0, false, outer_frame_right[row_y] + 0x11E, row_y, 0x27);
    }

    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, outer_frame_bottom[col_x] + 0x11E, 0x17, col_x);
    }

    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, x8x8_07[col_x] + 0x11E, 0x10, col_x);
    }

    Display::UpdateStart();
}


void seg037::DrawFrame_Combat() // draw8x8_06
{
    Display::UpdateStop();

    draw8x8_clear_area(0x17, 0x27, 0, 0);

    // Top Bar
    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, outer_frame_top[col_x] + 0x11E, 0, col_x);
    }

    // Three Vert Bars
    for (int row_y = 0; row_y <= 0x16; row_y++)
    {
        ovr038::Put8x8Symbol(0, false, unk_16F4D[row_y] + 0x11e, row_y, 0);
        ovr038::Put8x8Symbol(0, false, unk_16F64[row_y] + 0x11e, row_y, 0x16);
        ovr038::Put8x8Symbol(0, false, unk_16F7B[row_y] + 0x11e, row_y, 0x27);
    }

    // Bottom Bar
    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, outer_frame_bottom[col_x] + 0x11e, 0x16, col_x);
    }

    Display::UpdateStart();
}


void seg037::draw8x8_07()
{
    Display::UpdateStop();

    DrawFrame_Outer();

    for (int col_x = 0; col_x <= 0x27; col_x++)
    {
        ovr038::Put8x8Symbol(0, false, x8x8_07[col_x] + 0x11e, 2, col_x);
    }

    Display::UpdateStart();
}


void seg037::draw8x8_clear_area(TextRegion region)
{
    int r = (int)region;
    draw8x8_clear_area(seg041::bounds(r, 0), seg041::bounds(r, 1), seg041::bounds(r, 2), seg041::bounds(r, 3));
}

void seg037::draw8x8_clear_area(int yEnd, int xEnd, int yStart, int xStart)
{
    seg041::DrawRectangle(0, yEnd, xEnd, yStart, xStart);
}

}  // namespace engine

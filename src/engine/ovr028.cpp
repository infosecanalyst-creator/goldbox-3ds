// engine/ovr028.cpp - translation of coab engine/ovr028.cs (class MapCursor).
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

void MapCursor::SetPosition(int currentCity) //sub_6E005
{
    loc_X = city_map_x[currentCity];
    loc_Y = city_map_y[currentCity];
}


void MapCursor::Draw() /* sub_6E02E */
{
    seg040::ega_backup(gbl::cursor_bkup, loc_Y, loc_X);
    seg040::draw_picture(gbl::cursor, loc_Y, loc_X, 0);
}


void MapCursor::Restore() /* sub_6E05D */
{
    seg040::draw_picture(gbl::cursor_bkup, loc_Y, loc_X, 0);
}

}  // namespace engine

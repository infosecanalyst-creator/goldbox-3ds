#include "engine/engine.h"

namespace engine {

void ovr029::RedrawView() /* sub_6F0BA */
{
    if (gbl::lastDaxBlockId == 0x50)
    {
        gbl::can_draw_bigpic = false;
    }

    if (gbl::party_killed == false)
    {
        if (gbl::area_ptr->inDungeon != 0)
        {
            gbl::mapWallRoof = ovr031::get_wall_x2(gbl::mapPosY, gbl::mapPosX);

            if (gbl::mapWallRoof > 0x7F)
            {
                // indoor  
                gbl::sky_colour = sky_colours[gbl::area_ptr->indoor_sky_colour];
            }
            else
            {
                // outdoors
                gbl::sky_colour = sky_colours[gbl::area_ptr->outdoor_sky_colour];
            }

            if (gbl::area_ptr->block_area_view != 0 &&
                Cheats::always_show_areamap == false)
            {
                gbl::mapAreaDisplay = false;
            }

            ovr031::Draw3dWorld(gbl::mapDirection, gbl::mapPosY, gbl::mapPosX);
        }
        else if (gbl::can_draw_bigpic == true)
        {
            ovr030::draw_bigpic();
        }

        gbl::can_draw_bigpic = false;
    }
}

}  // namespace engine

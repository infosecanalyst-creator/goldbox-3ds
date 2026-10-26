// Classes/DaxFiles/DaxBlock.h - port of Classes/DaxFiles/DaxBlock.cs (namespace Classes)
#pragma once

#include "cs/cs.h"
#include "cs/io.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

class DaxBlock_ {
    // C#: new System.Random(unchecked((int)System.DateTime.Now.Ticks))
    // PORT-NOTE: seeded through cs::Determinism so replay runs are reproducible.
    inline static Random random_number = Random(Determinism::SeedFromTicks(DateTime::Now().Ticks));

public:
    int height = 0; // 0x0;
    int width = 0; // 0x2;
    int x_pos = 0; // 0x4;
    int y_pos = 0; // 0x6;
    int item_count = 0; // 0x8;
    Array<byte> field_9; // 0x9; byte[8]

    /// <summary>0x11 Bytes Per Picture</summary>
    int bpp = 0; // 0x11;
    //public byte[] data_ptr; // 0x13;
    Array<byte> data; // 0x17;

    DaxBlock_(int masked, int _item_count, int _width, int _height);

    DaxBlock_(Array<byte> pic_data, int masked, int mask_color);

    void FlipIconLeftToRight();

    void Recolor(bool useRandom, Array<byte> newColors, Array<byte> oldColors);

    void MergeIcons(DaxBlock srcIcon); /* icon_xx, could be implemented using alpha-blending */

    void DaxToPicture(int mask_colour, int masked, int block_offset, Array<byte> t_data);

private:
    void SetMaskedColor(int offset, int color, int masked, int mask_color);
};

}  // namespace Classes

// engine/ovr004.cpp - translation of coab engine/ovr004.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

void ovr004::copy_protection()
{
    String code_path_str;
    char input_expected;
    char input_key;

    ovr034::Load24x24Set(0x1A, 0, 1, "tiles");
    ovr034::Load24x24Set(0x16, 0x1A, 2, "tiles");

    seg037::DrawFrame_Outer();

    seg041::displayString("Align the espruar and dethek runes", 0, 10, 2, 3);
    seg041::displayString("shown below, on translation wheel", 0, 10, 3, 3);
    seg041::displayString("like this:", 0, 10, 4, 3);
    int attempt = 0;

    do
    {

        int var_6 = seg051::Random(26);
        int var_7 = seg051::Random(22);

        ovr034::DrawIsoTile(var_6, 3, 0x11);
        ovr034::DrawIsoTile(var_7 + 0x1a, 7, 0x11);

        seg040::DrawOverlay();
        int code_path = seg051::Random(3);

        switch (code_path)
        {
            case 0:
                code_path_str = "-..-..-..";
                break;

            case 1:
                code_path_str = "- - - - -";
                break;

            case 2:
                code_path_str = ".........";
                break;

            default:
                code_path_str = String::Empty;
                break;
        }

        int code_row = seg051::Random(6);

        String text = String("Type the character in box number ") + ToString(6 - code_row);

        seg041::displayString(text, 0, 10, 12, 3);

        seg041::displayString("under the ", 0, 10, 13, 3);
        seg041::displayString(code_path_str, 0, 15, 13, 14);
        seg041::displayString("path.", 0, 10, 13, 0x19);

        int code_index = var_6 + 0x22 - var_7 + (code_path * 12) + ((5 - code_row) << 1);

        while (code_index < 0)
        {
            code_index += 36;
        }

        while (code_index > 35)
        {
            code_index -= 36;
        }

        input_expected = codeWheel[code_row][code_index];

        String input = seg041::getUserInputString(1, 0, 13, "type character and press return: ");

        // C#: (input == null || input.Length == 0) - a null String is "" here.
        input_key = (input.Length() == 0) ? ' ' : input[0];
        attempt++;

        if (input_key != input_expected)
        {
            seg041::DisplayStatusText(0, 14, "Sorry, that's incorrect.");
        }
        else
        {
            return;
        }
    } while (input_key != input_expected && attempt < 3);

    if (attempt >= 3)
    {
        seg044::PlaySound(Sound::sound_1);
        seg044::PlaySound(Sound::sound_5);
        gbl::game_speed_var = 9;
        seg041::DisplayStatusText(0, 14, "An unseen force hurls you into the abyss!");
        seg049::SysDelay(0x3E8);
        seg043::print_and_exit();
    }
}

}  // namespace engine

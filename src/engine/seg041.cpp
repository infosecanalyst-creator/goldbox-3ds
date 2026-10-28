// engine/seg041.cpp - translation of coab engine/seg041.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"
#include "platform/platform.h"
#include "platform/readable_font.h"

#include <ctime>

namespace engine {

// PORT-NOTE: C# int.TryParse(string, out int) (NumberStyles.Integer, invariant-ish):
// optional surrounding whitespace, optional leading sign, decimal digits, must fit in int.
static bool seg041_TryParseInt(const String& s, int& result) {
    result = 0;
    String t = s.Trim();
    if (t.Length() == 0) return false;
    int i = 0;
    bool neg = false;
    if (t[0] == '+' || t[0] == '-') {
        neg = (t[0] == '-');
        i = 1;
    }
    if (i >= t.Length()) return false;
    int64_t v = 0;
    for (; i < t.Length(); i++) {
        char c = t[i];
        if (c < '0' || c > '9') return false;
        v = v * 10 + (c - '0');
        if (v > 2147483648LL) return false;
    }
    if (neg) v = -v;
    if (v > INT32_MAX || v < INT32_MIN) return false;
    result = (int)v;
    return true;
}

void seg041::DrawRectangle(byte color, int yEnd, int xEnd, int yStart, int xStart) {
    xStart *= 8;
    xEnd = (xEnd + 1) * 8;
    yStart *= 8;
    yEnd = (yEnd + 1) * 8;

    for (int x = xStart; x < xEnd; x++) {
        for (int y = yStart; y < yEnd; y++) {
            Display::SetPixel3(x, y, color);
        }
    }
}


void seg041::Load8x8Tiles() // load_8x8d1_201
{
    Array<byte> block_ptr;
    short block_size;

    seg042::load_decode_dax(block_ptr, block_size, 201, "8X8d1.dax");

    if (block_size != 0) {
        for (int i = 0, j = 0; i < block_size && j < 177; i += 8, j++) {
            for (int k = 0; k < 8 && (i + k) < block_size; k++) {
                gbl::dax_8x8d1_201(j, k) = block_ptr[i + k];
            }
        }
    }
}


void seg041::display_char01(char ch, int repeatCount, int bgColor, int fgColor, int YCol, int XCol) // display_char01
{
    if (XCol < 40 &&
        YCol < 25) {
        // PORT-NOTE: C# char is unsigned 16-bit; cast through unsigned char so chars >= 0x80 index like C#.
        char index = (char)((unsigned char)Char::ToUpper(ch) % 0x40);

        // goldbox-3ds: draw printable text in the clear font, keeping its case.
        const uint8_t* clear = gbl::portEnhancements ? ReadableGlyph((unsigned char)ch) : nullptr;
        for (int i = 0; i < 8; i++) {
            gbl::monoCharData[i] = clear ? clear[i] : gbl::dax_8x8d1_201(index, i);
        }

        for (int i = 0; i < repeatCount; i++) {
            Display::DisplayMono8x8(XCol + i, YCol, gbl::monoCharData, bgColor, fgColor);
            platform::TextDrawn(YCol, XCol + i, ch, fgColor, bgColor);  // goldbox-3ds: display-only
        }
    }
}

void seg041::displaySpaceChar(int yCol, int xCol) {
    if (xCol >= 0 && xCol <= 0x27 &&
        yCol >= 0 && yCol <= 0x18) {
        display_char01(' ', 1, 0, 0, yCol, xCol);

        Display::Update();
    }
}


void seg041::displayString(String str, int bgColor, int fgColor, int yCol, int xCol) {
    if (xCol <= 0x27 && yCol <= 0x27) {
        for (char ch : str) {
            display_char01(ch, 1, bgColor, fgColor, yCol, xCol);
            xCol++;
        }
        Display::Update();
    }
}



int seg041::displayStringSlow(String text
    , int text_index, int text_length, int fgColor) // sub_107DE
{
    while (text_index <= text_length) {
        display_char01(text[text_index - 1], 1, 0, fgColor, gbl::textYCol, gbl::textXCol);

        if (gbl::DelayBetweenCharacters) {
            seg049::SysDelay(gbl::game_speed_var * 3);
        }

        text_index += 1;
        gbl::textXCol++;
    }

    return text_index;
}


void seg041::text_skip_space(String text, int text_max, int& text_index) /* sub_10854 */
{
    while (text_index < text_max &&
        text[text_index - 1] == ' ') {
        text_index += 1;
    }
}

// internal static int[,] bounds = ... (initialised in engine/decl/seg041.h)

void seg041::press_any_key(String text, bool clearArea, int fgColor, TextRegion region) {
    int r = (int)region;
    press_any_key(text, clearArea, fgColor, bounds(r, 0), bounds(r, 1), bounds(r, 2), bounds(r, 3));
}

//static const char[] syms = { '!', ',', '-', '.', ':', ';', '?' };
// static Set puncutation = new Set(33, 44, 45, 46, 58, 59, 63); (initialised in engine/decl/seg041.h)

void seg041::press_any_key(String text, bool clearArea, int fgColor,
    int yEnd, int xEnd, int yStart, int xStart) {
    if (xStart > 0x27 || yStart > 0x18 ||
        (xEnd > 0x27 && yEnd > 0x27)) {
        return;
    }

    if (gbl::textXCol < xStart ||
        gbl::textXCol > xEnd ||
        gbl::textYCol < yStart ||
        gbl::textYCol > yEnd) {
        gbl::textXCol = xStart;
        gbl::textYCol = yStart;
    }

    if (clearArea == true) {
        seg037::draw8x8_clear_area(yEnd, xEnd, yStart, xStart);
        gbl::textXCol = xStart;
        gbl::textYCol = yStart;
    }

    int text_start = 1;
    int input_lenght = text.Length();

    if (input_lenght != 0) {
        do {
            int text_end = text_start;
            //text.LastIndexOfAny(syms, text_start);

            while (text_end < input_lenght &&
                puncutation->MemberOf(text[text_end - 1]) == true) {
                text_end++;
            }

            while (text_end < input_lenght &&
                puncutation->MemberOf(text[text_end - 1]) == false &&
                text[text_end - 1] != ' ') {
                text_end++;
            }

            if (text[text_end - 1] != ' ') {
                while (text_end + 1 < input_lenght &&
                    puncutation->MemberOf(text[text_end]) == true) {
                    text_end++;
                }
            }

            if (((text_end - text_start) + gbl::textXCol) > xEnd) {
                if (((text_end - text_start) + gbl::textXCol) == xEnd &&
                    text[text_end - 1] == ' ') {
                    text_end -= 1;
                    text_start = displayStringSlow(text, text_start, text_end, fgColor);
                }

                gbl::textXCol = xStart;
                gbl::textYCol++;
                text_skip_space(text, input_lenght, text_start);

                if (gbl::textYCol > yEnd &&
                    text_start < input_lenght) {
                    gbl::textXCol = xStart;
                    gbl::textYCol = yStart;

                    DisplayAndPause("Press any key to continue", 13);
                    seg043::clear_keyboard();

                    seg037::draw8x8_clear_area(yEnd, xEnd, yStart, xStart);

                    text_start = displayStringSlow(text, text_start, text_end, fgColor);
                }
            } else {
                text_start = displayStringSlow(text, text_start, text_end, fgColor);
                Display::Update();
            }
        } while (text_start <= input_lenght);

        if (gbl::textXCol > xEnd) {
            gbl::textXCol = xStart;
            gbl::textYCol++;
        }
    }
}


// goldbox-3ds: entry hints for the host (nested calls report only once).
static int s_entryDepth = 0;
struct EntryHint {
    explicit EntryHint(int kind, int maxValue) {
        if (s_entryDepth++ == 0) platform::EntryBegin(kind, maxValue);
    }
    ~EntryHint() {
        if (--s_entryDepth == 0) platform::EntryEnd();
    }
};

String seg041::getUserInputString(byte inputLen, byte bgColor, byte fgColor, String prompt) {
    EntryHint entryHint(prompt.StartsWith("Character name") ? 2 : 1, 0);
    ovr027::ClearPromptAreaNoUpdate();

    displayString(prompt, bgColor, fgColor, 0x18, 0);

    int xPos = prompt.Length();

    char ch;
    String resultString = String::Empty;

    do {
        ch = (char)seg043::GetInputKey();

        // PORT-NOTE: C# char is unsigned; compare as unsigned so key codes >= 0x80 behave the same.
        if ((unsigned char)ch >= 0x20 && (unsigned char)ch <= 0x7A) {
            if (resultString.Length() < inputLen) {
                resultString += ToString(ch);

                //var_2A++;

                displayString(ToString(ch), 0, 15, 0x18, xPos++);
            }
        } else if (ch == 8 && resultString.Length() > 0) {
            resultString = seg051::Copy(resultString.Length() - 1, 0, resultString);

            displaySpaceChar(24, --xPos);
            //xPos -= 1;
        }

    } while (ch != 0x0d && ch != 0x1B && gbl::inDemo == false);

    ovr027::ClearPromptAreaNoUpdate();

    return resultString.ToUpper();
}


ushort seg041::getUserInputShort(byte bgColor, byte fgColor, String prompt) {
    EntryHint entryHint(3, 0);
    bool good_input;
    int value = 0;

    do {
        String input = getUserInputString(6, bgColor, fgColor, prompt);

        good_input = seg041_TryParseInt(input, value);

        if (good_input) {
            good_input = (value < 0x00010000 && value >= 0);
        }
    } while (good_input == false);

    return (ushort)value;
}


void seg041::DisplayAndPause(String txt, byte fgColor) // displayAndDebug
{
    ovr027::ClearPromptAreaNoUpdate();

    displayString(txt, 0, fgColor, 0x18, 0);
    seg043::GetInputKey();
}


/// <summary>
/// Gets the centi seconds since midnight
/// </summary>
int seg041::time01() // time01
{
    // PORT-NOTE: cs::DateTime has no Hour/Minute/Second/Millisecond; read the local
    // wall clock directly (same value as System.DateTime.Now's fields).
    DateTime now = DateTime::Now();
    int64_t unixTicks = now.Ticks - 621355968000000000LL;
    std::time_t t = (std::time_t)(unixTicks / 10000000LL);
    std::tm dt = *std::localtime(&t);
    int Millisecond = (int)((unixTicks / 10000LL) % 1000);

    return (dt.tm_hour * 360000) + (dt.tm_min * 6000) + (dt.tm_sec * 100) + (Millisecond / 10);
}


void seg041::ClearScreen() {
    DrawRectangle(0, 0x18, 0x27, 0, 0);
}


void seg041::DisplayStatusText(byte bgColor, byte fgColor, String text) /* sub_10ECF */
{
    ovr027::ClearPromptAreaNoUpdate();

    displayString(text, bgColor, fgColor, 0x18, 0);

    GameDelay();

    ovr027::ClearPromptAreaNoUpdate();
}


void seg041::GameDelay() {
    //Display.Update();
    seg049::SysDelay(gbl::game_speed_var * 100);
}

}  // namespace engine

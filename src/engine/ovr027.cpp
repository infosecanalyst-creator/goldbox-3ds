// engine/ovr027.cpp - translation of coab engine/ovr027.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"
#include "platform/platform.h"

namespace engine {

// PORT-NOTE: cs::String::TrimStart() trims all whitespace; C# TrimStart(' ')
// trims only spaces. Local helper with the C# semantics.
static String TrimStartSpace(const String& s)
{
    int b = 0;
    while (b < s.Length() && s[b] == ' ') b++;
    return s.Substring(b);
}

MenuItem ovr027::getStringListEntry(List<MenuItem> list, int index)
{
    return (list.Count() > index) ? list[index] : nullptr;
}


String ovr027::highlight_::ToString() const
{
    return Format("{0} - {1}", start, end);
}

ovr027::HighlightSet_::HighlightSet_()
{
    highlights = Array<highlight>(length);

    Clear();
}


ovr027::highlight& ovr027::HighlightSet_::operator[](int index)
{
    return highlights[index];
}

void ovr027::HighlightSet_::Clear()
{
    for (int i = 0; i < length; i++)
    {
        highlights[i] = New<highlight_>();
    }
}

/// <summary>
/// sub_6C0DA
/// </summary>
ovr027::HighlightSet ovr027::BuildInputKeys(String menuText, int& highlighCount)
{
    HighlightSet highlighSet = New<HighlightSet_>();

    int index = 0;

    for (int idx = 0; idx < menuText.Length(); idx++)
    {
        if (highlightable_text->MemberOf(menuText[idx]) == true)
        {
            if (highlighSet[index]->start == -1)
            {
                highlighSet[index]->start = idx;
            }
            else
            {
                highlighSet[index]->end = idx - 2;
                index++;
                highlighSet[index]->start = idx;
            }
        }
    }

    highlighSet[index]->end = menuText.Length();
    highlighCount = index + 1;

    return highlighSet;
}


void ovr027::display_highlighed_text(int highlighed_word, int highlightFgColor,
    String text, int xOffset, int fgColor, HighlightSet highlights) /* sub_6C1E9 */
{
    platform::MenuHighlight(highlighed_word);  // goldbox-3ds: display-only hint for the host
    if (text.Length() > 0)
    {
        for (int i = 0; i < text.Length(); i++)
        {
            if (highlights[highlighed_word]->start <= i &&
                highlights[highlighed_word]->end >= i &&
                highlightFgColor != 0)
            {
                seg041::display_char01(text[i], 1, highlightFgColor, 0, 0x18, xOffset + i);
            }
            else if (highlightable_text->MemberOf(text[i]) == true)
            {
                seg041::display_char01(text[i], 1, 0, highlightFgColor, 0x18, xOffset + i);
            }
            else
            {
                seg041::display_char01(text[i], 1, 0, fgColor, 0x18, xOffset + i);
            }
        }

        if (text.Length() + xOffset < 0x27)
        {
            seg041::display_char01(' ', (0x27 - text.Length() - xOffset) + 1,
                0, 0, 0x18, xOffset + text.Length());
        }

        Display::Update();
    }
}

char ovr027::displayInput(bool useOverlay, byte arg_6, MenuColorSet colors, String displayInputString, String displayExtraString)
{
    bool dummyBool;
    return displayInput(dummyBool, useOverlay, arg_6, colors, displayInputString, displayExtraString);
}

char ovr027::displayInput(bool& specialKeyPressed, bool useOverlay, byte accept_ctrlkeys, MenuColorSet colors, String displayInputString, String displayExtraString)
{
    int highlistCount;

    gbl::displayInput_specialKeyPressed = false;

    bool var_8F = (colors->foreground != 0) || (colors->highlight != 0);

    HighlightSet highlights = BuildInputKeys(displayInputString, highlistCount);

    // goldbox-3ds: publish the menu to the host (3DS bottom screen buttons).
    // Display-only; no effect on game state.
    {
        std::vector<platform::MenuItem> items;
        for (int w = 0; w < highlistCount; w++) {
            int st = highlights[w]->start, en = highlights[w]->end;
            if (st < 0 || st >= displayInputString.Length()) continue;
            if (en >= displayInputString.Length()) en = displayInputString.Length() - 1;
            String label = displayInputString.Substring(st, Math::Max(1, en - st + 1)).Trim();
            items.push_back({label, displayInputString[st], displayExtraString.Length() + st, en - st + 1, w});
        }
        platform::MenuShow(displayExtraString, items, accept_ctrlkeys != 0);
    }
    struct MenuHideOnExit { ~MenuHideOnExit() { platform::MenuHide(); } } menuHideOnExit;

    if (gbl::menuSelectedWord >= highlistCount)
    {
        gbl::menuSelectedWord = 0;
    }

    char input_key = '\0';
    specialKeyPressed = false;
    bool stopLoop = false;

    auto timeStart = DateTime::Now();
    auto timeCursorOn = timeStart.AddMilliseconds(300);
    auto timeCursorOff = timeCursorOn.AddMilliseconds(500);

    if (displayExtraString.Length() != 0)
    {
        seg041::displayString(displayExtraString, 0, colors->prompt, 0x18, 0);
    }

    int displayInputXOffset = displayExtraString.Length();

    display_highlighed_text(gbl::menuSelectedWord, colors->highlight,
        displayInputString, displayInputXOffset, colors->foreground, highlights);

    if (gbl::game_state == GameState::WildernessMap &&
        gbl::bigpic_block_id == 0x79 &&
        gbl::lastDaxBlockId != 0x50)
    {
        MapCursor::SetPosition(gbl::area_ptr->current_city);
        MapCursor::Draw();
        MapCursor::Restore();
    }

    do
    {
        if (gbl::game_state == GameState::WildernessMap &&
            gbl::bigpic_block_id == 0x79 &&
            gbl::lastDaxBlockId != 0x50 &&
            DateTime::Now() >= timeCursorOn)
        {
            MapCursor::Draw();
            timeCursorOn = timeCursorOff.AddMilliseconds(300);
        }

        if ((gbl::area_ptr->picture_fade != 0 || useOverlay == true) &&
            gbl::byte_1D556->curFrame > 0)
        {
            ovr030::DrawMaybeOverlayed(gbl::byte_1D556->CurrentPicture(), useOverlay, 3, 3);

            int delay = gbl::byte_1D556->CurrentDelay() * 100;

            if ((DateTime::Now().Subtract(timeStart).TotalMilliseconds()) >= delay ||
                gbl::area_ptr->picture_fade != 0)
            {
                gbl::byte_1D556->NextFrame();

                timeStart = DateTime::Now();
            }
        }

        if (gbl::displayInputSecondsToWait > 0 &&
            DateTime::Now().Subtract(timeStart).TotalSeconds() >= gbl::displayInputSecondsToWait)
        {
            input_key = gbl::displayInputTimeoutValue;
            stopLoop = true;
        }
        else if (seg049::KEYPRESSED() == true)
        {
            input_key = (char)seg043::GetInputKey();

            if (input_key == 0)
            {
                input_key = (char)seg043::GetInputKey();

                if (accept_ctrlkeys != 0)
                {
                    specialKeyPressed = true;
                    stopLoop = true;
                }
            }
            else if (input_key == 0x1B)
            {
                stopLoop = true;
                input_key = '\0';
            }
            else if (input_key == 13)
            {
                if (var_8F)
                {
                    if (highlights[gbl::menuSelectedWord]->start != -1)
                    {
                        input_key = displayInputString[highlights[gbl::menuSelectedWord]->start];
                    }
                    else
                    {
                        input_key = '\r';
                    }

                    stopLoop = true;
                }
            }
            else if (input_key == ',')
            {
                if (gbl::menuSelectedWord == 0)
                {
                    gbl::menuSelectedWord = highlistCount - 1;
                }
                else
                {
                    gbl::menuSelectedWord--;
                }

                display_highlighed_text(gbl::menuSelectedWord, colors->highlight, displayInputString, displayInputXOffset, colors->foreground, highlights);
            }
            else if (input_key == '.')
            {
                gbl::menuSelectedWord++;

                if (gbl::menuSelectedWord >= highlistCount)
                {
                    gbl::menuSelectedWord = 0;
                }

                display_highlighed_text(gbl::menuSelectedWord, colors->highlight, displayInputString, displayInputXOffset, colors->foreground, highlights);
            }
            else
            {
                input_key = Char::ToUpper(input_key);
                if (alpha_number_input->MemberOf(input_key) == true)
                {
                    if (input_key == 0x20)
                    {
                        stopLoop = true;
                    }
                    else
                    {
                        for (int var_62 = 0; var_62 < displayInputString.Length(); var_62++)
                        {
                            if (displayInputString[var_62] == input_key)
                            {
                                stopLoop = true;
                                int var_61 = 0;

                                while (highlights[var_61]->start != var_62)
                                {
                                    var_61++;
                                }

                                gbl::menuSelectedWord = var_61;

                                display_highlighed_text(gbl::menuSelectedWord, colors->highlight, displayInputString, displayInputXOffset, colors->foreground, highlights);
                            }
                        }
                    }
                }

                if (accept_ctrlkeys != 0 &&
                    number_input->MemberOf(input_key) == true)
                {
                    if (input_key == 'W')
                    {
                        input_key = '7';
                    }
                    else
                    {
                        input_key = keypad_ctrl_codes[input_key - 0x31];
                    }

                    specialKeyPressed = true;
                    stopLoop = true;
                }
            }
        }

        if (gbl::game_state == GameState::WildernessMap &&
            gbl::bigpic_block_id == 0x79 &&
            gbl::lastDaxBlockId != 0x50 &&
            DateTime::Now() >= timeCursorOff)
        {
            MapCursor::Restore();

            timeCursorOff = timeCursorOn.AddMilliseconds(500);
        }

        platform::Sleep(20);

    } while (stopLoop == false);

    gbl::area_ptr->picture_fade = 0;

    if (gbl::game_state == GameState::WildernessMap &&
        gbl::bigpic_block_id == 0x79 &&
        gbl::lastDaxBlockId != 0x50)
    {
        MapCursor::Restore();
    }

    gbl::displayInput_specialKeyPressed = specialKeyPressed;

    return input_key;
}


void ovr027::ClearPromptArea() // redraw_screen
{
    ClearPromptAreaNoUpdate();

    Display::Update();
}

void ovr027::ClearPromptAreaNoUpdate()
{
    seg041::DrawRectangle(0, 0x18, 0x27, 0x18, 0);
}

void ovr027::sub_6C897(int index,
    int yEnd, int xEnd, int yStart, int xStart, List<MenuItem> list,
    int normalColor, int headingColor, int displayFillWidth) // sub_6C897
{
    seg037::draw8x8_clear_area(yEnd, xEnd, yStart, xStart);

    MenuItem var_4 = getStringListEntry(list, index);

    int yCol = yStart;
    int count = Math::Min(yEnd - yStart + 1, list.Count() - index);

    for (auto menu : list.GetRange(index, count))
    {
        seg041::displayString(menu->Text, 0, menu->Heading ? headingColor : normalColor, yCol, xStart);

        if (menu->Text.Length() < displayFillWidth)
        {
            seg041::display_char01(' ', displayFillWidth - menu->Text.Length(), 0, 0, yCol, menu->Text.Length() + xStart);
        }
        yCol++;
    }
}


int ovr027::getBegingOfString(String text)
{
    return text.Length() - TrimStartSpace(text).Length();
}

void ovr027::ListItemHighlighted(int index, List<MenuItem> stringList, int yCol, int xCol, int bgColor)
{
    MenuItem menu_item = getStringListEntry(stringList, index);

    int stringStart = getBegingOfString(menu_item->Text);

    seg041::displayString(
        menu_item->Text.Trim(),
        bgColor,
        0,
        yCol + (index - gbl::menuScreenIndex),
        xCol + stringStart);
}


void ovr027::ListItemNormal(int index, List<MenuItem> list, int yCol, int xCol, int normalColor, int headingColor)
{
    MenuItem menu_item = getStringListEntry(list, index);

    int var_5 = getBegingOfString(menu_item->Text);

    String text = menu_item->Text.Trim();

    if (menu_item->Heading)
    {
        seg041::displayString(text, 0, headingColor, yCol + (index - gbl::menuScreenIndex), xCol + var_5);
    }
    else
    {
        seg041::displayString(text, 0, normalColor, yCol + (index - gbl::menuScreenIndex), xCol + var_5);
    }
}


int ovr027::skipHeadings(bool backwardsStep, int index, List<MenuItem> list, int listDisplayHeight) // sub_6CC08
{
    int var_2 = 0;

    if (backwardsStep == true)
    {
        while (var_2 < listDisplayHeight && list[index]->Heading)
        {
            var_2++;
            index += 1;

            if ((gbl::menuScreenIndex + listDisplayHeight - 1) < index)
            {
                index = gbl::menuScreenIndex;
            }

            if ((list.Count() - 1) < index)
            {
                index = gbl::menuScreenIndex;
            }
        }
    }
    else
    {
        while (var_2 < listDisplayHeight && list[index]->Heading)
        {
            var_2++;
            index -= 1;

            if (index < gbl::menuScreenIndex)
            {
                index = (short)(gbl::menuScreenIndex + listDisplayHeight - 1);
            }

            if ((list.Count() - 1) < index)
            {
                index = (short)(list.Count() - 1);
            }
        }
    }

    return index;
}


void ovr027::menu_scroll_page(bool backwardsStep, int& index, List<MenuItem> list, int listDisplayHeight,
    int yEnd, int xEnd, int yStart, int xStart,
    int normalColor, int headingColor, int displayFillWidth) // sub_6CD38
{
    int screenOffset = index - gbl::menuScreenIndex;

    if (backwardsStep == true)
    {
        gbl::menuScreenIndex += listDisplayHeight;
        if ((list.Count() - listDisplayHeight) < gbl::menuScreenIndex)
        {
            gbl::menuScreenIndex = (short)(list.Count() - listDisplayHeight);
        }
    }
    else
    {
        gbl::menuScreenIndex -= listDisplayHeight;

        if (gbl::menuScreenIndex < 0)
        {
            gbl::menuScreenIndex = 0;
        }
    }

    index = gbl::menuScreenIndex + screenOffset;

    index = skipHeadings(backwardsStep, index, list, listDisplayHeight);

    sub_6C897(gbl::menuScreenIndex, yEnd, xEnd, yStart, xStart,
        list, normalColor, headingColor, displayFillWidth);
}


int ovr027::menu_scroll_in_page(bool backwardsStep, int index, List<MenuItem> list, int listDisplayHeight) // sub_6CDCA
{
    if (backwardsStep == true)
    {
        index += 1;

        if ((gbl::menuScreenIndex + listDisplayHeight - 1) < index)
        {
            index = gbl::menuScreenIndex;
        }

        if ((list.Count() - 1) < index)
        {
            index = gbl::menuScreenIndex;
        }
    }
    else
    {
        index -= 1;

        if (index < gbl::menuScreenIndex)
        {
            index = (short)(gbl::menuScreenIndex + listDisplayHeight - 1);
        }

        if ((list.Count() - 1) < index)
        {
            index = (short)(list.Count() - 1);
        }
    }

    return skipHeadings(backwardsStep, index, list, listDisplayHeight);
}


char ovr027::sl_select_item(MenuItem& result_ptr, int& index_ptr,
    bool& redrawMenuItems, bool showExit, List<MenuItem> stringList,
    int endY, int endX, int startY, int startX,
    MenuColorSet colors, String inputString, String extraTextString)
{
    char ret_val = '\0'; /* Simeon */
    result_ptr = nullptr; /*Simeon*/

    if (stringList == nullptr)
    {
        index_ptr = 0;
        result_ptr = nullptr;

        return '\0';
    }

    gbl::menuSelectedWord = 1;
    // goldbox-3ds: highlight the list's own action ("Select", "Use" ...) so
    // A/Enter picks the entry instead of Exit.
    if (gbl::portEnhancements && inputString.Length() > 0) gbl::menuSelectedWord = 0;
    struct ListHideOnExit { ~ListHideOnExit() { platform::ListHide(); } } listHideOnExit;

    int listDisplayWidth = (endX - startX) + 1;
    int listDisplayHeight = (short)((endY - startY) + 1);

    int listCount = stringList.Count();

    if (listCount <= listDisplayHeight)
    {
        gbl::menuScreenIndex = 0;
    }

    if (gbl::menuScreenIndex > index_ptr)
    {
        gbl::menuScreenIndex = index_ptr;
        redrawMenuItems = true;
    }

    if (gbl::menuScreenIndex > listCount)
    {
        gbl::menuScreenIndex = 0;
        redrawMenuItems = true;
    }

    index_ptr++;
    index_ptr = menu_scroll_in_page(false, index_ptr, stringList, listDisplayHeight);

    if (redrawMenuItems == true)
    {
        sub_6C897(gbl::menuScreenIndex, endY, endX, startY, startX,
            stringList, colors->foreground, colors->prompt, listDisplayWidth);
    }

    redrawMenuItems = false;

    bool loop_end = false;

    while (loop_end == false)
    {
        ListItemHighlighted(index_ptr, stringList, startY, startX, colors->highlight);
        String displayString = inputString;

        bool showNext = false;
        bool showPrevious = false;

        if ((listCount - listDisplayHeight) > gbl::menuScreenIndex)
        {
            displayString += " Next";
            showNext = true;
        }

        if (gbl::menuScreenIndex > 0)
        {
            displayString += " Prev";
            showPrevious = true;
        }

        if (showExit == true)
        {
            displayString += " Exit";
        }

        // goldbox-3ds: publish the visible rows to the host (display-only).
        platform::ListShow(startY, startX, endX,
            Math::Min(listDisplayHeight, listCount - gbl::menuScreenIndex), index_ptr - gbl::menuScreenIndex);

        bool speical_key;
        char input_key = displayInput(speical_key, false, 1, colors, displayString, extraTextString);

        ListItemNormal(index_ptr, stringList, startY, startX, colors->foreground, colors->prompt);

        if (speical_key == true)
        {
            switch (input_key)
            {
                case 'G':
                    index_ptr = menu_scroll_in_page(false, index_ptr, stringList, listDisplayHeight);
                    break;

                case 'O':
                    index_ptr = menu_scroll_in_page(true, index_ptr, stringList, listDisplayHeight);
                    break;

                case 'I':
                    if (showPrevious == true)
                    {
                        menu_scroll_page(false, index_ptr, stringList, listDisplayHeight, endY, endX, startY, startX, colors->foreground, colors->prompt, listDisplayWidth);
                    }
                    break;

                case 'Q':
                    if (showNext == true)
                    {
                        menu_scroll_page(true, index_ptr, stringList, listDisplayHeight, endY, endX, startY, startX, colors->foreground, colors->prompt, listDisplayWidth);
                    }
                    break;
            }
        }
        else
        {
            switch (input_key)
            {
                case 'P':
                    menu_scroll_page(false, index_ptr, stringList, listDisplayHeight, endY, endX, startY, startX, colors->foreground, colors->prompt, listDisplayWidth);
                    break;

                case 'N':

                    menu_scroll_page(true, index_ptr, stringList, listDisplayHeight, endY, endX, startY, startX, colors->foreground, colors->prompt, listDisplayWidth);
                    break;

                case (char)0x1B:
                case '\0':
                case 'E':
                    result_ptr = nullptr;
                    ret_val = '\0';
                    loop_end = true;
                    break;

                default:
                    result_ptr = getStringListEntry(stringList, index_ptr);
                    ret_val = input_key;
                    loop_end = true;
                    break;
            }
        }
    }

    return ret_val;
}


char ovr027::yes_no(MenuColorSet colors, String inputString)
{
    char inputKey;

    gbl::menuSelectedWord = 2;

    do
    {
        inputKey = displayInput(false, 0, colors, "Yes No", inputString);

    } while (inputKey != 'N' && inputKey != 'Y');

    return inputKey;
}

}  // namespace engine

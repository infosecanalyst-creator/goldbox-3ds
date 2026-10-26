// engine/decl/ovr027.h - declarations for engine/ovr027.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr027.cpp.
// ---------------------------------------------------------------------------
// ovr027.cs
// ---------------------------------------------------------------------------
struct ovr027 {
    static MenuItem getStringListEntry(List<MenuItem> list, int index);
    inline static Set highlightable_text = New<Set_>(Array<int>{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'});
    class highlight_ {
    public:
        int start = -1;
        int end = -1;
        String ToString() const;
    };
    using highlight = cs::Ref<highlight_>;

    class HighlightSet_ {
    public:
        Array<highlight> highlights;
        static constexpr int length = 20;
        HighlightSet_();
        highlight& operator[](int index); // C# indexer `internal highlight this[int index] { get; set; }`
        void Clear();
    };
    using HighlightSet = cs::Ref<HighlightSet_>;

    static HighlightSet BuildInputKeys(String menuText, int& highlighCount);
    static void display_highlighed_text(int highlighed_word, int highlightFgColor, String text, int xOffset, int fgColor, HighlightSet highlights); /* sub_6C1E9 */
    inline static Set alpha_number_input = New<Set_>(Array<int>{' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}); //unk_6C398
    inline static Set number_input = New<Set_>(Array<int>{'1', '2', '3', '4', '5', '6', '7', '8', '9', '\\'}); //unk_6C3B8
    inline static Array<char> keypad_ctrl_codes = { 'O', 'P', 'Q', 'K', ' ', 'M', 'G', 'H', 'I' };
    static char displayInput(bool useOverlay, byte arg_6, MenuColorSet colors, String displayInputString, String displayExtraString);
    static char displayInput(bool& specialKeyPressed, bool useOverlay, byte accept_ctrlkeys, MenuColorSet colors, String displayInputString, String displayExtraString);
    static void ClearPromptArea(); // redraw_screen
    static void ClearPromptAreaNoUpdate();
    static void sub_6C897(int index, int yEnd, int xEnd, int yStart, int xStart, List<MenuItem> list, int normalColor, int headingColor, int displayFillWidth); // sub_6C897
    static int getBegingOfString(String text);
    static void ListItemHighlighted(int index, List<MenuItem> stringList, int yCol, int xCol, int bgColor);
    static void ListItemNormal(int index, List<MenuItem> list, int yCol, int xCol, int normalColor, int headingColor);
    static int skipHeadings(bool backwardsStep, int index, List<MenuItem> list, int listDisplayHeight); // sub_6CC08
    static void menu_scroll_page(bool backwardsStep, int& index, List<MenuItem> list, int listDisplayHeight, int yEnd, int xEnd, int yStart, int xStart, int normalColor, int headingColor, int displayFillWidth); // sub_6CD38
    static int menu_scroll_in_page(bool backwardsStep, int index, List<MenuItem> list, int listDisplayHeight); // sub_6CDCA
    static char sl_select_item(MenuItem& result_ptr, int& index_ptr, bool& redrawMenuItems, bool showExit, List<MenuItem> stringList, int endY, int endX, int startY, int startX, MenuColorSet colors, String inputString, String extraTextString);
    static char yes_no(MenuColorSet colors, String inputString);
};


// engine/decl/ovr033.h - declarations for engine/ovr033.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr033.cpp.
// ---------------------------------------------------------------------------
// ovr033.cs
// ---------------------------------------------------------------------------
struct ovr033 {
    static constexpr int MaxSize = 4;
    static Array<Array<Point>> Steps;
    static Array<Point> GetSizeBasedMapDeltas(int size); /*sub_7400F*/
    static Array<Point> BuildSizeMap(int size, Point pos);
    static void calculatePlayerScreenPositions(); /* sub_74077 */
    static void Color_0_8_inverse();
    static void Color_0_8_normal();
    static void sub_7416E(Point pos); // sub_7416E
    static void DrawPlayerIconIfOnScreen(int player_index);
    static void RedrawPosition(Point pos); // sub_7431C
    static Array2<int> mapToPlayerIndex;
    static void setup_mapToPlayerIndex_and_playerScreen(); /* sub_743E7 */
    static int PlayerIndexAtMapXY(int posY, int posX); /* sub_74505 */
    static void AtMapXY(int& groundTile, int& playerIndex, Point pos);
    static void AtMapXY(int& groundTile, int& playerIndex, int posY, int posX); /* sub_74505 */
    static void RedrawPlayerBackground(int player_index, Point map); /* sub_74572 */
    static void RedrawPlayerBackground(int player_index); /* sub_74572 */
    static bool CoordOnScreen(Point pos); /* sub_74730 */
    static bool PlayerOnScreen(bool AllOnScreen, Player player); // sub_74761
    static bool PlayerOnScreen(bool AllOnScreen, int player_index); // sub_74761
    static bool ScreenMapCheck(int radius, Point pos);
    static void redrawCombatArea(int dir, int radius, Point map); /*sub_749DD*/
    static void draw_74B3F(bool arg_0, Icon iconState, int direction, Player player); /* sub_74B3F */
    static Point PlayerMapPos(Player player); /* sub_74C5A */
    static int PlayerMapSize(Player player); /*sub_74C82*/
    static int GetPlayerIndex(Player player);
    static void getGroundInformation(int& groundTile, int& playerIndex, byte direction, Player player); /* sub_74D04 */
    static void getGroundInformation(bool& isPoisonousCloud, bool& isNoxiousCloud, int& groundTile, int& playerIndex, int direction, Player player); /* sub_74D04 */
    static void CombatantKilled(Player player); // sub_74E6F
    static bool sub_7515A(bool arg_0, Point pos, Player player);
    static void RedrawCombatIfFocusOn(bool draw_cursor, byte radius, Player player); // sub_75356
};


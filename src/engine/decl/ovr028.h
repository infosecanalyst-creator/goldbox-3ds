// engine/decl/ovr028.h - declarations for engine/ovr028.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr028.cpp.
// ---------------------------------------------------------------------------
// ovr028.cs
// ---------------------------------------------------------------------------
struct MapCursor { // ovr028
    static Array<int> city_map_x;
    static Array<int> city_map_y;
    inline static int loc_X = 0; // word_1EF9C
    inline static int loc_Y = 0; // word_1EF9E
    static void SetPosition(int currentCity); //sub_6E005
    static void Draw(); /* sub_6E02E */
    static void Restore(); /* sub_6E05D */
};


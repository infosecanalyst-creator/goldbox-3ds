// engine/decl/ovr032.h - declarations for engine/ovr032.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr032.cpp.
// ---------------------------------------------------------------------------
// ovr032.cs
// ---------------------------------------------------------------------------
struct ovr032 {
    class MapReach_ {
    public:
        bool reach = false;
        Point target;
        int range = 0;
        MapReach_(bool r, int ra, Point p);
    };
    using MapReach = cs::Ref<MapReach_>;

    // PORT-NOTE: C# allocates a dense MapReach[1250, 1250, 2] (25 MB of pointers
    // on the 3DS). Same semantics with a sparse map: unset entries read as null.
    // coab never clears this cache between battles (line-of-sight results from
    // one battle map leak into the next) - kept for fidelity, see docs/KNOWN_COAB_BUGS.md.
    struct ReachCache {
        std::unordered_map<uint32_t, MapReach> m;
        MapReach& operator()(int a, int b, int c) { return m[((uint32_t)a * 1250u + (uint32_t)b) * 2u + (uint32_t)c]; }
    };
    inline static ReachCache mapReachCache;
    static MapReach MapCacheGet(Point p1, Point p2, bool ignoreWalls);
    static void canReachTarget(Point& target, Point attacker);
    static bool canReachTarget(int& range, Point target, Point attacker);
    static MapReach canReachTargetCalc(Struct_1D1BC groundTilesMap, Point outPos, Point attacker); /* sub_733F1 */
    static bool CanSeeCombatant(int direction, Point playerA, Point playerB); /* sub_7354A */
    static List<SortedCombatant> Rebuild_SortedCombatantList(Player player, int max_range, std::function<bool(Player)> filter); /* sub_738D8 */
    static List<SortedCombatant> Rebuild_SortedCombatantList(int size, int max_range, Point pos, std::function<bool(Player)> filter); /* sub_738D8 */
    static byte FindCombatantDirection(Point target, Point attacker);
};


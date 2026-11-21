// engine/decl/ovr019.h - declarations for engine/ovr019.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr019.cpp.
// ---------------------------------------------------------------------------
// ovr019.cs
// ---------------------------------------------------------------------------
struct ovr019 {
    static void SetPixel(byte colour, ushort row, ushort column);
    static byte GetPixel(ushort row, ushort column);
    static void sub_52068();
    class Struct_1ADFB_ {
    public:
        void Reset();
        byte field_0 = 0;
        byte field_1 = 0;
        byte field_2 = 0;
        byte field_3 = 0;
        byte field_4 = 0;
        byte operator[](int index) const; // C# indexer `internal byte this[int index] { get }`
    };
    using Struct_1ADFB = cs::Ref<Struct_1ADFB_>;

    static Array<Struct_1ADFB> unk_1ADFB;
    static void sub_520B8(Array<byte> arg_0, int arg_4, int arg_6, ushort arg_8, ushort arg_A);
    static void sub_524F7(Array<Struct_1ADF6> arg_2, int arg_6);
    static void sub_5279B(Array<byte> arg_0);
    static void endgame_5285E(byte arg_0, short arg_2, short& arg_4, ushort& arg_8, ushort& arg_C, ushort& arg_10); /* sub_5285E */
    static void endgame_529F4(); /* sub_529F4 */
    static void ShowAnimation(int num_loops, byte block_id, short row_y, short col_x); // sub_52B79
    static String aTyranthraxusSp;
    static String aStormGiant_You;
    static String aTheAmuletOfLyt;
    static String aYouOfYourVicto;
    static String aAsYouReachForT;
    static String aOutKeepTheGaun;
    static String aWillUnleashDan;
    static String aGauntletContac;
    static String aIAmTrappedWith;
    static String aWhereArmiesHav;
    static String aIsSlainThisDay;
    static String aNothingness_;
    static String aYouAreCertainH;
    static String aFinalBondFades;
    static String aHasFinallyBeen;
    static String aLast;
    static String aTheKnightsOfMy;
    static String aCongratulati_0;
    static String aWithThePowerOf;
    static String aThisFoulPlaceT;
    static String aYouAreTeleport;
    static String aHaveAlreadyBeg;
    static String aGharriAndNacac;
    static String aFromTheNearbyS;
    static void end_game_text();
};


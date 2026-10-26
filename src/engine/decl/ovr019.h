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

    inline static Array<Struct_1ADFB> unk_1ADFB = Array<Struct_1ADFB>(3);
    static void sub_520B8(Array<byte> arg_0, int arg_4, int arg_6, ushort arg_8, ushort arg_A);
    static void sub_524F7(Array<Struct_1ADF6> arg_2, int arg_6);
    static void sub_5279B(Array<byte> arg_0);
    static void endgame_5285E(byte arg_0, short arg_2, short& arg_4, ushort& arg_8, ushort& arg_C, ushort& arg_10); /* sub_5285E */
    static void endgame_529F4(); /* sub_529F4 */
    static void ShowAnimation(int num_loops, byte block_id, short row_y, short col_x); // sub_52B79
    inline static String aTyranthraxusSp = "Tyranthraxus' spirit coalesces over the slain ";
    inline static String aStormGiant_You = "storm giant. 'You have defeated me. Were it not for ";
    inline static String aTheAmuletOfLyt = "the Amulet of Lythander, I could possess you and rob ";
    inline static String aYouOfYourVicto = "you of your victory. Still I can escape through the pool.";
    inline static String aAsYouReachForT = "As you reach for the Pool of Radiance, he cries ";
    inline static String aOutKeepTheGaun = "out, 'Keep the Gauntlet of Moander away from there, you ";
    inline static String aWillUnleashDan = "will unleash dangerous energies. Stay back!' As the ";
    inline static String aGauntletContac = "gauntlet contacts the pool, it contracts and shatters it.";
    inline static String aIAmTrappedWith = "'I am trapped without escape, you have succeeded ";
    inline static String aWhereArmiesHav = "where armies have not. Gloat while you may, Tyranthraxus ";
    inline static String aIsSlainThisDay = "is slain this day.' Before your eyes he crumbles into ";
    inline static String aNothingness_ = "nothingness.";
    inline static String aYouAreCertainH = "You are certain he is destroyed because your ";
    inline static String aFinalBondFades = "final bond fades away. The Curse of the Azure Bonds ";
    inline static String aHasFinallyBeen = "has finally been lifted from you! You are free at ";
    inline static String aLast = "last!";
    inline static String aTheKnightsOfMy = "The Knights of Myth Drannor rush in, '";
    inline static String aCongratulati_0 = "Congratulations, you have destroyed the Flamed One. ";
    inline static String aWithThePowerOf = "With the power of Elminster, let us take you from ";
    inline static String aThisFoulPlaceT = "this  foul place, to a fine feast.'";
    inline static String aYouAreTeleport = "You are teleported to Shadowdale, where festivities ";
    inline static String aHaveAlreadyBeg = "have already begun. A huge cheer goes up at your arrival. ";
    inline static String aGharriAndNacac = "Gharri and Nacacia, arm in arm, yell congratulations ";
    inline static String aFromTheNearbyS = "from the nearby stands. 'You have won!'";
    static void end_game_text();
};


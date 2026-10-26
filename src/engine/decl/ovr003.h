// engine/decl/ovr003.h - declarations for engine/ovr003.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr003.cpp.
// ---------------------------------------------------------------------------
// ovr003.cs
// ---------------------------------------------------------------------------
struct ovr003 {
    static void CMD_Exit();
    static void CMD_Goto();
    static void CMD_Gosub();
    static void CMD_Compare(); // sub_2611D
    static void CMD_AddSubDivMulti(); // sub_2619A
    static void CMD_Random(); // sub_2623D
    static void CMD_Save();
    static void CMD_LoadCharacter(); /* sub_262E9 */
    static void CMD_SetupMonster(); /* sub_263C9 */
    static void CMD_LoadMonster(); /* sub_26465 */
    static void CMD_Approach(); // sub_26835
    static void CMD_Picture(); /* sub_26873 */
    static void CMD_InputNumber(); /* sub_2695E */
    static void CMD_InputString(); /* sub_269A4 */
    static void CMD_Print();
    static void CMD_Return();
    static void CMD_CompareAnd(); /* sub_26B0C */
    static void CMD_If();
    static void CMD_NewECL();
    static void CMD_LoadFiles(); /* sub_26C41 */
    static void CMD_AndOr(); /* sub_26DD0 */
    static void CMD_GetTable(); /* sub_26E3F */
    static void CMD_SaveTable(); /* sub_26E9D */
    static void CMD_VertMenu(); /* sub_26EE9 */
    static void CMD_HorizontalMenu();
    static void CMD_ClearMonsters(); /* sub_27240 */
    static void CMD_PartyStrength(); /* sub_272A9 */
    static void setMemoryFour(bool val_d, byte val_c, byte val_b, byte val_a, ushort loc_a, ushort loc_b, ushort loc_c, ushort loc_d); /* sub_273F6 */
    static void CMD_CheckParty(); /* sub_27454 */
    static void CMD_PartySurprise(); /* sub_2767E */
    static void CMD_Surprise(); /* sub_2771E */
    static void CMD_Combat(); // sub_277E4
    static void CMD_OnGotoGoSub(); /* sub_27AE5 */
    static void CMD_Treasure(); /* load_item */
    static void CMD_Rob(); /* sub_27F76*/
    static void CMD_EncounterMenu();
    static void CMD_Parlay(); /* talk_style */
    static void CMD_FindItem(); // sub_28856
    static void CMD_Delay();
    static void CMD_Damage(); /* sub_28958 */
    static void CMD_SpriteOff(); /* sub_28CB6 */
    static void CMD_EclClock(); /* sub_28CDA */
    static void CMD_PrintReturn(); // sub_28D0F
    static void CMD_ClearBox(); // sub_28D38
    static void CMD_Who(); // sub_28D7F
    static void CMD_AddNPC(); // sub_28DCA
    static void CMD_Spell();
    static void CMD_Call();
    static void TryEncamp();
    static void CMD_Program(); //YourHaveWon
    static void CMD_Protection(); // sub_2923F
    static void CMD_Dump(); // sub_29271
    static void CMD_FindSpecial(); // sub_292A5
    static void CMD_DestroyItems(); // sub_292F9
    inline static Dictionary<int, CmdItem> CommandTable;
    static void SetupCommandTable();
    static void SkipNextCommand();
    static void RunEclVm(ushort offset); // sub_29607
    static void sub_29677();
    static void sub_29758();
};


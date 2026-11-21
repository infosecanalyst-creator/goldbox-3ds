// engine/decl/seg049.h - declarations for engine/seg049.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg049.cpp.
// ---------------------------------------------------------------------------
// seg049.cs
// ---------------------------------------------------------------------------
struct seg049 {
    inline static byte skipReadFlag = 0;
    static void SysDelay(int milliseconds);
    static Queue<ushort> keysPressed;
    static cs::Mutex keysPressedLock; // PORT-NOTE: C# `lock (keysPressed)`
    // PORT-NOTE: C# `static Semaphore WaitForKey = new Semaphore(0, 1);` -> binary semaphore built from
    // a mutex + condition variable + count (0 or 1). Hand-ported.
    static cs::Mutex WaitForKeyLock;
    static cs::CondVar WaitForKey;
    inline static int WaitForKeyCount = 0;
    static void AddKey(ushort key);
    static ushort int_check_keyPressed();
    static ushort int_get_keyPressed();
    static bool KEYPRESSED();
    static byte READKEY();
    // goldbox-3ds: host shutdown request; the next key wait throws cs::EngineAbort.
    inline static bool abortRequested = false;
    static void RequestAbort();
    // goldbox-3ds: EXIT GAME's suspend save (rule 17). The host asks; the engine thread saves at its
    // next key wait (a point where the game itself waits for the player). 0 none, 1 asked, 2 saved,
    // 3 not possible here (combat, shop, start menu, no party), 4 saving.
    static volatile int suspendSave;
    static void RequestSuspendSave();
};


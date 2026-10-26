// engine/decl/seg001.h - declarations for engine/seg001.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg001.cpp.
// ---------------------------------------------------------------------------
// seg001.cs
// ---------------------------------------------------------------------------
struct seg001 {
    // PORT-NOTE: `internal static System.Threading.Thread EngineThread;` dropped; EngineStop() is hand-ported (platform::EngineStopped + throw cs::EngineAbort).
    using VoidDelegate = std::function<void()>;
    inline static VoidDelegate EngineStoppedCallback;
    static void EngineStop();
    static void __SystemInit(VoidDelegate stoppedCallback);
    static void ConfigGame();
    static void PROGRAM();
    static void InitFirst(); /* sub_39054 */
    static void InitAgain(); /* sub_396E5 */
};


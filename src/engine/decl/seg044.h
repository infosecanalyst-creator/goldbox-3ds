// engine/decl/seg044.h - declarations for engine/seg044.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg044.cpp.
// ---------------------------------------------------------------------------
// seg044.cs
// ---------------------------------------------------------------------------
struct seg044 {
    static void SetSound(bool On);
    static void SetPicture(bool On);
    static void SetAnimation(bool On);
    static void PlaySound(Sound arg_0); /*sub_120E0*/
    inline static Array<bool> soundLoaded; // PORT-NOTE: was `static System.Media.SoundPlayer[] sounds;` (13 slots); hand-ported against platform::LoadSound/PlaySound/StopAllSounds.
    static void SoundInit();
};


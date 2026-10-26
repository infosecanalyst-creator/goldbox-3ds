// Classes/Display.h - port of Classes/Display.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

// Hand-ported against platform/platform.h (see docs/TRANSLATION.md). The
// System.Drawing members (Bitmap bm, Rectangle rect) are dropped; RawCopy takes
// the raw video RAM and hands the frame to platform::PresentFrame.
struct IOSDisplay {
    virtual ~IOSDisplay() = default;
    virtual void Init(int height, int width) = 0;
    virtual void RawCopy(Array<byte> videoRam, int videoRamSize) = 0;
};

enum class TextRegion : int {
    NormalBottom,
    Normal2,
    CombatSummary,
};

struct Display {
private:
    inline static Array2<byte> OrigEgaColors{ { 0, 0, 0 }, { 0, 0, 173 }, { 0, 173, 0 }, { 0, 173, 173 }, { 173, 0, 0 }, { 173, 0, 173 }, { 173, 82, 0 }, { 173, 173, 173 }, { 82, 82, 82 }, { 82, 82, 255 }, { 82, 255, 82 }, { 82, 255, 255 }, { 255, 82, 82 }, { 255, 82, 255 }, { 255, 255, 82 }, { 255, 255, 255 } };
    inline static Array2<byte> egaColors{ { 0, 0, 0 }, { 0, 0, 173 }, { 0, 173, 0 }, { 0, 173, 173 }, { 173, 0, 0 }, { 173, 0, 173 }, { 173, 82, 0 }, { 173, 173, 173 }, { 82, 82, 82 }, { 82, 82, 255 }, { 82, 255, 82 }, { 82, 255, 255 }, { 255, 82, 82 }, { 255, 82, 255 }, { 255, 255, 82 }, { 255, 255, 255 } };
    inline static Array2<int> ram;
    inline static Array<byte> videoRam;
    inline static Array<byte> videoRamBkUp;
    inline static int videoRamSize = 0;
    inline static int scanLineWidth = 0;
    inline static int outputWidth = 0;
    inline static int outputHeight = 0;

    //static public Bitmap bm;                                  -- dropped (System.Drawing)
    //static Rectangle rect = new Rectangle(0, 0, 320, 200);   -- dropped (System.Drawing)

public:
    using VoidDeledate = std::function<void()>;

private:
    inline static VoidDeledate updateCallback;

public:
    // C# static set-only property UpdateCallback
    static void set_UpdateCallback(VoidDeledate value) { updateCallback = value; }

private:
    // static Display() { ... }  -> PORT-NOTE: static constructor; run before main()
    // through staticInit_.
    static bool StaticInit();
    inline static bool staticInit_ = StaticInit();

    inline static Array<int> MonoBitMask{ 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

public:
    static void DisplayMono8x8(int xCol, int yCol, Array<byte> monoData8x8, int bgColor, int fgColor);

    static void SetEgaPalette(int index, int colour);

private:
    static void SetVidPixel(int x, int y, int egaColor);

    inline static int noUpdateCount = 0;

public:
    static void UpdateStop();

    static void UpdateStart();

    static void Update();

    static void ForceUpdate();

    static void SaveVidRam();

    static void RestoreVidRam();

    static byte GetPixel(int x, int y);

    static void SetPixel3(int x, int y, int value);

    static void RawCopy(Array<byte> videoRam, int videoRamSize);
};

}  // namespace Classes

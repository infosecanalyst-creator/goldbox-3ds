// host/3ds/main.cpp - Nintendo 3DS host (New 3DS / New 3DS XL).
//
// Bottom screen (touch): the game itself, 320x200 pixel for pixel, with a
//   strip of big buttons in the 40 spare pixel rows underneath. Tap a word on
//   the game's menu line to pick it.
// Top screen: a dashboard - the current menu in large text and the controls.
//   While the on-screen keyboard is open (KEYS / SELECT) the keyboard takes the
//   bottom screen and the game moves to the top screen so you can see what you
//   are typing.
// Controls: D-pad / Circle Pad = arrows (or step the menu highlight in plain
//           menus), A = Enter, B = Esc, X = Yes, Y = Space,
//           L/R/ZL/ZR = diagonal moves (Home/PgUp/End/PgDn),
//           SELECT = keyboard, START+SELECT = quit.
//
// Taps and buttons only ever send the keys the original game expects, so the
// engine is unchanged (it still matches coab byte for byte in the tests).
//
// Data: put the game's files (*.DAX, ITEMS, ...) in sdmc:/3ds/goldbox/cotab/
// Saves and logs go to sdmc:/3ds/goldbox/user/.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include <3ds.h>

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "host/common/host.h"
#include "platform/keys.h"
#include "platform/platform.h"

namespace {

// ---------------------------------------------------------------------------
// Frame hand-off (engine thread -> main thread)
// ---------------------------------------------------------------------------
LightLock frameLock;
int egaFrame[320 * 200];
uint8_t framePal[16 * 3];
uint8_t font[64][8];  // the game's own 8x8 font, copied on the engine thread
bool fontReady = false;
volatile bool engineDone = false;

// ---------------------------------------------------------------------------
// Menu hand-off
// ---------------------------------------------------------------------------
LightLock menuLock;
std::string menuPrompt;
std::vector<platform::MenuItem> menuItems;
volatile bool menuAcceptsDirections = true;
volatile bool menuVisible = false;

// ---------------------------------------------------------------------------
// Sound
// ---------------------------------------------------------------------------
struct Sample {
    int16_t* pcm = nullptr;  // linear memory
    u32 frames = 0;
    u32 rate = 22050;
};
Sample samples[16];
bool ndspOk = false;
ndspWaveBuf waveBuf;

bool LoadWav(const char* path, Sample& out) {
    FILE* f = std::fopen(path, "rb");
    if (!f) return false;
    std::vector<uint8_t> d;
    uint8_t buf[4096];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof buf, f)) > 0) d.insert(d.end(), buf, buf + n);
    std::fclose(f);
    if (d.size() < 44 || std::memcmp(d.data(), "RIFF", 4) || std::memcmp(d.data() + 8, "WAVE", 4)) return false;
    size_t pos = 12;
    u16 fmt = 0, ch = 1, bits = 16;
    u32 rate = 22050;
    const uint8_t* data = nullptr;
    u32 dataLen = 0;
    while (pos + 8 <= d.size()) {
        u32 len = d[pos + 4] | (d[pos + 5] << 8) | (d[pos + 6] << 16) | ((u32)d[pos + 7] << 24);
        if (!std::memcmp(d.data() + pos, "fmt ", 4) && len >= 16) {
            fmt = d[pos + 8] | (d[pos + 9] << 8);
            ch = d[pos + 10] | (d[pos + 11] << 8);
            rate = d[pos + 12] | (d[pos + 13] << 8) | (d[pos + 14] << 16) | ((u32)d[pos + 15] << 24);
            bits = d[pos + 22] | (d[pos + 23] << 8);
        } else if (!std::memcmp(d.data() + pos, "data", 4)) {
            data = d.data() + pos + 8;
            dataLen = std::min<u32>(len, (u32)(d.size() - pos - 8));
        }
        pos += 8 + len + (len & 1);
    }
    if (fmt != 1 || !data || (bits != 16 && bits != 8)) return false;
    u32 frames = dataLen / (ch * (bits / 8));
    int16_t* pcm = (int16_t*)linearAlloc(frames * 2);
    if (!pcm) return false;
    for (u32 i = 0; i < frames; i++) {
        int v;
        if (bits == 16) v = (int16_t)(data[i * ch * 2] | (data[i * ch * 2 + 1] << 8));
        else v = ((int)data[i * ch] - 128) << 8;
        pcm[i] = (int16_t)v;
    }
    DSP_FlushDataCache(pcm, frames * 2);
    out.pcm = pcm;
    out.frames = frames;
    out.rate = rate;
    return true;
}

// ---------------------------------------------------------------------------
// Drawing straight into the framebuffers (column-major, 240 pixels per
// column, bottom-to-top, BGR8). Top is 400x240, bottom 320x240.
// ---------------------------------------------------------------------------
struct RGB {
    uint8_t r, g, b;
};
constexpr RGB kBlack{0, 0, 0};
constexpr RGB kDash{8, 12, 40};       // dashboard background (deep blue)
constexpr RGB kGold{255, 204, 64};
constexpr RGB kCyan{96, 224, 255};
constexpr RGB kWhite{240, 240, 240};
constexpr RGB kGrey{150, 150, 160};
constexpr RGB kRed{255, 96, 96};
constexpr RGB kBtn{40, 56, 120};       // button face
constexpr RGB kBtnEdge{120, 150, 230};
constexpr RGB kBtnDown{220, 180, 60};

struct Surface {
    u8* fb;
    int w;  // 400 (top) or 320 (bottom)
};

inline void Put(const Surface& s, int x, int y, RGB c) {
    if ((unsigned)x >= (unsigned)s.w || (unsigned)y >= 240u) return;
    u8* p = s.fb + ((size_t)x * 240 + (239 - y)) * 3;
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
}

void Fill(const Surface& s, int x0, int y0, int w, int h, RGB c) {
    int x1 = std::min(s.w, x0 + w), y1 = std::min(240, y0 + h);
    x0 = std::max(0, x0);
    y0 = std::max(0, y0);
    for (int x = x0; x < x1; x++) {
        u8* col = s.fb + (size_t)x * 240 * 3;
        for (int y = y0; y < y1; y++) {
            u8* p = col + (239 - y) * 3;
            p[0] = c.b;
            p[1] = c.g;
            p[2] = c.r;
        }
    }
}

void Frame(const Surface& s, int x, int y, int w, int h, RGB c) {
    Fill(s, x, y, w, 1, c);
    Fill(s, x, y + h - 1, w, 1, c);
    Fill(s, x, y, 1, h, c);
    Fill(s, x + w - 1, y, 1, h, c);
}

// Text in the game's font (upper case only, like the game).
void Text(const Surface& s, int x, int y, const std::string& str, RGB c, int scale = 1) {
    if (!fontReady) return;
    for (char ch : str) {
        int g = std::toupper((unsigned char)ch) % 64;
        for (int r = 0; r < 8; r++) {
            uint8_t bits = font[g][r];
            for (int i = 0; i < 8; i++)
                if (bits & (0x80 >> i)) {
                    if (scale == 1) Put(s, x + i, y + r, c);
                    else Fill(s, x + i * scale, y + r * scale, scale, scale, c);
                }
        }
        x += 8 * scale;
    }
}

int TextWidth(const std::string& str, int scale = 1) { return (int)str.size() * 8 * scale; }

// The game frame, 1:1, at (ox, oy).
void BlitGame(const Surface& s, int ox, int oy) {
    LightLock_Lock(&frameLock);
    for (int x = 0; x < 320; x++) {
        u8* col = s.fb + (size_t)(x + ox) * 240 * 3;
        for (int y = 0; y < 200; y++) {
            const uint8_t* c = framePal + egaFrame[y * 320 + x] * 3;
            u8* p = col + (239 - (y + oy)) * 3;
            p[0] = c[2];
            p[1] = c[1];
            p[2] = c[0];
        }
    }
    LightLock_Unlock(&frameLock);
}

// ---------------------------------------------------------------------------
// Touch buttons (pixel rectangles on the bottom screen)
// ---------------------------------------------------------------------------
constexpr uint16_t kToggleKeyboard = 0xFFFF;

struct Button {
    int x, y, w, h;
    uint16_t key;
    std::string label;
};

bool keyboardOpen = false;
int keyboardPage = 0;  // 0 = letters, 1 = digits/symbols
uint16_t flashKey = 0;  // last pressed button, drawn highlighted briefly
u64 flashUntil = 0;

std::vector<Button> StripButtons() {
    // The 40 rows under the game frame: y 200..239.
    static const struct {
        const char* label;
        uint16_t key;
    } defs[] = {{"ESC", keys::Escape}, {"YES", (uint16_t)'Y'}, {"NO", (uint16_t)'N'},
                {"ENTER", keys::Enter}, {"SPACE", keys::Space}, {"KEYS", kToggleKeyboard}};
    std::vector<Button> b;
    int n = 6, gap = 3, w = (320 - gap * (n + 1)) / n;
    for (int i = 0; i < n; i++) b.push_back({gap + i * (w + gap), 203, w, 34, defs[i].key, defs[i].label});
    return b;
}

std::vector<Button> KeyboardButtons() {
    std::vector<Button> b;
    static const char* letters[] = {"1234567890", "QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"};
    static const char* symbols[] = {"1234567890", "-.,", "", ""};
    const char** rows = keyboardPage == 0 ? letters : symbols;
    const int kw = 29, kh = 30, gap = 3;
    for (int r = 0; r < 4; r++) {
        int len = (int)std::strlen(rows[r]);
        int x0 = (320 - (len * kw + (len - 1) * gap)) / 2;
        for (int i = 0; i < len; i++) {
            char c = rows[r][i];
            uint16_t key = c == '-' ? keys::Minus : c == '.' ? keys::Period : c == ',' ? keys::Comma : keys::Letter(c);
            b.push_back({x0 + i * (kw + gap), 8 + r * (kh + gap), kw, kh, key, std::string(1, c)});
        }
    }
    int y = 8 + 4 * (kh + gap) + 4;
    b.push_back({4, y, 60, kh, keys::Escape, "ESC"});
    b.push_back({68, y, 116, kh, keys::Space, "SPACE"});
    b.push_back({188, y, 60, kh, keys::Backspace, "BKSP"});
    b.push_back({252, y, 64, kh, keys::Enter, "ENTER"});
    y += kh + gap;
    b.push_back({4, y, 60, kh, 0xFFFE, keyboardPage == 0 ? "-.," : "ABC"});
    b.push_back({68, y, 56, kh, keys::Home, "HOME"});
    b.push_back({128, y, 56, kh, keys::End, "END"});
    b.push_back({188, y, 60, kh, keys::Delete, "DEL"});
    b.push_back({252, y, 64, kh, kToggleKeyboard, "DONE"});
    return b;
}

void DrawButtons(const Surface& s, const std::vector<Button>& bs, u64 now) {
    for (auto& b : bs) {
        bool lit = flashKey == b.key && now < flashUntil;
        Fill(s, b.x, b.y, b.w, b.h, lit ? kBtnDown : kBtn);
        Frame(s, b.x, b.y, b.w, b.h, kBtnEdge);
        Text(s, b.x + (b.w - TextWidth(b.label)) / 2, b.y + (b.h - 8) / 2, b.label, lit ? kBlack : kWhite);
    }
}

// A tap on the game frame: the menu line is text row 24 (y 192..199). Accept
// taps a little above it too, and snap to the nearest word on that line.
uint16_t MenuWordAt(int px, int py, const std::vector<platform::MenuItem>& items) {
    if (py < 180 || py >= 200 || items.empty()) return 0;
    int best = -1, bestDist = 1 << 30;
    for (size_t i = 0; i < items.size(); i++) {
        int x0 = items[i].col * 8, x1 = (items[i].col + std::max(1, items[i].width)) * 8;
        int d = px < x0 ? x0 - px : px >= x1 ? px - x1 + 1 : 0;
        if (d < bestDist) {
            bestDist = d;
            best = (int)i;
        }
    }
    if (best < 0 || bestDist > 12) return 0;
    unsigned char k = (unsigned char)items[best].key;
    return keys::Letter((char)k);
}

// ---------------------------------------------------------------------------
// Screens
// ---------------------------------------------------------------------------
void DrawDashboard(const Surface& top, const std::string& prompt, const std::vector<platform::MenuItem>& items) {
    Fill(top, 0, 0, 400, 240, kDash);
    Text(top, (400 - TextWidth("CURSE OF THE AZURE BONDS", 2)) / 2, 8, "CURSE OF THE AZURE BONDS", kGold, 2);
    Fill(top, 16, 30, 368, 1, kBtnEdge);

    int y = 40;
    std::string p = prompt;
    while (!p.empty() && p.back() == ' ') p.pop_back();
    if (!p.empty()) {
        Text(top, 16, y, p.substr(0, 23), kCyan, 2);
        y += 22;
    }
    if (items.empty()) {
        Text(top, 16, y + 8, "TAP THE GAME OR USE THE BUTTONS", kGrey);
    } else {
        // Menu words in big type, two columns.
        int shown = std::min<int>((int)items.size(), 12);
        for (int i = 0; i < shown; i++) {
            int cx = 16 + (i % 2) * 188, cy = y + (i / 2) * 20;
            if (cy > 170) break;
            std::string label = items[i].label;
            if (label.size() > 11) label.resize(11);
            Text(top, cx, cy, label, kWhite, 2);
        }
    }

    Fill(top, 16, 196, 368, 1, kBtnEdge);
    Text(top, 16, 202, "TAP A WORD ON THE MENU LINE TO PICK IT", kGrey);
    Text(top, 16, 214, "D-PAD MOVE/STEP  A ENTER  B ESC  X YES  Y SPACE", kGrey);
    Text(top, 16, 226, "SELECT KEYBOARD   START+SELECT QUIT", kGrey);
    if (!ndspOk) Text(top, 16, 186, "NO SOUND: RUN DSP1 ONCE TO DUMP DSPFIRM.CDC", kRed);
}

void DrawKeyboardTop(const Surface& top) {
    Fill(top, 0, 0, 400, 240, kBlack);
    BlitGame(top, 40, 20);
    Text(top, (400 - TextWidth("KEYBOARD - TAP DONE TO RETURN")) / 2, 226, "KEYBOARD - TAP DONE TO RETURN", kGrey);
}

// ---------------------------------------------------------------------------
// Engine thread
// ---------------------------------------------------------------------------
void EngineEntry(void*) {
    host::Options opt;
    opt.dataDir = "sdmc:/3ds/goldbox/cotab";
    opt.userDir = "sdmc:/3ds/goldbox/user";
    host::RunEngine(opt);
}

}  // namespace

// ---------------------------------------------------------------------------
// platform.h implementation
// ---------------------------------------------------------------------------
namespace platform {

void PresentFrame(const int* ega, const uint8_t* palette) {
    uint8_t f[64][8];
    bool ok = host::CopyFont(f);  // engine thread: safe to read engine state here
    LightLock_Lock(&frameLock);
    std::memcpy(egaFrame, ega, sizeof egaFrame);
    std::memcpy(framePal, palette, sizeof framePal);
    if (ok) {
        std::memcpy(font, f, sizeof font);
        fontReady = true;
    }
    LightLock_Unlock(&frameLock);
}

void LoadSound(int id, const char* name) {
    if (id < 0 || id >= 16 || !ndspOk) return;
    std::string path = std::string("romfs:/sounds/") + name + ".wav";
    LoadWav(path.c_str(), samples[id]);
}

void PlaySound(int id) {
    if (!ndspOk || id < 0 || id >= 16 || !samples[id].pcm) return;
    ndspChnWaveBufClear(0);
    ndspChnSetRate(0, (float)samples[id].rate);
    std::memset(&waveBuf, 0, sizeof waveBuf);
    waveBuf.data_vaddr = samples[id].pcm;
    waveBuf.nsamples = samples[id].frames;
    ndspChnWaveBufAdd(0, &waveBuf);
}

void StopAllSounds() {
    if (ndspOk) ndspChnWaveBufClear(0);
}

void Sleep(int ms) { svcSleepThread((s64)ms * 1000000LL); }

void MenuShow(const std::string& prompt, const std::vector<MenuItem>& items, bool acceptsDirections) {
    LightLock_Lock(&menuLock);
    menuPrompt = prompt;
    menuItems = items;
    menuAcceptsDirections = acceptsDirections;
    menuVisible = !items.empty();
    LightLock_Unlock(&menuLock);
}

void MenuHide() {
    LightLock_Lock(&menuLock);
    menuItems.clear();
    menuPrompt.clear();
    menuVisible = false;
    menuAcceptsDirections = true;
    LightLock_Unlock(&menuLock);
}

void EngineStopped() { engineDone = true; }

}  // namespace platform

int main(int argc, char** argv) {
    osSetSpeedupEnable(true);  // New 3DS: 804 MHz + L2 cache
    gfxInitDefault();
    romfsInit();
    LightLock_Init(&frameLock);
    LightLock_Init(&menuLock);

    ndspOk = R_SUCCEEDED(ndspInit());
    if (ndspOk) {
        ndspSetOutputMode(NDSP_OUTPUT_MONO);
        ndspChnReset(0);
        ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
        ndspChnSetFormat(0, NDSP_FORMAT_MONO_PCM16);
        float mix[12] = {1.0f, 1.0f};
        ndspChnSetMix(0, mix);
    }

    // No game data? Explain where it goes instead of exiting silently.
    {
        FILE* probe = std::fopen("sdmc:/3ds/goldbox/cotab/GEO2.DAX", "rb");
        if (!probe) probe = std::fopen("sdmc:/3ds/goldbox/cotab/geo2.dax", "rb");
        if (!probe) {
            PrintConsole con;
            consoleInit(GFX_BOTTOM, &con);
            std::printf("\x1b[2;1H\x1b[31;1mGame data not found.\x1b[0m\n\n"
                        " Copy the Curse of the Azure Bonds\n"
                        " files (*.DAX, ITEMS, GAME.OVR ...)\n"
                        " from your GOG install to:\n\n"
                        "   sdmc:/3ds/goldbox/cotab/\n\n"
                        " Press START to exit.");
            while (aptMainLoop()) {
                hidScanInput();
                if (hidKeysDown() & KEY_START) break;
                gfxFlushBuffers();
                gfxSwapBuffers();
                gspWaitForVBlank();
            }
            romfsExit();
            gfxExit();
            return 0;
        }
        std::fclose(probe);
    }

    gfxSetDoubleBuffering(GFX_TOP, true);
    gfxSetDoubleBuffering(GFX_BOTTOM, true);

    // Engine thread: lower priority than the UI loop, 1 MB stack, on the
    // application core.
    s32 prio = 0x30;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    Thread engine = threadCreate(EngineEntry, nullptr, 1024 * 1024, prio + 1, -2, false);

    u64 nextRepeat = 0;
    uint16_t heldKey = 0;
    bool quit = false;

    auto press = [&](uint16_t k, u64 now) {
        flashKey = k;
        flashUntil = now + 150;
        if (k == kToggleKeyboard) keyboardOpen = !keyboardOpen;
        else if (k == 0xFFFE) keyboardPage ^= 1;
        else if (k) host::PushKey(k);
    };

    while (aptMainLoop() && !engineDone && !quit) {
        hidScanInput();
        u32 down = hidKeysDown();
        u32 held = hidKeysHeld();
        u64 now = osGetTime();

        if ((held & KEY_START) && (held & KEY_SELECT)) quit = true;
        if (down & KEY_SELECT && !(held & KEY_START)) keyboardOpen = !keyboardOpen;

        if (down & KEY_A) host::PushKey(keys::Enter);
        if (down & KEY_B) host::PushKey(keys::Escape);
        if (down & KEY_Y) host::PushKey(keys::Space);
        if (down & KEY_X) host::PushKey(keys::Letter('Y'));  // common "Yes" answer
        if (down & KEY_L) host::PushKey(keys::Home);
        if (down & KEY_R) host::PushKey(keys::PageUp);
        if (down & KEY_ZL) host::PushKey(keys::End);
        if (down & KEY_ZR) host::PushKey(keys::PageDown);

        // Directions with key repeat (D-pad, Circle Pad, C-stick).
        // In a plain menu (no movement accepted) coab moves the highlight with
        // ',' and '.', so the D-pad does that; A then picks the highlighted item.
        bool menuMode = menuVisible && !menuAcceptsDirections;
        uint16_t dirKey = 0;
        if (held & (KEY_DUP | KEY_CPAD_UP)) dirKey = menuMode ? keys::Comma : keys::Up;
        else if (held & (KEY_DDOWN | KEY_CPAD_DOWN)) dirKey = menuMode ? keys::Period : keys::Down;
        else if (held & (KEY_DLEFT | KEY_CPAD_LEFT)) dirKey = menuMode ? keys::Comma : keys::Left;
        else if (held & (KEY_DRIGHT | KEY_CPAD_RIGHT)) dirKey = menuMode ? keys::Period : keys::Right;
        if (dirKey != heldKey) {
            heldKey = dirKey;
            if (dirKey) {
                host::PushKey(dirKey);
                nextRepeat = now + 350;
            }
        } else if (dirKey && now >= nextRepeat) {
            host::PushKey(dirKey);
            nextRepeat = now + 120;
        }

        std::string prompt;
        std::vector<platform::MenuItem> items;
        LightLock_Lock(&menuLock);
        prompt = menuPrompt;
        items = menuItems;
        LightLock_Unlock(&menuLock);

        std::vector<Button> buttons = keyboardOpen ? KeyboardButtons() : StripButtons();

        if (down & KEY_TOUCH) {
            touchPosition t;
            hidTouchRead(&t);
            uint16_t hit = 0;
            bool onButton = false;
            for (auto& b : buttons)
                if (t.px >= b.x - 1 && t.px < b.x + b.w + 1 && t.py >= b.y - 2 && t.py < b.y + b.h + 2) {
                    hit = b.key;
                    onButton = true;
                    break;
                }
            if (!onButton && !keyboardOpen) hit = MenuWordAt(t.px, t.py, items);
            if (onButton || hit) press(hit, now);
        }

        u16 w, h;
        Surface top{gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &w, &h), 400};
        Surface bottom{gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, &w, &h), 320};

        if (keyboardOpen) {
            DrawKeyboardTop(top);
            Fill(bottom, 0, 0, 320, 240, kDash);
            DrawButtons(bottom, buttons, now);
        } else {
            DrawDashboard(top, prompt, items);
            BlitGame(bottom, 0, 0);
            Fill(bottom, 0, 200, 320, 40, kDash);
            DrawButtons(bottom, buttons, now);
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    host::RequestQuit();
    threadJoin(engine, U64_MAX);
    threadFree(engine);
    if (ndspOk) ndspExit();
    romfsExit();
    gfxExit();
    return 0;
}

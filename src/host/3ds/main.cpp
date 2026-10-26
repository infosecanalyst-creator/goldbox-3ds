// host/3ds/main.cpp - Nintendo 3DS host (New 3DS / New 3DS XL).
//
// Top screen: the game's 320x200 frame, 1:1, centred.
// Bottom screen (touch): the game's current menu as big buttons, plus an
// on-screen keyboard for names and text input.
// Controls: D-pad / Circle Pad = arrows, A = Enter, B = Esc, Y = Space,
//           L/R/ZL/ZR = diagonal moves (Home/PgUp/End/PgDn),
//           SELECT = toggle keyboard page, START+SELECT = quit.
//
// Data: put the game's files (*.DAX, ITEMS, ...) in sdmc:/3ds/goldbox/cotab/
// Saves and logs go to sdmc:/3ds/goldbox/user/.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include <3ds.h>

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
volatile bool engineDone = false;

// ---------------------------------------------------------------------------
// Menu hand-off
// ---------------------------------------------------------------------------
LightLock menuLock;
std::string menuPrompt;
std::vector<platform::MenuItem> menuItems;
volatile int menuVersion = 0;

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
// Bottom screen UI (libctru console: 40x30 cells of 8x8 pixels)
// ---------------------------------------------------------------------------
PrintConsole bottomConsole;

struct Button {
    int col, row, width;  // in console cells
    uint16_t key;
    std::string label;
};
std::vector<Button> buttons;
int keyboardPage = 0;  // 0 = letters, 1 = digits/symbols
int drawnMenuVersion = -1;
int drawnPage = -1;

void AddButton(int col, int row, const std::string& label, uint16_t key) {
    buttons.push_back({col, row, (int)label.size() + 2, key, label});
}

void DrawUI() {
    consoleSelect(&bottomConsole);
    consoleClear();
    buttons.clear();

    std::string prompt;
    std::vector<platform::MenuItem> items;
    LightLock_Lock(&menuLock);
    prompt = menuPrompt;
    items = menuItems;
    drawnMenuVersion = menuVersion;
    LightLock_Unlock(&menuLock);

    std::printf("\x1b[0;0H\x1b[33;1m CURSE OF THE AZURE BONDS\x1b[0m   goldbox-3ds");
    // Menu buttons: rows 2..13, wrapped
    if (!prompt.empty()) std::printf("\x1b[2;1H\x1b[36m%.38s\x1b[0m", prompt.c_str());
    int col = 1, row = items.empty() ? 3 : 4;
    for (auto& it : items) {
        int w = (int)it.label.size() + 2;
        if (col + w > 39) {
            col = 1;
            row += 2;
        }
        if (row > 13) break;
        unsigned char k = (unsigned char)it.key;
        if (k >= 'a' && k <= 'z') k = (unsigned char)(k - 32);
        AddButton(col, row, it.label, (uint16_t)k);
        col += w + 1;
    }
    if (items.empty()) std::printf("\x1b[4;2H\x1b[37m(no menu - use buttons or keyboard)\x1b[0m");

    // Keyboard: rows 16..27
    static const char* rowsLetters[] = {"QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"};
    static const char* rowsDigits[] = {"1234567890", "-.,", ""};
    const char** rows = keyboardPage == 0 ? rowsLetters : rowsDigits;
    for (int r = 0; r < 3; r++) {
        const char* s = rows[r];
        int len = (int)std::strlen(s);
        int start = (40 - len * 4) / 2;
        for (int i = 0; i < len; i++) {
            char c = s[i];
            uint16_t key = c == '-' ? keys::Minus : c == '.' ? keys::Period : c == ',' ? keys::Comma : (uint16_t)(unsigned char)c;
            AddButton(start + i * 4, 16 + r * 2, std::string(1, c), key);
        }
    }
    AddButton(1, 23, "ESC", keys::Escape);
    AddButton(7, 23, "SPACE", keys::Space);
    AddButton(15, 23, "BKSP", keys::Backspace);
    AddButton(22, 23, "DEL", keys::Delete);
    AddButton(28, 23, "ENTER", keys::Enter);
    AddButton(1, 26, keyboardPage == 0 ? "123" : "ABC", 0xFFFF);  // page toggle
    AddButton(8, 26, "HOME", keys::Home);
    AddButton(15, 26, "PGUP", keys::PageUp);
    AddButton(22, 26, "END", keys::End);
    AddButton(28, 26, "PGDN", keys::PageDown);
    drawnPage = keyboardPage;

    for (auto& b : buttons) {
        bool menu = b.row < 15;
        std::printf("\x1b[%d;%dH%s[%s]\x1b[0m", b.row, b.col, menu ? "\x1b[42;30m" : "\x1b[47;30m", b.label.c_str());
    }
    std::printf("\x1b[29;0H\x1b[37m A=Enter B=Esc Y=Space  START+SELECT=quit\x1b[0m");
}

uint16_t HitTest(int px, int py) {
    int c = px / 8, r = py / 8;
    for (auto& b : buttons) {
        // generous hit box: one row above/below the label
        if (c >= b.col - 1 && c < b.col + b.width && r >= b.row - 1 && r <= b.row + 1) return b.key;
    }
    return 0;
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

void BlitTop() {
    u16 w, h;
    u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &w, &h);  // 240 x 400, column-major, BGR8
    LightLock_Lock(&frameLock);
    for (int x = 0; x < 320; x++) {
        u8* col = fb + (size_t)(x + 40) * 240 * 3;
        for (int y = 0; y < 200; y++) {
            const uint8_t* c = framePal + egaFrame[y * 320 + x] * 3;
            u8* p = col + (239 - (y + 20)) * 3;
            p[0] = c[2];
            p[1] = c[1];
            p[2] = c[0];
        }
    }
    LightLock_Unlock(&frameLock);
}

}  // namespace

// ---------------------------------------------------------------------------
// platform.h implementation
// ---------------------------------------------------------------------------
namespace platform {

void PresentFrame(const int* ega, const uint8_t* palette) {
    LightLock_Lock(&frameLock);
    std::memcpy(egaFrame, ega, sizeof egaFrame);
    std::memcpy(framePal, palette, sizeof framePal);
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

void MenuShow(const std::string& prompt, const std::vector<MenuItem>& items) {
    LightLock_Lock(&menuLock);
    menuPrompt = prompt;
    menuItems = items;
    menuVersion++;
    LightLock_Unlock(&menuLock);
}

void MenuHide() {
    LightLock_Lock(&menuLock);
    if (!menuItems.empty() || !menuPrompt.empty()) {
        menuItems.clear();
        menuPrompt.clear();
        menuVersion++;
    }
    LightLock_Unlock(&menuLock);
}

void EngineStopped() { engineDone = true; }

}  // namespace platform

int main(int argc, char** argv) {
    osSetSpeedupEnable(true);  // New 3DS: 804 MHz + L2 cache
    gfxInitDefault();
    gfxSetDoubleBuffering(GFX_TOP, true);
    consoleInit(GFX_BOTTOM, &bottomConsole);
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
            consoleSelect(&bottomConsole);
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

    // Engine thread: lower priority than the UI loop, 1 MB stack, on the
    // application core.
    s32 prio = 0x30;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    Thread engine = threadCreate(EngineEntry, nullptr, 1024 * 1024, prio + 1, -2, false);

    u64 nextRepeat = 0;
    uint16_t heldKey = 0;
    bool quit = false;

    while (aptMainLoop() && !engineDone && !quit) {
        hidScanInput();
        u32 down = hidKeysDown();
        u32 held = hidKeysHeld();

        if ((held & KEY_START) && (held & KEY_SELECT)) quit = true;
        if (down & KEY_SELECT && !(held & KEY_START)) keyboardPage ^= 1;

        if (down & KEY_A) host::PushKey(keys::Enter);
        if (down & KEY_B) host::PushKey(keys::Escape);
        if (down & KEY_Y) host::PushKey(keys::Space);
        if (down & KEY_X) host::PushKey(keys::Letter('Y'));  // common "Yes" answer
        if (down & KEY_L) host::PushKey(keys::Home);
        if (down & KEY_R) host::PushKey(keys::PageUp);
        if (down & KEY_ZL) host::PushKey(keys::End);
        if (down & KEY_ZR) host::PushKey(keys::PageDown);

        // Directions with key repeat (D-pad, Circle Pad, C-stick).
        uint16_t dirKey = 0;
        if (held & (KEY_DUP | KEY_CPAD_UP)) dirKey = keys::Up;
        else if (held & (KEY_DDOWN | KEY_CPAD_DOWN)) dirKey = keys::Down;
        else if (held & (KEY_DLEFT | KEY_CPAD_LEFT)) dirKey = keys::Left;
        else if (held & (KEY_DRIGHT | KEY_CPAD_RIGHT)) dirKey = keys::Right;
        u64 now = osGetTime();
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

        if (down & KEY_TOUCH) {
            touchPosition t;
            hidTouchRead(&t);
            uint16_t k = HitTest(t.px, t.py);
            if (k == 0xFFFF) keyboardPage ^= 1;
            else if (k) host::PushKey(k);
        }

        if (drawnMenuVersion != menuVersion || drawnPage != keyboardPage) DrawUI();

        BlitTop();
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

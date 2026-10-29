// host/3ds/main.cpp - Nintendo 3DS host (New 3DS / New 3DS XL).
//
// Layout switches by itself:
//  * Exploring and menus: the game on the top screen (3D slider: the view
//    window sits behind the frame); the touch screen shows the automap and
//    the game's current choices as large buttons.
//  * Combat: the game on the touch screen (tap to aim), the combat menu as
//    big buttons underneath, and a combat panel (party HP) on top.
//  SELECT / SWAP flips the automatic choice until combat starts or ends.
// A keyboard opens by itself when the game wants text (names, riddles) and a
// number pad when it asks "How much..."; KEYS opens the keyboard any time.
//
// Taps and buttons only ever send the keys the original game expects (a tap
// on a list entry walks the highlight there with Home/End, as coab does), so
// the engine is unchanged (it still matches coab byte for byte in the tests).
//
// Controls: D-pad / Circle Pad = move (8 ways in combat), or move the
//           highlight in menus and lists; A/START = Enter; B = Esc; X = Yes;
//           Y = Space (Backspace while typing); L/R = previous/next party
//           member (combat: diagonals with ZL/ZR; "Aim:" previous/next
//           target); ZL/ZR = page up/down in lists; SELECT = swap screens;
//           hold START+SELECT = quit.
//
// Data: put the game's files (*.DAX, ITEMS, ...) in sdmc:/3ds/goldbox/cotab/
// Saves and logs go to sdmc:/3ds/goldbox/user/.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include <3ds.h>

#include <algorithm>
#include <dirent.h>
#include <array>
#include <map>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "host/common/host.h"
#include "platform/keys.h"
#include "platform/platform.h"
#include "platform/readable_font.h"

namespace {

// ---------------------------------------------------------------------------
// Frame hand-off (engine thread -> main thread)
// ---------------------------------------------------------------------------
LightLock frameLock;
int egaFrame[320 * 200];
uint8_t framePal[16 * 3];
uint8_t depthBuf[320 * 200];  // depth layer per pixel (0 screen .. 4 far)
volatile bool engineDone = false;

// ---------------------------------------------------------------------------
// Menu hand-off
// ---------------------------------------------------------------------------
LightLock menuLock;
std::string menuPrompt;
std::vector<platform::MenuItem> menuItems;
volatile bool menuAcceptsDirections = true;
volatile bool menuVisible = false;

// Scrolling list currently waiting for input (guarded by menuLock).
struct ListState {
    bool active = false;
    int row0 = 0, col0 = 0, col1 = 0, rows = 0, hl = 0;
};
ListState listState;

// What character the game last drew in each text cell (guarded by frameLock).
struct Cell {
    char ch = 0;
    uint8_t fg = 0, bg = 0;
};
Cell textMap[25][40];

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

// Text in the clear 8x8 font (platform/readable_font.h).
void Text(const Surface& s, int x, int y, const std::string& str, RGB c, int scale = 1) {
    for (char ch : str) {
        const uint8_t* g = ReadableGlyph((unsigned char)ch);
        if (!g) g = ReadableGlyph('?');
        for (int r = 0; r < 8; r++) {
            uint8_t bits = g[r];
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
// State published by the engine thread (guarded by menuLock)
// ---------------------------------------------------------------------------
struct RosterState {
    bool shown = false;
    int row0 = 0, col0 = 0, col1 = 0, count = 0, selected = -1;
};
RosterState rosterState;
std::vector<platform::PartyMember> partyMembers;

// The map the game last drew in 3D (for the automap).
struct MapState {
    bool valid = false;
    int version = 0;  // bumped by every MapView
    int area = 0, block = 0, x = 0, y = 0, dir = 0;
    uint8_t cells[256] = {};
};
MapState mapState;

// Explored squares per map (key = area * 256 + block), 256 bits each; saved
// to the user folder so the automap remembers where you have been.
std::map<int, std::array<uint8_t, 32>> explored;
bool exploredDirty = false;
const char* kAutomapFile = "sdmc:/3ds/goldbox/user/automap.bin";

void LoadAutomap() {
    FILE* f = std::fopen(kAutomapFile, "rb");
    if (!f) return;
    int32_t key;
    std::array<uint8_t, 32> bits;
    while (std::fread(&key, sizeof key, 1, f) == 1 && std::fread(bits.data(), 1, 32, f) == 32) explored[key] = bits;
    std::fclose(f);
}

void SaveAutomap() {
    FILE* f = std::fopen(kAutomapFile, "wb");
    if (!f) return;
    for (auto& e : explored) {
        int32_t key = e.first;
        std::fwrite(&key, sizeof key, 1, f);
        std::fwrite(e.second.data(), 1, 32, f);
    }
    std::fclose(f);
    exploredDirty = false;
}

void MarkExplored(int key, int x, int y) {
    if ((unsigned)x >= 16u || (unsigned)y >= 16u) return;
    auto& bits = explored[key];
    uint8_t& b = bits[(y * 16 + x) >> 3];
    uint8_t m = (uint8_t)(1 << ((y * 16 + x) & 7));
    if (!(b & m)) {
        b |= m;
        exploredDirty = true;
    }
}

bool IsExplored(int key, int x, int y) {
    auto it = explored.find(key);
    if (it == explored.end() || (unsigned)x >= 16u || (unsigned)y >= 16u) return false;
    return it->second[(y * 16 + x) >> 3] & (1 << ((y * 16 + x) & 7));
}
struct AimState {
    bool active = false;
    int manual = 0, mx = 0, my = 0, tx = 0, ty = 0;
};
AimState aimState;
int entryKind = 0;  // 0 none, 1 text, 2 character name, 3 number
int entryMax = 0;

constexpr int kStateWilderness = 3, kStateDungeon = 4, kStateCombat = 5;

// ---------------------------------------------------------------------------
// Buttons
// ---------------------------------------------------------------------------
// Special (non-game) button codes.
constexpr uint16_t kToggleKeyboard = 0xFFFF;
constexpr uint16_t kKbPage = 0xFFFE;
constexpr uint16_t kSwap = 0xFFFD;
constexpr uint16_t kRandomName = 0xFFFC;
constexpr uint16_t kNumAll = 0xFFFB;
constexpr uint16_t kNumClear = 0xFFFA;
constexpr uint16_t kBook = 0xFFF9;

struct Button {
    int x, y, w, h;
    uint16_t key;  // key to send (or a special code); 0 = see listRow / member
    std::string label;
    int listRow = -1;  // tap walks the list highlight here
    int member = -1;   // tap selects this party member
    bool selected = false;
};

// Hotkey menus have no highlight of their own; the host keeps one (D-pad
// moves it, A picks it).
int hotFocus = 0;
constexpr uint16_t kFocusPrev = 0xFFF0, kFocusNext = 0xFFF1;

bool keyboardOpen = false;
bool keyboardAuto = false;  // opened by an entry hint (closes with it)
int keyboardPage = 0;       // 0 = letters, 1 = digits/symbols
bool swapped = false;   // this frame: game on the top screen
bool userFlip = false;  // SELECT / SWAP: flip the automatic choice
uint16_t flashKey = 0;  // last pressed button, drawn highlighted briefly
int flashRow = -1, flashMember = -1;
u64 flashUntil = 0;

std::vector<Button> StripButtons(const std::string& bookLabel = "BOOK") {
    // The 40 rows under the game frame: y 200..239.
    static const struct {
        const char* label;
        uint16_t key;
    } defs[] = {{"ESC", keys::Escape}, {"YES", (uint16_t)'Y'}, {"NO", (uint16_t)'N'},
                {"ENTER", keys::Enter}, {"KEYS", kToggleKeyboard}, {"SWAP", kSwap}};
    std::vector<Button> b;
    int n = 7, gap = 2, w = (320 - gap * (n + 1)) / n;
    for (int i = 0; i < n - 1; i++) b.push_back({gap + i * (w + gap), 203, w, 34, defs[i].key, defs[i].label});
    Button book{gap + (n - 1) * (w + gap), 203, w, 34, kBook, bookLabel};
    book.selected = bookLabel != "BOOK";  // gold when the game points at a journal entry
    b.push_back(book);
    return b;
}

std::vector<Button> KeyboardButtons(bool nameEntry) {
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
            // '-' as the real IBM key (scan 0x0C, ASCII '-') so text entry keeps it
            uint16_t key = c == '-' ? (uint16_t)0x0C2D : c == '.' ? keys::Period : c == ',' ? keys::Comma : keys::Letter(c);
            b.push_back({x0 + i * (kw + gap), 8 + r * (kh + gap), kw, kh, key, std::string(1, c)});
        }
    }
    int y = 8 + 4 * (kh + gap) + 4;
    b.push_back({4, y, 60, kh, keys::Escape, "ESC"});
    b.push_back({68, y, 116, kh, keys::Space, "SPACE"});
    b.push_back({188, y, 60, kh, keys::Backspace, "BKSP"});
    b.push_back({252, y, 64, kh, keys::Enter, "ENTER"});
    y += kh + gap;
    b.push_back({4, y, 60, kh, kKbPage, keyboardPage == 0 ? "-.," : "ABC"});
    if (nameEntry) b.push_back({68, y, 116, kh, kRandomName, "RANDOM"});
    else b.push_back({68, y, 116, kh, kNumClear, "CLEAR"});
    b.push_back({188, y, 128, kh, kToggleKeyboard, "HIDE"});
    return b;
}

std::vector<Button> NumberPadButtons(int maxValue) {
    std::vector<Button> b;
    const int kw = 58, kh = 40, gap = 5, x0 = 8, y0 = 30;
    static const char* digits = "789456123";
    for (int i = 0; i < 9; i++)
        b.push_back({x0 + (i % 3) * (kw + gap), y0 + (i / 3) * (kh + gap), kw, kh, (uint16_t)digits[i], std::string(1, digits[i])});
    int y = y0 + 3 * (kh + gap);
    b.push_back({x0, y, kw, kh, kNumClear, "CLEAR"});
    b.push_back({x0 + kw + gap, y, kw, kh, (uint16_t)'0', "0"});
    b.push_back({x0 + 2 * (kw + gap), y, kw, kh, keys::Backspace, "BKSP"});
    // Right column: ALL / OK / CANCEL
    int xr = x0 + 3 * (kw + gap) + 4, wr = 320 - xr - 4;
    if (maxValue > 0) b.push_back({xr, y0, wr, kh, kNumAll, "ALL"});
    b.push_back({xr, y0 + (kh + gap), wr, 2 * kh + gap, keys::Enter, "OK"});
    b.push_back({xr, y, wr, kh, keys::Escape, "CANCEL"});
    return b;
}

void DrawButtons(const Surface& s, const std::vector<Button>& bs, u64 now) {
    for (auto& b : bs) {
        bool lit = now < flashUntil && ((b.key && flashKey == b.key) || (b.listRow >= 0 && flashRow == b.listRow) ||
                                        (b.member >= 0 && flashMember == b.member));
        RGB face = lit ? kBtnDown : b.selected ? RGB{90, 70, 20} : kBtn;
        Fill(s, b.x, b.y, b.w, b.h, face);
        Frame(s, b.x, b.y, b.w, b.h, b.selected ? kGold : kBtnEdge);
        std::string label = b.label;
        int maxChars = std::max(1, (b.w - 6) / 8);
        if ((int)label.size() > maxChars) label.resize(maxChars);
        Text(s, b.x + (b.w - TextWidth(label)) / 2, b.y + (b.h - 8) / 2, label, lit ? kBlack : b.selected ? kGold : kWhite);
    }
}

const Button* ButtonAt(const std::vector<Button>& bs, int px, int py) {
    for (auto& b : bs)
        if (px >= b.x - 1 && px < b.x + b.w + 1 && py >= b.y - 2 && py < b.y + b.h + 2) return &b;
    return nullptr;
}

// ---------------------------------------------------------------------------
// Reading the game screen
// ---------------------------------------------------------------------------
// Is the character the game drew at (row, col) still on screen? (Pictures
// can be drawn over old text.) Compares the frame's pixels with the glyph.
// Call with frameLock held.
bool CellShows(int r, int c) {
    if ((unsigned)r >= 25u || (unsigned)c >= 40u) return false;
    const Cell& cell = textMap[r][c];
    const uint8_t* g = ReadableGlyph((unsigned char)cell.ch);
    if (!g || cell.fg == cell.bg) return false;
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++) {
            int want = (g[y] & (0x80 >> x)) ? cell.fg : cell.bg;
            if (egaFrame[(r * 8 + y) * 320 + c * 8 + x] != want) return false;
        }
    return true;
}

bool CellShowsLocked(int r, int c) {
    LightLock_Lock(&frameLock);
    bool v = CellShows(r, c);
    LightLock_Unlock(&frameLock);
    return v;
}

// Text of one row of the game screen, as the game last drew it.
std::string RowText(int r, int c0, int c1) {
    std::string t;
    LightLock_Lock(&frameLock);
    for (int c = std::max(0, c0); c <= std::min(39, c1); c++) t += CellShows(r, c) ? textMap[r][c].ch : ' ';
    LightLock_Unlock(&frameLock);
    while (!t.empty() && t.back() == ' ') t.pop_back();
    size_t f = t.find_first_not_of(' ');
    return f == std::string::npos ? std::string() : t.substr(f);
}

// Menu words actually visible on the menu line (some menus use hidden keys).
std::vector<platform::MenuItem> VisibleWords(const std::vector<platform::MenuItem>& items) {
    std::vector<platform::MenuItem> out;
    for (auto& it : items)
        if (CellShowsLocked(24, it.col)) out.push_back(it);
    return out;
}

// Hotkey menus such as "Create New Character / Add Character ...": the game
// draws each line with its first letter in a brighter colour and reads the
// letter as a key. Find those lines for the current menu's keys.
struct HotLine {
    int row, c0, c1;
    uint16_t key;
    std::string label;
};

std::vector<HotLine> FindHotLines(const std::vector<platform::MenuItem>& items) {
    std::vector<HotLine> out;
    if (items.empty()) return out;
    for (auto& it : items)
        if (it.label.size() != 1) return out;  // only menus whose keys are bare letters
    std::string keyset;
    for (auto& it : items) keyset += (char)std::toupper((unsigned char)it.key);
    LightLock_Lock(&frameLock);
    for (int r = 0; r < 24; r++)
        for (int c = 0; c + 1 < 40; c++) {
            if (!CellShows(r, c)) continue;
            char k = (char)std::toupper((unsigned char)textMap[r][c].ch);
            if (!std::isalpha((unsigned char)k) || keyset.find(k) == std::string::npos) continue;
            if (c > 0 && CellShows(r, c - 1) && textMap[r][c - 1].ch != ' ') continue;
            if (!CellShows(r, c + 1) || textMap[r][c + 1].fg == textMap[r][c].fg) continue;
            bool dup = false;
            for (auto& h : out) dup |= h.key == (uint16_t)k;
            if (dup) continue;
            HotLine h{r, c, c, (uint16_t)k, std::string(1, textMap[r][c].ch)};
            for (int x = c + 1; x < 40 && CellShows(r, x); x++) {
                if (textMap[r][x].ch == ' ' && (x + 1 >= 40 || !CellShows(r, x + 1) || textMap[r][x + 1].ch == ' ')) break;
                h.label += textMap[r][x].ch;
                h.c1 = x;
            }
            if (h.label.size() >= 3) out.push_back(h);
        }
    LightLock_Unlock(&frameLock);
    return out;
}

// Is the party list on screen right now? (First letter of each name visible.)
bool RosterVisible(const RosterState& r) {
    if (!r.shown || r.count <= 0) return false;
    for (int i = 0; i < r.count; i++)
        if (!CellShowsLocked(r.row0 + i, r.col0)) return false;
    return true;
}

std::string MemberName(const RosterState& r, int i) { return RowText(r.row0 + i, r.col0, std::min(r.col1, r.col0 + 14)); }

// The list's own action word ("Select", "Use" ...): the first menu word that
// is not Next / Prev / Exit.
uint16_t ListActionKey(const std::vector<platform::MenuItem>& items) {
    for (auto& it : items) {
        std::string l = it.label;
        for (auto& ch : l) ch = (char)std::tolower((unsigned char)ch);
        if (l != "next" && l != "prev" && l != "exit") return keys::Letter(it.key);
    }
    return keys::Enter;
}

int ListVerbCount(const std::vector<platform::MenuItem>& items) {
    int n = 0;
    for (auto& it : items) {
        std::string l = it.label;
        for (auto& ch : l) ch = (char)std::tolower((unsigned char)ch);
        if (l != "next" && l != "prev" && l != "exit") n++;
    }
    return n;
}

bool MenuSaysContinue(const std::string& prompt, const std::vector<platform::MenuItem>& items) {
    std::string all = prompt;
    for (auto& it : items) all += " " + it.label;
    for (auto& ch : all) ch = (char)std::tolower((unsigned char)ch);
    return all.find("continue") != std::string::npos;
}

// ---------------------------------------------------------------------------
// Walking a highlight to a tapped entry (lists and the party list): one
// Home/End per step, waiting for the game to show each step.
// ---------------------------------------------------------------------------
struct Walk {
    int kind = 0;  // 0 none, 1 list row, 2 party member, 3 combat aim square
    int target = 0, steps = 0, seenAtPush = -1;
    u64 nextStep = 0;
    int ax = 0, ay = 0;  // kind 3: map square to aim at
    u64 started = 0;
    int row0 = 0;           // kind 1: the list's first row (a different list cancels)
    bool thenAct = false;   // kind 1: send the list's verb on arrival
    bool sawManual = false; // kind 3: the free cursor appeared
    int lastSeen = -1;
    u64 lastProgress = 0;
};
Walk walk;

void StartWalk(int kind, int target, u64 now) {
    walk = Walk{};
    walk.kind = kind;
    walk.target = target;
    walk.nextStep = now;
    walk.started = now;
    walk.lastProgress = now;
}

// ---------------------------------------------------------------------------
// Taps
// ---------------------------------------------------------------------------
struct Context {
    std::string prompt;
    std::vector<platform::MenuItem> items, words;
    ListState list;
    RosterState roster;
    bool rosterVisible = false;
    std::vector<HotLine> hot;
    bool acceptsDirections = true;
    AimState aim;
    std::vector<platform::PartyMember> party;
    MapState map;
    int state = 0;
    int entry = 0, entryMaxValue = 0;
};

// A tap on the game frame (game at 0,0 on the bottom screen).
// Returns a key to send (0: none); may start a walk instead.
uint16_t GameTap(int px, int py, const Context& cx, u64 now) {
    int r = py / 8;
    // Combat aiming: tap a square on the combat map (7x7 squares of 24 px).
    if (cx.aim.active && px < 7 * 24 && py < 7 * 24) {
        int ax = px / 24 + cx.aim.tx, ay = py / 24 + cx.aim.ty;
        if (ax == cx.aim.mx && ay == cx.aim.my) return (uint16_t)'T';
        StartWalk(3, 0, now);
        walk.ax = ax;
        walk.ay = ay;
        return cx.aim.manual ? 0 : (uint16_t)'M';  // "Manual" gives a free cursor
    }
    const ListState& list = cx.list;
    if (list.active && r >= list.row0 && r < list.row0 + list.rows && px >= list.col0 * 8 - 8 && px < (list.col1 + 1) * 8 + 8) {
        int target = r - list.row0;
        if (target == list.hl) return ListActionKey(cx.items);
        StartWalk(1, target, now);
        walk.row0 = list.row0;
        walk.thenAct = ListVerbCount(cx.items) == 1;  // one verb (Select): pick it; else just highlight
        flashRow = target;
        return 0;
    }
    // Menu line: nearest visible word.
    if (py >= 180 && !cx.words.empty()) {
        int best = -1, bestDist = 1 << 30;
        for (size_t i = 0; i < cx.words.size(); i++) {
            int x0 = cx.words[i].col * 8, x1 = (cx.words[i].col + std::max(1, cx.words[i].width)) * 8;
            int d = px < x0 ? x0 - px : px >= x1 ? px - x1 + 1 : 0;
            if (d < bestDist) {
                bestDist = d;
                best = (int)i;
            }
        }
        if (best >= 0 && bestDist <= 12) return keys::Letter(cx.words[best].key);
    }
    // Hotkey lines.
    {
        int best = -1, bestDist = 1 << 30;
        for (size_t i = 0; i < cx.hot.size(); i++) {
            int x0 = cx.hot[i].c0 * 8 - 8, x1 = (cx.hot[i].c1 + 1) * 8 + 8;
            if (px < x0 || px >= x1) continue;
            int cy = cx.hot[i].row * 8 + 4, d = py > cy ? py - cy : cy - py;
            if (d < bestDist) {
                bestDist = d;
                best = (int)i;
            }
        }
        if (best >= 0 && bestDist <= 8) return cx.hot[best].key;
    }
    // Party list: select the tapped member.
    if (cx.rosterVisible && !list.active && r >= cx.roster.row0 && r < cx.roster.row0 + cx.roster.count &&
        px >= cx.roster.col0 * 8 - 4) {
        int target = r - cx.roster.row0;
        if (target != cx.roster.selected) {
            StartWalk(2, target, now);
            flashMember = target;
        }
        return 0;
    }
    // 3D view (88x88 at 16,16): forward / turn left / turn right / turn round.
    if (cx.state == kStateDungeon && cx.acceptsDirections && !cx.items.empty() && cx.items[0].label == "Area" && px >= 16 && px < 104 && py >= 16 &&
        py < 104) {
        if (px < 45) return keys::Left;
        if (px >= 75) return keys::Right;
        return py < 70 ? keys::Up : keys::Down;
    }
    // "Press enter to continue" and plain key waits: tap anywhere.
    if ((cx.items.empty() || MenuSaysContinue(cx.prompt, cx.items)) && host::PendingKeys() == 0) return keys::Enter;
    return 0;
}

// ---------------------------------------------------------------------------
// The swapped layout: the game's choices as large buttons.
// ---------------------------------------------------------------------------
std::vector<Button> ChoiceButtons(const Context& cx) {
    std::vector<Button> b;
    const int x0 = 4, w = 312;
    int y = 4;
    if (cx.list.active) {
        // Up to 6 entries around the highlight.
        int first = std::max(0, std::min(cx.list.hl - 2, cx.list.rows - 6));
        for (int i = first; i < cx.list.rows && i < first + 6; i++) {
            Button bt{x0, y, w, 24, 0, RowText(cx.list.row0 + i, cx.list.col0, cx.list.col1)};
            bt.listRow = i;
            bt.selected = i == cx.list.hl;
            if (!bt.label.empty()) {
                b.push_back(bt);
                y += 27;
            }
        }
    } else if (!cx.hot.empty()) {
        int n = (int)cx.hot.size();
        if (n <= 6) {
            for (int i = 0; i < n; i++) {
                Button bt{x0, y, w, 24, cx.hot[i].key, cx.hot[i].label};
                bt.selected = i == hotFocus;
                b.push_back(bt);
                y += 27;
            }
        } else {  // up to 12 lines (the start menu) in two columns
            int bw = (w - 4) / 2, rows = (std::min(n, 12) + 1) / 2;
            for (int i = 0; i < n && i < 12; i++) {
                Button bt{x0 + (i / rows) * (bw + 4), y + (i % rows) * 30, bw, 27, cx.hot[i].key, cx.hot[i].label};
                bt.selected = i == hotFocus;
                b.push_back(bt);
            }
            y += rows * 30;
        }
    }
    if (MenuSaysContinue(cx.prompt, cx.items) && cx.hot.empty() && !cx.list.active) {
        b.push_back({x0, 60, w, 60, keys::Enter, "CONTINUE"});
        return b;
    }
    // Menu words: up to 4 per row.
    if (!cx.words.empty() && y < 170) {
        int n = (int)cx.words.size(), perRow = std::min(4, n);
        int bw = (w - (perRow - 1) * 4) / perRow;
        for (int i = 0; i < n && y + 28 <= 200; i++) {
            int col = i % perRow;
            b.push_back({x0 + col * (bw + 4), y, bw, 26, keys::Letter(cx.words[i].key), cx.words[i].label});
            if (col == perRow - 1 || i == n - 1) y += 30;
        }
    }
    // Party members: 2 per row.
    if (cx.rosterVisible && !cx.list.active && y + 24 <= 200) {
        int bw = (w - 4) / 2;
        for (int i = 0; i < cx.roster.count && y + 24 <= 200; i++) {
            Button bt{x0 + (i % 2) * (bw + 4), y, bw, 22, 0, MemberName(cx.roster, i)};
            bt.member = i;
            bt.selected = i == cx.roster.selected;
            b.push_back(bt);
            if (i % 2 == 1 || i == cx.roster.count - 1) y += 25;
        }
    }
    if (b.empty() && (cx.items.empty() || MenuSaysContinue(cx.prompt, cx.items)))
        b.push_back({x0, 60, w, 60, keys::Enter, "CONTINUE"});
    return b;
}

// ---------------------------------------------------------------------------
// Screens
// ---------------------------------------------------------------------------
void DrawDashboard(const Surface& top, const Context& cx) {
    Fill(top, 0, 0, 400, 240, kDash);
    Text(top, (400 - TextWidth("Curse of the Azure Bonds", 2)) / 2, 6, "Curse of the Azure Bonds", kGold, 2);
    Fill(top, 16, 26, 368, 1, kBtnEdge);

    int y = 32;
    std::string p = cx.prompt;
    while (!p.empty() && p.back() == ' ') p.pop_back();
    if (!p.empty()) {
        if (p.size() <= 23) {
            Text(top, 16, y, p, kCyan, 2);
            y += 20;
        } else {  // long prompts in normal size, up to two lines
            Text(top, 16, y, p.substr(0, 46), kCyan);
            if (p.size() > 46) Text(top, 16, y + 10, p.substr(46, 46), kCyan);
            y += p.size() > 46 ? 24 : 14;
        }
    }
    if (cx.list.active) {
        // The list itself, large, with the highlighted entry in gold.
        int first = std::max(0, std::min(cx.list.hl - 3, cx.list.rows - 7));
        for (int i = first; i < cx.list.rows && i < first + 7; i++) {
            std::string t = RowText(cx.list.row0 + i, cx.list.col0, cx.list.col1);
            if (t.size() > 22) t.resize(22);
            Text(top, 24, y + (i - first) * 18, t, i == cx.list.hl ? kGold : kWhite, 2);
        }
    } else if (!cx.hot.empty()) {
        bool big = cx.hot.size() <= 7;
        for (size_t i = 0; i < cx.hot.size() && i < 12; i++) {
            std::string t = cx.hot[i].label;
            if (big) {
                if (t.size() > 22) t.resize(22);
                Text(top, 24, y + (int)i * 18, t, (int)i == hotFocus ? kGold : kWhite, 2);
            } else {
                Text(top, 24, y + (int)i * 11, ((int)i == hotFocus ? "> " : "  ") + t, (int)i == hotFocus ? kGold : kWhite);
            }
        }
    } else if (!cx.words.empty()) {
        int shown = std::min<int>((int)cx.words.size(), 8);
        for (int i = 0; i < shown; i++) {
            std::string label = cx.words[i].label;
            if (label.size() > 11) label.resize(11);
            Text(top, 16 + (i % 2) * 188, y + (i / 2) * 18, label, kWhite, 2);
        }
        y += ((shown + 1) / 2) * 18 + 4;
    } else {
        Text(top, 16, y + 8, "Tap the game to continue", kGrey);
        y += 24;
    }
    // Party: name, HP, AC (from the game's own party data).
    if (!cx.list.active) {
        y = std::max(y + 6, 120);
        for (auto& m : cx.party) {
            if (y > 176) break;
            char line[64];
            std::snprintf(line, sizeof line, "%c %-15s HP %3d/%-3d AC %d", m.active ? '>' : ' ', m.name.substr(0, 15).c_str(),
                          m.hp, m.hpMax, m.ac);
            Text(top, 16, y, line, m.active ? kGold : kGrey);  // NOLINT
            y += cx.party.size() > 6 ? 9 : 11;
        }
    }

    Fill(top, 16, 190, 368, 1, kBtnEdge);
    Text(top, 16, 196, "Tap a choice on the game screen to pick it", kGrey);
    Text(top, 16, 208, "D-pad move/choose  A Enter  B Esc  X Yes", kGrey);
    Text(top, 16, 220, "L/R party member  ZL/ZR page  SELECT swap", kGrey);
    if (!ndspOk) Text(top, 16, 180, "No sound: run DSP1 once (dspfirm.cdc)", kRed);
}

void DrawGameOnTop(const Surface& top, const char* caption) {
    Fill(top, 0, 0, 400, 240, kBlack);
    BlitGame(top, 40, 20);
    Text(top, (400 - TextWidth(caption)) / 2, 226, caption, kGrey);
}

// ---------------------------------------------------------------------------
// Automap (bottom screen while exploring with the game on top)
// ---------------------------------------------------------------------------
void DrawAutomap(const Surface& s, int ox, int oy, int cell, const MapState& m) {
    const int size = cell * 16;
    Fill(s, ox - 2, oy - 2, size + 4, size + 4, kBtnEdge);
    Fill(s, ox, oy, size, size, RGB{4, 6, 20});
    int key = m.area * 256 + m.block;
    for (int y = 0; y < 16; y++)
        for (int x = 0; x < 16; x++) {
            if (!IsExplored(key, x, y)) continue;
            int px = ox + x * cell, py = oy + y * cell;
            Fill(s, px, py, cell, cell, RGB{40, 52, 96});
            uint8_t c = m.cells[y * 16 + x];
            RGB wall = kWhite, door = kGold;
            if (c & 1) Fill(s, px, py, cell, 1, (c & 16) ? door : wall);
            if (c & 4) Fill(s, px, py + cell - 1, cell, 1, (c & 64) ? door : wall);
            if (c & 8) Fill(s, px, py, 1, cell, (c & 128) ? door : wall);
            if (c & 2) Fill(s, px + cell - 1, py, 1, cell, (c & 32) ? door : wall);
            // doors drawn thicker in the middle of the wall
            int mid = cell / 2;
            if (c & 16) Fill(s, px + mid - 2, py, 4, 2, door);
            if (c & 64) Fill(s, px + mid - 2, py + cell - 2, 4, 2, door);
            if (c & 128) Fill(s, px, py + mid - 2, 2, 4, door);
            if (c & 32) Fill(s, px + cell - 2, py + mid - 2, 2, 4, door);
        }
    // The party: a gold arrow pointing the way you face.
    int cx = ox + m.x * cell + cell / 2, cy = oy + m.y * cell + cell / 2;
    for (int i = 0; i < cell / 2; i++) {
        int half = i / 2;
        switch (m.dir & 6) {
            case 0: Fill(s, cx - half, cy - cell / 4 + i, half * 2 + 1, 1, kGold); break;           // north
            case 4: Fill(s, cx - half, cy + cell / 4 - i, half * 2 + 1, 1, kGold); break;           // south
            case 2: Fill(s, cx + cell / 4 - i, cy - half, 1, half * 2 + 1, kGold); break;           // east
            default: Fill(s, cx - cell / 4 + i, cy - half, 1, half * 2 + 1, kGold); break;          // west
        }
    }
}

bool ExplorePanelWanted(const Context& cx, const MapState& m) {
    return m.valid && cx.state == kStateDungeon && !cx.list.active && cx.hot.empty() && cx.entry == 0;
}

// Explore layout: automap on the left, the menu words as a column on the right.
std::vector<Button> ExploreButtons(const Context& cx) {
    std::vector<Button> b;
    int x = 158, w = 158, y = 4, n = (int)cx.words.size();
    if (MenuSaysContinue(cx.prompt, cx.items)) {  // "press <enter>/<return> to continue"
        b.push_back({x, 4, w, 60, keys::Enter, "CONTINUE"});
        return b;
    }
    int h = n > 6 ? 20 : 24, gap = n > 6 ? 2 : 3;
    for (int i = 0; i < n && y + h <= 198; i++) {
        b.push_back({x, y, w, h, keys::Letter(cx.words[i].key), cx.words[i].label});
        y += h + gap;
    }
    if (b.empty() && (cx.items.empty() || MenuSaysContinue(cx.prompt, cx.items)))
        b.push_back({x, 60, w, 60, keys::Enter, "CONTINUE"});
    return b;
}

// Combat with the game on the touch screen: the strip shows the combat menu
// words (Move, View, Aim, Use, Cast ...) as big buttons, plus ESC.
std::vector<Button> CombatStrip(const Context& cx) {
    std::vector<Button> b;
    int n = std::min<int>((int)cx.words.size(), 7) + 1, gap = 2, w = (320 - gap * (n + 1)) / n;
    for (int i = 0; i < n - 1; i++)
        b.push_back({gap + i * (w + gap), 203, w, 34, keys::Letter(cx.words[i].key), cx.words[i].label});
    b.push_back({gap + (n - 1) * (w + gap), 203, w, 34, keys::Escape, "ESC"});
    return b;
}

// ---------------------------------------------------------------------------
// Combat panel (top screen while the game is on the touch screen in combat)
// ---------------------------------------------------------------------------
void DrawCombatPanel(const Surface& top, const std::vector<platform::PartyMember>& party) {
    Fill(top, 0, 0, 400, 240, RGB{24, 6, 10});
    Text(top, (400 - TextWidth("Combat", 2)) / 2, 6, "Combat", kRed, 2);
    Fill(top, 16, 26, 368, 1, RGB{160, 60, 60});
    int y = 34;
    for (auto& m : party) {
        if (y > 176) break;
        bool down = m.hp <= 0;
        RGB name = m.active ? kGold : down ? kGrey : kWhite;
        std::string n = m.name.substr(0, 12);
        if (m.active) Fill(top, 8, y - 3, 384, 22, RGB{70, 50, 10});
        Text(top, 16, y, (m.active ? ">" : " ") + n, name, 2);
        // HP bar
        int bx = 236, bw = 100, frac = m.hpMax > 0 ? std::max(0, std::min(bw, m.hp * bw / m.hpMax)) : 0;
        RGB bar = m.hp * 3 > m.hpMax * 2 ? RGB{60, 200, 80} : m.hp * 3 > m.hpMax ? RGB{230, 200, 60} : RGB{230, 70, 60};
        Fill(top, bx, y + 2, bw, 10, RGB{50, 30, 30});
        Fill(top, bx, y + 2, frac, 10, bar);
        Text(top, bx + bw + 6, y + 3, std::to_string(m.hp), name);
        Text(top, 236, y + 13, "AC " + std::to_string(m.ac) + (m.status.empty() || m.status == "Okay" ? "" : "  " + m.status), kGrey);
        y += party.size() > 6 ? 19 : 26;
    }
    Fill(top, 16, 190, 368, 1, RGB{160, 60, 60});
    Text(top, 16, 196, "Tap a square to aim, a word to act", kGrey);
    Text(top, 16, 208, "D-pad/Circle Pad move (diagonals OK)", kGrey);
    Text(top, 16, 220, "L/ZL R/ZR diagonals   SELECT swap", kGrey);
}

// ---------------------------------------------------------------------------
// The game on the top screen, with optional 3D: the view/picture window
// (88x88 at 16,16) sits behind the screen; frame and text at the screen.
// ---------------------------------------------------------------------------
void BlitGameShifted(const Surface& s, int ox, int oy, int shift) {
    LightLock_Lock(&frameLock);
    for (int x = 0; x < 320; x++) {
        u8* col = s.fb + (size_t)(x + ox) * 240 * 3;
        for (int y = 0; y < 200; y++) {
            int sx = x;
            bool black = false;
            if (shift && x >= 16 && x < 104 && y >= 16 && y < 104) {
                sx = x - shift;
                black = sx < 16 || sx > 103;  // uncovered by the shift: dark, not smeared
            }
            static const uint8_t kZero[3] = {0, 0, 0};
            const uint8_t* c = black ? kZero : framePal + egaFrame[y * 320 + sx] * 3;
            u8* p = col + (239 - (y + oy)) * 3;
            p[0] = c[2];
            p[1] = c[1];
            p[2] = c[0];
        }
    }
    LightLock_Unlock(&frameLock);
}

// The game scaled 6/5 to 384x240 (fills the top screen's height, keeps the
// shape), with the same recessed view window for 3D.
// eye: -1 left, +1 right, 0 flat. strength: the 3D slider (0..1). Each pixel's
// depth layer L (from the game's 3D view: 1 near walls .. 4 sky/floor) is
// shown shifted by eye * L * maxShift source pixels, so the corridor recedes
// into the screen; the nearest layer claiming a pixel wins.
void BlitGameScaled(const Surface& s, int ox, int eye, float strength) {
    // Disparity (source pixels, per eye) for a depth d in 0..4: at most
    // ~2.5 px for the furthest things at full slider; walls by distance,
    // floor and sky graded by row (near the view's edges = close, the
    // horizon = far) so the ground doesn't look like a flat backdrop.
    bool stereo = eye != 0 && strength > 0.01f;
    const float maxD = 2.5f * strength;
    auto shiftFor = [&](float d) { return eye * (int)(maxD * d / 4.0f + 0.5f); };
    const int wallShift[4] = {0, shiftFor(1.3f), shiftFor(2.3f), shiftFor(3.1f)};
    LightLock_Lock(&frameLock);
    for (int x = 0; x < 384; x++) {
        u8* col = s.fb + (size_t)(x + ox) * 240 * 3;
        int sx0 = x * 5 / 6;
        for (int y = 0; y < 240; y++) {
            int sy = y * 5 / 6, sx = sx0;
            if (stereo && sx0 >= 16 && sx0 < 104 && sy >= 16 && sy < 104) {
                // inside the 3D view: the nearest layer that lands here wins
                float dist = std::abs(sy - 60) / 44.0f;  // 0 at the horizon, 1 at the edges
                int bgShift = shiftFor(4.0f - 3.0f * std::min(1.0f, dist));
                int found = -1;
                for (int L = 0; L < 5 && found < 0; L++) {
                    int cx = sx0 - (L < 4 ? wallShift[L] : bgShift);
                    if (cx >= 16 && cx < 104 && depthBuf[sy * 320 + cx] == L) found = cx;
                }
                // a gap opened by the shift: show what is behind (the ground/sky)
                sx = found >= 0 ? found : std::max(16, std::min(103, sx0 - bgShift));
            }
            const uint8_t* c = framePal + egaFrame[sy * 320 + sx] * 3;
            u8* p = col + (239 - y) * 3;
            p[0] = c[2];
            p[1] = c[1];
            p[2] = c[0];
        }
    }
    // Text: scaling pixels by 6/5 makes letters uneven, so redraw every text
    // cell the game shows with its glyph unscaled (crisp), centred in the
    // enlarged 9.6 x 9.6 cell.
    for (int r = 0; r < 25; r++)
        for (int c = 0; c < 40; c++) {
            if (!CellShows(r, c)) continue;
            const Cell& cell = textMap[r][c];
            const uint8_t* g = ReadableGlyph((unsigned char)cell.ch);
            int x0 = c * 48 / 5, x1 = (c + 1) * 48 / 5, y0 = r * 48 / 5, y1 = (r + 1) * 48 / 5;
            const uint8_t* bg = framePal + cell.bg * 3;
            const uint8_t* fg = framePal + cell.fg * 3;
            Fill(s, ox + x0, y0, x1 - x0, y1 - y0, RGB{bg[0], bg[1], bg[2]});
            int gx = ox + x0 + (x1 - x0 - 8) / 2, gy = y0 + (y1 - y0 - 8) / 2;
            for (int yy = 0; yy < 8; yy++)
                for (int xx = 0; xx < 8; xx++)
                    if (g[yy] & (0x80 >> xx)) Put(s, gx + xx, gy + yy, RGB{fg[0], fg[1], fg[2]});
        }
    LightLock_Unlock(&frameLock);
}

void DrawGameTop(const Surface& left, const Surface& right, float depth, const char* /*caption*/) {
    Fill(left, 0, 0, 8, 240, kBlack);
    Fill(left, 392, 0, 8, 240, kBlack);
    BlitGameScaled(left, 8, -1, depth);
    if (depth > 0.01f) {
        Fill(right, 0, 0, 8, 240, kBlack);
        Fill(right, 392, 0, 8, 240, kBlack);
        BlitGameScaled(right, 8, 1, depth);
    } else {
        std::memcpy(right.fb, left.fb, 400 * 240 * 3);
    }
}

// ---------------------------------------------------------------------------
// The book: controls manual (romfs), journal / game manual / quick reference
// (text files on the SD card, made from the user's own PDFs; not shipped).
// Standard controls for all the 3DS ports: Circle Pad / C-Stick scroll the
// text (hold to repeat), D-Pad moves between section buttons (A opens),
// L/R previous/next section, tap a tab or section, B or START closes.
// ---------------------------------------------------------------------------
struct BookSection {
    std::string title;
    std::vector<std::string> lines;  // wrapped to the top screen
};
struct Book {
    std::string title;
    std::vector<BookSection> sections;
};
std::vector<Book> books;

struct Viewer {
    bool open = false;
    int book = 0, section = 0, scroll = 0, focus = 0;
    u64 nextScroll = 0, nextFocus = 0;
} viewer;

constexpr int kWrap = 48, kTextRows = 23;

void WrapInto(std::vector<std::string>& out, const std::string& para) {
    std::string line;
    size_t i = 0;
    while (i < para.size()) {
        size_t j = para.find(' ', i);
        if (j == std::string::npos) j = para.size();
        std::string word = para.substr(i, j - i);
        while (word.size() > (size_t)kWrap) {  // very long token
            if (!line.empty()) { out.push_back(line); line.clear(); }
            out.push_back(word.substr(0, kWrap));
            word = word.substr(kWrap);
        }
        if (!line.empty() && line.size() + 1 + word.size() > (size_t)kWrap) {
            out.push_back(line);
            line.clear();
        }
        if (!word.empty()) line += (line.empty() ? "" : " ") + word;
        i = j + 1;
    }
    if (!line.empty()) out.push_back(line);
}

void LoadBookFile(const std::string& path) {
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return;
    Book b;
    std::string para, line;
    auto flush = [&]() {
        if (!para.empty() && !b.sections.empty()) {
            WrapInto(b.sections.back().lines, para);
            b.sections.back().lines.push_back("");
        }
        para.clear();
    };
    char buf[1024];
    while (std::fgets(buf, sizeof buf, f)) {
        line = buf;
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
        if (line.rfind("## ", 0) == 0) {
            flush();
            b.sections.push_back({line.substr(3), {}});
        } else if (line.rfind("# ", 0) == 0) {
            flush();
            b.title = line.substr(2);
        } else if (line.empty()) {
            flush();
        } else {
            para += (para.empty() ? "" : " ") + line;
        }
    }
    flush();
    std::fclose(f);
    if (b.title.empty()) b.title = "Book";
    if (!b.sections.empty()) books.push_back(b);
}

void LoadBooks() {
    books.clear();
    for (const char* dir : {"romfs:/manual", "sdmc:/3ds/goldbox/manual"}) {
        std::vector<std::string> names;
        if (DIR* d = opendir(dir)) {
            while (dirent* e = readdir(d)) {
                std::string n = e->d_name;
                if (n.size() > 4 && n.substr(n.size() - 4) == ".txt") names.push_back(n);
            }
            closedir(d);
        }
        std::sort(names.begin(), names.end());
        for (auto& n : names) LoadBookFile(std::string(dir) + "/" + n);
    }
}

bool IsJournal(const Book& b) { return b.title == "Journal"; }

// Short label for a section button ("Entry 12" -> "12" in the journal).
std::string SectionLabel(const Book& b, const BookSection& s) {
    if (IsJournal(b) && s.title.rfind("Entry ", 0) == 0) return s.title.substr(6);
    if (IsJournal(b)) return "Notes";
    return s.title;
}

void OpenViewerAtEntry(int entry) {
    viewer.open = true;
    viewer.scroll = 0;
    if (entry > 0)
        for (size_t bi = 0; bi < books.size(); bi++)
            if (IsJournal(books[bi]))
                for (size_t si = 0; si < books[bi].sections.size(); si++)
                    if (books[bi].sections[si].title == "Entry " + std::to_string(entry)) {
                        viewer.book = (int)bi;
                        viewer.section = viewer.focus = (int)si;
                        return;
                    }
    if (viewer.book >= (int)books.size()) viewer.book = 0;
}

// Section buttons for the current book: a grid of numbers for the journal,
// two columns of titles otherwise. Scrolls to keep the focus in view.
std::vector<Button> ViewerButtons() {
    std::vector<Button> b;
    // Tabs
    int nb = (int)books.size() + 1, gap = 2, tw = (320 - gap * (nb + 1)) / nb;
    for (int i = 0; i < (int)books.size(); i++) {
        Button t{gap + i * (tw + gap), 2, tw, 24, (uint16_t)(0xF000 + i), books[i].title};
        t.selected = i == viewer.book;
        b.push_back(t);
    }
    b.push_back({gap + (nb - 1) * (tw + gap), 2, tw, 24, kBook, "CLOSE"});
    if (books.empty()) return b;
    const Book& bk = books[viewer.book];
    int n = (int)bk.sections.size();
    bool grid = IsJournal(bk);
    int cols = grid ? 8 : 2, bw = grid ? 37 : 156, bh = grid ? 24 : 22, rowsVisible = grid ? 8 : 9;
    int focusRow = viewer.focus / cols, firstRow = std::max(0, focusRow - rowsVisible / 2);
    int totalRows = (n + cols - 1) / cols;
    firstRow = std::max(0, std::min(firstRow, totalRows - rowsVisible));
    for (int i = firstRow * cols; i < n && i < (firstRow + rowsVisible) * cols; i++) {
        int r = i / cols - firstRow, c = i % cols;
        Button sb{4 + c * (bw + 2), 30 + r * (bh + 2), bw, bh, (uint16_t)(0xE000 + i), SectionLabel(bk, bk.sections[i])};
        sb.selected = i == viewer.section;
        b.push_back(sb);
    }
    return b;
}

void ViewerFrame(u32 down, u32 held, u64 now, const Surface& top, const Surface& topR, const Surface& bottom) {
    if (books.empty()) LoadBooks();
    if (books.empty()) {
        viewer.open = false;
        return;
    }
    viewer.book = std::max(0, std::min(viewer.book, (int)books.size() - 1));
    Book& bk = books[viewer.book];
    int n = (int)bk.sections.size();
    viewer.section = std::max(0, std::min(viewer.section, n - 1));
    viewer.focus = std::max(0, std::min(viewer.focus, n - 1));
    auto openSection = [&](int i) {
        viewer.section = viewer.focus = std::max(0, std::min(i, n - 1));
        viewer.scroll = 0;
    };
    if (down & (KEY_B | KEY_START)) viewer.open = false;
    if (down & KEY_L) openSection(viewer.section - 1);
    if (down & KEY_R) openSection(viewer.section + 1);
    if (down & KEY_A) openSection(viewer.focus);
    // D-Pad: move the focus between section buttons (repeat when held).
    int cols = IsJournal(bk) ? 8 : 2, df = 0;
    if (held & KEY_DUP) df = -cols;
    else if (held & KEY_DDOWN) df = cols;
    else if (held & KEY_DLEFT) df = -1;
    else if (held & KEY_DRIGHT) df = 1;
    if (df && ((down & (KEY_DUP | KEY_DDOWN | KEY_DLEFT | KEY_DRIGHT)) || now >= viewer.nextFocus)) {
        viewer.focus = std::max(0, std::min(viewer.focus + df, n - 1));
        viewer.nextFocus = now + ((down & (KEY_DUP | KEY_DDOWN | KEY_DLEFT | KEY_DRIGHT)) ? 350 : 110);
    }
    // Circle Pad / C-Stick: scroll the text (repeat when held).
    const BookSection& sec = bk.sections[viewer.section];
    int maxScroll = std::max(0, (int)sec.lines.size() - kTextRows);
    int ds = 0;
    if (held & (KEY_CPAD_UP | KEY_CSTICK_UP)) ds = -1;
    else if (held & (KEY_CPAD_DOWN | KEY_CSTICK_DOWN)) ds = 1;
    if (ds && now >= viewer.nextScroll) {
        viewer.scroll = std::max(0, std::min(viewer.scroll + ds, maxScroll));
        viewer.nextScroll = now + 60;
    }

    std::vector<Button> buttons = ViewerButtons();
    if (down & KEY_TOUCH) {
        touchPosition t;
        hidTouchRead(&t);
        if (const Button* b = ButtonAt(buttons, t.px, t.py)) {
            if (b->key == kBook) viewer.open = false;
            else if (b->key >= 0xF000) {
                viewer.book = b->key - 0xF000;
                viewer.section = viewer.focus = viewer.scroll = 0;
            } else if (b->key >= 0xE000) openSection(b->key - 0xE000);
        }
    }

    // Top: the text
    const Book& bk2 = books[viewer.book];
    const BookSection& s2 = bk2.sections[std::min(viewer.section, (int)bk2.sections.size() - 1)];
    int maxS2 = std::max(0, (int)s2.lines.size() - kTextRows);
    viewer.scroll = std::min(viewer.scroll, maxS2);
    Fill(top, 0, 0, 400, 240, RGB{14, 12, 6});
    Text(top, 8, 4, bk2.title + "  -  " + s2.title.substr(0, 34), kGold);
    Fill(top, 8, 14, 384, 1, RGB{120, 100, 40});
    for (int i = 0; i < kTextRows && viewer.scroll + i < (int)s2.lines.size(); i++)
        Text(top, 8, 19 + i * 9, s2.lines[viewer.scroll + i], RGB{235, 225, 200});
    if (maxS2 > 0) {  // scroll bar
        int h = std::max(10, 210 * kTextRows / (int)s2.lines.size());
        int y = 19 + (210 - h) * viewer.scroll / maxS2;
        Fill(top, 395, 19, 3, 210, RGB{50, 44, 30});
        Fill(top, 395, y, 3, h, kGold);
    }
    std::memcpy(topR.fb, top.fb, 400 * 240 * 3);

    // Bottom: tabs, section buttons, focus, help
    Fill(bottom, 0, 0, 320, 240, kDash);
    DrawButtons(bottom, buttons, now);
    for (auto& b : buttons)
        if (b.key == 0xE000 + viewer.focus) Frame(bottom, b.x - 2, b.y - 2, b.w + 4, b.h + 4, kWhite);
    Text(bottom, 4, 218, "Pad/C-Stick scroll  D-Pad+A pick  L/R prev/next", kGrey);
    Text(bottom, 4, 229, "Tap a section     B or START close", kGrey);
}

// "...READ JOURNAL ENTRY 12..." on the game screen: the entry number, or 0.
int DetectJournalEntry() {
    bool journal = false;
    int entry = 0;
    for (int r = 0; r < 25; r++) {
        std::string t = RowText(r, 0, 39);
        for (auto& ch : t) ch = (char)std::toupper((unsigned char)ch);
        if (t.find("JOURNAL") != std::string::npos) journal = true;
        size_t p = t.find("ENTRY");
        while (p != std::string::npos) {
            size_t q = p + 5;
            while (q < t.size() && t[q] == ' ') q++;
            int v = 0, digits = 0;
            while (q < t.size() && std::isdigit((unsigned char)t[q]) && digits < 3) v = v * 10 + (t[q++] - '0'), digits++;
            if (digits) entry = v;
            p = t.find("ENTRY", p + 5);
        }
    }
    return journal ? entry : 0;
}

// ---------------------------------------------------------------------------
// Random names for new characters (Forgotten Realms flavour).
// ---------------------------------------------------------------------------
const char* const kNames[] = {"Aldric", "Brenna", "Corwin", "Dagny", "Elric", "Fenna", "Gareth", "Hilde",
                              "Ivor", "Jessa", "Kerrik", "Lyra", "Marek", "Nessa", "Orrin", "Perrin",
                              "Quillon", "Rhiannon", "Soren", "Talia", "Ulric", "Vala", "Wulfgar", "Xandra",
                              "Yorick", "Zara", "Thorin", "Mirabel", "Garrick", "Seraphine", "Dorn", "Elowen"};

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

void PresentDepth(const uint8_t* depth) {
    LightLock_Lock(&frameLock);
    std::memcpy(depthBuf, depth, sizeof depthBuf);
    LightLock_Unlock(&frameLock);
}

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

void ListShow(int firstRow, int firstCol, int lastCol, int rows, int highlightRow) {
    LightLock_Lock(&menuLock);
    listState = {true, firstRow, firstCol, lastCol, rows, highlightRow};
    LightLock_Unlock(&menuLock);
}

void ListHide() {
    LightLock_Lock(&menuLock);
    listState.active = false;
    LightLock_Unlock(&menuLock);
}

void RosterShow(int firstRow, int firstCol, int lastCol, int count, int selected) {
    LightLock_Lock(&menuLock);
    rosterState = {true, firstRow, firstCol, lastCol, count, selected};
    LightLock_Unlock(&menuLock);
}

void AimShow(int manual, int mapX, int mapY, int topX, int topY) {
    LightLock_Lock(&menuLock);
    aimState = {true, manual, mapX, mapY, topX, topY};
    LightLock_Unlock(&menuLock);
}

void AimHide() {
    LightLock_Lock(&menuLock);
    aimState.active = false;
    LightLock_Unlock(&menuLock);
}

void PartyStatus(const std::vector<PartyMember>& members) {
    LightLock_Lock(&menuLock);
    partyMembers = members;
    LightLock_Unlock(&menuLock);
}

void MapView(int area, int block, int x, int y, int dir, const uint8_t* cells) {
    LightLock_Lock(&menuLock);
    mapState.valid = true;
    mapState.version++;
    mapState.area = area;
    mapState.block = block;
    mapState.x = x;
    mapState.y = y;
    mapState.dir = dir;
    std::memcpy(mapState.cells, cells, 256);
    LightLock_Unlock(&menuLock);
}

void EntryBegin(int kind, int maxValue) {
    LightLock_Lock(&menuLock);
    entryKind = kind;
    entryMax = maxValue;
    LightLock_Unlock(&menuLock);
}

void EntryEnd() {
    LightLock_Lock(&menuLock);
    entryKind = 0;
    entryMax = 0;
    LightLock_Unlock(&menuLock);
}

void TextDrawn(int row, int col, char ch, int fg, int bg) {
    if ((unsigned)row >= 25u || (unsigned)col >= 40u) return;
    LightLock_Lock(&frameLock);
    textMap[row][col] = {ch, (uint8_t)fg, (uint8_t)bg};
    LightLock_Unlock(&frameLock);
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
    gfxSet3D(true);  // the game window gets depth when the 3D slider is up
    LoadAutomap();
    std::srand((unsigned)osGetTime());

    // Engine thread: lower priority than the UI loop, 1 MB stack, on the
    // application core.
    s32 prio = 0x30;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    Thread engine = threadCreate(EngineEntry, nullptr, 1024 * 1024, prio + 1, -2, false);

    struct {
        std::string sig;
        std::vector<HotLine> hot;
        std::vector<platform::MenuItem> words;
        bool rosterVisible = false;
        u64 at = 0;
    } cache;
    u64 nextRepeat = 0;
    uint16_t heldKey = 0;
    bool dirSent = false;
    u64 dirSince = 0;
    int lastEntry = 0;
    bool quit = false;
    int journalEntry = 0;
    u64 journalSeenAt = 0;
    u64 quitSince = 0;
    std::vector<platform::MenuItem> lastItems;
    std::string lastPrompt;
    bool lastAccepts = true;
    u64 lastMenuAt = 0;
    int mapLeftVersion = -1;
    bool startUsedForCombo = false;
    bool lastCombat = false;
    int lastMapX = -1, lastMapY = -1, lastMapDir = -1, lastMapBlock = -1;
    u64 lastSave = 0;

    while (aptMainLoop() && !engineDone && !quit) {
        hidScanInput();
        u32 down = hidKeysDown();
        u32 held = hidKeysHeld();
        u64 now = osGetTime();

        // Snapshot what the engine has published.
        Context cx;
        LightLock_Lock(&menuLock);
        cx.prompt = menuPrompt;
        cx.items = menuItems;
        cx.list = listState;
        cx.roster = rosterState;
        cx.aim = aimState;
        cx.party = partyMembers;
        cx.map = mapState;
        cx.acceptsDirections = menuAcceptsDirections;
        cx.entry = entryKind;
        cx.entryMaxValue = entryMax;
        LightLock_Unlock(&menuLock);
        cx.state = host::GameStateNow();
        bool combat = cx.state == kStateCombat;
        // Between two menus (e.g. while a step in the 3D view is drawn) the
        // game shows no menu for a moment; keep the last one on screen so
        // the touch screen doesn't flicker.
        if (!cx.items.empty()) {
            lastItems = cx.items;
            lastPrompt = cx.prompt;
            lastAccepts = cx.acceptsDirections;
            lastMenuAt = now;
        } else if (now < lastMenuAt + 400 && cx.entry == 0) {
            cx.items = lastItems;
            cx.prompt = lastPrompt;
            cx.acceptsDirections = lastAccepts;
        }
        // Leaving the 3D view (wilderness, load, combat) makes the map stale
        // until the game draws the 3D view again.
        if (cx.state != kStateDungeon) mapLeftVersion = cx.map.version;
        if (cx.map.version == mapLeftVersion) cx.map.valid = false;
        // Automatic layout: the game on the top screen while exploring and in
        // menus, on the touch screen in combat (stylus aiming). SELECT/SWAP
        // flips it until combat starts or ends.
        if (combat != lastCombat) {
            userFlip = false;
            lastCombat = combat;
        }
        swapped = !combat != userFlip;

        // Automap: mark where the party stands and what it can see ahead.
        if (cx.map.valid && cx.state == kStateDungeon &&
            (cx.map.x != lastMapX || cx.map.y != lastMapY || cx.map.dir != lastMapDir || cx.map.block != lastMapBlock)) {
            int key = cx.map.area * 256 + cx.map.block;
            MarkExplored(key, cx.map.x, cx.map.y);
            static const int dx[4] = {0, 1, 0, -1}, dy[4] = {-1, 0, 1, 0};
            int d = (cx.map.dir & 6) / 2, x = cx.map.x, y = cx.map.y;
            for (int step = 0; step < 3; step++) {
                if (cx.map.cells[(y & 15) * 16 + (x & 15)] & (1 << d)) break;  // wall ahead
                x += dx[d];
                y += dy[d];
                if ((unsigned)x >= 16u || (unsigned)y >= 16u) break;
                MarkExplored(key, x, y);
            }
            lastMapX = cx.map.x;
            lastMapY = cx.map.y;
            lastMapDir = cx.map.dir;
            lastMapBlock = cx.map.block;
        }
        if (exploredDirty && (now >= lastSave + 60000 || cx.state != kStateDungeon)) {
            SaveAutomap();
            lastSave = now;
        }
        bool menuMode = !cx.items.empty() && !cx.acceptsDirections;

        // Keyboard opens and closes by itself for text entry.
        if (cx.entry != lastEntry) {
            if (cx.entry == 1 || cx.entry == 2) {
                keyboardOpen = true;
                keyboardAuto = true;
                keyboardPage = 0;
            } else if (keyboardAuto) {
                keyboardOpen = false;
                keyboardAuto = false;
            }
            lastEntry = cx.entry;
        }
        bool numberPad = cx.entry == 3;
        bool overlay = keyboardOpen || numberPad;

        if (!overlay) {
            cx.words = VisibleWords(cx.items);
            cx.hot = cx.list.active ? std::vector<HotLine>() : FindHotLines(cx.items);
            cx.rosterVisible = RosterVisible(cx.roster);
            // While the game redraws, text can be missing for a frame or two;
            // keep the last choices for the same menu so buttons don't flicker.
            std::string sig = cx.prompt + "|";
            for (auto& it : cx.items) sig += it.key;
            if (sig == cache.sig && now < cache.at + 500) {
                if (cx.hot.empty() && !cx.list.active) cx.hot = cache.hot;
                if (cx.words.empty()) cx.words = cache.words;
                if (!cx.rosterVisible) cx.rosterVisible = cache.rosterVisible;
            }
            if (sig != cache.sig) hotFocus = 0;
            if (sig != cache.sig || !cx.hot.empty() || !cx.words.empty() || cx.rosterVisible) {
                cache.sig = sig;
                cache.hot = cx.hot;
                cache.words = cx.words;
                cache.rosterVisible = cx.rosterVisible;
                cache.at = now;
            }
            if (hotFocus >= (int)cx.hot.size()) hotFocus = 0;
        }
        bool hotMenu = !overlay && !cx.hot.empty() && !cx.list.active;
        // The 3D view's own menu ("Area Cast View Encamp Search Look"): arrows walk.
        bool explore3d = false;
        if (cx.state == kStateDungeon)
            for (auto& it : cx.items) explore3d |= it.label == "Area";

        // The book takes over both screens and all input while open.
        if (viewer.open) {
            u16 vw, vh;
            Surface vtop{gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &vw, &vh), 400};
            Surface vtopR{gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, &vw, &vh), 400};
            Surface vbottom{gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, &vw, &vh), 320};
            ViewerFrame(down, held, now, vtop, vtopR, vbottom);
            gfxFlushBuffers();
            gfxSwapBuffers();
            gspWaitForVBlank();
            continue;
        }
        if (!overlay) {
            int e = DetectJournalEntry();
            if (e > 0) {
                journalEntry = e;
                journalSeenAt = now;
            } else if (journalEntry && now > journalSeenAt + 120000) {
                journalEntry = 0;  // the hint fades two minutes after the text is gone
            }
        }

        // ---- buttons ----
        // Quit: hold START+SELECT for a second (a warning shows meanwhile).
        if ((held & KEY_START) && (held & KEY_SELECT)) {
            if (!quitSince) quitSince = now;
            if (now >= quitSince + 1000) quit = true;
            startUsedForCombo = true;
        } else {
            quitSince = 0;
        }
        if ((down & KEY_SELECT) && !(held & KEY_START)) {
            userFlip = !userFlip;
            swapped = !swapped;
        }

        if (down & KEY_A) host::PushKey(hotMenu ? cx.hot[hotFocus].key : keys::Enter);
        // START = Enter on release, unless it was part of START+SELECT.
        if (down & KEY_START) startUsedForCombo = (held & KEY_SELECT) != 0;
        if ((hidKeysUp() & KEY_START) && !startUsedForCombo) host::PushKey(keys::Enter);
        if (down & KEY_B) host::PushKey(keys::Escape);
        if (cx.entry == 1 || cx.entry == 2) {
            if (down & KEY_Y) host::PushKey(keys::Backspace);  // typing: Y deletes, X does nothing
        } else {
            if (down & KEY_Y) host::PushKey(keys::Space);
            if (down & KEY_X) host::PushKey(keys::Letter('Y'));  // common "Yes" answer
        }
        if (combat && cx.aim.active && !cx.aim.manual) {
            // "Aim:" menu: L/R = previous / next target.
            if (down & KEY_L) host::PushKey(keys::Letter('P'));
            if (down & KEY_R) host::PushKey(keys::Letter('N'));
        } else if (combat) {
            // Shoulders = diagonals: left shoulders go left, right go right.
            if (down & KEY_L) host::PushKey(keys::Home);
            if (down & KEY_ZL) host::PushKey(keys::End);
            if (down & KEY_R) host::PushKey(keys::PageUp);
            if (down & KEY_ZR) host::PushKey(keys::PageDown);
        } else {
            // coab: Home / End = previous / next party member; PgUp / PgDn
            // page through long lists.
            if (down & KEY_L) host::PushKey(keys::Home);
            if (down & KEY_R) host::PushKey(keys::End);
            if (down & KEY_ZL) host::PushKey(keys::PageUp);
            if (down & KEY_ZR) host::PushKey(keys::PageDown);
        }

        // ---- directions (D-pad, Circle Pad) with key repeat ----
        bool up = held & (KEY_DUP | KEY_CPAD_UP), dn = held & (KEY_DDOWN | KEY_CPAD_DOWN);
        bool lf = held & (KEY_DLEFT | KEY_CPAD_LEFT), rt = held & (KEY_DRIGHT | KEY_CPAD_RIGHT);
        uint16_t dirKey = 0;
        if (overlay) {
            dirKey = 0;
        } else if (hotMenu) {
            if (up || lf) dirKey = kFocusPrev;
            else if (dn || rt) dirKey = kFocusNext;
        } else if (cx.list.active) {
            // coab lists move their highlight with Home / End.
            if (up) dirKey = keys::Home;
            else if (dn) dirKey = keys::End;
            else if (lf) dirKey = keys::Comma;
            else if (rt) dirKey = keys::Period;
        } else if (menuMode) {
            if (up || lf) dirKey = keys::Comma;
            else if (dn || rt) dirKey = keys::Period;
        } else if (!cx.items.empty() && !combat && cx.state != kStateWilderness && !explore3d) {
            // Menus that take control keys only to change party member (camp,
            // shop, start menu ...): up/down = previous/next member (Home/End),
            // left/right = step the menu highlight.
            if (up) dirKey = keys::Home;
            else if (dn) dirKey = keys::End;
            else if (lf) dirKey = keys::Comma;
            else if (rt) dirKey = keys::Period;
        } else if (combat) {
            if (up && lf) dirKey = keys::Home;
            else if (up && rt) dirKey = keys::PageUp;
            else if (dn && lf) dirKey = keys::End;
            else if (dn && rt) dirKey = keys::PageDown;
            else if (up) dirKey = keys::Up;
            else if (dn) dirKey = keys::Down;
            else if (lf) dirKey = keys::Left;
            else if (rt) dirKey = keys::Right;
        } else {
            if (up) dirKey = keys::Up;
            else if (dn) dirKey = keys::Down;
            else if (lf) dirKey = keys::Left;
            else if (rt) dirKey = keys::Right;
        }
        auto sendDir = [&](uint16_t k) {
            int n = (int)cx.hot.size();
            if (k == kFocusPrev) hotFocus = n ? (hotFocus + n - 1) % n : 0;
            else if (k == kFocusNext) hotFocus = n ? (hotFocus + 1) % n : 0;
            else host::PushKey(k);
        };
        // In combat a step can't be taken back, so give a diagonal ~80 ms to
        // form (both directions pressed) before the first move is sent.
        if (dirKey != heldKey) {
            heldKey = dirKey;
            if (dirKey) {
                if (combat) {
                    dirSent = false;
                    dirSince = now;
                } else {
                    sendDir(dirKey);
                    dirSent = true;
                    nextRepeat = now + 350;
                }
            }
        } else if (dirKey && !dirSent && now >= dirSince + 80) {
            sendDir(dirKey);
            dirSent = true;
            nextRepeat = now + 350;
        } else if (dirKey && dirSent && now >= nextRepeat && host::PendingKeys() == 0) {
            // repeat only once the game has taken the last key, so movement
            // stops when you let go
            sendDir(dirKey);
            nextRepeat = now + 120;
        }

        // ---- walking a highlight to a tapped entry ----
        if (walk.kind == 3) {
            // Step the free aim cursor one square at a time, then Target.
            // Stops on the square: a second tap there sends Target, so a
            // single tap never commits an attack or spell.
            const AimState& a = cx.aim;
            int cur = a.mx * 100 + a.my;
            if (a.active && a.manual) walk.sawManual = true;
            if (cur != walk.lastSeen) {
                walk.lastSeen = cur;
                walk.lastProgress = now;
            }
            if (walk.steps > 60 || !combat || now > walk.lastProgress + 1500 || (a.active && !a.manual && walk.sawManual))
                walk.kind = 0;
            else if (a.active && a.manual) {
                if (a.mx == walk.ax && a.my == walk.ay) {
                    walk.kind = 0;
                } else if (host::PendingKeys() == 0 && now >= walk.nextStep && (cur != walk.seenAtPush || now >= walk.nextStep + 300)) {
                    int dx = (walk.ax > a.mx) - (walk.ax < a.mx), dy = (walk.ay > a.my) - (walk.ay < a.my);
                    uint16_t k = dy < 0 ? (dx < 0 ? keys::Home : dx > 0 ? keys::PageUp : keys::Up)
                               : dy > 0 ? (dx < 0 ? keys::End : dx > 0 ? keys::PageDown : keys::Down)
                                        : (dx < 0 ? keys::Left : keys::Right);
                    host::PushKey(k);
                    walk.seenAtPush = cur;
                    walk.steps++;
                    walk.nextStep = now + 60;
                }
            }
        } else if (walk.kind) {
            bool active = walk.kind == 1 ? (cx.list.active && cx.list.row0 == walk.row0)
                                         : (cx.rosterVisible && !cx.list.active && cx.roster.selected >= 0);
            int cur = walk.kind == 1 ? cx.list.hl : cx.roster.selected;
            if (cur != walk.lastSeen) {
                walk.lastSeen = cur;
                walk.lastProgress = now;
            }
            int maxSteps = (walk.kind == 1 ? cx.list.rows : cx.roster.count) + 2;
            if (!active || walk.steps > maxSteps || now > walk.lastProgress + 2000) walk.kind = 0;
            else if (cur == walk.target) {
                if (walk.kind == 1 && walk.thenAct) host::PushKey(ListActionKey(cx.items));
                walk.kind = 0;
            } else if (host::PendingKeys() == 0 && now >= walk.nextStep && (cur != walk.seenAtPush || now >= walk.nextStep + 300)) {
                host::PushKey(cur > walk.target ? keys::Home : keys::End);
                walk.seenAtPush = cur;
                walk.steps++;
                walk.nextStep = now + 60;
            }
        }

        // ---- touch ----
        std::vector<Button> buttons;
        if (keyboardOpen) buttons = KeyboardButtons(cx.entry == 2);
        else if (numberPad) buttons = NumberPadButtons(cx.entryMaxValue);
        else {
            buttons = (combat && !swapped && !cx.words.empty())
                          ? CombatStrip(cx)
                          : StripButtons(journalEntry ? "J." + std::to_string(journalEntry) : std::string("BOOK"));
            if (swapped) {
                auto choices = ExplorePanelWanted(cx, cx.map) ? ExploreButtons(cx) : ChoiceButtons(cx);
                buttons.insert(buttons.end(), choices.begin(), choices.end());
            }
        }

        if (down & KEY_TOUCH) {
            touchPosition t;
            hidTouchRead(&t);
            const Button* b = ButtonAt(buttons, t.px, t.py);
            uint16_t k = 0;
            if (b) {
                flashKey = b->key;
                flashRow = b->listRow;
                flashMember = b->member;
                flashUntil = now + 150;
                if (b->listRow >= 0) {
                    if (b->listRow == cx.list.hl) k = ListActionKey(cx.items);
                    else {
                        StartWalk(1, b->listRow, now);
                        walk.row0 = cx.list.row0;
                        walk.thenAct = ListVerbCount(cx.items) == 1;
                    }
                } else if (b->member >= 0) {
                    if (b->member != cx.roster.selected) StartWalk(2, b->member, now);
                } else if (b->key == kToggleKeyboard) {
                    keyboardOpen = !keyboardOpen;
                    keyboardAuto = false;
                } else if (b->key == kBook) {
                    OpenViewerAtEntry(journalEntry);
                } else if (b->key == kKbPage) {
                    keyboardPage ^= 1;
                } else if (b->key == kSwap) {
                    userFlip = !userFlip;
                } else if (b->key == kRandomName) {
                    for (int i = 0; i < 16; i++) host::PushKey(keys::Backspace);
                    const char* n = kNames[std::rand() % (sizeof kNames / sizeof kNames[0])];
                    for (const char* c = n; *c; c++) host::PushKey(keys::Letter(*c));
                } else if (b->key == kNumAll) {
                    for (int i = 0; i < 8; i++) host::PushKey(keys::Backspace);
                    for (int i = 0; i < 6; i++) host::PushKey((uint16_t)'9');  // the game caps it at the maximum
                } else if (b->key == kNumClear) {
                    for (int i = 0; i < 8; i++) host::PushKey(keys::Backspace);
                } else {
                    k = b->key;
                }
            } else if (!overlay && !swapped && t.py < 200) {
                k = GameTap(t.px, t.py, cx, now);
                flashKey = 0;
                flashUntil = now + 150;
            }
            if (k) host::PushKey(k);
        }

        // ---- draw ----
        u16 w, h;
        Surface top{gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &w, &h), 400};
        Surface topR{gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, &w, &h), 400};
        Surface bottom{gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, &w, &h), 320};
        float depth = osGet3DSliderState();
        bool topDone = false;

        if (overlay) {
            DrawGameTop(top, topR, 0, keyboardOpen ? "Keyboard - HIDE to return" : "How much? - OK when done");
            topDone = true;
            Fill(bottom, 0, 0, 320, 240, kDash);
            if (numberPad) {
                std::string cap = cx.entryMaxValue > 0 ? "Enter an amount (max " + std::to_string(cx.entryMaxValue) + ")"
                                                       : "Enter a number";
                Text(bottom, (320 - TextWidth(cap)) / 2, 10, cap, kCyan);
            }
            DrawButtons(bottom, buttons, now);
        } else if (swapped) {
            DrawGameTop(top, topR, cx.state == kStateDungeon ? depth : 0.0f, "SELECT: game on the touch screen");
            topDone = true;
            Fill(bottom, 0, 0, 320, 240, kDash);
            if (ExplorePanelWanted(cx, cx.map)) {
                DrawAutomap(bottom, 6, 6, 9, cx.map);
                static const char* dirs[4] = {"North", "East", "South", "West"};
                char info[40];
                std::snprintf(info, sizeof info, "%d,%d facing %s", cx.map.x, cx.map.y, dirs[(cx.map.dir & 6) / 2]);
                Text(bottom, 6, 158, info, kGrey);
                Text(bottom, 6, 172, "L/R party  B back", kGrey);
            }
            DrawButtons(bottom, buttons, now);
        } else {
            if (combat) DrawCombatPanel(top, cx.party);
            else DrawDashboard(top, cx);
            BlitGame(bottom, 0, 0);
            if (hotMenu) {  // the host's highlight on the hotkey line
                const HotLine& h = cx.hot[hotFocus];
                Frame(bottom, h.c0 * 8 - 3, h.row * 8 - 2, (h.c1 - h.c0 + 1) * 8 + 6, 12, kGold);
            }
            Fill(bottom, 0, 200, 320, 40, kDash);
            DrawButtons(bottom, buttons, now);
        }

        if (quitSince) {  // holding START+SELECT
            Fill(bottom, 40, 90, 240, 50, kRed);
            Text(bottom, 60, 104, "Keep holding START+SELECT", kWhite);
            Text(bottom, 60, 120, "to quit (saves nothing)", kWhite);
        }
        if (!topDone) std::memcpy(topR.fb, top.fb, 400 * 240 * 3);

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    if (exploredDirty) SaveAutomap();
    host::RequestQuit();
    threadJoin(engine, U64_MAX);
    threadFree(engine);
    if (ndspOk) ndspExit();
    romfsExit();
    gfxExit();
    return 0;
}

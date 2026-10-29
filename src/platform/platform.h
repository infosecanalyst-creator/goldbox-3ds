// platform/platform.h - everything the engine needs from the host.
//
// The engine runs on its own thread (as in coab) and blocks in
// engine::seg049::READKEY waiting for input. Hosts (SDL2 PC build, headless test
// runner, Nintendo 3DS) implement these functions and push keys with
// engine::seg049::AddKey(ushort ibmKey).
//
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace platform {

// Called by Classes::Display whenever coab would repaint its window.
// `ega` is the 320x200 frame as EGA colour indices (0..15, row-major);
// `palette` is 16 RGB triples (the current EGA palette).
void PresentFrame(const int* ega, const uint8_t* palette);

// Sound effects. `id` is coab's sample slot (1..12, see seg044); `name` the
// resource name ("hit", "miss", ...). Load is called once at start-up.
void LoadSound(int id, const char* name);
void PlaySound(int id);
void StopAllSounds();

void Sleep(int milliseconds);

// Menus. The engine's horizontal menus ("Move View Cast Encamp ...") are
// published here while they wait for input, so a host can offer them as
// buttons (3DS bottom screen). `key` is the hotkey character to send.
// `acceptsDirections`: the engine also takes arrow keys right now (movement);
// when false, arrows do nothing in coab, so hosts can map the D-pad to the
// menu's own highlight keys (',' previous / '.' next).
// `col`/`width`: where the word sits on the game's menu line (text row 24,
// pixels y 192..199), in 8-pixel text columns, so a touch host can map a tap
// on the game screen to the word under the stylus.
struct MenuItem {
    std::string label;
    char key;
    int col = 0;
    int width = 0;
};
void MenuShow(const std::string& prompt, const std::vector<MenuItem>& items, bool acceptsDirections);
void MenuHide();

// Scrolling lists (race, class, items, spells ...): published while the list
// waits for input so a touch host can map a tap to a row. Rows are text rows
// (8 pixels); `highlightRow` is the highlighted entry, counted from firstRow.
// In coab such lists move their highlight with Home / End (keypad 7 / 1).
void ListShow(int firstRow, int firstCol, int lastCol, int rows, int highlightRow);
void ListHide();

// The party list (name, AC, HP rows) was drawn: rows firstRow.., one per
// member; `selected` is the selected member's index (-1: none). In coab
// Home / End select the previous / next member wherever the list is shown.
void RosterShow(int firstRow, int firstCol, int lastCol, int count, int selected);

// Text / number entry started or finished (display-only hint so a touch host
// can open a keyboard or number pad). kind: 1 = text, 2 = character name,
// 3 = number (maxValue > 0: the largest amount accepted, 0: no fixed max).
void EntryBegin(int kind, int maxValue);
void EntryEnd();

// Combat aiming. manual = 0: the "Aim:" menu, cursor on the current target;
// manual = 1: free cursor ("Use Cursor keys"). Map squares; the combat view
// shows squares topX..topX+6 / topY..topY+6, 24 pixels each, from (0,0).
void AimShow(int manual, int mapX, int mapY, int topX, int topY);
void AimHide();

// The party (published whenever the game draws the party list or a combat
// summary). `active`: the member the game is showing / whose turn it is.
struct PartyMember {
    std::string name, status;
    int hp = 0, hpMax = 0, ac = 0;
    bool active = false;
};
void PartyStatus(const std::vector<PartyMember>& members);

// The 3D view was drawn (dungeon / town). Map identity (area, geo block),
// party square and facing (0 N, 2 E, 4 S, 6 W) and the 16x16 map: per cell
// bits 0-3 walls N/E/S/W, bits 4-7 doors N/E/S/W. Index [y * 16 + x].
void MapView(int area, int block, int x, int y, int dir, const uint8_t* cells);

// Every character cell the game draws (text row 0..24, column 0..39, EGA
// colours), so a host can find tappable text such as hotkey menus.
void TextDrawn(int row, int col, char ch, int fg, int bg);

// Called when the engine thread finishes (coab's EngineStoppedCallback).
void EngineStopped();

}  // namespace platform

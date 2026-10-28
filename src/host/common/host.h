// host/common/host.h - shared start-up for every host (PC, headless, 3DS).
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#pragma once

#include <cstdint>
#include <string>

namespace host {

struct Options {
    std::string dataDir = ".";     // folder with the game's *.DAX files
    std::string userDir;           // logs + saves (defaults per platform)
    int64_t fixedSeed = 0;         // non-zero: deterministic RNG (for diff testing)
    bool consoleLog = false;       // echo coab's debug log to stdout
};

// Runs coab's engine (seg001::__SystemInit + PROGRAM) on the calling thread.
// Returns when the engine stops. Throws nothing.
void RunEngine(const Options& opt);

// Queue a key for the engine (thread-safe). See platform/keys.h.
void PushKey(uint16_t ibmKey);

// Ask the engine thread to unwind (it exits at its next key wait).
void RequestQuit();

// Copy the game's own 8x8 font (64 glyphs, indexed by upper-case ASCII % 64,
// bit 7 = leftmost pixel) once the engine has loaded it. Call from the engine
// thread (e.g. inside platform::PresentFrame). Returns false if not loaded.
bool CopyFont(uint8_t out[64][8]);

// The game's current state (coab GameState: 0 start menu, 1 shop, 2 camping,
// 3 wilderness map, 4 dungeon/town 3D view, 5 combat, 6 after combat, 7 end).
// Safe to poll from any thread (a plain read of one int).
int GameStateNow();

}  // namespace host

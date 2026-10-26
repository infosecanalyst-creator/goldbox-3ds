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

}  // namespace host

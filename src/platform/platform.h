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

// Called when the engine thread finishes (coab's EngineStoppedCallback).
void EngineStopped();

}  // namespace platform

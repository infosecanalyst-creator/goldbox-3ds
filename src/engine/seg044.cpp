// engine/seg044.cpp - hand port of engine/seg044.cs (sound).
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include "engine/engine.h"
#include "platform/platform.h"

namespace engine {

void seg044::SetSound(bool On) { gbl::soundType = On ? SoundType::PC : SoundType::None; }

void seg044::SetPicture(bool On) { gbl::PicsOn = On; }

void seg044::SetAnimation(bool On) { gbl::AnimationsOn = On; }

void seg044::PlaySound(Sound arg_0) /*sub_120E0*/
{
    if (gbl::soundType == SoundType::PC) {
        if (arg_0 == Sound::sound_0) {
            platform::StopAllSounds();
        } else if (arg_0 == Sound::sound_1) {
        } else if (arg_0 == Sound::sound_FF)  // off maybe.
        {
            platform::StopAllSounds();
        } else if (arg_0 >= Sound::sound_2 && arg_0 <= Sound::sound_e) {
            int sampleId = (int)arg_0 - 1;
            if (!soundLoaded.IsNull() && sampleId < soundLoaded.Length() && soundLoaded[sampleId]) {
                platform::PlaySound(sampleId);
            } else {
            }
        } else if (arg_0 == Sound::sound_f) {
        }
    }
}

void seg044::SoundInit() {
    soundLoaded = Array<bool>(13);
    auto load = [](int id, const char* name) {
        platform::LoadSound(id, name);
        soundLoaded[id] = true;
    };
    load(1, "missle");
    load(2, "magic_hit");
    load(4, "death");
    load(5, "sound_5");
    load(6, "hit");
    load(8, "miss");
    load(9, "step");
    load(10, "sound_10");
    load(12, "start_sound");
}

}  // namespace engine

// host/pc/main.cpp - SDL2 desktop host (Windows/Linux/macOS).
// Plays the role of coab's WinForms MainForm: shows the 320x200 frame, turns
// keyboard input into IBM key codes, plays the sound samples.
//
// Usage: goldbox [--data <folder with *.DAX>] [--user <logs/saves folder>]
//                [--scale N] [--seed N] [--log]
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include <SDL.h>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "host/common/host.h"
#include "platform/keys.h"
#include "platform/platform.h"

namespace {

std::mutex frameMutex;
uint32_t frame[320 * 200];
bool frameDirty = true;
std::atomic<bool> engineDone{false};

struct Sample {
    std::vector<uint8_t> pcm;
};
Sample samples[16];
SDL_AudioDeviceID audioDev = 0;
SDL_AudioSpec audioSpec;
std::string soundDir = "sounds";

uint16_t MapKey(const SDL_Keysym& ks) {
    SDL_Keycode k = ks.sym;
    if (k >= SDLK_0 && k <= SDLK_9) return keys::Digit(k - SDLK_0);
    if (k >= SDLK_a && k <= SDLK_z) return keys::Letter((char)k);
    switch (k) {
        case SDLK_RETURN: case SDLK_KP_ENTER: return keys::Enter;
        case SDLK_SPACE: return keys::Space;
        case SDLK_DELETE: return keys::Delete;
        case SDLK_BACKSPACE: return keys::Backspace;
        case SDLK_HOME: case SDLK_KP_7: case SDLK_LEFTBRACKET: return keys::Home;
        case SDLK_UP: case SDLK_KP_8: return keys::Up;
        case SDLK_PAGEUP: case SDLK_KP_9: return keys::PageUp;
        case SDLK_LEFT: case SDLK_KP_4: return keys::Left;
        case SDLK_KP_5: return keys::Center;
        case SDLK_RIGHT: case SDLK_KP_6: return keys::Right;
        case SDLK_END: case SDLK_KP_1: case SDLK_RIGHTBRACKET: return keys::End;
        case SDLK_DOWN: case SDLK_KP_2: return keys::Down;
        case SDLK_PAGEDOWN: case SDLK_KP_3: return keys::PageDown;
        case SDLK_MINUS: case SDLK_KP_MINUS: return keys::Minus;
        case SDLK_ESCAPE: return keys::Escape;
        case SDLK_COMMA: return keys::Comma;
        case SDLK_PERIOD: return keys::Period;
        default: return 0;  // coab sends space for unknown keys; we ignore them
    }
}

}  // namespace

namespace platform {

void PresentFrame(const int* ega, const uint8_t* palette) {
    std::lock_guard<std::mutex> g(frameMutex);
    for (int i = 0; i < 320 * 200; i++) {
        const uint8_t* c = palette + ega[i] * 3;
        frame[i] = 0xFF000000u | ((uint32_t)c[0] << 16) | ((uint32_t)c[1] << 8) | c[2];
    }
    frameDirty = true;
}

void LoadSound(int id, const char* name) {
    if (id < 0 || id >= 16) return;
    std::string path = soundDir + "/" + name + ".wav";
    SDL_AudioSpec spec;
    Uint8* buf = nullptr;
    Uint32 len = 0;
    if (!SDL_LoadWAV(path.c_str(), &spec, &buf, &len)) return;
    SDL_AudioCVT cvt;
    if (SDL_BuildAudioCVT(&cvt, spec.format, spec.channels, spec.freq, audioSpec.format, audioSpec.channels, audioSpec.freq) > 0) {
        cvt.len = (int)len;
        std::vector<uint8_t> tmp((size_t)len * (size_t)cvt.len_mult);
        std::memcpy(tmp.data(), buf, len);
        cvt.buf = tmp.data();
        SDL_ConvertAudio(&cvt);
        tmp.resize((size_t)cvt.len_cvt);
        samples[id].pcm = std::move(tmp);
    } else {
        samples[id].pcm.assign(buf, buf + len);
    }
    SDL_FreeWAV(buf);
}

void PlaySound(int id) {
    if (!audioDev || id < 0 || id >= 16 || samples[id].pcm.empty()) return;
    SDL_ClearQueuedAudio(audioDev);
    SDL_QueueAudio(audioDev, samples[id].pcm.data(), (Uint32)samples[id].pcm.size());
}

void StopAllSounds() {
    if (audioDev) SDL_ClearQueuedAudio(audioDev);
}

void Sleep(int ms) { SDL_Delay((Uint32)ms); }

void MenuShow(const std::string&, const std::vector<MenuItem>&, bool) {}
void MenuHide() {}
void ListShow(int, int, int, int, int) {}
void ListHide() {}
void AimShow(int, int, int, int, int) {}
void AimHide() {}
void EntryBegin(int, int) {}
void EntryEnd() {}
void RosterShow(int, int, int, int, int) {}
void TextDrawn(int, int, char, int, int) {}

void EngineStopped() { engineDone = true; }

}  // namespace platform

int main(int argc, char** argv) {
    host::Options opt;
    int scale = 3;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        auto next = [&]() -> std::string { return i + 1 < argc ? argv[++i] : ""; };
        if (a == "--data") opt.dataDir = next();
        else if (a == "--user") opt.userDir = next();
        else if (a == "--scale") scale = std::atoi(next().c_str());
        else if (a == "--seed") opt.fixedSeed = std::atoll(next().c_str());
        else if (a == "--log") opt.consoleLog = true;
        else if (a == "--sounds") soundDir = next();
    }
    if (opt.userDir.empty()) {
        char* pref = SDL_GetPrefPath("goldbox-3ds", "Curse of the Azure Bonds");
        if (pref) {
            opt.userDir = pref;
            SDL_free(pref);
        }
    }
    if (soundDir == "sounds") {
        char* base = SDL_GetBasePath();
        if (base) {
            soundDir = std::string(base) + "sounds";
            SDL_free(base);
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    SDL_AudioSpec want{};
    want.freq = 22050;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 1024;
    audioDev = SDL_OpenAudioDevice(nullptr, 0, &want, &audioSpec, 0);
    if (audioDev) SDL_PauseAudioDevice(audioDev, 0);

    SDL_Window* win = SDL_CreateWindow("Curse of the Azure Bonds - goldbox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       320 * scale, 200 * scale, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(ren, 320, 200);
    SDL_RenderSetIntegerScale(ren, SDL_TRUE);
    SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 320, 200);

    std::thread engineThread([opt] { host::RunEngine(opt); });

    bool quit = false;
    while (!quit && !engineDone) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN) {
                uint16_t k = MapKey(e.key.keysym);
                if (k) host::PushKey(k);
            }
        }
        {
            std::lock_guard<std::mutex> g(frameMutex);
            if (frameDirty) {
                SDL_UpdateTexture(tex, nullptr, frame, 320 * 4);
                frameDirty = false;
            }
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, nullptr, nullptr);
        SDL_RenderPresent(ren);
    }
    host::RequestQuit();
    engineThread.join();
    SDL_Quit();
    return 0;
}

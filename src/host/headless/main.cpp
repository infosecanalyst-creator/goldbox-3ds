// host/headless/main.cpp - windowless runner for automated testing.
//
// Runs the engine with a fixed RNG seed and feeds keys from a script, saving
// frames as PNG/PPM on request. Used to compare this C++ build against coab.
//
// Usage: goldbox_headless --data <dir> --script <file> [--seed N] [--out <dir>]
//        goldbox_headless --data <dir> --harness <keys.txt> --trace <out.txt> [--seed N]
// Script lines:  sleep <ms> | key <name-or-hex> | text <chars> | snap <name> | quit
//   key names: enter esc space up down left right home end pgup pgdn bksp del
//              or hex like 0x4800, or a single character.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "engine/harness.h"
#include "host/common/host.h"
#include "platform/keys.h"
#include "platform/platform.h"

namespace {
std::mutex frameMutex;
uint8_t rgb[320 * 200 * 3];
std::atomic<int> frameCount{0};
std::atomic<bool> engineDone{false};
std::string outDir = ".";

void SavePPM(const std::string& name) {
    std::lock_guard<std::mutex> g(frameMutex);
    std::string path = outDir + "/" + name + ".ppm";
    if (std::FILE* f = std::fopen(path.c_str(), "wb")) {
        std::fprintf(f, "P6\n320 200\n255\n");
        std::fwrite(rgb, 1, sizeof rgb, f);
        std::fclose(f);
        std::printf("snap %s (frame %d)\n", path.c_str(), frameCount.load());
    }
}

uint16_t ParseKey(const std::string& s) {
    if (s == "enter") return keys::Enter;
    if (s == "esc") return keys::Escape;
    if (s == "space") return keys::Space;
    if (s == "up") return keys::Up;
    if (s == "down") return keys::Down;
    if (s == "left") return keys::Left;
    if (s == "right") return keys::Right;
    if (s == "home") return keys::Home;
    if (s == "end") return keys::End;
    if (s == "pgup") return keys::PageUp;
    if (s == "pgdn") return keys::PageDown;
    if (s == "bksp") return keys::Backspace;
    if (s == "del") return keys::Delete;
    if (s.size() > 2 && s[0] == '0' && s[1] == 'x') return (uint16_t)std::strtoul(s.c_str(), nullptr, 16);
    if (s.size() == 1) return keys::Letter(s[0]);
    return 0;
}
}  // namespace

namespace platform {
void PresentFrame(const int* ega, const uint8_t* palette) {
    std::lock_guard<std::mutex> g(frameMutex);
    for (int i = 0; i < 320 * 200; i++) {
        const uint8_t* c = palette + ega[i] * 3;
        rgb[i * 3 + 0] = c[0];
        rgb[i * 3 + 1] = c[1];
        rgb[i * 3 + 2] = c[2];
    }
    frameCount++;
}
void LoadSound(int, const char*) {}
void PlaySound(int id) { std::printf("sound %d\n", id); }
void StopAllSounds() {}
void Sleep(int ms) {
    if (!engine::Harness::enabled) std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
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
    std::string script, harnessScript, tracePath = "trace.txt";
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        auto next = [&]() -> std::string { return i + 1 < argc ? argv[++i] : ""; };
        if (a == "--data") opt.dataDir = next();
        else if (a == "--user") opt.userDir = next();
        else if (a == "--seed") opt.fixedSeed = std::atoll(next().c_str());
        else if (a == "--script") script = next();
        else if (a == "--out") outDir = next();
        else if (a == "--log") opt.consoleLog = true;
        else if (a == "--harness") harnessScript = next();
        else if (a == "--trace") tracePath = next();
    }
    if (opt.fixedSeed == 0) opt.fixedSeed = 12345;
    if (opt.userDir.empty()) opt.userDir = outDir + "/user";

    if (!harnessScript.empty()) {
        // Differential-test mode: keys come synchronously from the harness script.
        if (!engine::Harness::Load(harnessScript, tracePath)) {
            std::fprintf(stderr, "cannot load harness script %s\n", harnessScript.c_str());
            return 1;
        }
        host::RunEngine(opt);
        engine::Harness::Close();
        return 0;
    }

    std::thread engineThread([opt] { host::RunEngine(opt); });

    std::ifstream in(script);
    std::string line;
    while (!engineDone && std::getline(in, line)) {
        std::istringstream ls(line);
        std::string cmd, arg;
        ls >> cmd;
        std::getline(ls, arg);
        if (!arg.empty() && arg[0] == ' ') arg.erase(0, 1);
        if (cmd.empty() || cmd[0] == '#') continue;
        if (cmd == "sleep") std::this_thread::sleep_for(std::chrono::milliseconds(std::atoi(arg.c_str())));
        else if (cmd == "key") host::PushKey(ParseKey(arg));
        else if (cmd == "text") for (char c : arg) host::PushKey(c == ' ' ? keys::Space : keys::Letter(c));
        else if (cmd == "snap") SavePPM(arg);
        else if (cmd == "quit") break;
    }
    SavePPM("final");
    host::RequestQuit();
    engineThread.join();
    return 0;
}

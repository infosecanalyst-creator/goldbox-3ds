// host/common/host.cpp - engine start-up shared by all hosts.
// Mirrors coab Main/Program.cs EngineThread().
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include "host/common/host.h"

#include "engine/engine.h"
#include "engine/harness.h"
#include "platform/platform.h"

#include <cstdio>
#ifdef _WIN32
#include <direct.h>
#define chdir _chdir
#else
#include <unistd.h>
#endif

namespace host {

// coab's MainForm.setSettings() with its Settings.settings defaults. Not
// applied in differential-test runs (the coab harness doesn't apply them).
static void ApplyDefaultSettings() {
    using namespace Classes;
    Cheats::PlayerAlwaysSavesSet(false);
    Cheats::AlwayShowAreaMapSet(false);
    Cheats::FreeTrainingSet(false);
    Cheats::SkipCopyProtectionSet(true);
    Cheats::AllowGodsInterveneSet(false);
    Cheats::DisplayFullItemNamesSet(false);
    Cheats::ViewItemStatsSet(false);
    Cheats::SkipTitleScreenSet(false);
    Cheats::ImprovedAreaMapSet(false);
    Cheats::NoRaceLevelLimits(false);
    Cheats::NoRaceClassRestrictions(false);
    Cheats::SortTreasureSet(false);
    engine::seg044::SetSound(true);
    engine::seg044::SetPicture(true);
    engine::seg044::SetAnimation(true);
    gbl::portEnhancements = true;
}

void RunEngine(const Options& opt) {
    if (!opt.dataDir.empty() && chdir(opt.dataDir.c_str()) != 0) {
        std::fprintf(stderr, "goldbox: cannot open data folder '%s'\n", opt.dataDir.c_str());
        platform::EngineStopped();
        return;
    }
    cs::Determinism::fixedSeedTicks = opt.fixedSeed;
    Logging::Config::Setup(opt.userDir.empty() ? cs::String("goldbox-user") : cs::String(opt.userDir));
    Logging::Logger::echoToConsole = opt.consoleLog;
    Logging::Logger::SetExitFunc([] { engine::seg043::print_and_exit(); });

    try {
        engine::seg001::__SystemInit([] { platform::EngineStopped(); });
        if (!engine::Harness::enabled) ApplyDefaultSettings();
        engine::seg001::PROGRAM();
    } catch (const cs::EngineAbort&) {
        // normal shutdown path (coab: Thread.Abort)
    } catch (const std::exception& e) {
        cs::String log = cs::IO::Path::Combine(Logging::Logger::GetPath(), "Crash Log.txt");
        if (std::FILE* f = std::fopen(log.c_str(), "a")) {
            std::fprintf(f, "\ngoldbox-3ds\n%s\nUnhandled exception: %s\n", cs::DateTime::Now().ToString().c_str(), e.what());
            std::fclose(f);
        }
        std::fprintf(stderr, "goldbox: unhandled exception: %s\n", e.what());
    }
    platform::EngineStopped();
}

bool CopyFont(uint8_t out[64][8]) {
    auto& f = Classes::gbl::dax_8x8d1_201;
    if (f.IsNull() || f.GetLength(0) < 64 || f.GetLength(1) < 8) return false;
    bool any = false;
    for (int g = 0; g < 64; g++)
        for (int r = 0; r < 8; r++) {
            out[g][r] = f(g, r);
            any |= out[g][r] != 0;
        }
    return any;
}

int GameStateNow() { return (int)Classes::gbl::game_state; }

void PushKey(uint16_t ibmKey) { engine::seg049::AddKey(ibmKey); }

void RequestQuit() { engine::seg049::RequestAbort(); }

}  // namespace host

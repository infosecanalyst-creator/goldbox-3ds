// engine/harness.cpp - differential-testing harness. Keep in lock-step with
// tools/coab-harness/Harness.cs: every line written here must be written
// identically there.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include "engine/harness.h"

#include "engine/engine.h"

#include <cstdio>
#include <cstring>
#include <deque>
#include <fstream>
#include <sstream>

namespace engine {

namespace {
struct Item {
    bool idle;
    int count;       // idle polls remaining
    cs::ushort key;  // for key items
};
std::deque<Item> script;
std::FILE* trace = nullptr;
long randomCount = 0;
long keyCount = 0;
long maxRandoms = -1;
bool verbose = false;  // `verbose` script line: add a party/combatant state hash to every R/F line  // `randoms N` script line: end the run after N RNG calls

cs::ushort ParseKey(const std::string& s) {
    if (s == "enter") return 0x1C0D;
    if (s == "esc") return 0x001B;
    if (s == "space") return 0x0020;
    if (s == "up") return 0x4800;
    if (s == "down") return 0x5000;
    if (s == "left") return 0x4B00;
    if (s == "right") return 0x4D00;
    if (s == "home") return 0x4700;
    if (s == "end") return 0x4F00;
    if (s == "pgup") return 0x4900;
    if (s == "pgdn") return 0x5100;
    if (s == "bksp") return 0x0008;
    if (s == "del") return 0x5300;
    if (s == "minus") return 0x2D00;
    if (s == "comma") return 0x002C;
    if (s == "period") return 0x002E;
    if (s.size() > 2 && s[0] == '0' && s[1] == 'x') return (cs::ushort)std::strtoul(s.c_str(), nullptr, 16);
    if (s.size() == 1) {
        char c = s[0];
        if (c >= 'a' && c <= 'z') c = (char)(c - 32);
        return (cs::ushort)(unsigned char)c;
    }
    return 0x0020;
}

uint32_t Fnv(const cs::Array<cs::byte>& a) {
    uint32_t h = 2166136261u;
    if (a.IsNull()) return 0;
    for (int i = 0; i < a.Length(); i++) {
        h ^= a[i];
        h *= 16777619u;
    }
    return h;
}

uint32_t ScreenHash() {
    uint32_t h = 2166136261u;
    for (int y = 0; y < 200; y++)
        for (int x = 0; x < 320; x++) {
            h ^= Classes::Display::GetPixel(x, y);
            h *= 16777619u;
        }
    return h;
}

uint32_t TeamHash() {
    uint32_t h = 2166136261u;
    if (gbl::TeamList.IsNull()) return 0;
    for (auto p : gbl::TeamList) {
        h ^= Fnv(p->ToByteArray());
        h *= 16777619u;
        h ^= (uint32_t)(p->in_combat ? 1 : 0);
        h *= 16777619u;
    }
    return h;
}

void Line(const std::string& s) {
    if (!trace) return;
    std::fputs(s.c_str(), trace);
    std::fputc('\n', trace);
}

void DumpState(cs::ushort key) {
    char buf[256];
    std::snprintf(buf, sizeof buf, "K %ld %04X pos=%d,%d,%d ecl=%04X area1=%08X area2=%08X screen=%08X team=%d", keyCount, key,
                  gbl::mapPosX, gbl::mapPosY, (int)gbl::mapDirection, (int)gbl::ecl_offset,
                  gbl::area_ptr == nullptr ? 0u : Fnv(gbl::area_ptr->ToByteArray()),
                  gbl::area2_ptr == nullptr ? 0u : Fnv(gbl::area2_ptr->ToByteArray()), ScreenHash(),
                  gbl::TeamList.IsNull() ? -1 : gbl::TeamList.Count());
    std::string s = buf;
    if (!gbl::TeamList.IsNull()) {
        for (auto p : gbl::TeamList) {
            std::snprintf(buf, sizeof buf, " p=%08X/%d/%d", Fnv(p->ToByteArray()), p->items.Count(), p->affects.Count());
            s += buf;
        }
    }
    Line(s);
    std::fflush(trace);
}
}  // namespace

bool Harness::Load(const cs::String& scriptPath, const cs::String& tracePath) {
    std::ifstream in(scriptPath);
    if (!in) return false;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ls(line);
        std::string cmd, arg;
        ls >> cmd >> arg;
        if (cmd.empty() || cmd[0] == '#') continue;
        if (cmd == "key") script.push_back({false, 0, ParseKey(arg)});
        else if (cmd == "idle") script.push_back({true, std::atoi(arg.c_str()), 0});
        else if (cmd == "verbose") verbose = true;
        else if (cmd == "randoms") maxRandoms = std::atol(arg.c_str());
        else if (cmd == "stop") break;
    }
    cs::Determinism::virtualTime = true;
    trace = std::fopen(tracePath.c_str(), "w");
    enabled = trace != nullptr;
    return enabled;
}

void Harness::Close() {
    if (trace) std::fclose(trace);
    trace = nullptr;
}

cs::ushort Harness::CheckKey() {
    while (!script.empty() && script.front().idle && script.front().count <= 0) script.pop_front();
    if (script.empty()) throw cs::EngineAbort();
    if (script.front().idle) {
        script.front().count--;
        return 0;
    }
    return script.front().key;
}

cs::ushort Harness::GetKey() {
    while (!script.empty() && script.front().idle) script.pop_front();
    if (script.empty()) {
        Line("END");
        throw cs::EngineAbort();
    }
    cs::ushort k = script.front().key;
    script.pop_front();
    keyCount++;
    DumpState(k);
    return k;
}

void Harness::OnRandom(int arg, int raw) {
    char buf[128];
    std::snprintf(buf, sizeof buf, "R %ld %d %d", ++randomCount, arg, raw);
    if (verbose) std::snprintf(buf + std::strlen(buf), sizeof buf - std::strlen(buf), " s=%08X/%d", TeamHash(), gbl::TeamList.IsNull() ? -1 : gbl::TeamList.Count());
    Line(buf);
    if (maxRandoms >= 0 && randomCount >= maxRandoms) {
        Line("END");
        throw cs::EngineAbort();
    }
}

void Harness::OnRandomReal(double raw) {
    char buf[128];
    unsigned long long bits;
    std::memcpy(&bits, &raw, sizeof bits);
    std::snprintf(buf, sizeof buf, "F %ld %016llX", ++randomCount, bits);
    if (verbose) std::snprintf(buf + std::strlen(buf), sizeof buf - std::strlen(buf), " s=%08X/%d", TeamHash(), gbl::TeamList.IsNull() ? -1 : gbl::TeamList.Count());
    Line(buf);
    if (maxRandoms >= 0 && randomCount >= maxRandoms) {
        Line("END");
        throw cs::EngineAbort();
    }
}

}  // namespace engine

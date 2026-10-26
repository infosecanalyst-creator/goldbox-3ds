// engine/harness.h - differential-testing harness (goldbox-3ds addition).
//
// The same harness exists in our patched copy of coab (tools/coab-harness/
// Harness.cs). Given the same seed and the same key script, both programs must
// produce byte-identical trace files:
//   R <n> <arg> <raw>        every call into seg051.Random (raw = Random.Next())
//   F <n> <bits>             every seg051.Random__Real (IEEE-754 bits, hex)
//   K <n> <key> <state...>   every time the engine blocks for a key
// Script format: one item per line
//   key <name|0xHHHH|c>   next key the engine reads
//   idle <n>              the next n "is a key waiting?" polls answer no
//   randoms <n>           end the run after n RNG calls (anywhere in the script)
//   stop                  stop reading the script here
//   # comment
// When the script runs out, the run ends (cs::EngineAbort).
#pragma once

#include "cs/cs.h"

namespace engine {

struct Harness {
    inline static bool enabled = false;

    static bool Load(const cs::String& scriptPath, const cs::String& tracePath);
    static void Close();

    // seg049 hooks
    static cs::ushort CheckKey();  // int_check_keyPressed
    static cs::ushort GetKey();    // int_get_keyPressed

    // seg051 hooks
    static void OnRandom(int arg, int raw);
    static void OnRandomReal(double raw);
};

}  // namespace engine

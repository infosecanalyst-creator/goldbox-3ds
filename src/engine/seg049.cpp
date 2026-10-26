// engine/seg049.cpp - hand port of engine/seg049.cs (keyboard queue).
// The C# Semaphore(0, 1) becomes a binary semaphore on a mutex + condvar.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include "engine/engine.h"
#include "engine/harness.h"
#include "platform/platform.h"

namespace engine {

void seg049::SysDelay(int milliseconds) {
    if (milliseconds != 0 && !Harness::enabled) {  // harness runs at full speed
        platform::Sleep(milliseconds);
    }
}

static void WaitForKey_Release() {
    std::lock_guard<std::mutex> g(seg049::WaitForKeyLock);
    if (seg049::WaitForKeyCount == 0) seg049::WaitForKeyCount = 1;  // maximumCount = 1
    seg049::WaitForKey.notify_one();
}

static void WaitForKey_WaitOne() {
    std::unique_lock<std::mutex> g(seg049::WaitForKeyLock);
    seg049::WaitForKey.wait(g, [] { return seg049::WaitForKeyCount > 0 || seg049::abortRequested; });
    if (seg049::abortRequested) throw EngineAbort();
    seg049::WaitForKeyCount = 0;
}

void seg049::RequestAbort() {
    std::lock_guard<std::mutex> g(WaitForKeyLock);
    abortRequested = true;
    WaitForKey.notify_all();
}

void seg049::AddKey(ushort key) {
    std::lock_guard<std::mutex> g(keysPressedLock);
    keysPressed.Enqueue(key);

    if (keysPressed.Count() == 1) {
        WaitForKey_Release();
    }
}

ushort seg049::int_check_keyPressed() {
    // INT 16 - KEYBOARD - CHECK FOR KEYSTROKE (AH = 01h): peek, don't remove.
    if (abortRequested) throw EngineAbort();
    if (Harness::enabled) return Harness::CheckKey();
    std::lock_guard<std::mutex> g(keysPressedLock);
    if (keysPressed.Count() > 0) {
        return keysPressed.Peek();
    }
    return 0;
}

ushort seg049::int_get_keyPressed() {
    // INT 16 - KEYBOARD - GET KEYSTROKE (AH = 00h)
    ushort key = 0;

    if (Harness::enabled) return Harness::GetKey();

    WaitForKey_WaitOne();

    std::lock_guard<std::mutex> g(keysPressedLock);
    key = keysPressed.Dequeue();

    if (keysPressed.Count() > 0) {
        WaitForKey_Release();
    }

    return key;
}

bool seg049::KEYPRESSED() {
    if (skipReadFlag == 0) {
        return (int_check_keyPressed() != 0);
    } else {
        return true;
    }
}

byte seg049::READKEY() {
    byte lastCode = skipReadFlag;
    skipReadFlag = 0;

    if (lastCode == 0) {
        ushort responce = int_get_keyPressed();
        lastCode = (byte)responce;

        if ((responce & 0x00ff) == 0) {
            skipReadFlag = (byte)(responce >> 8);

            if (skipReadFlag == 0) {
                lastCode = 3;
            }
        }
    }

    return lastCode;
}

}  // namespace engine

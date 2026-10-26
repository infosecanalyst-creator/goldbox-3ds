// cs/sync.h - minimal mutex / condition variable used by the engine core.
// On the 3DS this uses libctru's LightLock/CondVar (no dependency on the C++
// thread library); elsewhere it wraps std::mutex / std::condition_variable.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#pragma once

#if defined(__3DS__)
// libctru's individual headers have no extern "C" guard (only <3ds.h> does).
extern "C" {
#include <3ds/types.h>
#include <3ds/synchronization.h>
}
#else
#include <condition_variable>
#include <mutex>
#endif

namespace cs {

#if defined(__3DS__)
class Mutex {
    LightLock l;

public:
    Mutex() { LightLock_Init(&l); }
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
    void lock() { LightLock_Lock(&l); }
    void unlock() { LightLock_Unlock(&l); }
    LightLock* native() { return &l; }
};

class CondVar {
    ::CondVar cv;

public:
    CondVar() { CondVar_Init(&cv); }
    CondVar(const CondVar&) = delete;
    CondVar& operator=(const CondVar&) = delete;
    // `m` must be locked by the caller.
    template <class Pred>
    void wait(Mutex& m, Pred pred) {
        while (!pred()) CondVar_Wait(&cv, m.native());
    }
    void notify_one() { CondVar_Signal(&cv); }
    void notify_all() { CondVar_Broadcast(&cv); }
};
#else
class Mutex {
    std::mutex m;

public:
    void lock() { m.lock(); }
    void unlock() { m.unlock(); }
    std::mutex& native() { return m; }
};

class CondVar {
    std::condition_variable_any cv;

public:
    template <class Pred>
    void wait(Mutex& m, Pred pred) {
        cv.wait(m, pred);
    }
    void notify_one() { cv.notify_one(); }
    void notify_all() { cv.notify_all(); }
};
#endif

class LockGuard {
    Mutex& m;

public:
    explicit LockGuard(Mutex& mu) : m(mu) { m.lock(); }
    ~LockGuard() { m.unlock(); }
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

}  // namespace cs

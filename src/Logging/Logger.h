// Logging/Logger.h - port of coab's Logging project (Logger, DebugWriter, Config).
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#pragma once

#include "cs/cs.h"
#include "cs/io.h"

#include <cstdio>
#include <functional>
#include "cs/sync.h"

namespace Logging {
using namespace cs;

class DebugWriter {
    String filename;
    std::FILE* writer = nullptr;
    Mutex iolock;
    long bytes = 0;

    // GB-07: with a cap (the 3DS log) the file rolls over to <name>.old past capBytes.
    void Open() {
        if (writer != nullptr && capBytes > 0 && bytes > capBytes) {
            std::fclose(writer);
            writer = nullptr;
            std::string old = filename + ".old";
            std::remove(old.c_str());
            std::rename(filename.c_str(), old.c_str());
        }
        if (writer == nullptr && !filename.empty()) {
            writer = std::fopen(filename.c_str(), "a");
            if (writer != nullptr) {
                if (capBytes > 0) std::setvbuf(writer, nullptr, _IOFBF, 16384);
                std::fseek(writer, 0, SEEK_END);
                bytes = std::ftell(writer);
            }
        }
    }

public:
    long capBytes = 0;   // 0: no cap, no extra buffer (PC, and the debug dump files)

    explicit DebugWriter(const String& _filename) : filename(_filename) {}
    ~DebugWriter() { Close(); }
    void WriteLine(const String& s, bool flush = true) {
        LockGuard g(iolock);
        Open();
        if (writer != nullptr) {
            std::fputs(s.c_str(), writer);
            std::fputc('\n', writer);
            bytes += (long)s.Length() + 1;
            if (flush) std::fflush(writer);
        }
    }
    void Write(const String& s) {
        LockGuard g(iolock);
        Open();
        if (writer != nullptr) {
            std::fputs(s.c_str(), writer);
            bytes += (long)s.Length();
        }
    }
    void Close() {
        LockGuard g(iolock);
        if (writer != nullptr) std::fclose(writer);
        writer = nullptr;
    }
};

struct Logger {
    using VoidDelegate = std::function<void()>;

    inline static String logPath;
    inline static DebugWriter* debug = nullptr;
    inline static VoidDelegate ExitFuncCallback;
    // Console echo of every Debug line (coab writes everything to stdout too).
    inline static bool echoToConsole = true;
    // GB-07: Debug()/DebugWrite() lines (engine field reads, VM traces: many per second)
    // only when this is set. The 3DS host sets it from sdmc:/3ds/goldbox/debug.txt and then
    // buffers them (bufferDebug) with a 1 MB cap. Log() lines (exit save, errors) always go
    // out and are flushed at once.
    inline static bool debugEnabled = true;
    inline static bool bufferDebug = false;
    inline static long capBytes = 0;

    static void Setup(const String& path) {
        logPath = path;
        delete debug;
        debug = new DebugWriter(IO::Path::Combine(logPath, "Debugging.txt"));
        debug->capBytes = capBytes;
    }
    static String GetPath() { return logPath; }
    static void SetExitFunc(VoidDelegate f) { ExitFuncCallback = std::move(f); }

    static void Console(const String& s) {
        if (echoToConsole) {
            std::fputs(s.c_str(), stdout);
            std::fputc('\n', stdout);
        }
    }
    static void ConsoleWrite(const String& s) {
        if (echoToConsole) std::fputs(s.c_str(), stdout);
    }

    template <class... A>
    static void Log(const String& fmt, const A&... args) {
        String s = sizeof...(A) ? Format(fmt, args...) : fmt;
        Console(s);
        if (debug) debug->WriteLine(s);
    }
    template <class... A>
    static void LogAndExit(const String& fmt, const A&... args) {
        Log(fmt, args...);
        if (ExitFuncCallback) ExitFuncCallback();
    }
    template <class... A>
    static void Debug(const String& fmt, const A&... args) {
        if (!debugEnabled) return;
        String s = sizeof...(A) ? Format(fmt, args...) : fmt;
        Console(s);
        if (debug) debug->WriteLine(s, !bufferDebug);
    }
    template <class... A>
    static void DebugWrite(const String& fmt, const A&... args) {
        if (!debugEnabled) return;
        String s = sizeof...(A) ? Format(fmt, args...) : fmt;
        ConsoleWrite(s);
        if (debug) debug->Write(s);
    }
    static void Close() {
        if (debug) debug->Close();
    }
};

// Port of Logging/Config.cs. Base path is chosen by the platform layer
// (Documents folder on PC, sdmc:/3ds/goldbox on the 3DS).
struct Config {
    inline static String basePath;
    inline static String logPath;
    inline static String savePath;

    static void Setup(const String& base) {
        basePath = base;
        if (!IO::Directory::Exists(basePath)) IO::Directory::CreateDirectory(basePath);
        logPath = IO::Path::Combine(basePath, "Logs");
        if (!IO::Directory::Exists(logPath)) IO::Directory::CreateDirectory(logPath);
        savePath = IO::Path::Combine(basePath, "Save");
        if (!IO::Directory::Exists(savePath)) IO::Directory::CreateDirectory(savePath);
        Logger::Setup(logPath);
    }
    static String GetLogPath() { return logPath; }
    static String GetSavePath() { return savePath; }
    static String GetBasePath() { return basePath; }
};

}  // namespace Logging

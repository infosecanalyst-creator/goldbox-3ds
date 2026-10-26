// Logging/Logger.h - port of coab's Logging project (Logger, DebugWriter, Config).
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#pragma once

#include "cs/cs.h"
#include "cs/io.h"

#include <cstdio>
#include <functional>
#include <mutex>

namespace Logging {
using namespace cs;

class DebugWriter {
    String filename;
    std::FILE* writer = nullptr;
    std::mutex iolock;

public:
    explicit DebugWriter(const String& _filename) : filename(_filename) {}
    ~DebugWriter() { Close(); }
    void WriteLine(const String& s) {
        std::lock_guard<std::mutex> g(iolock);
        if (writer == nullptr && !filename.empty()) writer = std::fopen(filename.c_str(), "a");
        if (writer != nullptr) {
            std::fputs(s.c_str(), writer);
            std::fputc('\n', writer);
            std::fflush(writer);
        }
    }
    void Write(const String& s) {
        std::lock_guard<std::mutex> g(iolock);
        if (writer == nullptr && !filename.empty()) writer = std::fopen(filename.c_str(), "a");
        if (writer != nullptr) std::fputs(s.c_str(), writer);
    }
    void Close() {
        std::lock_guard<std::mutex> g(iolock);
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

    static void Setup(const String& path) {
        logPath = path;
        delete debug;
        debug = new DebugWriter(IO::Path::Combine(logPath, "Debugging.txt"));
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
        String s = sizeof...(A) ? Format(fmt, args...) : fmt;
        Console(s);
        if (debug) debug->WriteLine(s);
    }
    template <class... A>
    static void DebugWrite(const String& fmt, const A&... args) {
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

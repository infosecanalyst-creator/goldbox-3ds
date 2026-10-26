// cs/io.h - the System.IO / System.DateTime subset coab uses.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#pragma once

#include "cs/cs.h"

#include <chrono>
#include <ctime>
#include <cstdio>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif

namespace cs {

// ---------------------------------------------------------------------------
// DateTime / TimeSpan
// ---------------------------------------------------------------------------
struct TimeSpan {
    int64_t ticks = 0;  // 100ns units, like .NET
    double TotalMilliseconds() const { return ticks / 10000.0; }
    double TotalSeconds() const { return ticks / 10000000.0; }
};

struct DateTime {
    int64_t Ticks = 0;  // 100ns units since an arbitrary epoch

    static DateTime Now() {
        using namespace std::chrono;
        auto ns = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
        // .NET ticks since 0001-01-01; only differences and seeding matter.
        return DateTime{(int64_t)(ns / 100) + 621355968000000000LL};
    }
    DateTime AddSeconds(double s) const { return DateTime{Ticks + (int64_t)(s * 10000000.0)}; }
    DateTime AddMilliseconds(double ms) const { return DateTime{Ticks + (int64_t)(ms * 10000.0)}; }
    TimeSpan Subtract(const DateTime& o) const { return TimeSpan{Ticks - o.Ticks}; }
    bool operator<(const DateTime& o) const { return Ticks < o.Ticks; }
    bool operator>(const DateTime& o) const { return Ticks > o.Ticks; }
    bool operator<=(const DateTime& o) const { return Ticks <= o.Ticks; }
    bool operator>=(const DateTime& o) const { return Ticks >= o.Ticks; }
    String ToString() const {
        std::time_t t = (std::time_t)((Ticks - 621355968000000000LL) / 10000000LL);
        char buf[64];
        std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        return buf;
    }
};
inline String ToString(const DateTime& d) { return d.ToString(); }

namespace IO {

enum class SeekOrigin { Begin, Current, End };
enum class FileMode { CreateNew, Create, Open, OpenOrCreate, Truncate, Append };
enum class FileAccess { Read, Write, ReadWrite };
enum class FileShare { None, Read, Write, ReadWrite };

struct Path {
    static String Combine(const String& a, const String& b) {
        if (a.empty()) return b;
        if (b.empty()) return a;
        if (b[0] == '/' || b[0] == '\\' || (b.size() > 1 && b[1] == ':')) return b;
        char last = a[a.Length() - 1];
        if (last == '/' || last == '\\' || last == ':') return a + b;
        return a + "/" + b;
    }
    static int LastSep(const String& p) {
        int i = p.Length() - 1;
        while (i >= 0 && p[i] != '/' && p[i] != '\\' && p[i] != ':') i--;
        return i;
    }
    static String GetFileName(const String& p) { return p.Substring(LastSep(p) + 1); }
    static String GetDirectoryName(const String& p) {
        int i = LastSep(p);
        return i < 0 ? String("") : p.Substring(0, i);
    }
    static String GetExtension(const String& p) {
        String f = GetFileName(p);
        int d = f.LastIndexOf('.');
        return d < 0 ? String("") : f.Substring(d);
    }
    static String GetFileNameWithoutExtension(const String& p) {
        String f = GetFileName(p);
        int d = f.LastIndexOf('.');
        return d < 0 ? f : f.Substring(0, d);
    }
    static String ChangeExtension(const String& p, const String& ext) {
        String dir = GetDirectoryName(p);
        String base = GetFileNameWithoutExtension(p);
        String e = ext;
        if (!e.empty() && e[0] != '.') e = String(".") + e;
        String name = base + e;
        return dir.empty() ? name : Combine(dir, name);
    }
};

// Case-insensitive file lookup: the DOS data files are upper case on disk but
// coab sometimes asks for them in other cases. Resolves `path` to an existing
// file whose name matches ignoring case; returns `path` unchanged otherwise.
String ResolveCase(const String& path);

struct File {
    static bool Exists(const String& p) {
        struct stat st;
        return stat(ResolveCase(p).c_str(), &st) == 0 && !(st.st_mode & S_IFDIR);
    }
    static void Delete(const String& p) { std::remove(ResolveCase(p).c_str()); }
    static Array<byte> ReadAllBytes(const String& p);
    static void WriteAllBytes(const String& p, const Array<byte>& data);
    static Ref<class FileStream_> Open(const String& p, FileMode mode, FileAccess access = FileAccess::ReadWrite);
};

struct Directory {
    static bool Exists(const String& p) {
        struct stat st;
        return stat(p.c_str(), &st) == 0 && (st.st_mode & S_IFDIR);
    }
    static void CreateDirectory(const String& p) {
        // create each level
        String cur;
        for (int i = 0; i < p.Length(); i++) {
            cur.push_back(p[i]);
            if ((p[i] == '/' || p[i] == '\\' || i == p.Length() - 1) && cur.Length() > 1 && !Exists(cur)) {
#ifdef _WIN32
                _mkdir(cur.c_str());
#else
                mkdir(cur.c_str(), 0777);
#endif
            }
        }
    }
    static String GetCurrentDirectory();
    // pattern supports a single '*' wildcard, e.g. "*.sav" or "CHRDAT*.SAV" (case-insensitive)
    static Array<String> GetFiles(const String& dir, const String& pattern);
};

class FileStream_ {
    std::FILE* f = nullptr;

public:
    String name;
    FileStream_(const String& path, FileMode mode, FileAccess access = FileAccess::ReadWrite,
                FileShare share = FileShare::ReadWrite);
    ~FileStream_() { Close(); }
    FileStream_(const FileStream_&) = delete;
    FileStream_& operator=(const FileStream_&) = delete;

    bool IsOpen() const { return f != nullptr; }
    int Read(const Array<byte>& buf, int offset, int count) {
        if (!f) throw IOException("stream closed");
        return (int)std::fread(buf.data() + offset, 1, (std::size_t)count, f);
    }
    int ReadByte() {
        if (!f) throw IOException("stream closed");
        int c = std::fgetc(f);
        return c == EOF ? -1 : c;
    }
    void Write(const Array<byte>& buf, int offset, int count) {
        if (!f) throw IOException("stream closed");
        std::fwrite(buf.data() + offset, 1, (std::size_t)count, f);
    }
    void WriteByte(byte b) { std::fputc(b, f); }
    int64_t Seek(int64_t off, SeekOrigin o) {
        std::fseek(f, (long)off, o == SeekOrigin::Begin ? SEEK_SET : (o == SeekOrigin::Current ? SEEK_CUR : SEEK_END));
        return Position();
    }
    int64_t Position() const { return f ? (int64_t)std::ftell(f) : 0; }
    void set_Position(int64_t p) { Seek(p, SeekOrigin::Begin); }
    int64_t Length() const {
        long cur = std::ftell(f);
        std::fseek(f, 0, SEEK_END);
        long len = std::ftell(f);
        std::fseek(f, cur, SEEK_SET);
        return len;
    }
    void SetLength(int64_t len);
    void Flush() { if (f) std::fflush(f); }
    void Close() {
        if (f) {
            std::fclose(f);
            f = nullptr;
        }
    }
};
using FileStream = Ref<FileStream_>;

inline FileStream File::Open(const String& p, FileMode mode, FileAccess access) {
    return New<FileStream_>(p, mode, access);
}

class BinaryReader_ {
public:
    FileStream BaseStream;
    explicit BinaryReader_(const FileStream& s) : BaseStream(s) {}
    byte ReadByte() {
        int c = BaseStream->ReadByte();
        if (c < 0) throw IOException("EndOfStream");
        return (byte)c;
    }
    sbyte ReadSByte() { return (sbyte)ReadByte(); }
    short ReadInt16() { int lo = ReadByte(); int hi = ReadByte(); return (short)(lo | (hi << 8)); }
    ushort ReadUInt16() { int lo = ReadByte(); int hi = ReadByte(); return (ushort)(lo | (hi << 8)); }
    int ReadInt32() {
        uint32_t a = ReadByte(), b = ReadByte(), c = ReadByte(), d = ReadByte();
        return (int)(a | (b << 8) | (c << 16) | (d << 24));
    }
    uint ReadUInt32() { return (uint)ReadInt32(); }
    Array<byte> ReadBytes(int n) {
        Array<byte> a(n);
        int got = BaseStream->Read(a, 0, n);
        if (got < n) {
            Array<byte> b(got);
            if (got > 0) Array<byte>::Copy(a, b, got);
            return b;
        }
        return a;
    }
    void Close() { BaseStream->Close(); }
};
using BinaryReader = Ref<BinaryReader_>;

// Minimal text writer used by Classes::Text (debug dumps only).
class TextWriter_ {
    std::FILE* f;
    bool own;

public:
    explicit TextWriter_(std::FILE* fp) : f(fp), own(false) {}
    explicit TextWriter_(const String& path, bool append = false) : f(std::fopen(path.c_str(), append ? "a" : "w")), own(true) {}
    ~TextWriter_() { Close(); }
    void Write(const String& s) { if (f) std::fputs(s.c_str(), f); }
    void WriteLine(const String& s = "") { if (f) { std::fputs(s.c_str(), f); std::fputc('\n', f); std::fflush(f); } }
    void Close() { if (f && own) std::fclose(f); f = nullptr; }
};
using TextWriter = Ref<TextWriter_>;
using StreamWriter_ = TextWriter_;

class TextReader_ {
    std::FILE* f;
    bool own;

public:
    explicit TextReader_(std::FILE* fp) : f(fp), own(false) {}
    explicit TextReader_(const String& path) : f(std::fopen(path.c_str(), "r")), own(true) {}
    ~TextReader_() { if (f && own) std::fclose(f); }
    String ReadLine() {
        String s;
        int c;
        while (f && (c = std::fgetc(f)) != EOF && c != '\n') if (c != '\r') s.push_back((char)c);
        return s;
    }
};
using TextReader = Ref<TextReader_>;
using StreamReader_ = TextReader_;

}  // namespace IO
}  // namespace cs

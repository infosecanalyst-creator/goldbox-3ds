// cs/io.cpp - System.IO subset implementation.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include "cs/io.h"

namespace cs {
namespace IO {

static bool WildMatch(const String& pat, const String& name) {
    String p = pat.ToUpper(), n = name.ToUpper();
    int star = p.IndexOf('*');
    if (star < 0) return p == n;
    String pre = p.Substring(0, star), post = p.Substring(star + 1);
    return n.Length() >= pre.Length() + post.Length() && n.StartsWith(pre) && n.EndsWith(post);
}

static std::vector<String> ListDir(const String& dir) {
    std::vector<String> out;
#ifdef _WIN32
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA((dir.empty() ? String(".\\*") : Path::Combine(dir, "*")).c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return out;
    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) out.emplace_back(fd.cFileName);
    } while (FindNextFileA(h, &fd));
    FindClose(h);
#else
    DIR* d = opendir(dir.empty() ? "." : dir.c_str());
    if (!d) return out;
    while (dirent* e = readdir(d)) {
        String n = e->d_name;
        if (n == "." || n == "..") continue;
        out.push_back(n);
    }
    closedir(d);
#endif
    return out;
}

String ResolveCase(const String& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) return path;
    String dir = Path::GetDirectoryName(path);
    String name = Path::GetFileName(path);
    for (auto& n : ListDir(dir)) {
        if (n.ToUpper() == name.ToUpper()) return dir.empty() ? n : Path::Combine(dir, n);
    }
    return path;
}

Array<byte> File::ReadAllBytes(const String& p) {
    FileStream fs = File::Open(p, FileMode::Open, FileAccess::Read);
    int len = (int)fs->Length();
    Array<byte> a(len);
    fs->Read(a, 0, len);
    return a;
}

void File::WriteAllBytes(const String& p, const Array<byte>& data) {
    FileStream fs = File::Open(p, FileMode::Create, FileAccess::Write);
    fs->Write(data, 0, data.Length());
}

String Directory::GetCurrentDirectory() {
    char buf[1024];
#ifdef _WIN32
    if (_getcwd(buf, sizeof buf)) return buf;
#else
    if (getcwd(buf, sizeof buf)) return buf;
#endif
    return ".";
}

Array<String> Directory::GetFiles(const String& dir, const String& pattern) {
    std::vector<String> out;
    for (auto& n : ListDir(dir)) {
        if (WildMatch(pattern, n)) out.push_back(Path::Combine(dir, n));
    }
    std::sort(out.begin(), out.end());
    return Array<String>(out);
}

FileStream_::FileStream_(const String& path, FileMode mode, FileAccess access, FileShare) : name(path) {
    String p = ResolveCase(path);
    struct stat st;
    bool exists = stat(p.c_str(), &st) == 0;
    const char* m = "rb";
    switch (mode) {
        case FileMode::Open:
            if (!exists) throw IOException(String("File not found: ") + path);
            m = access == FileAccess::Read ? "rb" : "r+b";
            break;
        case FileMode::OpenOrCreate:
            m = exists ? (access == FileAccess::Read ? "rb" : "r+b") : "w+b";
            break;
        case FileMode::Create:
        case FileMode::Truncate:
            m = access == FileAccess::Read ? "rb" : "w+b";
            break;
        case FileMode::CreateNew:
            if (exists) throw IOException(String("File exists: ") + path);
            m = "w+b";
            break;
        case FileMode::Append:
            m = "a+b";
            break;
    }
    f = std::fopen(p.c_str(), m);
    if (!f) throw IOException(String("Cannot open: ") + path);
}

void FileStream_::SetLength(int64_t len) {
    if (!f) return;
    std::fflush(f);
#ifdef _WIN32
    _chsize(_fileno(f), (long)len);
#else
    if (ftruncate(fileno(f), (off_t)len) != 0) throw IOException("SetLength");
#endif
    if (Position() > len) Seek(len, SeekOrigin::Begin);
}

}  // namespace IO
}  // namespace cs

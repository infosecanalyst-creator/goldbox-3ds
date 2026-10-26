// cs.h - small C#-compatibility runtime used by the C++ translation of coab.
//
// The goal is to keep the translated engine code line-for-line close to the
// original C# so it can be diffed against coab and against the original DOS
// game's behaviour. See docs/TRANSLATION.md for the rules translators follow.
//
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <deque>

namespace cs {

// ---------------------------------------------------------------------------
// Primitive aliases (C# names). C# `int`, `short`, `bool`, `char`, `double`
// map to the C++ keywords of the same name. C# `long` -> int64_t (never `long`).
// ---------------------------------------------------------------------------
using byte = std::uint8_t;
using sbyte = std::int8_t;
using ushort = std::uint16_t;
using uint = std::uint32_t;

// ---------------------------------------------------------------------------
// Exceptions
// ---------------------------------------------------------------------------
struct Exception : std::runtime_error {
    explicit Exception(const std::string& msg = "Exception") : std::runtime_error(msg) {}
    std::string Message() const { return what(); }
};
struct ApplicationException : Exception { using Exception::Exception; };
struct NotImplementedException : Exception {
    explicit NotImplementedException(const std::string& msg = "NotImplementedException") : Exception(msg) {}
};
struct NotSupportedException : Exception { using Exception::Exception; };
struct ArgumentException : Exception { using Exception::Exception; };
struct ArgumentOutOfRangeException : ArgumentException { using ArgumentException::ArgumentException; };
struct IndexOutOfRangeException : Exception {
    explicit IndexOutOfRangeException(const std::string& msg = "IndexOutOfRangeException") : Exception(msg) {}
};
struct NullReferenceException : Exception {
    explicit NullReferenceException(const std::string& msg = "NullReferenceException") : Exception(msg) {}
};
struct InvalidOperationException : Exception { using Exception::Exception; };
struct SerializationException : Exception { using Exception::Exception; };
struct IOException : Exception { using Exception::Exception; };
// Thrown from blocking platform calls to unwind the engine thread on exit
// (replaces Thread.Abort in the C# version).
struct EngineAbort {};

// ---------------------------------------------------------------------------
// Ref<T>: reference-type handle. Every instantiable C# class `X` becomes a C++
// class `X_` plus `using X = cs::Ref<X_>;`, so declarations read like C#.
// ---------------------------------------------------------------------------
template <class T>
class Ref : public std::shared_ptr<T> {
public:
    using Base = std::shared_ptr<T>;
    using element_type = T;
    Ref() noexcept = default;
    Ref(std::nullptr_t) noexcept {}
    Ref(const Base& b) noexcept : Base(b) {}
    Ref(Base&& b) noexcept : Base(std::move(b)) {}
    template <class U, class = std::enable_if_t<std::is_convertible<U*, T*>::value>>
    Ref(const Ref<U>& o) noexcept : Base(o) {}
    template <class U, class = std::enable_if_t<std::is_convertible<U*, T*>::value>>
    Ref(const std::shared_ptr<U>& o) noexcept : Base(o) {}

    T* operator->() const {
        T* p = Base::get();
        if (!p) throw NullReferenceException();
        return p;
    }
    T& operator*() const {
        T* p = Base::get();
        if (!p) throw NullReferenceException();
        return *p;
    }
    // C# indexer on a reference type: obj[i] -> (*obj)[i]
    template <class I>
    decltype(auto) operator[](I&& i) const { return (**this)[std::forward<I>(i)]; }
};

template <class T, class... A>
inline Ref<T> New(A&&... a) { return Ref<T>(std::make_shared<T>(std::forward<A>(a)...)); }

// C# `x is T` (T is the C++ class name, e.g. Player_)
template <class T, class U>
inline bool Is(const Ref<U>& r) { return r && dynamic_cast<T*>(r.get()) != nullptr; }
// C# `x as T`
template <class T, class U>
inline Ref<T> As(const Ref<U>& r) { return Ref<T>(std::dynamic_pointer_cast<T>(std::shared_ptr<U>(r))); }

// ---------------------------------------------------------------------------
// String: std::string with C#-style members. C# `null` strings become "".
// ---------------------------------------------------------------------------
template <class T>
class Array;

class String : public std::string {
public:
    String() = default;
    String(const char* s) : std::string(s ? s : "") {}
    String(std::nullptr_t) {}
    String(const std::string& s) : std::string(s) {}
    String(std::string&& s) : std::string(std::move(s)) {}
    String(std::size_t n, char c) : std::string(n, c) {}
    String(const char* s, std::size_t n) : std::string(s, n) {}

    int Length() const { return (int)size(); }
    char operator[](int i) const {
        if (i < 0 || i >= (int)size()) throw IndexOutOfRangeException();
        return std::string::operator[]((std::size_t)i);
    }

    String Substring(int start) const {
        if (start < 0 || start > Length()) throw ArgumentOutOfRangeException("Substring");
        return String(substr((std::size_t)start));
    }
    String Substring(int start, int len) const {
        if (start < 0 || len < 0 || start + len > Length()) throw ArgumentOutOfRangeException("Substring");
        return String(substr((std::size_t)start, (std::size_t)len));
    }
    int IndexOf(char c) const { auto p = find(c); return p == npos ? -1 : (int)p; }
    int IndexOf(char c, int start) const { auto p = find(c, (std::size_t)start); return p == npos ? -1 : (int)p; }
    int IndexOf(const std::string& s) const { auto p = find(s); return p == npos ? -1 : (int)p; }
    int IndexOf(const std::string& s, int start) const { auto p = find(s, (std::size_t)start); return p == npos ? -1 : (int)p; }
    int LastIndexOf(char c) const { auto p = rfind(c); return p == npos ? -1 : (int)p; }
    bool Contains(const std::string& s) const { return find(s) != npos; }
    bool StartsWith(const std::string& s) const { return compare(0, s.size(), s) == 0 && size() >= s.size(); }
    bool EndsWith(const std::string& s) const {
        return size() >= s.size() && compare(size() - s.size(), s.size(), s) == 0;
    }
    String ToUpper() const {
        String r(*this);
        for (auto& ch : r) ch = (char)std::toupper((unsigned char)ch);
        return r;
    }
    String ToLower() const {
        String r(*this);
        for (auto& ch : r) ch = (char)std::tolower((unsigned char)ch);
        return r;
    }
    String Trim() const {
        std::size_t b = 0, e = size();
        while (b < e && std::isspace((unsigned char)at(b))) b++;
        while (e > b && std::isspace((unsigned char)at(e - 1))) e--;
        return String(substr(b, e - b));
    }
    String TrimEnd() const {
        std::size_t e = size();
        while (e > 0 && std::isspace((unsigned char)at(e - 1))) e--;
        return String(substr(0, e));
    }
    String TrimStart() const {
        std::size_t b = 0;
        while (b < size() && std::isspace((unsigned char)at(b))) b++;
        return String(substr(b));
    }
    String PadRight(int n, char c = ' ') const {
        String r(*this);
        if (r.Length() < n) r.append((std::size_t)(n - r.Length()), c);
        return r;
    }
    String PadLeft(int n, char c = ' ') const {
        String r(*this);
        if (r.Length() < n) r.insert(0, (std::size_t)(n - r.Length()), c);
        return r;
    }
    String Replace(const std::string& from, const std::string& to) const {
        if (from.empty()) return *this;
        String r;
        std::size_t pos = 0, f;
        while ((f = find(from, pos)) != npos) {
            r.append(*this, pos, f - pos);
            r.append(to);
            pos = f + from.size();
        }
        r.append(*this, pos, npos);
        return r;
    }
    String Replace(char from, char to) const {
        String r(*this);
        for (auto& ch : r) if (ch == from) ch = to;
        return r;
    }
    String Remove(int start) const { return String(substr(0, (std::size_t)start)); }
    String Remove(int start, int count) const {
        String r(*this);
        r.erase((std::size_t)start, (std::size_t)count);
        return r;
    }
    String Insert(int at, const std::string& s) const {
        String r(*this);
        r.insert((std::size_t)at, s);
        return r;
    }
    std::vector<String> Split(char sep) const {
        std::vector<String> out;
        std::size_t pos = 0, f;
        while ((f = find(sep, pos)) != npos) {
            out.emplace_back(substr(pos, f - pos));
            pos = f + 1;
        }
        out.emplace_back(substr(pos));
        return out;
    }
    char* ToCharArrayUnsafe() = delete;
    // C# Trim(char[]) / TrimStart(char) / TrimEnd(char)
    String Trim(const Array<char>& chars) const;
    String TrimStart(char c) const {
        std::size_t b = 0;
        while (b < size() && at(b) == c) b++;
        return String(substr(b));
    }
    String TrimEnd(char c) const {
        std::size_t e = size();
        while (e > 0 && at(e - 1) == c) e--;
        return String(substr(0, e));
    }

    static bool IsNullOrEmpty(const String& s) { return s.empty(); }
    static int Compare(const String& a, const String& b) { int c = a.compare(b); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    static int Compare(const String& a, const String& b, bool ignoreCase) {
        return ignoreCase ? Compare(a.ToUpper(), b.ToUpper()) : Compare(a, b);
    }
    static const String Empty;
};
inline const String String::Empty{};

inline String operator+(const String& a, const String& b) { return String(static_cast<const std::string&>(a) + static_cast<const std::string&>(b)); }
inline String operator+(const String& a, const char* b) { return String(static_cast<const std::string&>(a) + b); }
inline String operator+(const char* a, const String& b) { return String(a + static_cast<const std::string&>(b)); }
inline String operator+(const String& a, char b) { String r(a); r.push_back(b); return r; }
inline String operator+(char a, const String& b) { String r(1, a); r.append(b); return r; }

// ---------------------------------------------------------------------------
// ToString / Format.  cs::ToString(x) and cs::Format("{0} {1,4:X}", a, b)
// implement the subset of .NET formatting coab uses: {n}, {n,width},
// {n:X}, {n:Xk}, {n:Dk}, {n,width:X}, and ToString("00").
// Enums print as their numeric value unless an EnumName() overload exists.
// ---------------------------------------------------------------------------
inline String ToString(const String& s) { return s; }
inline String ToString(const char* s) { return String(s); }
inline String ToString(const std::string& s) { return String(s); }
inline String ToString(bool b) { return b ? "True" : "False"; }
inline String ToString(char c) { return String(1, c); }
inline String ToString(double d) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.15g", d);
    return buf;
}
template <class T, std::enable_if_t<std::is_integral<T>::value && !std::is_same<T, bool>::value && !std::is_same<T, char>::value, int> = 0>
inline String ToString(T v) { return String(std::to_string((long long)v)); }
template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
inline String ToString(T v) { return ToString((long long)v); }
template <class T>
inline auto ToString(const Ref<T>& r) -> decltype(r->ToString(), String()) { return r ? r->ToString() : String(); }

// Numeric formatting with a .NET format spec ("X", "X2", "D3", "00").
template <class T, std::enable_if_t<std::is_integral<T>::value || std::is_enum<T>::value, int> = 0>
inline String ToString(T v, const std::string& spec) {
    long long n = (long long)v;
    if (spec.empty()) return ToString(n);
    char kind = spec[0];
    int digits = spec.size() > 1 ? std::atoi(spec.c_str() + 1) : 0;
    char buf[64];
    if (kind == 'X' || kind == 'x') {
        unsigned long long u = (unsigned long long)n;
        if (n < 0) u &= (sizeof(T) >= 4 ? 0xFFFFFFFFull : (sizeof(T) == 2 ? 0xFFFFull : 0xFFull));
        std::snprintf(buf, sizeof buf, kind == 'X' ? "%0*llX" : "%0*llx", digits, u);
        return buf;
    }
    if (kind == 'D' || kind == 'd') {
        std::snprintf(buf, sizeof buf, "%0*lld", digits, n);
        return buf;
    }
    bool allZero = std::all_of(spec.begin(), spec.end(), [](char c) { return c == '0'; });
    if (allZero) {
        std::snprintf(buf, sizeof buf, "%0*lld", (int)spec.size(), n);
        return buf;
    }
    return ToString(n);
}

namespace detail {
inline void FormatArgs(std::vector<std::function<String(const std::string&)>>&) {}
template <class T, class... R>
inline void FormatArgs(std::vector<std::function<String(const std::string&)>>& out, const T& v, const R&... rest) {
    out.push_back([&v](const std::string& spec) -> String {
        if constexpr (std::is_integral<T>::value || std::is_enum<T>::value) {
            if (!spec.empty()) return cs::ToString(v, spec);
        }
        return cs::ToString(v);
    });
    FormatArgs(out, rest...);
}
}  // namespace detail

template <class... A>
inline String Format(const std::string& fmt, const A&... args) {
    std::vector<std::function<String(const std::string&)>> fs;
    detail::FormatArgs(fs, args...);
    String out;
    for (std::size_t i = 0; i < fmt.size(); i++) {
        char c = fmt[i];
        if (c == '{') {
            if (i + 1 < fmt.size() && fmt[i + 1] == '{') { out.push_back('{'); i++; continue; }
            std::size_t close = fmt.find('}', i);
            if (close == std::string::npos) throw ArgumentException("Format");
            std::string inner = fmt.substr(i + 1, close - i - 1);
            std::string spec;
            int width = 0;
            auto colon = inner.find(':');
            if (colon != std::string::npos) { spec = inner.substr(colon + 1); inner = inner.substr(0, colon); }
            auto comma = inner.find(',');
            if (comma != std::string::npos) { width = std::atoi(inner.c_str() + comma + 1); inner = inner.substr(0, comma); }
            int idx = std::atoi(inner.c_str());
            if (idx < 0 || idx >= (int)fs.size()) throw ArgumentException("Format index");
            String s = fs[(std::size_t)idx](spec);
            if (width > 0 && s.Length() < width) s = s.PadLeft(width);
            if (width < 0 && s.Length() < -width) s = s.PadRight(-width);
            out += s;
            i = close;
        } else if (c == '}') {
            if (i + 1 < fmt.size() && fmt[i + 1] == '}') i++;
            out.push_back('}');
        } else {
            out.push_back(c);
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Array<T> (C# T[]): reference semantics, default-constructed == null.
// ---------------------------------------------------------------------------
template <class T>
class Array {
    std::shared_ptr<std::vector<T>> p;

public:
    Array() = default;
    Array(std::nullptr_t) {}
    explicit Array(int n) : p(std::make_shared<std::vector<T>>((std::size_t)(n < 0 ? 0 : n))) {}
    Array(std::initializer_list<T> il) : p(std::make_shared<std::vector<T>>(il)) {}
    explicit Array(std::vector<T> v) : p(std::make_shared<std::vector<T>>(std::move(v))) {}

    bool IsNull() const { return !p; }
    explicit operator bool() const { return (bool)p; }
    friend bool operator==(const Array& a, std::nullptr_t) { return !a.p; }
    friend bool operator!=(const Array& a, std::nullptr_t) { return (bool)a.p; }
    friend bool operator==(const Array& a, const Array& b) { return a.p == b.p; }
    friend bool operator!=(const Array& a, const Array& b) { return a.p != b.p; }

    int Length() const { return p ? (int)p->size() : throw NullReferenceException("Array.Length"); }
    T& operator[](int i) const {
        if (!p) throw NullReferenceException("Array[]");
        if (i < 0 || i >= (int)p->size()) throw IndexOutOfRangeException();
        return (*p)[(std::size_t)i];
    }
    Array Clone() const { return p ? Array(*p) : Array(); }
    T* data() const { return p ? p->data() : nullptr; }
    std::vector<T>& vec() const { return *p; }
    typename std::vector<T>::iterator begin() const { return p->begin(); }
    typename std::vector<T>::iterator end() const { return p->end(); }

    // System.Array statics
    static void Copy(const Array& src, int si, const Array& dst, int di, int n) {
        if (n <= 0) return;
        if (si < 0 || di < 0 || si + n > src.Length() || di + n > dst.Length()) throw ArgumentException("Array.Copy");
        if (src.p == dst.p) std::move(src.p->begin() + si, src.p->begin() + si + n, dst.p->begin() + di);
        else std::copy(src.p->begin() + si, src.p->begin() + si + n, dst.p->begin() + di);
        if (src.p == dst.p && di > si) {  // overlapping forward copy: redo safely
            std::vector<T> tmp(src.p->begin() + si, src.p->begin() + si + n);
            std::copy(tmp.begin(), tmp.end(), dst.p->begin() + di);
        }
    }
    static void Copy(const Array& src, const Array& dst, int n) { Copy(src, 0, dst, 0, n); }
    static void Clear(const Array& a, int start, int n) {
        for (int i = start; i < start + n; i++) a[i] = T();
    }
    template <class P>
    static bool Exists(const Array& a, P pred) { return std::any_of(a.begin(), a.end(), pred); }
    template <class P>
    static int FindIndex(const Array& a, P pred) {
        for (int i = 0; i < a.Length(); i++) if (pred(a[i])) return i;
        return -1;
    }
    template <class F>
    static void ForEach(const Array& a, F f) { for (auto& x : a) f(x); }
};

inline String String::Trim(const Array<char>& chars) const {
    auto in = [&](char c) { for (int i = 0; i < chars.Length(); i++) if (chars[i] == c) return true; return false; };
    std::size_t b = 0, e = size();
    while (b < e && in(at(b))) b++;
    while (e > b && in(at(e - 1))) e--;
    return String(substr(b, e - b));
}

// C# int.Parse / int.TryParse (decimal, optional sign, surrounding whitespace)
struct Int32 {
    static bool TryParse(const String& s, int& result) {
        String t = s.Trim();
        result = 0;
        if (t.empty()) return false;
        std::size_t i = 0;
        bool neg = false;
        if (t[0] == '+' || t[0] == '-') { neg = t[0] == '-'; i = 1; }
        if (i >= t.size()) return false;
        long long v = 0;
        for (; i < t.size(); i++) {
            char c = t[(int)i];
            if (c < '0' || c > '9') return false;
            v = v * 10 + (c - '0');
            if (v > 2147483648LL) return false;
        }
        if (neg) v = -v;
        if (v > INT32_MAX || v < INT32_MIN) return false;
        result = (int)v;
        return true;
    }
    static int Parse(const String& s) {
        int r;
        if (!TryParse(s, r)) throw Exception("FormatException");
        return r;
    }
};

// C# T[,]  ->  a(i, j)
template <class T>
class Array2 {
    std::shared_ptr<std::vector<T>> p;
    int d0 = 0, d1 = 0;

public:
    Array2() = default;
    Array2(std::nullptr_t) {}
    Array2(int n0, int n1) : p(std::make_shared<std::vector<T>>((std::size_t)(n0 * n1))), d0(n0), d1(n1) {}
    Array2(std::initializer_list<std::initializer_list<T>> il) {
        d0 = (int)il.size();
        d1 = d0 ? (int)il.begin()->size() : 0;
        p = std::make_shared<std::vector<T>>();
        p->reserve((std::size_t)(d0 * d1));
        for (auto& row : il) {
            if ((int)row.size() != d1) throw ArgumentException("ragged Array2 initializer");
            p->insert(p->end(), row.begin(), row.end());
        }
    }
    bool IsNull() const { return !p; }
    friend bool operator==(const Array2& a, std::nullptr_t) { return !a.p; }
    friend bool operator!=(const Array2& a, std::nullptr_t) { return (bool)a.p; }
    int GetLength(int dim) const { return dim == 0 ? d0 : d1; }
    int Length() const { return d0 * d1; }
    T& operator()(int i, int j) const {
        if (!p) throw NullReferenceException("Array2");
        if (i < 0 || i >= d0 || j < 0 || j >= d1) throw IndexOutOfRangeException();
        return (*p)[(std::size_t)(i * d1 + j)];
    }
};

// C# T[,,]  ->  a(i, j, k)
template <class T>
class Array3 {
    std::shared_ptr<std::vector<T>> p;
    int d0 = 0, d1 = 0, d2 = 0;

public:
    Array3() = default;
    Array3(std::nullptr_t) {}
    Array3(int n0, int n1, int n2) : p(std::make_shared<std::vector<T>>((std::size_t)(n0 * n1 * n2))), d0(n0), d1(n1), d2(n2) {}
    Array3(std::initializer_list<std::initializer_list<std::initializer_list<T>>> il) {
        d0 = (int)il.size();
        d1 = d0 ? (int)il.begin()->size() : 0;
        d2 = (d0 && d1) ? (int)il.begin()->begin()->size() : 0;
        p = std::make_shared<std::vector<T>>();
        for (auto& a : il) {
            if ((int)a.size() != d1) throw ArgumentException("ragged Array3 initializer");
            for (auto& b : a) {
                if ((int)b.size() != d2) throw ArgumentException("ragged Array3 initializer");
                p->insert(p->end(), b.begin(), b.end());
            }
        }
    }
    bool IsNull() const { return !p; }
    friend bool operator==(const Array3& a, std::nullptr_t) { return !a.p; }
    friend bool operator!=(const Array3& a, std::nullptr_t) { return (bool)a.p; }
    int GetLength(int dim) const { return dim == 0 ? d0 : (dim == 1 ? d1 : d2); }
    T& operator()(int i, int j, int k) const {
        if (!p) throw NullReferenceException("Array3");
        if (i < 0 || i >= d0 || j < 0 || j >= d1 || k < 0 || k >= d2) throw IndexOutOfRangeException();
        return (*p)[(std::size_t)((i * d1 + j) * d2 + k)];
    }
};

// ---------------------------------------------------------------------------
// List<T>: reference semantics. NOTE: default-constructed List is an EMPTY list
// (not null), matching `new List<T>()`, which is how coab initialises nearly
// every list. Use `List<T>(nullptr)` for an explicit null.
// ---------------------------------------------------------------------------
template <class T>
class List {
    std::shared_ptr<std::vector<T>> p;

    std::vector<T>& v() const {
        if (!p) throw NullReferenceException("List");
        return *p;
    }

public:
    List() : p(std::make_shared<std::vector<T>>()) {}
    List(std::nullptr_t) {}
    List(std::initializer_list<T> il) : p(std::make_shared<std::vector<T>>(il)) {}
    explicit List(const std::vector<T>& src) : p(std::make_shared<std::vector<T>>(src)) {}
    explicit List(const Array<T>& src) : p(std::make_shared<std::vector<T>>(src.vec())) {}
    static List New() { return List(); }
    static List New(const List& other) { return List(other.v()); }

    bool IsNull() const { return !p; }
    friend bool operator==(const List& a, std::nullptr_t) { return !a.p; }
    friend bool operator!=(const List& a, std::nullptr_t) { return (bool)a.p; }
    friend bool operator==(const List& a, const List& b) { return a.p == b.p; }
    friend bool operator!=(const List& a, const List& b) { return a.p != b.p; }

    int Count() const { return (int)v().size(); }
    T& operator[](int i) const {
        auto& vv = v();
        if (i < 0 || i >= (int)vv.size()) throw ArgumentOutOfRangeException("List[]");
        return vv[(std::size_t)i];
    }
    void Add(const T& x) const { v().push_back(x); }
    void AddRange(const List& o) const { auto c = o.v(); v().insert(v().end(), c.begin(), c.end()); }
    void Insert(int i, const T& x) const {
        if (i < 0 || i > Count()) throw ArgumentOutOfRangeException("List.Insert");
        v().insert(v().begin() + i, x);
    }
    bool Remove(const T& x) const {
        auto& vv = v();
        auto it = std::find(vv.begin(), vv.end(), x);
        if (it == vv.end()) return false;
        vv.erase(it);
        return true;
    }
    void RemoveAt(int i) const {
        if (i < 0 || i >= Count()) throw ArgumentOutOfRangeException("List.RemoveAt");
        v().erase(v().begin() + i);
    }
    void RemoveRange(int i, int n) const { v().erase(v().begin() + i, v().begin() + i + n); }
    template <class P>
    int RemoveAll(P pred) const {
        auto& vv = v();
        auto old = vv.size();
        vv.erase(std::remove_if(vv.begin(), vv.end(), pred), vv.end());
        return (int)(old - vv.size());
    }
    void Clear() const { v().clear(); }
    bool Contains(const T& x) const { return std::find(v().begin(), v().end(), x) != v().end(); }
    int IndexOf(const T& x) const {
        auto it = std::find(v().begin(), v().end(), x);
        return it == v().end() ? -1 : (int)(it - v().begin());
    }
    template <class P>
    T Find(P pred) const {
        for (auto& x : v()) if (pred(x)) return x;
        return T();
    }
    template <class P>
    T FindLast(P pred) const {
        auto& vv = v();
        for (auto it = vv.rbegin(); it != vv.rend(); ++it) if (pred(*it)) return *it;
        return T();
    }
    template <class P>
    int FindIndex(P pred) const {
        for (int i = 0; i < Count(); i++) if (pred((*this)[i])) return i;
        return -1;
    }
    template <class P>
    List FindAll(P pred) const {
        List r;
        for (auto& x : v()) if (pred(x)) r.Add(x);
        return r;
    }
    template <class P>
    bool Exists(P pred) const { return std::any_of(v().begin(), v().end(), pred); }
    template <class P>
    bool TrueForAll(P pred) const { return std::all_of(v().begin(), v().end(), pred); }
    template <class F>
    void ForEach(F f) const {
        auto copy = v();  // C# throws if the list is modified; iterate a snapshot
        for (auto& x : copy) f(x);
    }
    List GetRange(int i, int n) const { return List(std::vector<T>(v().begin() + i, v().begin() + i + n)); }
    Array<T> ToArray() const { return Array<T>(v()); }
    void Reverse() const { std::reverse(v().begin(), v().end()); }
    // Sort() uses T's CompareTo (IComparable); Sort(cmp) takes a Comparison.
    void Sort() const;
    template <class C>
    void Sort(C cmp) const {
        std::stable_sort(v().begin(), v().end(), [&](const T& a, const T& b) { return cmp(a, b) < 0; });
    }
    T Max() const { return *std::max_element(v().begin(), v().end()); }
    T Min() const { return *std::min_element(v().begin(), v().end()); }

    typename std::vector<T>::iterator begin() const { return v().begin(); }
    typename std::vector<T>::iterator end() const { return v().end(); }
    std::vector<T>& vec() const { return v(); }
};

namespace detail {
template <class T>
inline int CompareTo(const T& a, const T& b) { return a->CompareTo(b); }
template <class T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
inline int CompareTo(const T& a, const T& b) { return a < b ? -1 : (b < a ? 1 : 0); }
}  // namespace detail

template <class T>
inline void List<T>::Sort() const {
    std::stable_sort(v().begin(), v().end(), [](const T& a, const T& b) { return detail::CompareTo(a, b) < 0; });
}

// ---------------------------------------------------------------------------
// Dictionary<K,V>, Queue<T>, Stack<T>
// ---------------------------------------------------------------------------
template <class K, class V>
class Dictionary {
    std::shared_ptr<std::map<K, V>> p;

public:
    Dictionary() : p(std::make_shared<std::map<K, V>>()) {}
    Dictionary(std::nullptr_t) {}
    Dictionary(std::initializer_list<std::pair<const K, V>> il) : p(std::make_shared<std::map<K, V>>(il)) {}
    bool IsNull() const { return !p; }
    friend bool operator==(const Dictionary& a, std::nullptr_t) { return !a.p; }
    friend bool operator!=(const Dictionary& a, std::nullptr_t) { return (bool)a.p; }
    int Count() const { return (int)p->size(); }
    V& operator[](const K& k) const { return (*p)[k]; }  // C# throws on missing get; callers check first
    const V& Get(const K& k) const {
        auto it = p->find(k);
        if (it == p->end()) throw ArgumentException("KeyNotFound");
        return it->second;
    }
    void Add(const K& k, const V& val) const {
        if (!p->emplace(k, val).second) throw ArgumentException("duplicate key");
    }
    bool ContainsKey(const K& k) const { return p->count(k) != 0; }
    bool TryGetValue(const K& k, V& out) const {
        auto it = p->find(k);
        if (it == p->end()) { out = V(); return false; }
        out = it->second;
        return true;
    }
    bool Remove(const K& k) const { return p->erase(k) != 0; }
    void Clear() const { p->clear(); }
    typename std::map<K, V>::iterator begin() const { return p->begin(); }
    typename std::map<K, V>::iterator end() const { return p->end(); }
};

template <class T>
class Queue {
    std::shared_ptr<std::deque<T>> p = std::make_shared<std::deque<T>>();

public:
    int Count() const { return (int)p->size(); }
    void Enqueue(const T& x) const { p->push_back(x); }
    T Dequeue() const {
        if (p->empty()) throw InvalidOperationException("Queue empty");
        T x = p->front();
        p->pop_front();
        return x;
    }
    T Peek() const {
        if (p->empty()) throw InvalidOperationException("Queue empty");
        return p->front();
    }
    void Clear() const { p->clear(); }
};

template <class T>
class Stack {
    std::shared_ptr<std::vector<T>> p = std::make_shared<std::vector<T>>();

public:
    int Count() const { return (int)p->size(); }
    void Push(const T& x) const { p->push_back(x); }
    T Pop() const {
        if (p->empty()) throw InvalidOperationException("Stack empty");
        T x = p->back();
        p->pop_back();
        return x;
    }
    T Peek() const {
        if (p->empty()) throw InvalidOperationException("Stack empty");
        return p->back();
    }
    void Clear() const { p->clear(); }
};

// ---------------------------------------------------------------------------
// Math
// ---------------------------------------------------------------------------
struct Math {
    static constexpr double PI = 3.14159265358979323846;
    template <class A, class B>
    static auto Min(A a, B b) -> std::common_type_t<A, B> {
        using C = std::common_type_t<A, B>;
        return (C)a < (C)b ? (C)a : (C)b;
    }
    template <class A, class B>
    static auto Max(A a, B b) -> std::common_type_t<A, B> {
        using C = std::common_type_t<A, B>;
        return (C)a > (C)b ? (C)a : (C)b;
    }
    template <class A>
    static A Abs(A a) { return a < 0 ? (A)-a : a; }
    template <class A>
    static int Sign(A a) { return a < 0 ? -1 : (a > 0 ? 1 : 0); }
    static double Sin(double d) { return std::sin(d); }
    static double Cos(double d) { return std::cos(d); }
    static double Sqrt(double d) { return std::sqrt(d); }
    static double Floor(double d) { return std::floor(d); }
    static double Ceiling(double d) { return std::ceil(d); }
    static double Round(double d) { return std::nearbyint(d); }  // banker's rounding, like .NET
    static double Pow(double a, double b) { return std::pow(a, b); }
};

// char helpers (C# char statics)
struct Char {
    static char ToUpper(char c) { return (char)std::toupper((unsigned char)c); }
    static char ToLower(char c) { return (char)std::tolower((unsigned char)c); }
    static bool IsDigit(char c) { return std::isdigit((unsigned char)c) != 0; }
    static bool IsLetter(char c) { return std::isalpha((unsigned char)c) != 0; }
    static bool IsUpper(char c) { return std::isupper((unsigned char)c) != 0; }
    static bool IsLower(char c) { return std::islower((unsigned char)c) != 0; }
    static bool IsWhiteSpace(char c) { return std::isspace((unsigned char)c) != 0; }
};

// ---------------------------------------------------------------------------
// Random: bit-exact port of .NET's seeded System.Random (Knuth subtractive
// generator; identical in .NET Framework and in .NET 5+ for seeded instances).
// Bit-exactness lets us replay coab and the C++ build with the same seed and
// diff the game state.
// ---------------------------------------------------------------------------
class Random {
    int inext = 0, inextp = 21;
    int SeedArray[56] = {};

    int InternalSample() {
        int locINext = inext, locINextp = inextp;
        if (++locINext >= 56) locINext = 1;
        if (++locINextp >= 56) locINextp = 1;
        int retVal = SeedArray[locINext] - SeedArray[locINextp];
        if (retVal == 0x7fffffff) retVal--;
        if (retVal < 0) retVal += 0x7fffffff;
        SeedArray[locINext] = retVal;
        inext = locINext;
        inextp = locINextp;
        return retVal;
    }

public:
    explicit Random(int Seed) {
        int ii;
        int mj, mk;
        int subtraction = (Seed == INT32_MIN) ? INT32_MAX : (Seed < 0 ? -Seed : Seed);
        mj = 161803398 - subtraction;
        SeedArray[55] = mj;
        mk = 1;
        for (int i = 1; i < 55; i++) {
            ii = (21 * i) % 55;
            SeedArray[ii] = mk;
            mk = mj - mk;
            if (mk < 0) mk += 0x7fffffff;
            mj = SeedArray[ii];
        }
        for (int k = 1; k < 5; k++) {
            for (int i = 1; i < 56; i++) {
                SeedArray[i] -= SeedArray[1 + (i + 30) % 55];
                if (SeedArray[i] < 0) SeedArray[i] += 0x7fffffff;
            }
        }
        inext = 0;
        inextp = 21;
    }
    double Sample() { return InternalSample() * (1.0 / 0x7fffffff); }
    int Next() { return InternalSample(); }
    int Next(int maxValue) { return (int)(Sample() * maxValue); }
    int Next(int minValue, int maxValue) {
        long long range = (long long)maxValue - minValue;
        return (int)(Sample() * range) + minValue;
    }
    double NextDouble() { return Sample(); }
};

// ---------------------------------------------------------------------------
// Enum operators: C# allows enum +/- int, ++/--, and bitwise ops on enums.
// Applied to every enum type in namespaces that `using namespace cs::enum_ops`.
// ---------------------------------------------------------------------------
namespace enum_ops {
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
constexpr E operator|(E a, E b) { using U = std::underlying_type_t<E>; return (E)((U)a | (U)b); }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
constexpr E operator&(E a, E b) { using U = std::underlying_type_t<E>; return (E)((U)a & (U)b); }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
constexpr E operator^(E a, E b) { using U = std::underlying_type_t<E>; return (E)((U)a ^ (U)b); }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
constexpr E operator~(E a) { using U = std::underlying_type_t<E>; return (E)(~(U)a); }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
E& operator|=(E& a, E b) { return a = a | b; }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
E& operator&=(E& a, E b) { return a = a & b; }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
E& operator^=(E& a, E b) { return a = a ^ b; }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
constexpr E operator+(E a, int b) { return (E)((int)a + b); }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
constexpr E operator-(E a, int b) { return (E)((int)a - b); }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
constexpr int operator-(E a, E b) { return (int)a - (int)b; }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
E& operator++(E& a) { return a = (E)((int)a + 1); }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
E operator++(E& a, int) { E o = a; a = (E)((int)a + 1); return o; }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
E& operator--(E& a) { return a = (E)((int)a - 1); }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
E operator--(E& a, int) { E o = a; a = (E)((int)a - 1); return o; }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
E& operator+=(E& a, int b) { return a = a + b; }
template <class E, std::enable_if_t<std::is_enum<E>::value, int> = 0>
E& operator-=(E& a, int b) { return a = a - b; }
}  // namespace enum_ops

}  // namespace cs

// Most translated files do `using namespace cs;` at namespace scope.

namespace cs {
// Deterministic-run hook: when non-zero, every place coab seeds System.Random
// from DateTime.Now.Ticks uses this value instead (see seg051::Randomize and
// DaxBlock). Set by the host for replay/diff testing.
struct Determinism {
    inline static int64_t fixedSeedTicks = 0;
    // Virtual clock for the differential-test harness (see cs/io.h DateTime::Now).
    inline static bool virtualTime = false;
    inline static int64_t virtualTicks = 630000000000000000LL;
    static int SeedFromTicks(int64_t ticks) { return (int)(fixedSeedTicks != 0 ? fixedSeedTicks : ticks); }
};
}  // namespace cs

// ===========================================================================
// ADDED BY Classes-headers translator (phase 1).
// Array<bool> specialisation: the generic Array<T> stores std::vector<T>, and
// std::vector<bool> cannot hand out `bool&`, so `flags[i] = true` (gbl.*
// bool[] tables, GasCloud.present, many engine locals) failed to compile.
// This keeps the same interface but stores std::deque<bool> (real bool refs).
// ===========================================================================
namespace cs {
template <>
class Array<bool> {
    std::shared_ptr<std::deque<bool>> p;

public:
    Array() = default;
    Array(std::nullptr_t) {}
    explicit Array(int n) : p(std::make_shared<std::deque<bool>>((std::size_t)(n < 0 ? 0 : n), false)) {}
    Array(std::initializer_list<bool> il) : p(std::make_shared<std::deque<bool>>(il)) {}
    explicit Array(const std::vector<bool>& v) : p(std::make_shared<std::deque<bool>>(v.begin(), v.end())) {}

    bool IsNull() const { return !p; }
    explicit operator bool() const { return (bool)p; }
    friend bool operator==(const Array& a, std::nullptr_t) { return !a.p; }
    friend bool operator!=(const Array& a, std::nullptr_t) { return (bool)a.p; }
    friend bool operator==(const Array& a, const Array& b) { return a.p == b.p; }
    friend bool operator!=(const Array& a, const Array& b) { return a.p != b.p; }

    int Length() const { return p ? (int)p->size() : throw NullReferenceException("Array.Length"); }
    bool& operator[](int i) const {
        if (!p) throw NullReferenceException("Array[]");
        if (i < 0 || i >= (int)p->size()) throw IndexOutOfRangeException();
        return (*p)[(std::size_t)i];
    }
    Array Clone() const {
        Array r;
        if (p) r.p = std::make_shared<std::deque<bool>>(*p);
        return r;
    }
    std::deque<bool>::iterator begin() const { return p->begin(); }
    std::deque<bool>::iterator end() const { return p->end(); }

    static void Copy(const Array& src, int si, const Array& dst, int di, int n) {
        if (n <= 0) return;
        if (si < 0 || di < 0 || si + n > src.Length() || di + n > dst.Length()) throw ArgumentException("Array.Copy");
        std::deque<bool> tmp(src.p->begin() + si, src.p->begin() + si + n);
        std::copy(tmp.begin(), tmp.end(), dst.p->begin() + di);
    }
    static void Copy(const Array& src, const Array& dst, int n) { Copy(src, 0, dst, 0, n); }
    static void Clear(const Array& a, int start, int n) {
        for (int i = start; i < start + n; i++) a[i] = false;
    }
    template <class P>
    static bool Exists(const Array& a, P pred) { return std::any_of(a.begin(), a.end(), pred); }
    template <class P>
    static int FindIndex(const Array& a, P pred) {
        for (int i = 0; i < a.Length(); i++) if (pred(a[i])) return i;
        return -1;
    }
    template <class F>
    static void ForEach(const Array& a, F f) { for (auto& x : a) f(x); }
};
}  // namespace cs
// ===========================================================================

#!/usr/bin/env python3
"""Move non-trivial 'inline static T x = init;' members out of headers into one .cpp so the
initialisation code exists once, not in every translation unit that includes the header."""
import re, sys, os

SCALAR = re.compile(r'^(const\s+)?(int|bool|byte|sbyte|short|ushort|uint|long|ulong|char|float|double|size_t|int64_t|uint64_t|int32_t|uint32_t|u8|u16|u32)\s*$')

def scan(text):
    """yield (start, end) of 'inline static ... ;' statements and the scope stack there"""
    i = 0; n = len(text); stack = []  # (name, kind, depth)
    depth = 0
    out = []
    pending = None
    while i < n:
        c = text[i]
        if text.startswith('//', i):
            j = text.find('\n', i); i = n if j < 0 else j; continue
        if text.startswith('/*', i):
            j = text.find('*/', i); i = n if j < 0 else j + 2; continue
        if c == '"' or c == "'":
            q = c; i += 1
            while i < n and text[i] != q:
                i += 2 if text[i] == '\\' else 1
            i += 1; continue
        if c == '{':
            # what opened it?
            head = text[max(0, text.rfind(';', 0, i), text.rfind('}', 0, i), text.rfind('{', 0, i)) + 1:i]
            head = re.sub(r'//[^\n]*|/\*.*?\*/', ' ', head, flags=re.S)
            m = re.search(r'\bnamespace\s+(\w+)\s*$', head.strip()) 
            m2 = re.search(r'\b(struct|class)\s+(\w+)\s*(final\s*)?(:[^{]*)?$', head.strip())
            if m: stack.append((m.group(1), 'ns', depth))
            elif m2 and '(' not in head.split(m2.group(2))[0][-1:]: stack.append((m2.group(2), 'cls', depth))
            depth += 1; i += 1; continue
        if c == '}':
            depth -= 1
            while stack and stack[-1][2] >= depth: stack.pop()
            i += 1; continue
        m = re.compile(r'(inline\s+static|static\s+inline)\b').match(text, i)
        if m and (i == 0 or not (text[i-1].isalnum() or text[i-1] == '_')):
            # statement end: ; at brace/paren depth 0
            j = m.end(); d = 0
            while j < n:
                ch = text[j]
                if text.startswith('//', j):
                    j = text.find('\n', j); continue
                if text.startswith('/*', j):
                    j = text.find('*/', j) + 2; continue
                if ch == '"' or ch == "'":
                    q = ch; j += 1
                    while text[j] != q: j += 2 if text[j] == '\\' else 1
                    j += 1; continue
                if ch in '{(': d += 1
                elif ch in '})': d -= 1
                elif ch == ';' and d == 0: break
                j += 1
            out.append((i, j + 1, list(stack), m.end()))
            i = j + 1; continue
        i += 1
    return out

def split_decl(body):
    """body = text between 'inline static' and ';'. returns type, name, init (including '=' or braces) or None"""
    b = re.sub(r'//[^\n]*', '', body)
    b = re.sub(r'/\*.*?\*/', '', b, flags=re.S).strip()
    ang = 0; k = 0
    while k < len(b):
        ch = b[k]
        if ch == '<': ang += 1
        elif ch == '>': ang -= 1
        elif ang == 0 and ch in '={(':
            break
        k += 1
    head = b[:k].strip(); init = b[k:].strip()
    m = re.match(r'^(.*?)(\w+)\s*(\[[^\]]*\])?$', head, flags=re.S)
    if not m: return None
    return m.group(1).strip(), m.group(2), init, m.group(3)

def main(files, out_cpp, includes):
    defs = []
    for path in files:
        fdefs = []
        text = open(path).read()
        stmts = scan(text)
        new = text
        for (s, e, stack, mend) in reversed(stmts):
            body = text[mend:e - 1]
            if 'constexpr' in body: continue
            r = split_decl(body)
            if not r: print('SKIP (unparsed)', path, body[:60].replace('\n', ' ')); continue
            typ, name, init, arr = r
            if arr: print('SKIP (array)', path, name); continue
            if SCALAR.match(typ) and re.match(r'^=\s*(-?[0-9][0-9a-fA-FxX.\'uUlL]*|true|false|nullptr|[A-Za-z_:]+::[A-Za-z_]+)\s*$', init or '= 0'):
                continue   # literal scalar: constant-initialised, no code
            if SCALAR.match(typ) and not init: continue
            if re.search(r'\bPoint\s*$', typ) is None and typ.endswith('*'): continue
            if typ.startswith('const ') or ' const' in typ and False: pass
            qual = '::'.join(n for n, k, _ in stack)
            if not stack or stack[-1][1] != 'cls': print('SKIP (no class)', path, name); continue
            # find trailing comment on the same line to preserve
            decl = 'static ' + typ + ' ' + name + ';'
            new = new[:s] + decl + new[e:]
            raw_init = re.sub(r'/\*.*?\*/', '', body, flags=re.S)
            # take init text from the original body but keep line comments out
            i0 = body.find(init[:1], body.find(name)) if init else 0
            init_txt = body[body.find(name, 0) + len(name):]
            init_txt = re.sub(r'//[^\n]*', '', init_txt).strip()
            fdefs.append((path, f'{typ} {qual}::{name} {init_txt};'))
        defs.extend(reversed(fdefs))
        if new != text:
            open(path, 'w').write(new)
    with open(out_cpp, 'w') as f:
        f.write('// Generated once by tools/outline_statics.py (kept in the tree, edit freely).\n'
                '// The non-trivial "inline static" tables used to live in the headers, so every translation unit\n'
                '// that includes them carried its own copy of their initialisation code (about 5 MB of the 3DS\n'
                '// code segment). They are defined here instead, once.\n')
        for inc in includes: f.write('#include "%s"\n' % inc)
        f.write('\n')
        for p, d in defs: f.write('// %s\n%s\n\n' % (os.path.basename(p), d))
    print('moved', len(defs))

if __name__ == '__main__':
    out = sys.argv[1]; incs = sys.argv[2].split(','); main(sys.argv[3:], out, incs)

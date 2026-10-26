#!/usr/bin/env python3
"""Patch a pristine coab checkout so it can run the goldbox-3ds differential
test harness.  Usage:  python3 apply.py <path-to-coab>

Changes (all behaviour-neutral unless Main.exe is started with --harness):
  * adds Classes/Harness.cs (copied from this folder) to the Classes project
  * seg049: keyboard reads/polls go through Harness when enabled; SysDelay skips
  * seg051: every Random/Random__Real call is traced; Randomize uses the fixed seed
  * DaxBlock: its static Random is seeded lazily via Harness.SeedFromTicks
  * every DateTime.Now in engine/Classes -> Classes.Harness.Now (virtual clock)
  * every Thread.Sleep in engine -> Classes.Harness.Sleep
  * Main/Program.cs: `Main.exe --harness keys.txt --trace out.txt --seed N`
"""
import os
import re
import shutil
import sys

root = sys.argv[1]
here = os.path.dirname(os.path.abspath(__file__))


def rd(p):
    with open(os.path.join(root, p), encoding="latin-1", newline="") as f:
        return f.read()


def wr(p, s):
    with open(os.path.join(root, p), "w", encoding="latin-1", newline="") as f:
        f.write(s)


def sub(p, old, new, count=1):
    s = rd(p)
    if old not in s:
        if new in s:
            return  # already applied
        raise SystemExit(f"{p}: pattern not found:\n{old}")
    wr(p, s.replace(old, new, count))


# 1. Harness.cs into the Classes project
shutil.copy(os.path.join(here, "Harness.cs"), os.path.join(root, "Classes", "Harness.cs"))
proj = rd("Classes/Classes.csproj")
if 'Include="Harness.cs"' not in proj:
    proj = proj.replace('<Compile Include="Action.cs">', '<Compile Include="Harness.cs" />\r\n    <Compile Include="Action.cs">', 1)
    wr("Classes/Classes.csproj", proj)

# 2. seg049 keyboard
sub("engine/seg049.cs", "System.Threading.Thread.Sleep(milliseconds);", "Classes.Harness.Sleep(milliseconds);")
s = rd("engine/seg049.cs")
if "Harness.CheckKey" not in s:
    s = re.sub(r"(static ushort int_check_keyPressed\(\)\s*\{)", r"\1\r\n            if (Classes.Harness.enabled) return Classes.Harness.CheckKey();", s, count=1)
    s = re.sub(r"(static ushort int_get_keyPressed\(\)\s*\{[^\n]*\n(?:\s*//[^\n]*\n)*\s*ushort key = 0;)",
               r"\1\r\n            if (Classes.Harness.enabled) return Classes.Harness.GetKey();", s, count=1)
    wr("engine/seg049.cs", s)
assert "Harness.CheckKey" in rd("engine/seg049.cs") and "Harness.GetKey" in rd("engine/seg049.cs")

# 3. seg051 RNG
sub("engine/seg051.cs", "return (byte)(random_number.Next() % arg_0);",
    "int raw = random_number.Next(); if (Classes.Harness.enabled) Classes.Harness.OnRandom(arg_0, raw); return (byte)(raw % arg_0);")
sub("engine/seg051.cs", "return random_number.Next() % arg_0;",
    "int raw = random_number.Next(); if (Classes.Harness.enabled) Classes.Harness.OnRandom(arg_0, raw); return raw % arg_0;")
sub("engine/seg051.cs", "return random_number.NextDouble();",
    "double raw = random_number.NextDouble(); if (Classes.Harness.enabled) Classes.Harness.OnRandomReal(raw); return raw;")
sub("engine/seg051.cs", "random_number = new System.Random(unchecked((int)System.DateTime.Now.Ticks));",
    "random_number = new System.Random(Classes.Harness.SeedFromTicks(Classes.Harness.Now.Ticks));")

# 4. DaxBlock lazy RNG
sub("Classes/DaxFiles/DaxBlock.cs",
    "static Random random_number = new System.Random(unchecked((int)System.DateTime.Now.Ticks));",
    "static Random random_number_;\r\n        static Random random_number { get { if (random_number_ == null) random_number_ = new System.Random(Classes.Harness.SeedFromTicks(Classes.Harness.Now.Ticks)); return random_number_; } }")

# 5/6. Virtual clock + sleeps everywhere else
for folder in ("engine", "Classes"):
    for dirpath, _, files in os.walk(os.path.join(root, folder)):
        for fn in files:
            if not fn.endswith(".cs") or fn == "Harness.cs":
                continue
            rel = os.path.relpath(os.path.join(dirpath, fn), root)
            s = rd(rel)
            t = re.sub(r"(?<![\w.])(System\.)?DateTime\.Now\b", "Classes.Harness.Now", s)
            t = re.sub(r"(?<![\w.])(System\.)?(Threading\.)?Thread\.Sleep\(", "Classes.Harness.Sleep(", t)
            if t != s:
                wr(rel, t)

# 7. Main entry point
p = "Main/Program.cs"
s = rd(p)
if "RunHarness" not in s:
    # (the existing UnhandledException lambda already uses `args`, so name ours cmdArgs)
    s = s.replace("static void Main()", "static void Main(string[] cmdArgs)", 1)
    s = re.sub(r"(static void Main\(string\[\] cmdArgs\)\s*\{)",
               lambda m: m.group(1) + '\r\n            if (cmdArgs.Length > 0 && cmdArgs[0] == "--harness") { RunHarness(cmdArgs); return; }', s, count=1)
    runner = '''
        // goldbox-3ds differential-test harness entry point.
        static void RunHarness(string[] args)
        {
            string script = null, tracePath = "trace.txt";
            int seed = 12345;
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == "--harness") script = args[++i];
                else if (args[i] == "--trace") tracePath = args[++i];
                else if (args[i] == "--seed") seed = int.Parse(args[++i]);
            }
            Config.Setup();
            Logger.SetExitFunc(engine.seg043.print_and_exit);
            Classes.Harness.Load(script, tracePath, seed);
            try
            {
                engine.seg001.__SystemInit(delegate () { });
                engine.seg001.PROGRAM();
            }
            catch (Classes.HarnessAbort)
            {
            }
            catch (System.Threading.ThreadAbortException)
            {
                System.Threading.Thread.ResetAbort();
            }
            Classes.Harness.Close();
        }
'''
    idx = s.index("public delegate void VoidDelegate();")
    s = s[:idx] + runner.lstrip("\n").replace("\n", "\r\n") + "\r\n        " + s[idx:]
    wr(p, s)

print("coab patched for the goldbox-3ds harness:", root)

// Harness.cs - differential-testing harness for coab (goldbox-3ds tooling).
// Added to coab's Classes project by tools/coab-harness/apply.py; the entry
// point lives in the patched Main/Program.cs (RunHarness).
// Must stay in lock-step with goldbox-3ds src/engine/harness.cpp: for the same
// seed and key script, both programs must write byte-identical traces.
using System;
using System.Collections.Generic;
using System.IO;
using Classes;

namespace Classes
{
    public class HarnessAbort : Exception { }

    public static class Harness
    {
        class Item { public bool idle; public int count; public ushort key; }

        public static bool enabled = false;
        public static int seed = 0;
        static LinkedList<Item> script = new LinkedList<Item>();
        static TextWriter trace;
        static long randomCount = 0;
        static long keyCount = 0;
        static long maxRandoms = -1;
        static bool verbose = false;
        static long virtualTicks = 630000000000000000L;

        // Virtual clock: +10 ms per call (cs::DateTime::Now in the C++ build).
        public static DateTime Now
        {
            get
            {
                if (!enabled) return DateTime.Now;
                virtualTicks += 100000;
                return new DateTime(virtualTicks);
            }
        }

        public static int SeedFromTicks(long ticks)
        {
            return enabled ? seed : unchecked((int)ticks);
        }

        public static void Sleep(int ms)
        {
            if (!enabled) System.Threading.Thread.Sleep(ms);
        }

        static ushort ParseKey(string s)
        {
            switch (s)
            {
                case "enter": return 0x1C0D;
                case "esc": return 0x001B;
                case "space": return 0x0020;
                case "up": return 0x4800;
                case "down": return 0x5000;
                case "left": return 0x4B00;
                case "right": return 0x4D00;
                case "home": return 0x4700;
                case "end": return 0x4F00;
                case "pgup": return 0x4900;
                case "pgdn": return 0x5100;
                case "bksp": return 0x0008;
                case "del": return 0x5300;
                case "minus": return 0x2D00;
                case "comma": return 0x002C;
                case "period": return 0x002E;
            }
            if (s.Length > 2 && s[0] == '0' && s[1] == 'x') return Convert.ToUInt16(s.Substring(2), 16);
            if (s.Length == 1)
            {
                char c = s[0];
                if (c >= 'a' && c <= 'z') c = (char)(c - 32);
                return (ushort)c;
            }
            return 0x0020;
        }

        public static bool Load(string scriptPath, string tracePath, int fixedSeed)
        {
            seed = fixedSeed;
            foreach (string raw in System.IO.File.ReadAllLines(scriptPath))
            {
                string[] parts = raw.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length == 0 || parts[0].StartsWith("#")) continue;
                string arg = parts.Length > 1 ? parts[1] : "";
                if (parts[0] == "key") script.AddLast(new Item { idle = false, key = ParseKey(arg) });
                else if (parts[0] == "idle") script.AddLast(new Item { idle = true, count = int.Parse(arg) });
                else if (parts[0] == "verbose") verbose = true;
                else if (parts[0] == "randoms") maxRandoms = long.Parse(arg);
                else if (parts[0] == "stop") break;
            }
            trace = new StreamWriter(tracePath, false);
            ((StreamWriter)trace).NewLine = "\n";
            enabled = true;
            return true;
        }

        public static void Close()
        {
            if (trace != null) trace.Close();
            trace = null;
        }

        static void Line(string s)
        {
            if (trace != null) trace.WriteLine(s);
        }

        static uint Fnv(byte[] a)
        {
            if (a == null) return 0;
            uint h = 2166136261u;
            foreach (byte b in a) { h ^= b; h *= 16777619u; }
            return h;
        }

        static uint TeamHash()
        {
            if (gbl.TeamList == null) return 0;
            uint h = 2166136261u;
            foreach (Player p in gbl.TeamList)
            {
                h ^= Fnv(p.ToByteArray()); h *= 16777619u;
                h ^= (uint)(p.in_combat ? 1 : 0); h *= 16777619u;
            }
            return h;
        }

        static string Verbose()
        {
            if (!verbose) return "";
            return string.Format(" s={0:X8}/{1}", TeamHash(), gbl.TeamList == null ? -1 : gbl.TeamList.Count);
        }

        static uint ScreenHash()
        {
            uint h = 2166136261u;
            for (int y = 0; y < 200; y++)
                for (int x = 0; x < 320; x++) { h ^= Display.GetPixel(x, y); h *= 16777619u; }
            return h;
        }

        static void DumpState(ushort key)
        {
            string s = string.Format("K {0} {1:X4} pos={2},{3},{4} ecl={5:X4} area1={6:X8} area2={7:X8} screen={8:X8} team={9}",
                keyCount, key, gbl.mapPosX, gbl.mapPosY, (int)gbl.mapDirection, (int)gbl.ecl_offset,
                gbl.area_ptr == null ? 0u : Fnv(gbl.area_ptr.ToByteArray()),
                gbl.area2_ptr == null ? 0u : Fnv(gbl.area2_ptr.ToByteArray()), ScreenHash(),
                gbl.TeamList == null ? -1 : gbl.TeamList.Count);
            if (gbl.TeamList != null)
            {
                foreach (Player p in gbl.TeamList)
                {
                    s += string.Format(" p={0:X8}/{1}/{2}", Fnv(p.ToByteArray()), p.items.Count, p.affects.Count);
                }
            }
            Line(s);
            trace.Flush();
        }

        // seg049.int_check_keyPressed
        public static ushort CheckKey()
        {
            while (script.Count > 0 && script.First.Value.idle && script.First.Value.count <= 0) script.RemoveFirst();
            if (script.Count == 0) throw new HarnessAbort();
            if (script.First.Value.idle)
            {
                script.First.Value.count--;
                return 0;
            }
            return script.First.Value.key;
        }

        // seg049.int_get_keyPressed
        public static ushort GetKey()
        {
            while (script.Count > 0 && script.First.Value.idle) script.RemoveFirst();
            if (script.Count == 0)
            {
                Line("END");
                throw new HarnessAbort();
            }
            ushort k = script.First.Value.key;
            script.RemoveFirst();
            keyCount++;
            DumpState(k);
            return k;
        }

        public static void OnRandom(int arg, int raw)
        {
            Line(string.Format("R {0} {1} {2}", ++randomCount, arg, raw) + Verbose());
            if (maxRandoms >= 0 && randomCount >= maxRandoms)
            {
                Line("END");
                throw new HarnessAbort();
            }
        }

        public static void OnRandomReal(double raw)
        {
            Line(string.Format("F {0} {1:X16}", ++randomCount, BitConverter.DoubleToInt64Bits(raw)) + Verbose());
            if (maxRandoms >= 0 && randomCount >= maxRandoms)
            {
                Line("END");
                throw new HarnessAbort();
            }
        }
    }
}

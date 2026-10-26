// engine/decl/seg051.h - declarations for engine/seg051.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg051.cpp.
// ---------------------------------------------------------------------------
// seg051.cs
// ---------------------------------------------------------------------------
struct seg051 {
    inline static cs::Random random_number{0}; // PORT-NOTE: C# is null until Randomize(); cs::Random has no default ctor. Spelled cs::Random because seg051::Random(...) methods hide the type name.
    static String Copy(int CopyLen, int StartAt, String InString);
    static byte Random(byte arg_0);
    static int Random(int arg_0);
    static double Random__Real();
    static void Randomize();
    static void Reset(File arg_4);
    static void Rewrite(File arg_2);
    static void Close(File arg_0);
    static int BlockRead(int count, Array<byte> data, File file);
    static void BlockWrite(int arg_4, Array<byte> arg_6, File arg_A);
    static void FillChar(byte fill_byte, int buffer_size, Array<byte> buffer);
};


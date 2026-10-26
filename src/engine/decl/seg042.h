// engine/decl/seg042.h - declarations for engine/seg042.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/seg042.cpp.
// ---------------------------------------------------------------------------
// seg042.cs
// ---------------------------------------------------------------------------
struct seg042 {
    static void debug_display(String text);
    static void delete_file(String fileString);
    static bool find_and_open_file(File& file_ptr, bool noError, String full_file_name);
    static bool file_find(String filePath);
    inline static Array<char> unk_16FA9 = { ' ', '.', '*', ',', '?', '/', '\\', ':', ';', '|' };
    static String clean_string(String s);
    static bool setupDaxFiles(IO::BinaryReader& fileA, IO::BinaryReader& fileB, short& arg_8, String file_name);
    static void load_decode_dax(Array<byte>& out_data, short& decodeSize, int block_id, String file_name);
    static void set_game_area(byte arg_0);
    static void restore_game_area();
};


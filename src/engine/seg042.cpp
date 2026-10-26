// engine/seg042.cpp - translation of coab engine/seg042.cs.
// Original C#: coab (c) Simeon Pilgrim, BSD-3. Translation: goldbox-3ds contributors.
#include "engine/engine.h"

namespace engine {

// PORT-NOTE: cs::String has no Trim(char[]); local helper with .NET semantics
// (strip any of the given chars from both ends).
static String TrimChars(const String& s, const Array<char>& chars)
{
    auto isTrim = [&](char c) {
        for (int i = 0; i < chars.Length(); i++) if (chars[i] == c) return true;
        return false;
    };
    int b = 0, e = s.Length();
    while (b < e && isTrim(s[b])) b++;
    while (e > b && isTrim(s[e - 1])) e--;
    return s.Substring(b, e - b);
}

void seg042::debug_display(String text)
{
    Logging::Logger::Log(text);
    seg043::GetInputKey();
}


void seg042::delete_file(String fileString)
{
    if (IO::File::Exists(fileString))
    {
        IO::File::Delete(fileString);
    }
}



bool seg042::find_and_open_file(File& file_ptr, bool noError, String full_file_name)
{
    String file_name = IO::Path::GetFileName(full_file_name);
    String dir_path = IO::Path::GetDirectoryName(full_file_name);

    if (dir_path.Length() == 0)
    {
        dir_path = gbl::exe_path;
    }

    bool file_found;

    file_found = IO::File::Exists(IO::Path::Combine(dir_path, file_name));

    if (file_found == false && noError == false)
    {
        debug_display(String("Couldn't find ") + file_name + ". Check install.");
    }

    if (file_found == true)
    {
        file_ptr = New<File_>();
        file_ptr->Assign(IO::Path::Combine(dir_path, file_name));

        seg051::Reset(file_ptr);
    }
    else
    {
        file_ptr = nullptr;
    }

    return file_found;
}


bool seg042::file_find(String filePath)
{
    return IO::File::Exists(filePath);
}


String seg042::clean_string(String s)
{
    String cleanStr = TrimChars(s, unk_16FA9).ToLower();

    if (cleanStr.Length() > 8)
    {
        cleanStr = cleanStr.Substring(0, 8);
    }

    return cleanStr;
}


bool seg042::setupDaxFiles(IO::BinaryReader& fileA, IO::BinaryReader& fileB, short& arg_8, String file_name)
{
    fileA = nullptr;
    fileB = nullptr;
    arg_8 = 0;

    if (IO::File::Exists(file_name) == false)
    {
        /*TODO Add message about missing file here.*/
        return false;
    }

    try
    {
        IO::FileStream fsA = New<IO::FileStream_>(file_name, IO::FileMode::Open, IO::FileAccess::Read, IO::FileShare::Read);
        IO::FileStream fsB = New<IO::FileStream_>(file_name, IO::FileMode::Open, IO::FileAccess::Read, IO::FileShare::Read);

        fileA = New<IO::BinaryReader_>(fsA);
        fileB = New<IO::BinaryReader_>(fsB);
    }
    catch (const ApplicationException&)
    {
        /*TODO Add message about error here.*/
        return false;
    }

    arg_8 = fileA->ReadInt16();
    arg_8 += 2;

    fileB->BaseStream->Seek(arg_8, IO::SeekOrigin::Begin);
    return true;
}


void seg042::load_decode_dax(Array<byte>& out_data, short& decodeSize, int block_id, String file_name)
{
    seg044::PlaySound(Sound::sound_0);

    out_data = Classes::DaxFiles::DaxCache::LoadDax(file_name.ToLower(), block_id);
    decodeSize = out_data == nullptr ? (short)0 : (short)out_data.Length();
}


void seg042::set_game_area(byte arg_0)
{
    gbl::game_area_backup = gbl::game_area;
    gbl::game_area = arg_0;
}


void seg042::restore_game_area()
{
    gbl::game_area = gbl::game_area_backup;
}

}  // namespace engine

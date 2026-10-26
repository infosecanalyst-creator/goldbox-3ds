// Classes/DaxFiles/DaxFileCache.cpp - port of Classes/DaxFiles/DaxFileCache.cs
#include "Classes/Classes.h"

namespace Classes {
namespace DaxFiles {
using namespace cs;
using namespace cs::enum_ops;

DaxFileCache_::DaxFileCache_(String filename)
{
    entries = Dictionary<int, Array<byte>>();

    LoadFile(filename);
}

void DaxFileCache_::LoadFile(String filename)
{
    int dataOffset = 0;

    if (IO::File::Exists(filename) == false)
    {
        return;
    }

    IO::BinaryReader fileA;

    try
    {
        IO::FileStream fsA = New<IO::FileStream_>(filename, IO::FileMode::Open, IO::FileAccess::Read, IO::FileShare::Read);

        fileA = New<IO::BinaryReader_>(fsA);
    }
    catch (const ApplicationException&)
    {
        return;
    }

    dataOffset = fileA->ReadInt16() + 2;

    List<DaxHeaderEntry> headers = List<DaxHeaderEntry>();

    const int headerEntrySize = 9;

    for (int i = 0; i < ((dataOffset - 2) / headerEntrySize); i++)
    {
        DaxHeaderEntry dhe = New<DaxHeaderEntry_>();
        dhe->id = fileA->ReadByte();
        dhe->offset = fileA->ReadInt32();
        dhe->rawSize = fileA->ReadInt16();
        dhe->compSize = fileA->ReadUInt16();

        headers.Add(dhe);
    }

    for (DaxHeaderEntry dhe : headers)
    {
        Array<byte> comp = Array<byte>(dhe->compSize);
        Array<byte> raw = Array<byte>(dhe->rawSize);

        fileA->BaseStream->Seek(dataOffset + dhe->offset, IO::SeekOrigin::Begin);

        comp = fileA->ReadBytes(dhe->compSize);

        Decode(dhe->rawSize, dhe->compSize, raw, comp);

        entries.Add(dhe->id, raw);
    }

    fileA->Close();
}

void DaxFileCache_::Decode(int decodeSize, int dataLength, Array<byte> output_ptr, Array<byte> input_ptr)
{
    sbyte run_length;
    int output_index;
    int input_index;

    input_index = 0;
    output_index = 0;

    do
    {
        run_length = (sbyte)input_ptr[input_index];

        if (run_length >= 0)
        {
            for (int i = 0; i <= run_length; i++)
            {
                output_ptr[output_index + i] = input_ptr[input_index + i + 1];
            }

            input_index += run_length + 2;
            output_index += run_length + 1;
        }
        else
        {
            run_length = (sbyte)(-run_length);

            for (int i = 0; i < run_length; i++)
            {
                output_ptr[output_index + i] = input_ptr[input_index + 1];
            }

            input_index += 2;
            output_index += run_length;
        }
    } while (input_index < dataLength);
}

Array<byte> DaxFileCache_::GetData(int block_id)
{
    Array<byte> orig;
    if (entries.TryGetValue(block_id, orig) == false)
    {
        return nullptr;
    }

    return orig.Clone();
}

}  // namespace DaxFiles
}  // namespace Classes

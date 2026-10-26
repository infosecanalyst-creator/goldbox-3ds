#include "engine/engine.h"
#include "engine/harness.h"

namespace engine {

// static System.Random random_number;  -> declared in decl/seg051.h (cs::Random, see PORT-NOTE there)
/*static int random_number;*/


String seg051::Copy(int CopyLen, int StartAt, String InString)
{
    String OutString;

    if (CopyLen >= InString.Length() - StartAt)
    {
        CopyLen = InString.Length() - StartAt;
    }

    if (CopyLen > 0)
    {
        OutString = InString.Substring(StartAt, CopyLen);
    }
    else
    {
        OutString = String::Empty;
    }

    return OutString;
}

byte seg051::Random(byte arg_0)
{
    if (arg_0 == 0)
    {
        return 0;
    }

    int raw = random_number.Next();
    if (Harness::enabled) Harness::OnRandom(arg_0, raw);  // goldbox-3ds: diff harness
    return (byte)(raw % arg_0);
}

int seg051::Random(int arg_0)
{
    if (arg_0 == 0)
    {
        return 0;
    }

    int raw = random_number.Next();
    if (Harness::enabled) Harness::OnRandom(arg_0, raw);  // goldbox-3ds: diff harness
    return raw % arg_0;
}

double seg051::Random__Real()
{
    double raw = random_number.NextDouble();
    if (Harness::enabled) Harness::OnRandomReal(raw);  // goldbox-3ds: diff harness
    return raw;
}


void seg051::Randomize()
{
    // PORT-NOTE: seeded via Determinism::SeedFromTicks so deterministic test runs can fix the seed.
    random_number = cs::Random(Determinism::SeedFromTicks((DateTime::Now().Ticks)));
}


void seg051::Reset(File arg_4)
{
    arg_4->stream->Seek(0, IO::SeekOrigin::Begin);
}

void seg051::Rewrite(File arg_2)
{
    arg_2->stream->SetLength(0);
}


void seg051::Close(File arg_0)
{
    arg_0->stream->Close();
}

int seg051::BlockRead(int count, Array<byte> data, File file)
{
    return file->stream->Read(data, 0, count);
}


void seg051::BlockWrite(int arg_4, Array<byte> arg_6, File arg_A)
{
    arg_A->stream->Write(arg_6, 0, arg_4);
}

void seg051::FillChar(byte fill_byte, int buffer_size, Array<byte> buffer)
{
    for (int i = 0; i < buffer_size; i++)
    {
        buffer[i] = fill_byte;
    }
}

}  // namespace engine

// Classes/File.cpp - port of Classes/File.cs
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

File_::File_()
{
    // TODO tidy-up this pascal based concept.
}

void File_::Assign(String fileString)
{
    name = fileString;
    stream = IO::File::Open(fileString, IO::FileMode::OpenOrCreate);
}

}  // namespace Classes

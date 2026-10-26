// Classes/File.h - port of Classes/File.cs
#pragma once

#include "cs/cs.h"
#include "Classes/fwd.h"
#include "cs/io.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

/// <summary>
/// Summary description for File.
/// </summary>
class File_ {
public:
    File_();

    String name;

    IO::FileStream stream;

    void Assign(String fileString);
};

}  // namespace Classes

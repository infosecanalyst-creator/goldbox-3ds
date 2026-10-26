// Classes/MenuItem.cpp - port of Classes/MenuItem.cs
#include "Classes/Classes.h"

#include "cs/cs.h"
using namespace cs;
using namespace cs::enum_ops;

namespace Classes {

MenuItem_::MenuItem_()
{
    Heading = false;
    Text = String::Empty;
    Item = nullptr;
}

MenuItem_::MenuItem_(String text)
{
    Heading = false;
    Text = text;
    Item = nullptr;
}

MenuItem_::MenuItem_(String text, bool heading)
{
    Heading = heading;
    Text = text;
    Item = nullptr;
}

MenuItem_::MenuItem_(String text, Classes::Item item)
{
    Heading = false;
    Text = text;
    Item = item;
}

}  // namespace Classes

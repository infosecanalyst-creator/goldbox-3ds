// Classes/Gbl.cpp - port of the method bodies in Classes/Gbl.cs.
// Every gbl member is a static field initialised in Gbl.h, MenuColorSet and
// gbl::SetBlock have inline bodies in their headers; only Point's out-of-line
// methods live here.
#include "Classes/Classes.h"

namespace Classes {
using namespace cs;
using namespace cs::enum_ops;

void Point::MapBoundaryTrunc()
{
    x = Math::Max(Math::Min(x, MapMaxX - 1), MapMinX);
    y = Math::Max(Math::Min(y, MapMaxY - 1), MapMinY);
}

bool Point::MapInBounds() const
{
    return x < MapMaxX && x >= MapMinX && y < MapMaxY && y >= MapMinY;
}

String Point::ToString() const
{
    return Format("x: {0} y: {1}", x, y);
}

}  // namespace Classes

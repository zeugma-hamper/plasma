
/* (c)  oblong industries */

#include "AxisAlignedBox.h"
using namespace oblong::loam;

Vect AxisAlignedBox::CornerAt (Vertex v) const
{
  return NthCorner (static_cast<int32> (v));
}

Vect AxisAlignedBox::NthCorner (int32 i) const
{
  if (i < 0 || i > 7)
    OB_LOG_BUG ("invalid corner index");
  return Vect (((i >> 0) & 1) ? minimum.x : maximum.x,
               ((i >> 1) & 1) ? minimum.y : maximum.y,
               ((i >> 2) & 1) ? minimum.z : maximum.z);
}

std::array<Vect, 8> AxisAlignedBox::Corners () const
{
  std::array<Vect, 8> ret = {{NthCorner (0), NthCorner (1), NthCorner (2),
                              NthCorner (3), NthCorner (4), NthCorner (5),
                              NthCorner (6), NthCorner (7)}};

  return ret;
}

std::array<Plane, 6> AxisAlignedBox::Planes () const
{
  std::array<Plane, 6> ret = {{
    Plane (maximum, Vect (-1.0, 0.0, 0.0)),
    Plane (maximum, Vect (0.0, -1.0, 0.0)),
    Plane (maximum, Vect (0.0, 0.0, -1.0)),
    Plane (minimum, Vect (1.0, 0.0, 0.0)),
    Plane (minimum, Vect (0.0, 1.0, 0.0)),
    Plane (minimum, Vect (0.0, 0.0, 1.0)),
  }};
  return ret;
}

Str AxisAlignedBox::AsStr () const
{
  return "AxisAlignedBox[max=" + maximum.AsStr () + ", min=" + minimum.AsStr ()
         + "]";
}

bool AxisAlignedBox::IsValid () const
{
  return maximum.IsValid () && minimum.IsValid ();
}

AxisAlignedBox AxisAlignedBox::Invalid ()
{
  return AxisAlignedBox ({Vect::Invalid ()});
}

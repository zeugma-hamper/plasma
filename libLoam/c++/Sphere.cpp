
/* (c)  oblong industries */

#include "Sphere.h"

using namespace oblong::loam;
Sphere::Sphere (const Vect &center_, float64 radius_)
    : center (center_), radius (radius_)
{
}

Vect Sphere::Center () const
{
  return center;
}

float64 Sphere::Radius () const
{
  return radius;
}

Str Sphere::AsStr () const
{
  return "Sphere[c=" + center.AsStr () + Str ().Sprintf (", r=%f]", radius);
}

bool Sphere::IsValid () const
{
  return center.IsValid () && radius >= 0;
}

Sphere Sphere::Invalid ()
{
  return Sphere (Vect::Invalid (), OB_NAN);
}

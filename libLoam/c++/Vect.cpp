
/* (c)  oblong industries */

#include "libLoam/c/ob-rand.h"
#include <libLoam/c++/ob-math-utils.h>
#include <libLoam/c++/Vect.h>
#include <libLoam/c++/Str.h>
#include <libLoam/c++/LoamStreams.h>

#include <ostream>


using namespace oblong::loam;


Vect &Vect::UnitRandomize ()
{
  // normal deviates ensure uniform spherical distribution
  // note: not thread-safe due to global rand state
  x = ob_rand_normal (&y);
  z = ob_rand_normal ();
  return NormSelf ();
}


Str Vect::AsStr () const
{
  return Str ().Sprintf ("v3f64(%f, %f, %f)", x, y, z);
}

void Vect::SpewToStderr () const
{
  fprintf (stderr, "%s", AsStr ().utf8 ());
}

Vect Vect::Rotate (const Vect &axis, float64 angle) const
{
  float64 sina, cosa, n1, n2, n3, n1sq, n2sq, n3sq, C1;
  float64 r[3][3];

  Vect av = axis.Norm ();
  if (av.Dot (av) == 0.0)
    {
      return *this;
    }

  sina = sin (angle);
  cosa = cos (angle);
  n1 = av.x;
  n2 = av.y;
  n3 = av.z;
  n1sq = n1 * n1;
  n2sq = n2 * n2;
  n3sq = n3 * n3;
  C1 = (1.0 - cosa);

  r[0][0] = n1sq + (1.0 - n1sq) * cosa;
  r[0][1] = n1 * n2 * C1 + n3 * sina;
  r[0][2] = n1 * n3 * C1 - n2 * sina;

  r[1][0] = n1 * n2 * C1 - n3 * sina;
  r[1][1] = n2sq + (1.0 - n2sq) * cosa;
  r[1][2] = n2 * n3 * C1 + n1 * sina;

  r[2][0] = n1 * n3 * C1 + n2 * sina;
  r[2][1] = n2 * n3 * C1 - n1 * sina;
  r[2][2] = n3sq + (1.0 - n3sq) * cosa;

  av.Set (x * r[0][0] + y * r[1][0] + z * r[2][0],
          x * r[0][1] + y * r[1][1] + z * r[2][1],
          x * r[0][2] + y * r[1][2] + z * r[2][2]);

  return av;
}


namespace oblong {
namespace loam {

::std::ostream &operator<< (::std::ostream &os, const oblong::loam::Vect &v)
{
  return os << v.AsStr ();
}
}
}  // end oblong::loam

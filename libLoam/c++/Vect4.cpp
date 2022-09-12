
/* (c)  oblong industries */

#include <libLoam/c++/Vect4.h>
#include <libLoam/c++/Str.h>
#include <libLoam/c++/LoamStreams.h>

#include <ostream>


using namespace oblong::loam;


Str Vect4::AsStr () const
{
  return Str ().Sprintf ("v4f64(%f, %f, %f, %f)", x, y, z, w);
}

void Vect4::SpewToStderr () const
{
  fprintf (stderr, "%s", AsStr ().utf8 ());
}


namespace oblong {
namespace loam {

::std::ostream &operator<< (::std::ostream &os, const oblong::loam::Vect4 &v)
{
  return os << v.AsStr ();
}
}
}  // end oblong::loam


/* (c)  oblong industries */

#include <libLoam/c++/ObColor.h>
#include <libLoam/c++/Str.h>
#include <libLoam/c++/LoamStreams.h>

#include <math.h>

#include <ostream>


using namespace oblong::loam;


const ObColor &ObColor::SetHSB (float64 h_in, float64 s_in, float64 b_in)
{
  float64 hsb[3] = {h_in, s_in, b_in};
  float64 rgb[3] = {0, 0, 0};

  if (hsb[1] == 0)
    rgb[0] = rgb[1] = rgb[2] = hsb[2];
  else
    {
      float64 h = (hsb[0] - floor (hsb[0])) * 6.0;
      float64 f = h - floor (h);
      float64 p = hsb[2] * (1.0 - hsb[1]);
      float64 q = hsb[2] * (1.0 - hsb[1] * f);
      float64 t = hsb[2] * (1.0 - (hsb[1] * (1.0 - f)));

      switch ((int) h)
        {
          case 0:
            rgb[0] = hsb[2];
            rgb[1] = t;
            rgb[2] = p;
            break;
          case 1:
            rgb[0] = q;
            rgb[1] = hsb[2];
            rgb[2] = p;
            break;
          case 2:
            rgb[0] = p;
            rgb[1] = hsb[2];
            rgb[2] = t;
            break;
          case 3:
            rgb[0] = p;
            rgb[1] = q;
            rgb[2] = hsb[2];
            break;
          case 4:
            rgb[0] = t;
            rgb[1] = p;
            rgb[2] = hsb[2];
            break;
          case 5:
            rgb[0] = hsb[2];
            rgb[1] = p;
            rgb[2] = q;
            break;
        }
    }

  return Set (rgb[0], rgb[1], rgb[2]);
}


Str ObColor::AsStr () const
{
  return Str::Format ("ObColor(%f, %f, %f, %f)", r, g, b, a);
}


namespace oblong {
namespace loam {

::std::ostream &operator<< (::std::ostream &os, const ObColor &c)
{
  return os << c.AsStr ();
}
}
}  // end oblong::loam

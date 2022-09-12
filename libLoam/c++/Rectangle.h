
/* (c)  oblong industries */

#pragma once

#include "libLoam/c++/Str.h"
#include "libLoam/c++/Vect.h"

namespace oblong {
namespace loam {

/**
 * Use this type to represent a rectangle's geometry. In fact, this type can
 * hold any parallelogram, but we expect you're only going to store parallelograms
 * with right angles.
 */
class OB_LOAMXX_API Rectangle
{
 public:
  /**
   * You are advised to read but not alter these public fields.
   */
  Vect corner;
  Vect leg1;
  Vect leg2;

  /**
     Construct a Rectangle from a given corner location, and two vectors
     that determine the direction and length of the parallel sides.

          o------------------------o
         /                        /
        /                        / \
       /         [leg1]         /   [leg2]
      /         /              /
     o------------------------o
     \
     [corner]

   */
  Rectangle (Vect corner, Vect leg1, Vect leg2);
  Rectangle () = default;

  Str AsStr () const;

  bool IsValid () const;

  static Rectangle Invalid ();

};  // Rectangle

}  // end namespace loam
}  // end namespace oblong

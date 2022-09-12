
/* (c)  oblong industries */

#pragma once

#include "libLoam/c++/Str.h"
#include "libLoam/c++/Vect.h"

namespace oblong {
namespace loam {

/**
 *  Sphere represents a simple bounding sphere
 */
class OB_LOAMXX_API Sphere
{
 public:
  /**
   * You are advised to read but not alter these public fields.
   */
  Vect center;
  float64 radius;

  Sphere () = default;
  Sphere (const Vect &center, float64 radius);

  /**
   *  Retrieve the center of the sphere.
   */
  Vect Center () const;

  /**
   *  Retrieve the radius of the sphere.
   */
  float64 Radius () const;

  Str AsStr () const;

  bool IsValid () const;

  static Sphere Invalid ();

};  // sphere

}  // end namespace loam
}  // end namespace oblong

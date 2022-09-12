
/* (c)  oblong industries */

#ifndef __PLANE3_H__
#define __PLANE3_H__


#include <libLoam/c++/Vect.h>


namespace oblong {
namespace loam {


/**
 * Plane represents a plane in 3-space defined by a point on the
 * plane and a normal.
 */
class OB_LOAMXX_API Plane
{
 public:
  Plane ();

  Plane (const Vect &point, const Vect &normal);

  // relying on the compiler for copy and move constructors and assignments

  const Vect &Point () const { return point; }
  const Vect &Normal () const { return norm; }

  bool IsValid () const;

  static Plane Invalid ();

  void Set (const Vect &point, const Vect &normal);
  void SetPoint (const Vect &v) { point = v; }
  void SetNormal (const Vect &v) { norm = v; }

  /**
   * \return signed distance:
   *   negative = opposite side from normal,
   *   0 = on plane,
   *   positive = same side as normal
   */
  float64 DistFrom (const Vect &p) const;
  inline float64 Dist (const Vect &p) const { return DistFrom (p); }

  /**
   * \return point on this plane that is closest to the given point
   * (i.e., project the given point onto the plane)
   */
  Vect Project (const Vect &p) const;

  /**
   * \return intersection of the given line with this plane; \a pu =
   * storage for u where intersection point = p + u*dir
   */
  Vect Intersect (const Line &line, float64 *pu = NULL) const;

  /**
   * \return intersection of the self-plane with another plane
   */
  Line Intersect (const Plane &pln) const;

 OB_PRIVATE:
  Vect point, norm;
};
}
}  // close namespace


#endif

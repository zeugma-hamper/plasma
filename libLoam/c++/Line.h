
/* (c)  oblong industries */

#ifndef __LINE_H__
#define __LINE_H__


#include <libLoam/c++/Vect.h>


namespace oblong {
namespace loam {

/**
 * Line represents a line in 3-space represented by a point on the
 * line and a direction.
 */
class OB_LOAMXX_API Line
{
 public:
  Line ();

  Line (const Vect &pointOnLine, const Vect &direction);

  void Set (const Vect &pointOnLine, const Vect &direction);

  // relying on the compiler for copy and move constructors and assignments

  const Vect &Point () const { return point; }
  const Vect &Dir () const { return dir; }

  bool IsValid () const;

  static Line Invalid ();

  /**
   * \return point \a u units along line from defining "point on line"
   */
  Vect PointAlong (float64 u) const { return point + dir * u; }

  /**
   * Project the given point onto this line; the optional \a qu parameter
   * gives the length along this line (starting from the defining
   * "point on line") where the projected point falls.  In other words
   * qu = | p - q | and is exposed here as an optimization because it
   * is calculated as a byproduct of the projection and is often
   * useful.
   */
  Vect Project (const Vect &q, float64 *qu = NULL) const;

  /**
   * \return perpendicular distance from the given point to this line
   */
  float64 DistFrom (const Vect &q) const;
  inline float64 Dist (const Vect &q) const { return DistFrom (q); }

  /**
   * \return point of intersection between this line and the given
   * line.  If there is no precise intersection (i.e., the lines are
   * skew), then the point that minimizes the distance to the two
   * lines is returned.  If the lines are parallel or invalid, then an
   * "invalid" point is returned.
   */
  Vect Intersect (const Line &line) const;

 OB_PRIVATE:
  Vect point, dir;
};
}
}  // close namespace


#endif

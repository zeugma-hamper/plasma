
/* (c)  oblong industries */

#ifndef GEOM_SLAB_SMACKDOWN
#define GEOM_SLAB_SMACKDOWN


#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-attrs.h"
#include "libLoam/c/ob-types.h"
#include "libLoam/c++/LoamForward.h"
#include "libLoam/c++/Plane.h"
#include <algorithm>
#include <limits>

namespace oblong {
namespace loam {

/**
 *  GeomSlab is a collection of simple and ubiquitous hit-checking / ray-casting routines.
 */

namespace GeomSlab {

/**
   * If there's an intersection of the directed ray specified by the
   * \a from and \a to position vectors and the rectangle described by
   * \a corner, \a over, and \a up, it's returned as a new Vect (which
   * it's your sworn duty to \c delete eventually). If no
   * intersection, \c NULL is returned. Note that only intersections
   * in the 'forward' direction count; i.e. we start at \a from and
   * move toward \a to, and the rectangle has to be 'in front of'
   * us. Also note: \a over and \a up are expected to be perpendicular
   * to each other.
   */
//@{
OB_LOAMXX_API bool RayRectIntersection (const Line &ray, const Rectangle &rect,
                                        v3float64 *fill_v);
OB_LOAMXX_API bool RayRectIntersection (const Vect &from, const Vect &to,
                                        const Vect &corner, const Vect &over,
                                        const Vect &up,
                                        v3float64 *fill_v = nullptr);
//@}

/**
   * This test presupposes (which means 'requires', if you want the
   * answer to be correct) that the point \a p lies in the plane that
   * also contains the rect defined by \a cent, \a halfOver, and \a
   * halfUp. Given that, you get back a nonzero integer if point \b is
   * indeed inside the rectangle; zero otherwise.
   */
//@{
OB_LOAMXX_API bool PointInsideRectTest (const Vect &point,
                                        const Rectangle &rect);
OB_LOAMXX_API bool PointInsideRectTest (const Vect &p, const Vect &cent,
                                        const Vect &half_over,
                                        const Vect &half_up);
//@}

/**
   * Like RayRectInterection() above, except that \a over and \a up
   * needn't be perpendicular -- you can describe an arbitrary
   * parallelogram.
   */
//@{
OB_LOAMXX_API bool RayQuadIntersection (const Line &ray,
                                        const Rectangle &parallelogram,
                                        v3float64 *fill_v);
OB_LOAMXX_API bool RayQuadIntersection (const Vect &from, const Vect &to,
                                        const Vect &corner, const Vect &over,
                                        const Vect &up,
                                        v3float64 *fill_v = nullptr);
//@}

/**
   * Just like it sounds. The triangle is defined by a single position
   * vector (\a corner) and two displacement vectors (\a leg1 and \a
   * leg2) that take you from the specified corner to the other two.
   */
//@{
OB_LOAMXX_API bool RayTriangleIntersection (const Vect &from, const Vect &to,
                                            const Vect &corner,
                                            const Vect &leg1, const Vect &leg2,
                                            v3float64 *fill_v = nullptr);
//@}

/**
   * The pattern ought to be emerging by now... the intersection is
   * this time with an arbitrary ellipse given by its center (\a cent)
   * and its major and minor axes (\a ax1 and \a ax2).  Mark well: the
   * magnitude of, e.g., the major axis is half the 'width' of the
   * ellipse; so too with the minor axis, naturally.
   */
//@{
OB_LOAMXX_API bool RayEllipseIntersection (const Vect &from, const Vect &to,
                                           const Vect &cent, const Vect &ax1,
                                           const Vect &ax2,
                                           v3float64 *fill_v = nullptr);
//@}

/**
   * This time it's the intersection with an infinite plane, expressed
   * as an arbitrary point in that plane (\a plane_p) and the plane's
   * normal (\a norm).
   */
//@{
OB_LOAMXX_API bool RayPlaneIntersection (const Line &ray, const Plane &plane,
                                         v3float64 *fill_v);
OB_LOAMXX_API bool RayPlaneIntersection (const Vect &from, const Vect &to,
                                         const Vect &plane_p, const Vect &norm,
                                         v3float64 *fill_v = nullptr);
//@}

/**
   * Like the immediately foregoing, but it's also now an infinite
   * line (unlike the half-line or 'ray', previously) -- so you get an
   * intersection whether the plane is 'in front of' or 'behind'
   * you...
   */
//@{
OB_LOAMXX_API bool LinePlaneIntersection (const Line &line, const Plane &plane,
                                          v3float64 *fill_v);
OB_LOAMXX_API bool LinePlaneIntersection (const Vect &from, const Vect &to,
                                          const Vect &plane_p, const Vect &norm,
                                          v3float64 *fill_v = nullptr);
//@}

/**
   * Now things get trickier: there are distinct ways that a ray can
   * intersect a sphere. The ray can miss entirely. The ray can, with
   * some devilish precision, glancingly hit the sphere at exactly one
   * point (the ray is 'tangent to' the sphere then). The ray can
   * pierce the sphere twice: once going in, and once coming out the
   * other 'side'. Or, if the ray's origin (\a from) is inside the
   * sphere, it will (and also must) intersect the sphere exactly once
   * -- although understand that, as a token of mathematical goodwill,
   * we even in this case let you get at the 'backwards' intersecton,
   * i.e. the one 'behind' you when you're inside the sphere.
   *
   * The new geomslab approaches these with a different philosophy from the
   * old one.
   *
   * The old one attempted to save work by allowing one to first intersect
   * a ray with a sphere and then query for various intersection points.
   *
   * The new code does not explicitly do that.
   *
   * Instead, if you want both intersections, you can either call a function
   * that gives you both, or you can call two functions, which will repeat
   * some of the same work.
   * You may still get a performance boost over the old GeomSlab, however,
   * as the new functionality avoids unnecessary calls to malloc.
   */
//@{

/** Assumes along_line is normalized.
   *
   *  This is the base intersection test that all the other sphere
   *  intersection tests depend on.
   */
OB_LOAMXX_API bool LineSphereBothIntersections (const Line &line,
                                                const Sphere &sphere,
                                                v3float64 *fill_isect_midpoint,
                                                float64 *fill_half_dist,
                                                float64 *fill_along_dist);
OB_LOAMXX_API bool
LineSphereBothIntersections (const Vect &pt_on_line, const Vect &normalized_dir,
                             const Vect &sph_cent, float64 sph_rad,
                             v3float64 *fill_isect_midpoint,
                             float64 *fill_half_dist, float64 *fill_along_dist);

/**
   *  Find the closest of the two intersection points to the ray origin.
   *
   *  Note that the semantics of this are different from the related
   *  deprecated function RaySphereNearIntersection,
   *  which would return the closest of the two points, unless one
   *  was in front and the other was behind, in which case the one in front
   *  would have been returned.
   */
OB_LOAMXX_API bool RaySphereClosestIntersection (const Line &ray,
                                                 const Sphere &sphere,
                                                 v3float64 *fill_v);
OB_LOAMXX_API bool RaySphereClosestIntersection (const Vect &from,
                                                 const Vect &to,
                                                 const Vect &sph_cent,
                                                 float64 sph_rad,
                                                 v3float64 *fill_v);

/**
   *  Find the furthest of the two intersection points from the ray origin.
   *
   *  Note that the semantics of this are different from the deprecated
   *  function RaySphereFarIntersection,
   *  which would return the furthest of the two points, unless one
   *  was in front and the other was behind, in which case it would return
   *  the one in front, or unless both points were behind the ray origin,
   *  in which case it would return the closest.
   */
OB_LOAMXX_API bool RaySphereFurthestIntersection (const Line &ray,
                                                  const Sphere &sphere,
                                                  v3float64 *fill_v);
OB_LOAMXX_API bool RaySphereFurthestIntersection (const Vect &from,
                                                  const Vect &to,
                                                  const Vect &sph_cent,
                                                  float64 sph_rad,
                                                  v3float64 *fill_v);

/**
   *  Find the closest of the two intersection points from the ray origin.
   *  unless one point is in front of the ray origin and the other is
   *  behind, in which case return the one in front.
   *
   *  mildly DEPRECATED
   */
OB_LOAMXX_API bool RaySphereNearIntersection (const Line &ray,
                                              const Sphere &sphere,
                                              v3float64 *fill_v);
OB_LOAMXX_API bool RaySphereNearIntersection (const Vect &from, const Vect &to,
                                              const Vect &sph_cent,
                                              float64 sph_rad,
                                              v3float64 *fill_v);

/**
   *  Find the furthest of the two intersection points from the ray origin.
   *  unless one point is in front of the ray origin and the other is
   *  behind, in which case return the one in front, or unless both points
   *  are behind the ray origin, in which case it returns the closest
   *  (this is to attempt to preserve backwards compatibility).
   *
   *  mildly DEPRECATED
   */
OB_LOAMXX_API bool RaySphereFarIntersection (const Line &ray,
                                             const Sphere &sphere,
                                             v3float64 *fill_v);
OB_LOAMXX_API bool RaySphereFarIntersection (const Vect &from, const Vect &to,
                                             const Vect &sph_cent,
                                             float64 sph_rad,
                                             v3float64 *fill_v);

/**
   *  Find the first intersection of a ray with a sphere found by
   *  going forwards along the ray direction.  If both intersection points
   *  are behind the ray origin, return false.
   *
   *  Note that this may be behaviorally different from the old code,
   *  even if the documentation claims otherwise
   */
OB_LOAMXX_API bool RaySphereForwardIntersection (const Line &ray,
                                                 const Sphere &sphere,
                                                 v3float64 *fill_v);
OB_LOAMXX_API bool RaySphereForwardIntersection (const Vect &from,
                                                 const Vect &to,
                                                 const Vect &sph_cent,
                                                 float64 sph_rad,
                                                 v3float64 *fill_v);

/**
   *  Find the first intersection of a ray with a sphere found by
   *  going backwards opposite the ray direction.  If both intersection points
   *  are in front of the ray origin, return false.
   *
   *  Note that this may be behaviorally different from the old code,
   *  even if the documentation claims otherwise
   */
OB_LOAMXX_API bool RaySphereBackwardIntersection (const Line &ray,
                                                  const Sphere &sphere,
                                                  v3float64 *fill_v);
OB_LOAMXX_API bool RaySphereBackwardIntersection (const Vect &from,
                                                  const Vect &to,
                                                  const Vect &sph_cent,
                                                  float64 sph_rad,
                                                  v3float64 *fill_v);
//@}



/** takes two pairs of three Vects, returning a Quat,
   *    qu = SpaceSpaceAlignment (a0, a1, a2, b0, b1, b2);
   *  such that:
   *    (b1-b0).Rotate (qu->ExtractAxis (), qu->ExtractAngle ()) == (a1-a0)
   *    (b2-b0).Rotate (qu->ExtractAxis (), qu->ExtractAngle ()) == (a2-a0)
   */
//@{
OB_LOAMXX_API void SpaceSpaceAlignment (Vect a0, Vect a1, Vect a2, Vect b0,
                                        Vect b1, Vect b2, Quat *fill_q);
//@}


/**
 * Test whether a plane intersects the convex hull of a cloud of points.
 * Returns 0 if points fall on both sides of the plane or a point lies on the plane
 * Otherwise, returns the signed distance to the closest point, where the sign
 * indicates alignment with the plane normal, i.e. this function returns a positive
 * value if all points are in front of the plane, and a negative one if all points
 * lie behind the plane.
 */
template <typename T>
float64 PointsPlaneIntersection (const T points, const Plane &plane)
{
  float64 min_dist = std::numeric_limits<float64>::max (),
          max_dist = -1.0 * std::numeric_limits<float64>::max ();
  for (Vect p : points)
    {
      const float64 dist = plane.DistFrom (p);
      if (dist == 0.0 || dist == -0.0)
        {
          return 0.0;
        }
      min_dist = std::min (min_dist, dist);
      max_dist = std::max (max_dist, dist);
    }
  if (max_dist < 0)
    return max_dist;
  if (min_dist > 0)
    return min_dist;
  return 0.0;
}

/**
 * Tests whether a plane intersects an AABB
 * Returns 0 for plane slicing through the box volume
 * Otherwise, returns the signed distance to the closest corner, where the sign
 * indicates alignment with the plane normal.
 */
OB_LOAMXX_API float64 BoxPlaneIntersection (const AxisAlignedBox &box,
                                            const Plane &plane);

/**
 * Returns true when the box, or some part of it, lies within the frustum
 * Returns false when the box and frustum do not intersect
 */
OB_LOAMXX_API bool BoxFrustumIntersect (const AxisAlignedBox &box,
                                        const Frustum &frustum);

/**
 * Tests whether an AABB and a sphere have any points of intersection.
 * Returns true when the AABB and sphere have any overlap
 * Returns false when the AABB and sphere have no overlap
 */
OB_LOAMXX_API bool BoxSphereIntersect (const AxisAlignedBox &box,
                                       const Sphere &sphere);

/**
 * Returns true when the sphere and frustum intersect
 * Returns false when the sphere and frustum do not intersect
 */
OB_LOAMXX_API bool FrustumSphereIntersect (const Frustum &frustum,
                                           const Sphere &sphere);

/**
 * Returns true when the point lies within the box.
 * Returns false when the point is not inside the box.
 */
OB_LOAMXX_API bool PointInsideBox (const Vect &point,
                                   const AxisAlignedBox &box);

/**
 * Returns true when the point lies within the frustum volume
 * Returns false if the point is outside of the frustum
 */
OB_LOAMXX_API bool PointInsideFrustum (const Vect &point,
                                       const Frustum &frustum);

/**
 * Returns true when the boxes have any overlap.
 * Returns false when the boxes have no overlap.
 */
OB_LOAMXX_API bool BoxBoxIntersect (const AxisAlignedBox &one,
                                    const AxisAlignedBox &two);

/**
 *  Returns true if the ray passes through the box, even if that intersection
 *  is but a single point.
 *  Returns false if the ray does not intersect the box.
 */
OB_LOAMXX_API bool BoxRayIntersection (const AxisAlignedBox &box,
                                       const Line &ray,
                                       v3float64 *first_intersection = nullptr);

/**
 *  Returns true if the ray passes through the sphere, even if that intersection
 *  is but a single point.
 *  Returns false if the ray does not intersect the sphere.
 */
OB_LOAMXX_API bool SphereRayIntersect (const Sphere &sphere, const Line &ray);

}  // namespace GeomSlab
}
}  //  the untimely passing of namespaces loam and oblong...



#endif


/* (c)  oblong industries */

#include "libLoam/c++/GeomSlab.h"
#include "libLoam/c++/AxisAlignedBox.h"
#include "libLoam/c++/Frustum.h"
#include "libLoam/c++/Line.h"
#include "libLoam/c++/Plane.h"
#include "libLoam/c++/Quat.h"
#include "libLoam/c++/Rectangle.h"
#include "libLoam/c++/Sphere.h"
#include "libLoam/c++/Vect.h"
#include <cmath>

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-types.h"

using namespace oblong::loam;

/**
 * If there's an intersection of the directed ray specified by the
 * \a from and \a to position vectors and the rectangle described by
 * \a corner, \a over, and \a up, it's returned as a new Vect (which
 * it's your sworn duty to \c delete eventually). If no
 * intersection, \c nullptr is returned. Note that only intersections
 * in the 'forward' direction count; i.e. we start at \a from and
 * move toward \a to, and the rectangle has to be 'in front of'
 * us. Also note: \a over and \a up are expected to be perpendicular
 * to each other.
 */
bool GeomSlab::RayRectIntersection (const Line &ray, const Rectangle &rect,
                                    v3float64 *fill_v)
{
  return RayRectIntersection (ray.Point (), ray.Point () + ray.Dir (),
                              rect.corner, rect.leg1, rect.leg2, fill_v);
}

bool GeomSlab::RayRectIntersection (const Vect &from, const Vect &to,
                                    const Vect &corner, const Vect &over,
                                    const Vect &up, v3float64 *fill_v)
{
  // implementation modified from old code
  Vect aimer = to - from;
  Vect n = over.Cross (up).Norm ();
  Vect pos = corner + 0.5 * (over + up);
  float64 tmp, t = aimer.Dot (n);
  if (t == 0.0)  // aim parallel to target plane, you see
    return false;
  t = (pos - from).Dot (n) / t;
  if (t < 0.0)  // it's all behind us, now
    return false;
  Vect hit (from + t * aimer);
  Vect deltaHit (hit - pos);
  if ((t = deltaHit.Dot (over)) < -(tmp = 0.5 * over.AutoDot ()) || t > tmp)
    return false;
  if ((t = deltaHit.Dot (up)) < -(tmp = 0.5 * up.AutoDot ()) || t > tmp)
    return false;

  if (nullptr != fill_v)
    *fill_v = hit;

  return true;
}


/**
 * This test presupposes (which means 'requires', if you want the
 * answer to be correct) that the point \a p lies in the plane that
 * also contains the rect defined by \a cent, \a halfOver, and \a
 * halfUp. Given that, you get back a nonzero integer if point \b is
 * indeed inside the rectangle; zero otherwise.
 */
bool GeomSlab::PointInsideRectTest (const Vect &point, const Rectangle &rect)
{
  const Vect center = rect.corner + 0.5 * (rect.leg1 + rect.leg2);
  return PointInsideRectTest (point, center, rect.leg1 / 2, rect.leg2 / 2);
}
bool GeomSlab::PointInsideRectTest (const Vect &p, const Vect &cent,
                                    const Vect &half_over, const Vect &half_up)
{
  // literally copied verbatim from old version
  Vect delta = p - cent;
  float64 span = delta.Dot (half_over);
  float64 edge = half_over.AutoDot ();
  if (span > edge || span < -edge)
    return false;
  span = delta.Dot (half_up);
  edge = half_up.AutoDot ();
  if (span > edge || span < -edge)
    return false;
  return true;
}

/**
 * Like RayRectInterection() above, except that \a over and \a up
 * needn't be perpendicular -- you can describe an arbitrary
 * parallelogram.
 */
bool GeomSlab::RayQuadIntersection (const Line &ray,
                                    const Rectangle &parallelogram,
                                    v3float64 *fill_v)
{
  return RayQuadIntersection (ray.Point (), ray.Dir () + ray.Point (),
                              parallelogram.corner, parallelogram.leg1,
                              parallelogram.leg2, fill_v);
}
bool GeomSlab::RayQuadIntersection (const Vect &from, const Vect &to,
                                    const Vect &corner, const Vect &over,
                                    const Vect &up, v3float64 *fill_v)
{
  Vect aimer = to - from;
  Vect leg1N (over.Norm ()), leg2N (up.Norm ());
  Vect n = over.Cross (up).Norm ();
  float64 t = aimer.Dot (n);
  if (t == 0.0)  // aim parallel to target plane, you see
    return false;
  t = (corner - from).Dot (n) / t;
  if (t < 0.0)  // it's all behind us, now
    return false;
  Vect hit (from + t * aimer);

  // now.
  Vect d_hit (hit - corner);
  float64 cossy = leg1N.Dot (leg2N);
  float64 dhDotL1 = d_hit.Dot (leg1N);
  float64 dhDotL2 = d_hit.Dot (leg2N);
  float64 alpha = (dhDotL1 - cossy * dhDotL2);
  float64 beta = (dhDotL2 - cossy * dhDotL1);
  alpha *= (cossy = 1.0 / (1.0 - cossy * cossy));
  beta *= cossy;
  if ((alpha < 0.0) || (beta < 0.0) || (alpha * alpha > over.AutoDot ())
      || (beta * beta > up.AutoDot ()))
    return false;
  if (nullptr != fill_v)
    *fill_v = hit;
  return true;
}


/**
 * Just like it sounds. The triangle is defined by a single position
 * vector (\a corner) and two displacement vectors (\a leg1 and \a
 * leg2) that take you from the specified corner to the other two.
 */
bool GeomSlab::RayTriangleIntersection (const Vect &from, const Vect &to,
                                        const Vect &corner, const Vect &leg1,
                                        const Vect &leg2, v3float64 *fill_v)
{
  // Almost-verbatim copy of old code.
  // Fix to use standard tricks if it proves buggy via tests
  Vect aimer = to - from;
  Vect n = leg1.Cross (leg2).Norm ();
  float64 t = aimer.Dot (n);
  if (t == 0.0)  // aim parallel to target plane, you see
    return false;
  t = (corner - from).Dot (n) / t;
  if (t < 0.0)  // it's all behind us, now
    return false;
  Vect hit (from + t * aimer);

  Vect c1 = hit - corner;
  Vect c2 = c1 - leg1;
  Vect c3 = c1 - leg2;

  Vect n1 = leg1.Cross (c1);
  Vect n2 = (leg2 - leg1).Cross (c2);
  Vect n3 = (-leg2).Cross (c3);

  if (n1.Dot (n2) < 0.0 || n2.Dot (n3) < 0.0)
    return false;

  if (nullptr != fill_v)
    *fill_v = hit;
  return true;
}


/**
 * The pattern ought to be emerging by now... the intersection is
 * this time with an arbitrary ellipse given by its center (\a cent)
 * and its major and minor axes (\a ax1 and \a ax2).  Mark well: the
 * magnitude of, e.g., the major axis is half the 'width' of the
 * ellipse; so too with the minor axis, naturally.
 */

bool GeomSlab::RayEllipseIntersection (const Vect &from, const Vect &to,
                                       const Vect &cent, const Vect &ax1,
                                       const Vect &ax2, v3float64 *fill_v)
{
  // also copied and modified from old GeomSlab.
  Vect aimer = to - from;
  Vect n = ax1.Cross (ax2).Norm ();
  float64 t = aimer.Dot (n);
  if (t == 0.0)  // aim parallel to target plane, you see
    return false;
  t = (cent - from).Dot (n) / t;
  if (t < 0.0)  // it's all behind us, now
    return false;
  Vect hit (from + t * aimer - cent);

  float64 a = ax1.Dot (hit) / ax1.AutoDot ();
  float64 b = ax2.Dot (hit) / ax2.AutoDot ();
  if (a * a + b * b > 1.0)
    return false;
  if (nullptr != fill_v)
    *fill_v = (hit + cent);
  return true;
}


/**
 * This time it's the intersection with an infinite plane, expressed
 * as an arbitrary point in that plane (\a plane_p) and the plane's
 * normal (\a norm).
 */
bool GeomSlab::RayPlaneIntersection (const Line &ray, const Plane &plane,
                                     v3float64 *fill_v)
{
  return RayPlaneIntersection (ray.Point (), ray.Dir () + ray.Point (),
                               plane.Point (), plane.Normal (), fill_v);
}
bool GeomSlab::RayPlaneIntersection (const Vect &from, const Vect &to,
                                     const Vect &plane_p, const Vect &norm,
                                     v3float64 *fill_v)
{
  Vect aimer = to - from;
  Vect n = norm.Norm ();
  float64 t = aimer.Dot (n);
  if (t == 0.0)  // aim parallel to target plane, you see
    return false;
  t = (plane_p - from).Dot (n) / t;
  if (t < 0.0)  // it's all behind us, now
    return false;
  if (nullptr != fill_v)
    *fill_v = (from + t * aimer);
  return true;
}


/**
 * Like the immediately foregoing, but it's also now an infinite
 * line (unlike the half-line or 'ray', previously) -- so you get an
 * intersection whether the plane is 'in front of' or 'behind'
 * you...
 */
bool GeomSlab::LinePlaneIntersection (const Line &line, const Plane &plane,
                                      v3float64 *fill_v)
{
  return LinePlaneIntersection (line.Point (), line.Dir () + line.Point (),
                                plane.Point (), plane.Normal (), fill_v);
}

bool GeomSlab::LinePlaneIntersection (const Vect &from, const Vect &to,
                                      const Vect &plane_p, const Vect &norm,
                                      v3float64 *fill_v)
{
  // also nearly identical to old copy
  Vect aimer = to - from;
  Vect n = norm.Norm ();
  float64 t = aimer.Dot (n);
  if (t == 0.0)  // aim parallel to target plane, you see
    return false;
  t = (plane_p - from).Dot (n) / t;
  if (nullptr != fill_v)
    *fill_v = (from + t * aimer);
  return true;
}

void GeomSlab::SpaceSpaceAlignment (Vect a0, Vect a1, Vect a2, Vect b0, Vect b1,
                                    Vect b2, Quat *fill_q)
{
  Quat q1, q2;
  a1 -= a0;
  a2 -= a0;
  b1 -= b0;
  b2 -= b0;
  a1.NormSelf ();
  a2.NormSelf ();
  b1.NormSelf ();
  b2.NormSelf ();
  Vect nA = a2.Cross (a1).Norm ();
  Vect nB = b2.Cross (b1).Norm ();
  Vect ax = nB.Cross (nA);
  float64 coss = nB.Dot (nA);
  // half the work
  if (!ax.IsZero ())
    {
      float64 theta = asin (ax.Mag ());
      if (coss < 0.0)
        theta = M_PI - theta;
      b1.RotateSelf (ax, theta);
      b2.RotateSelf (ax, theta);
      q1.LoadQRotFromAxisAngle (ax, theta);
    }
  else if (coss < 0.0)  // anti-parallel case
    {
      b1.RotateSelf (a1, M_PI);
      b2.RotateSelf (a1, M_PI);
      q1.LoadQRotFromAxisAngle (ax, M_PI);
    }
  else  // parallel case
    q1.LoadQRotFromAxisAngle (ax, 0.0);
  // the other half of the work
  coss = b1.Dot (a1);
  ax = b1.Cross (a1);
  q2.LoadQRotFromAxisAngle (nA,
                            acos (coss) * ((ax.Dot (nA) < 0.0) ? -1.0 : 1.0));
  *fill_q = (q2 * q1);
}


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

/**
 * Assumes "along_line" is normalized.
 */
bool GeomSlab::LineSphereBothIntersections (const Line &line,
                                            const Sphere &sphere,
                                            v3float64 *fill_isect_midpoint,
                                            float64 *fill_half_dist,
                                            float64 *fill_along_dist)
{
  return LineSphereBothIntersections (line.Point (), line.Dir (), sphere.center,
                                      sphere.radius, fill_isect_midpoint,
                                      fill_half_dist, fill_along_dist);
}
bool GeomSlab::LineSphereBothIntersections (
  const Vect &pt_on_line, const Vect &normalized_dir, const Vect &sph_cent,
  float64 sph_rad, v3float64 *fill_isect_midpoint, float64 *fill_half_dist,
  float64 *fill_along_dist)
{
  Vect relCenter (sph_cent - pt_on_line);
  float64 distAlongRay = normalized_dir.Dot (relCenter);

  Vect closestPoint (pt_on_line + distAlongRay * normalized_dir);

  float64 closestDistanceSquared = closestPoint.SquaredDistFrom (sph_cent);
  float64 halfDistSquared = sph_rad * sph_rad - closestDistanceSquared;

  // The order in which we do this prevents us from taking sqrts of negative
  // numbers -- even tiny ones like -0.0
  if (halfDistSquared < 0.0)
    return false;

  if (nullptr != fill_isect_midpoint)
    *fill_isect_midpoint = closestPoint;

  if (nullptr != fill_half_dist)
    *fill_half_dist = sqrt (halfDistSquared);

  if (nullptr != fill_along_dist)
    *fill_along_dist = distAlongRay;

  return true;
}

bool GeomSlab::RaySphereClosestIntersection (const Line &ray,
                                             const Sphere &sphere,
                                             v3float64 *fill_v)
{
  return RaySphereClosestIntersection (ray.Point (), ray.Dir () + ray.Point (),
                                       sphere.center, sphere.radius, fill_v);
}
bool GeomSlab::RaySphereClosestIntersection (const Vect &from, const Vect &to,
                                             const Vect &sph_cent,
                                             float64 sph_rad, v3float64 *fill_v)
{
  Vect solutionMidpoint;
  float64 solutionMidpointDist;
  float64 distBetweenSolutions;

  Vect normalizedDir = (to - from).Norm ();

  if (!LineSphereBothIntersections (from, normalizedDir, sph_cent, sph_rad,
                                    &solutionMidpoint, &distBetweenSolutions,
                                    &solutionMidpointDist))
    return false;

  if (nullptr != fill_v)
    {
      if (fabs (solutionMidpointDist - distBetweenSolutions)
          < fabs (solutionMidpointDist + distBetweenSolutions))
        *fill_v = (solutionMidpoint - normalizedDir * distBetweenSolutions);
      else
        *fill_v = (solutionMidpoint + normalizedDir * distBetweenSolutions);
    }

  return true;
}
bool GeomSlab::RaySphereFurthestIntersection (const Line &ray,
                                              const Sphere &sphere,
                                              v3float64 *fill_v)
{
  return RaySphereFurthestIntersection (ray.Point (), ray.Dir () + ray.Point (),
                                        sphere.center, sphere.radius, fill_v);
}
bool GeomSlab::RaySphereFurthestIntersection (const Vect &from, const Vect &to,
                                              const Vect &sph_cent,
                                              float64 sph_rad,
                                              v3float64 *fill_v)
{
  Vect solutionMidpoint;
  float64 solutionMidpointDist;
  float64 distBetweenSolutions;

  Vect normalizedDir = (to - from).Norm ();

  if (!LineSphereBothIntersections (from, normalizedDir, sph_cent, sph_rad,
                                    &solutionMidpoint, &distBetweenSolutions,
                                    &solutionMidpointDist))
    return false;

  if (nullptr != fill_v)
    {
      if (fabs (solutionMidpointDist - distBetweenSolutions)
          > fabs (solutionMidpointDist + distBetweenSolutions))
        *fill_v = (solutionMidpoint - normalizedDir * distBetweenSolutions);
      else
        *fill_v = (solutionMidpoint + normalizedDir * distBetweenSolutions);
    }

  return true;
}

/**
 *  Find the closest of the two intersection points from the ray origin.
 *  unless one point is in front of the ray origin and the other is
 *  behind, in which case return the one in front.
 *
 *  mildly DEPRECATED
 *  WARNING : NO TESTS FOR THIS METHOD!!!
 */
bool GeomSlab::RaySphereNearIntersection (const Line &ray, const Sphere &sphere,
                                          v3float64 *fill_v)
{
  return RaySphereNearIntersection (ray.Point (), ray.Dir () + ray.Point (),
                                    sphere.center, sphere.radius, fill_v);
}
bool GeomSlab::RaySphereNearIntersection (const Vect &from, const Vect &to,
                                          const Vect &sph_cent, float64 sph_rad,
                                          v3float64 *fill_v)
{
  Vect solutionMidpoint;
  float64 solutionMidpointDist;
  float64 distBetweenSolutions;

  Vect normalizedDir = (to - from).Norm ();

  if (!LineSphereBothIntersections (from, normalizedDir, sph_cent, sph_rad,
                                    &solutionMidpoint, &distBetweenSolutions,
                                    &solutionMidpointDist))
    return false;

  if (nullptr != fill_v)
    {
      if (solutionMidpointDist + distBetweenSolutions > 0.0
          && solutionMidpointDist - distBetweenSolutions < 0.0)
        *fill_v = (solutionMidpoint + normalizedDir * distBetweenSolutions);
      else if (fabs (solutionMidpointDist - distBetweenSolutions)
               < fabs (solutionMidpointDist + distBetweenSolutions))
        *fill_v = (solutionMidpoint - normalizedDir * distBetweenSolutions);
      else
        *fill_v = (solutionMidpoint + normalizedDir * distBetweenSolutions);
    }

  return true;
}

/**
 *  Find the furthest of the two intersection points from the ray origin.
 *  unless one point is in front of the ray origin and the other is
 *  behind, in which case return the one in front, or unless both points
 *  are behind the ray origin, in which case it returns the closest
 *  (this is to attempt to preserve backwards compatibility).
 *
 *  mildly DEPRECATED
 *  WARNING : NO TESTS FOR THIS METHOD!!!
 */
bool GeomSlab::RaySphereFarIntersection (const Line &ray, const Sphere &sphere,
                                         v3float64 *fill_v)
{
  return RaySphereFarIntersection (ray.Point (), ray.Dir () + ray.Point (),
                                   sphere.center, sphere.radius, fill_v);
}
bool GeomSlab::RaySphereFarIntersection (const Vect &from, const Vect &to,
                                         const Vect &sph_cent, float64 sph_rad,
                                         v3float64 *fill_v)
{
  Vect solutionMidpoint;
  float64 solutionMidpointDist;
  float64 distBetweenSolutions;

  Vect normalizedDir = (to - from).Norm ();

  if (!LineSphereBothIntersections (from, normalizedDir, sph_cent, sph_rad,
                                    &solutionMidpoint, &distBetweenSolutions,
                                    &solutionMidpointDist))
    return false;

  if (nullptr != fill_v)
    *fill_v = (solutionMidpoint + normalizedDir * distBetweenSolutions);


  return true;
}

bool GeomSlab::RaySphereForwardIntersection (const Line &ray,
                                             const Sphere &sphere,
                                             v3float64 *fill_v)
{
  return RaySphereForwardIntersection (ray.Point (), ray.Dir () + ray.Point (),
                                       sphere.center, sphere.radius, fill_v);
}
bool GeomSlab::RaySphereForwardIntersection (const Vect &from, const Vect &to,
                                             const Vect &sph_cent,
                                             float64 sph_rad, v3float64 *fill_v)
{
  Vect solutionMidpoint;
  float64 solutionMidpointDist;
  float64 distBetweenSolutions;

  Vect normalizedDir = (to - from).Norm ();

  if (!LineSphereBothIntersections (from, normalizedDir, sph_cent, sph_rad,
                                    &solutionMidpoint, &distBetweenSolutions,
                                    &solutionMidpointDist))
    return false;

  if (solutionMidpointDist + distBetweenSolutions < 0.0)
    return false;  // no forward solution.

  if (nullptr != fill_v)
    {
      if (solutionMidpointDist - distBetweenSolutions > 0.0)
        *fill_v = (solutionMidpoint - normalizedDir * distBetweenSolutions);
      else
        *fill_v = (solutionMidpoint + normalizedDir * distBetweenSolutions);
    }

  return true;
}

bool GeomSlab::RaySphereBackwardIntersection (const Line &ray,
                                              const Sphere &sphere,
                                              v3float64 *fill_v)
{
  return RaySphereBackwardIntersection (ray.Point (), ray.Dir () + ray.Point (),
                                        sphere.center, sphere.radius, fill_v);
}
bool GeomSlab::RaySphereBackwardIntersection (const Vect &from, const Vect &to,
                                              const Vect &sph_cent,
                                              float64 sph_rad,
                                              v3float64 *fill_v)
{
  Vect solutionMidpoint;
  float64 solutionMidpointDist;
  float64 distBetweenSolutions;

  Vect normalizedDir = (to - from).Norm ();

  if (!LineSphereBothIntersections (from, normalizedDir, sph_cent, sph_rad,
                                    &solutionMidpoint, &distBetweenSolutions,
                                    &solutionMidpointDist))
    return false;

  if (solutionMidpointDist - distBetweenSolutions > -0.0)
    return false;  // no backward solution.

  if (nullptr != fill_v)
    {
      if (solutionMidpointDist + distBetweenSolutions < 0.0)
        *fill_v = (solutionMidpoint - normalizedDir * distBetweenSolutions);
      else
        *fill_v = (solutionMidpoint + normalizedDir * distBetweenSolutions);
    }

  return true;
}

/**
 * Tests whether a plane intersects an AABB
 * Returns 0 for plane slicing through the box volume
 * Otherwise, returns the signed distance to the closest corner, where the sign
 * indicates alignment with the plane normal.
 */
float64 GeomSlab::BoxPlaneIntersection (const AxisAlignedBox &box,
                                        const Plane &plane)
{
  return PointsPlaneIntersection (box.Corners (), plane);
}

/**
 * Returns true when the box, or some part of it, lies within the frustum
 * Returns false when the box and frustum do not intersect
 */
bool GeomSlab::BoxFrustumIntersect (const AxisAlignedBox &box,
                                    const Frustum &frustum)
{
  for (auto fplane : frustum.Planes ())
    {
      if (BoxPlaneIntersection (box, fplane) < 0)
        {
          return false;
        }
    }
  for (auto bplane : box.Planes ())
    {
      if (PointsPlaneIntersection (frustum.Corners (), bplane) < 0)
        {
          return false;
        }
    }
  return true;
}

/**
 * Tests whether an AABB and a sphere have any points of intersection.
 * Returns true when the AABB and sphere have any overlap
 * Returns false when the AABB and sphere have no overlap
 */
bool GeomSlab::BoxSphereIntersect (const AxisAlignedBox &box,
                                   const Sphere &sphere)
{
  float64 square_sum = 0.0;
  const float64 rad_squared = sphere.Radius () * sphere.Radius ();
  for (auto fplane : box.Planes ())
    {
      const float64 curr_dist =
        std::min (0.0, fplane.DistFrom (sphere.Center ()));
      square_sum += curr_dist * curr_dist;
      if (square_sum > rad_squared)
        return false;
    }
  return true;
}

/**
 * Returns true when the sphere and frustum intersect
 * Returns false when the sphere and frustum do not intersect
 */
bool GeomSlab::FrustumSphereIntersect (const Frustum &frustum,
                                       const Sphere &sphere)
{
  for (auto fplane : frustum.Planes ())
    {
      if (fplane.DistFrom (sphere.Center ()) + sphere.Radius () < 0.0)
        return false;
    }
  return true;
}

/**
 * Returns true when the point lies within the box.
 * Returns false when the point is not inside the box.
 */
bool GeomSlab::PointInsideBox (const Vect &point, const AxisAlignedBox &box)
{
  return point.x >= box.minimum.x && point.x <= box.maximum.x
         && point.y >= box.minimum.y && point.y <= box.maximum.y
         && point.z >= box.minimum.z && point.z <= box.maximum.z;
}

/**
 * Returns true when the point lies within the frustum volume
 * Returns false if the point is outside of the frustum
 */
bool GeomSlab::PointInsideFrustum (const Vect &point, const Frustum &frustum)
{
  for (auto fplane : frustum.Planes ())
    {
      if (fplane.DistFrom (point) < 0.0)
        return false;
    }
  return true;
}

/**
 * Returns true when the boxes have any overlap.
 * Returns false when the boxes have no overlap.
 */
bool GeomSlab::BoxBoxIntersect (const AxisAlignedBox &one,
                                const AxisAlignedBox &two)
{
  return one.minimum.x <= two.maximum.x && one.maximum.x >= two.minimum.x
         && one.minimum.y <= two.maximum.y && one.maximum.y >= two.minimum.y
         && one.minimum.z <= two.maximum.z && one.maximum.z >= two.minimum.z;
}

/**
 *  Returns true if the ray passes through the box, even if that intersection
 *  is but a single point.
 *  Returns false if the ray does not intersect the box.
 */
bool GeomSlab::BoxRayIntersection (const AxisAlignedBox &box, const Line &ray,
                                   v3float64 *out)
{
  // An Efficient and Robust Ray–Box Intersection Algorithm
  // Amy Williams; Steve Barrus; R. Keith Morley; Peter Shirley
  // http://people.csail.mit.edu/amy/papers/box-jgt.pdf

  const Vect ray_inverse_direction =
    Vect (1.0 / ray.Dir ().x, 1.0 / ray.Dir ().y, 1.0 / ray.Dir ().z);
  const bool inv_dir_x_negative = ray_inverse_direction.x < 0;
  const bool inv_dir_y_negative = ray_inverse_direction.y < 0;
  const bool inv_dir_z_negative = ray_inverse_direction.z < 0;

  float64 tmin, tmax, tymin, tymax, tzmin, tzmax;
  tmin = ((inv_dir_x_negative ? box.maximum : box.minimum).x - ray.Point ().x)
         * ray_inverse_direction.x;
  tmax = ((inv_dir_x_negative ? box.minimum : box.maximum).x - ray.Point ().x)
         * ray_inverse_direction.x;
  tymin = ((inv_dir_y_negative ? box.maximum : box.minimum).y - ray.Point ().y)
          * ray_inverse_direction.y;
  tymax = ((inv_dir_y_negative ? box.minimum : box.maximum).y - ray.Point ().y)
          * ray_inverse_direction.y;
  if ((tmin > tymax) || (tymin > tmax))
    return false;
  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;
  tzmin = ((inv_dir_z_negative ? box.maximum : box.minimum).z - ray.Point ().z)
          * ray_inverse_direction.z;
  tzmax = ((inv_dir_z_negative ? box.minimum : box.maximum).z - ray.Point ().z)
          * ray_inverse_direction.z;
  if ((tmin > tzmax) || (tzmin > tmax))
    return false;
  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;
  const bool intersect = tmax >= 0;
  if (out && intersect)
    {
      const float64 minimum_dist = std::min (tmax, std::max (0.0, tmin));
      *out = ray.Point () + minimum_dist * ray.Dir ();
    }
  return intersect;
}

/**
 *  Returns true if the ray passes through the sphere, even if that intersection
 *  is but a single point.
 *  Returns false if the ray does not intersect the sphere.
 */
bool GeomSlab::SphereRayIntersect (const Sphere &sphere, const Line &ray)
{
  // https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
  const float64 radicand =
    std::pow (ray.Dir ().Dot (ray.Point () - sphere.center), 2)
    - (ray.Point () - sphere.center).AutoDot () + sphere.radius * sphere.radius;
  if (radicand < 0)
    return false;
  if (-ray.Dir ().Dot (ray.Point () - sphere.center) + std::sqrt (radicand) < 0)
    return false;
  return true;
}

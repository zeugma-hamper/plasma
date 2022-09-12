
/* (c)  oblong industries */

/* This tests geometry classes (Plane and Line) against GeomSlab */

#include <gtest/gtest.h>

#include "libLoam/c++/GeomSlab.h"
#include "libLoam/c++/Vect.h"
#include "libLoam/c++/Plane.h"
#include "libLoam/c++/Line.h"

using namespace oblong::loam;

const static float64 eps = 1e-9;
const static Vect Zero (0, 0, 0);
const static Vect XAxis (1, 0, 0);
const static Vect YAxis (0, 1, 0);
const static Vect ZAxis (0, 0, 1);

TEST (GeomClassesTest, LineTest)
{
  Line line, line2;
  Vect w, a, b;
  float64 u;

  EXPECT_TRUE (Line ().IsValid ());
  EXPECT_TRUE (Line ().Point ().DistFrom (Zero) < eps);
  EXPECT_TRUE (Line ().Dir ().Dot (ZAxis) > (1.0 - eps));

  line.Set (Vect::Invalid (), XAxis);
  EXPECT_FALSE (line.IsValid ());

  EXPECT_FALSE (Line::Invalid ().IsValid ());

  line.Set (Zero, XAxis);
  EXPECT_TRUE (line.IsValid ());
  EXPECT_TRUE (fabs (Zero.DistFrom (line.Point ())) < eps);
  EXPECT_TRUE (fabs (XAxis.DistFrom (line.Dir ())) < eps);

  w = line.PointAlong (3.14);
  EXPECT_TRUE (fabs (w.DistFrom (Vect (3.14, 0, 0))) < eps);

  w = line.PointAlong (-2.718);
  EXPECT_TRUE (fabs (w.DistFrom (Vect (-2.718, 0, 0))) < eps);

  w.Set (3.14, 2.718, 1.618);
  a.Set (3.14, 0, 0);
  b = line.Project (w, &u);
  EXPECT_TRUE (fabs (u - a.x) < eps);
  EXPECT_TRUE (fabs (b.DistFrom (a)) < eps);

  EXPECT_TRUE (fabs (line.Dist (w) - w.DistFrom (a)) < eps);

  line2.Set (Zero, YAxis);
  a = line.Intersect (line2);
  b = line2.Intersect (line);
  EXPECT_TRUE (fabs (a.DistFrom (b)) < eps);
  EXPECT_TRUE (fabs (a.DistFrom (Zero)) < eps);

  w.Set (2.718, 0, 0);
  line2.Set (w, Vect (-3.2, 2.4, -0.01));
  a = line.Intersect (line2);
  b = line2.Intersect (line);
  EXPECT_TRUE (fabs (a.DistFrom (b)) < eps);
  EXPECT_TRUE (fabs (a.DistFrom (w)) < eps);

  // test parallel line intersection
  line2.Set (YAxis, XAxis);
  w = line.Intersect (line2);
  EXPECT_FALSE (w.IsValid ());

  line2.Set (XAxis, XAxis);
  w = line.Intersect (line2);
  EXPECT_FALSE (w.IsValid ());

  // test skew line intersection
  line2.Set (YAxis, ZAxis);
  w = line.Intersect (line2);
  EXPECT_TRUE (fabs (w.DistFrom ((Zero + YAxis) / 2)) < eps);
}


TEST (GeomClassesTest, PlaneTest)
{
  Plane plane;
  Line line;
  Vect w, a, b;
  float64 u;

  EXPECT_TRUE (Plane ().IsValid ());
  EXPECT_TRUE (Plane ().Point ().DistFrom (Zero) < eps);
  EXPECT_TRUE (Plane ().Normal ().Dot (ZAxis) > (1.0 - eps));

  plane.Set (Vect::Invalid (), XAxis);
  EXPECT_FALSE (plane.IsValid ());

  EXPECT_FALSE (Plane::Invalid ().IsValid ());

  plane.Set (Zero, ZAxis);
  EXPECT_TRUE (plane.IsValid ());
  EXPECT_TRUE (fabs (Zero.DistFrom (plane.Point ())) < eps);
  EXPECT_TRUE (fabs (ZAxis.DistFrom (plane.Normal ())) < eps);

  w.Set (3.14, 2.718, 1.618);
  u = plane.DistFrom (w);
  EXPECT_TRUE (fabs (u - 1.618) < eps);

  a = plane.Project (w);
  EXPECT_TRUE (fabs (a.DistFrom (w) - 1.618) < eps);
  EXPECT_TRUE (fabs (a.DistFrom (Vect (3.14, 2.718, 0))) < eps);

  line.Set (a, w);
  b = plane.Intersect (line, &u);
  EXPECT_TRUE (fabs (a.DistFrom (b)) < eps);
  EXPECT_TRUE (fabs (u) < eps);
}

TEST (GeomClassesTest, PlanePlaneIntersectTest)
{
  Plane pl1 (Vect (2.0, 0.0, 0.0), Vect (-1.0, 0.0, 0.0));
  Plane pl2 (Vect (0.0, 3.0, 0.0), Vect (0.0, -1.0, 0.0));
  Line lyne = pl1.Intersect (pl2);
  EXPECT_TRUE (lyne.DistFrom (Vect (2.0, 3.0, 100.0)) < eps);
  EXPECT_TRUE (lyne.Dir ().Dot (ZAxis) > (1.0 - eps));

  lyne = pl1.Intersect (pl1);
  EXPECT_FALSE (lyne.IsValid ());
}

TEST (GeomClassesTest, EquivGeomSlab)
{
  Plane plane;
  Line line;

  Vect from (2.41, -8.21, 1.09);
  Vect to (-1.32, 4.51, 1.25);
  Vect plane_p (5.63, -1.23, -4.19);
  Vect norm (1.55, 1.24, -0.24);
  norm.NormSelf ();

  Vect v1;
  bool hit = GeomSlab::LinePlaneIntersection (from, to, plane_p, norm, &v1);
  EXPECT_TRUE (hit);
  plane.Set (plane_p, norm);
  line.Set (from, to - from);
  Vect v2 = plane.Intersect (line);
  EXPECT_TRUE (fabs (v2.DistFrom (v1)) < eps);
}


/* (c)  oblong industries */

#include <gtest/gtest.h>
#include <libLoam/c/ob-rand.h>
#include "libLoam/c++/Str.h"

#include "libLoam/c++/Vect.h"

using namespace oblong::loam;

const Vect XZ (1, 0, 1);

TEST (VectTest, InitVect)
{
  Vect v;
  EXPECT_EQ (0.0, v.x);
  EXPECT_EQ (0.0, v.y);
  EXPECT_EQ (0.0, v.z);

  Vect v1 (1.0, 2.0, 3.0);
  EXPECT_EQ (1.0, v1.x);
  EXPECT_EQ (2.0, v1.y);
  EXPECT_EQ (3.0, v1.z);

  Vect v2 (v1);
  EXPECT_EQ (1.0, v2.x);
  EXPECT_EQ (2.0, v2.y);
  EXPECT_EQ (3.0, v2.z);

  v3float32 f32 = v3float32{1.0, 2.0, 3.0};
  Vect v3 (f32);
  EXPECT_EQ (1.0, v3.x);
  EXPECT_EQ (2.0, v3.y);
  EXPECT_EQ (3.0, v3.z);
}

TEST (VectTest, UnitRandomize)
{
  Vect v (XZ);
  EXPECT_DOUBLE_EQ (sqrt (2.0), v.Mag ());
  v.UnitRandomize ();
  EXPECT_DOUBLE_EQ (1.0, v.Mag ());
}

// Test that this has been fixed:
// c:\documents and settings\oblong\src\yoiz\libloam\c++\vect.h(150): warning C4715: 'oblong::loam::Vect::Set' : not all control paths return a value [c:\Documents and Settings\Oblong\src\yoiz\libLoam\c++\win32\libLoamWin32C++.vcxproj]
TEST (VectTest, SetNull)
{
  Vect v (XZ);
  Vect *nv = NULL;
  Str s = v.Set (nv).AsStr ();
}

// Test that invalid Vects behave properly
TEST (VectTest, CheckInvalid)
{
  Vect v (1.0, 2.0, 3.0);
  Vect w = Vect::Invalid ();

  EXPECT_TRUE (v.IsValid ());
  EXPECT_FALSE (v.IsInvalid ());
  EXPECT_FALSE (w.IsValid ());
  EXPECT_TRUE (w.IsInvalid ());
  EXPECT_FALSE (v == w);
  EXPECT_TRUE (v != w);
  EXPECT_TRUE (v == v);
  EXPECT_FALSE (v != v);
  EXPECT_FALSE (w == w);
  EXPECT_TRUE (w != w);

  Vect u = v + w;
  EXPECT_FALSE (u.IsValid ());

  w.Set (2.0, 3.0, 4.0);
  EXPECT_TRUE (w.IsValid ());
}

TEST (VectTest, v3float32Conversion)
{
  float64 x64 = ob_rand_float64 ();
  float64 y64 = ob_rand_float64 ();
  float64 z64 = ob_rand_float64 ();
  float32 x32 = (float32) x64;
  float32 y32 = (float32) y64;
  float32 z32 = (float32) z64;
  Vect v64 (x64, y64, z64);

  EXPECT_EQ (x32, v64.ToV3Float32 ().x);
  EXPECT_EQ (y32, v64.ToV3Float32 ().y);
  EXPECT_EQ (z32, v64.ToV3Float32 ().z);
  EXPECT_EQ (x32, ((v3float32) v64).x);
  EXPECT_EQ (y32, ((v3float32) v64).y);
  EXPECT_EQ (z32, ((v3float32) v64).z);
}

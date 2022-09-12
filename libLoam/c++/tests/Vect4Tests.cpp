
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include <libLoam/c/ob-rand.h>
#include "libLoam/c++/Str.h"

#include "libLoam/c++/Vect4.h"

using namespace oblong::loam;

const Vect4 XZ (1, 0, 1, 0);

TEST (Vect4Test, InitVect4)
{
  Vect4 v;
  EXPECT_EQ (0.0, v.x);
  EXPECT_EQ (0.0, v.y);
  EXPECT_EQ (0.0, v.z);
  EXPECT_EQ (0.0, v.w);

  Vect4 v1 (1.0, 2.0, 3.0, 4.0);
  EXPECT_EQ (1.0, v1.x);
  EXPECT_EQ (2.0, v1.y);
  EXPECT_EQ (3.0, v1.z);
  EXPECT_EQ (4.0, v1.w);

  Vect4 v2 (v1);
  EXPECT_EQ (1.0, v2.x);
  EXPECT_EQ (2.0, v2.y);
  EXPECT_EQ (3.0, v2.z);
  EXPECT_EQ (4.0, v2.w);

  v4float32 f32 = v4float32{1.0, 2.0, 3.0, 4.0};
  Vect4 v3 (f32);
  EXPECT_EQ (1.0, v3.x);
  EXPECT_EQ (2.0, v3.y);
  EXPECT_EQ (3.0, v3.z);
  EXPECT_EQ (4.0, v3.w);
}

// Test that this has been fixed:
// c:\documents and settings\oblong\src\yoiz\libloam\c++\vect.h(150): warning C4715: 'oblong::loam::Vect4::Set' : not all control paths return a value [c:\Documents and Settings\Oblong\src\yoiz\libLoam\c++\win32\libLoamWin32C++.vcxproj]
TEST (Vect4Test, SetNull)
{
  Vect4 v (XZ);
  Vect4 *nv = NULL;
  Str s = v.Set (nv).AsStr ();
}

// Test that invalid Vect4s behave properly
TEST (Vect4Test, CheckInvalid)
{
  Vect4 v (1.0, 2.0, 3.0, 4.0);
  Vect4 w = Vect4::Invalid ();

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

  Vect4 u = v + w;
  EXPECT_FALSE (u.IsValid ());

  w.Set (2.0, 3.0, 4.0, 5.0);
  EXPECT_TRUE (w.IsValid ());
}

TEST (Vect4Test, v4float32Conversion)
{
  float64 x64 = ob_rand_float64 ();
  float64 y64 = ob_rand_float64 ();
  float64 z64 = ob_rand_float64 ();
  float64 w64 = ob_rand_float64 ();
  float32 x32 = (float32) x64;
  float32 y32 = (float32) y64;
  float32 z32 = (float32) z64;
  float32 w32 = (float32) w64;
  Vect4 v64 (x64, y64, z64, w64);

  EXPECT_EQ (x32, v64.ToV4Float32 ().x);
  EXPECT_EQ (y32, v64.ToV4Float32 ().y);
  EXPECT_EQ (z32, v64.ToV4Float32 ().z);
  EXPECT_EQ (w32, v64.ToV4Float32 ().w);
  EXPECT_EQ (x32, ((v4float32) v64).x);
  EXPECT_EQ (y32, ((v4float32) v64).y);
  EXPECT_EQ (z32, ((v4float32) v64).z);
  EXPECT_EQ (w32, ((v4float32) v64).w);
}


/* (c)  oblong industries */

// Unit tests for slaw-traits.h

#include "SlawTypesTest.h"

#include "libLoam/c++/LoamStreams.h"
#include "libPlasma/c++/PlasmaStreams.h"
#include <libPlasma/c++/slaw-traits.h>
#include <libPlasma/c++/SlawIterator.h>

using namespace oblong::plasma;
using namespace oblong::loam;

namespace {

class SlawTraitsTest : public SlawTypesTest
{
 protected:
  void wrapped_slaw_test (const Callbacks &c) override
  {
    for (size_t i = 0; i < c.Count (); ++i)
      {
        slaw s (c.CSlawFromNthValue (i));
        EXPECT_TRUE (c.CheckNthValue (bslaw (s), i));
        slaw_free (s);
      }
  }
};

DEFINE_SLAW_TESTS_FOR_CLASS (SlawTraitsTest);

TEST_F (SlawTraitsTest, SimpleArrays)
{
  const int32 values[] = {1, 8, 0, 42, -28};
  slaw s = slaw_traits<const int32 *>::Make (values, 5);
  EXPECT_TRUE (slaw_traits<const int32 *>::TypeP (s));
  EXPECT_NE (slaw (NULL), s);
  EXPECT_TRUE (slaw_is_numeric_array (s));
  EXPECT_EQ (5, slaw_numeric_array_count (s));
  const int32 *sv = slaw_cast<const int32 *> (s);
  EXPECT_NE ((const int32 *) (NULL), sv);
  for (size_t i = 0; i < 5; ++i)
    EXPECT_EQ (values[i], sv[i]);
  slaw_free (s);
}

TEST_F (SlawTraitsTest, SimpleVconstructors)
{
  v2int32 v = make_v2int32 (4, 1);
  EXPECT_EQ (4, v.x);
  EXPECT_EQ (1, v.y);
  v3float64 vf = make_v3float64 (1.0, 2.0, 3.0);
  EXPECT_DOUBLE_EQ (1.0, vf.x);
  EXPECT_DOUBLE_EQ (2.0, vf.y);
  EXPECT_DOUBLE_EQ (3.0, vf.z);
  v4unt8 v4 = make_v4unt8 (1, 2, 3, 4);
  EXPECT_EQ (1, v4.x);
  EXPECT_EQ (2, v4.y);
  EXPECT_EQ (3, v4.z);
  EXPECT_EQ (4, v4.w);
}

TEST_F (SlawTraitsTest, TroveArrays)
{
  ObTrove<v3float64> points;
  v3float64 a = {23.0, 24.0, 25.0};
  points.Append (a);
  v3float64 b = {26.0, 27.0, 28.0};
  points.Append (b);

  Slaw s_v3 (points);
  EXPECT_EQ (2, s_v3.Count ());

  ObTrove<v3float64> points_2 (s_v3.Emit<ObTrove<v3float64>> ());
  EXPECT_EQ (points.Count (), points_2.Count ());
  EXPECT_EQ (points.Nth (0), points_2.Nth (0));
  EXPECT_EQ (points.Nth (1), points_2.Nth (1));

  ObTrove<Vect> points_V;
  points_V.Append (Vect (23.0, 24.0, 25.0));
  points_V.Append (Vect (24.0, 25.0, 26.0));
  points_V.Append (Vect (27.0, 28.0, 29.0));

  Slaw s_V (points_V);
  EXPECT_EQ (3, s_V.Count ());

  ObTrove<Vect> points_V2 (s_V.Emit<ObTrove<Vect>> ());
  EXPECT_EQ (points_V.Count (), points_V2.Count ());
  EXPECT_EQ (points_V.Nth (0), points_V2.Nth (0));
  EXPECT_EQ (points_V.Nth (1), points_V2.Nth (1));
  EXPECT_EQ (points_V.Nth (2), points_V2.Nth (2));

  ObTrove<Vect> points_V3;
  ASSERT_TRUE (s_V.Into (points_V3));
  EXPECT_EQ (points_V.Count (), points_V.Count ());
  EXPECT_EQ (points_V.Nth (0), points_V3.Nth (0));
  EXPECT_EQ (points_V.Nth (1), points_V3.Nth (1));
  EXPECT_EQ (points_V.Nth (2), points_V3.Nth (2));
}

class Matrix44Test : public ::testing::Test
{
 public:
  const Matrix44 the_matrix;

  Matrix44Test ();
  void DoTest (const char *y);
};

Matrix44Test::Matrix44Test ()
    : the_matrix (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
{
}

void Matrix44Test::DoTest (const char *y)
{
  ObRetort tort;
  Str s ("%YAML 1.1\n%TAG ! tag:oblong.com,2009:slaw/\n---\n");
  s.Append (y);
  Slaw yam (Slaw::FromString (s, &tort));
  EXPECT_TRUE (tort.IsSplend ());
  Matrix44 m1 (yam.Emit<Matrix44> ());
  EXPECT_EQ (the_matrix, m1);
  Matrix44 m2;
  EXPECT_TRUE (yam.Into (m2));
  EXPECT_EQ (the_matrix, m2);
  EXPECT_TRUE (yam.CanEmit<Matrix44> ());

  // Here's an evil variation
  // (Unfortunately, replacing a list element with itself is only
  // a nop if the code is working properly, and often it isn't.)
  if (yam.IsList ())
    {
      Slaw evil (yam.ListReplaceNth (0, yam.Nth (0)));
      EXPECT_TRUE (evil.CanEmit<Matrix44> ());
      Matrix44 m3 (evil.Emit<Matrix44> ());
      EXPECT_EQ (the_matrix, m3);
      Matrix44 m4;
      EXPECT_TRUE (evil.Into (m4));
      EXPECT_EQ (the_matrix, m4);
    }

  // Now let's try some variations that *shouldn't* work.
  if (yam.IsList ())
    {
      const Slaw wrench ("monkey");
      const int64 vonCount = yam.Count ();
      for (int64 i = 0; i < vonCount; i++)
        {
          Slaw x (yam.ListReplaceNth (i, wrench));
          if (0 == i % 2)
            // Sadly, whether we execute this line or not can make a
            // difference, due to the way Jao caches things in Slaw
            // in a way that can become inconsistent.  Blech!
            x.ToString ();
          EXPECT_FALSE (x.CanEmit<Matrix44> ());
          Matrix44 the_woods;
          EXPECT_FALSE (x.Into (the_woods));
          the_woods = x.Emit<Matrix44> ();
          EXPECT_EQ (Matrix44::idMat, the_woods);
        }
    }
}

TEST_F (Matrix44Test, RoundTrip)
{
  Slaw s_m (the_matrix);
  EXPECT_EQ (4, s_m.Count ());

  Matrix44 the_matrix_reloaded (s_m.Emit<Matrix44> ());
  EXPECT_EQ (the_matrix, the_matrix_reloaded);

  Matrix44 the_matrix_revolutions;
  ASSERT_TRUE (s_m.Into (the_matrix_revolutions));
  EXPECT_EQ (the_matrix, the_matrix_revolutions);

  Matrix44 emit_failed (Slaw ("not a matrix").Emit<Matrix44> ());
  EXPECT_EQ (Matrix44::idMat, emit_failed);
}

TEST_F (Matrix44Test, V4Float64Array)
{
  DoTest ("!array [!vector [0.0, 1.0, 2.0, 3.0], "
          "!vector [4.0, 5.0, 6.0, 7.0], "
          "!vector [8.0, 9.0, 10.0, 11.0], "
          "!vector [12.0, 13.0, 14.0, 15.0]]");
}

TEST_F (Matrix44Test, Float64Array)
{
  DoTest ("!array [0.0, 1.0, 2.0, 3.0, "
          "4.0, 5.0, 6.0, 7.0, "
          "8.0, 9.0, 10.0, 11.0, "
          "12.0, 13.0, 14.0, 15.0]");
}

TEST_F (Matrix44Test, V4Float64List)
{
  DoTest ("[!vector [0.0, 1.0, 2.0, 3.0], "
          "!vector [4.0, 5.0, 6.0, 7.0], "
          "!vector [8.0, 9.0, 10.0, 11.0], "
          "!vector [12.0, 13.0, 14.0, 15.0]]");
}

TEST_F (Matrix44Test, Float64List)
{
  DoTest ("[0.0, 1.0, 2.0, 3.0, "
          "4.0, 5.0, 6.0, 7.0, "
          "8.0, 9.0, 10.0, 11.0, "
          "12.0, 13.0, 14.0, 15.0]");
}

TEST_F (Matrix44Test, Float64ArrayList)
{
  DoTest ("[!array [0.0, 1.0, 2.0, 3.0], "
          "!array [4.0, 5.0, 6.0, 7.0], "
          "!array [8.0, 9.0, 10.0, 11.0], "
          "!array [12.0, 13.0, 14.0, 15.0]]");
}

TEST (QuatTest, V4Float64)
{
  v4float64 v = {0, 1, 2, 3};
  Slaw s (v);
  Quat q;
  EXPECT_TRUE (s.Into (q));
  EXPECT_EQ (Quat (0, 1, 2, 3), q);
}

TEST (QuatTest, Float64Array)
{
  float64 a[] = {0, 1, 2, 3};
  Slaw s (a, 4);
  Quat q;
  EXPECT_TRUE (s.Into (q));
  EXPECT_EQ (Quat (0, 1, 2, 3), q);
}

TEST (QuatTest, V4Float32)
{
  v4float32 v = {0, 1, 2, 3};
  Slaw s (v);
  Quat q = s.Emit<Quat> ();
  EXPECT_EQ (v.x, q.a);
  EXPECT_EQ (v.y, q.i);
  EXPECT_EQ (v.z, q.j);
  EXPECT_EQ (v.w, q.k);

  EXPECT_FALSE (s.Is<Quat> ());
}

TEST (Vect4Test, V4Float64)
{
  v4float64 v = {0, 1, 2, 3};
  Slaw s (v);
  Vect4 ve;
  EXPECT_TRUE (s.Into (ve));
  EXPECT_EQ (Vect4 (0, 1, 2, 3), ve);
}

TEST (Vect4Test, Float64Array)
{
  float64 a[] = {0, 1, 2, 3};
  Slaw s (a, 4);
  Vect4 ve;
  EXPECT_TRUE (s.Into (ve));
  EXPECT_EQ (Vect4 (0, 1, 2, 3), ve);
}

TEST (Vect4Test, V4Float32)
{
  v4float32 v = {0, 1, 2, 3};
  Slaw s (v);
  Vect4 ve = s.Emit<Vect4> ();
  EXPECT_EQ (v.x, ve.x);
  EXPECT_EQ (v.y, ve.y);
  EXPECT_EQ (v.z, ve.z);
  EXPECT_EQ (v.w, ve.w);

  EXPECT_FALSE (s.Is<Vect4> ());
}

TEST (VectTest, V3Float64)
{
  v3float64 v = {0, 1, 2};
  Slaw s (v);
  Vect ve;
  EXPECT_TRUE (s.Into (ve));
  EXPECT_EQ (Vect (0, 1, 2), ve);
}

TEST (VectTest, Float64Array)
{
  float64 a[] = {0, 1, 2};
  Slaw s (a, 3);
  Vect ve;
  EXPECT_TRUE (s.Into (ve));
  EXPECT_EQ (Vect (0, 1, 2), ve);
}

TEST (VectTest, V3Float32)
{
  v3float32 v = {0, 1, 2};
  Slaw s (v);
  Vect ve = s.Emit<Vect> ();
  EXPECT_EQ (v.x, ve.x);
  EXPECT_EQ (v.y, ve.y);
  EXPECT_EQ (v.z, ve.z);

  EXPECT_FALSE (s.Is<Vect> ());
}

TEST (SlawTest, RoundTrip)
{
  const Slaw a = Slaw ("hi");
  EXPECT_EQ (Slaw ("hi"), a.Emit<Slaw> ());
  const Slaw b = Slaw (true);
  EXPECT_EQ (Slaw (true), b.Emit<Slaw> ());
}

}  // namespace

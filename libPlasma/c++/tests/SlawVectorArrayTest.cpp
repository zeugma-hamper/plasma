
/* (c)  oblong industries */

#include "libLoam/c++/LoamStreams.h"
#include "libPlasma/c++/Plasma.h"

#include <gtest/gtest.h>

using namespace oblong::plasma;

namespace {

const int64 COUNT = 100;

TEST (SlawVectorArrayTest, v4float64)
{
  v4float64 v = {1.0, 1.5, 2.0, 2.5};

  Slaw s (slaw_v4float64_array_filled (COUNT, v));
  EXPECT_EQ (COUNT, s.Count ());
  for (int64 i = 0; i < COUNT; i++)
    {
      Slaw q = s.Nth (i);
      v4float64 u;
      EXPECT_TRUE (q.Into (u));
      EXPECT_EQ (v, u);
      EXPECT_EQ (v, q.Emit<v4float64> ());
      EXPECT_TRUE (q.Is<v4float64> ());
      EXPECT_TRUE (slaw_is_v4float64 (q.SlawValue ()));
    }
}

TEST (SlawVectorArrayTest, v4float32)
{
  v4float32 v = {0.1, 5.1, 0.2, 5.2};

  Slaw s (slaw_v4float32_array_filled (COUNT, v));
  EXPECT_EQ (COUNT, s.Count ());
  for (int64 i = 0; i < COUNT; i++)
    {
      Slaw q = s.Nth (i);
      v4float32 u;
      EXPECT_TRUE (q.Into (u));
      EXPECT_EQ (v, u);
      EXPECT_EQ (v, q.Emit<v4float32> ());
      EXPECT_TRUE (q.Is<v4float32> ());
      EXPECT_TRUE (slaw_is_v4float32 (q.SlawValue ()));
    }
}

#if 0  // these don't even compile!
TEST(SlawVectorArrayTest, v3unt16c)
{
  v3unt16c v;
  v.x.re = 0;
  v.x.im = 1;
  v.y.re = 2;
  v.y.im = 3;
  v.z.re = 4;
  v.z.im = 5;

  Slaw s (slaw_v3unt16c_array_filled (COUNT, v));
  EXPECT_EQ (COUNT, s.Count());
  for (int64 i = 0 ; i < COUNT ; i++)
    { Slaw q = s.Nth (i);
      v3unt16c u;
      bool ok = q.Emit(u);
      EXPECT_TRUE (ok);
      int result = memcmp (&v, &u, sizeof (v));
      EXPECT_EQ (0, result);
      bool good = slaw_is_v3unt16c (q.SlawValue ());
      EXPECT_TRUE (good);
    }
}

TEST(SlawVectorArrayTest, float64c)
{
  float64c v;
  v.re = 1.25;
  v.im = 3.75;

  Slaw s (slaw_float64c_array_filled (COUNT, v));
  EXPECT_EQ (COUNT, s.Count());
  for (int64 i = 0 ; i < COUNT ; i++)
    { Slaw q = s.Nth (i);
      float64c u;
      bool ok = q.Emit(u);
      EXPECT_TRUE (ok);
      int result = memcmp (&v, &u, sizeof (v));
      EXPECT_EQ (0, result);
      bool good = slaw_is_float64c (q.SlawValue ());
      EXPECT_TRUE (good);
    }
}

TEST(SlawVectorArrayTest, m5float64)
{
  m5float64 v;
  for (int i = 0 ; i < 32 ; i++)
    v.coef[i] = i + 0.25;

  Slaw s (slaw_m5float64_array_filled (COUNT, v));
  EXPECT_EQ (COUNT, s.Count());
  for (int64 i = 0 ; i < COUNT ; i++)
    { Slaw q = s.Nth (i);
      m5float64 u;
      bool ok = q.Emit(u);
      EXPECT_TRUE (ok);
      int result = memcmp (&v, &u, sizeof (v));
      EXPECT_EQ (0, result);
      bool good = slaw_is_m5float64 (q.SlawValue ());
      EXPECT_TRUE (good);
    }
}
#endif

}  // namespace

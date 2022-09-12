
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include <stdio.h>
#include "libLoam/c/ob-rand.h"

TEST (ObMath, Basic)
{
  const int N = 1000000;
  const double range = 100000;
  double y;

  ob_rand_seed_int32 ();
  ob_rand_t *state = ob_rand_allocate_state ();

  // basic rand contract: [0,1)
  for (int i = 0; i < N; i++)
    {
      y = ob_rand_float64 ();
      ASSERT_GE (y, 0);
      ASSERT_LT (y, 1.0);

      y = ob_rand_float64 ();
      ASSERT_GE (y, 0);
      ASSERT_LT (y, 1.0);

      y = ob_rand_state_float64 (0.0, 1.0, state);
      ASSERT_GE (y, 0);
      ASSERT_LT (y, 1.0);
    }

  // test ob_rand_float64(low, high) -- double version
  for (int i = 0; i < N; i++)
    {
      double a, b;
      while (true)
        {
          a = (ob_rand_float64 () - 0.5) * range;
          b = (ob_rand_float64 () - 0.5) * range;
          if (a == b)
            continue;  // unclear what should happen in this case
          if (a > b)
            {
              double t = a;
              a = b;
              b = t;
            }
          break;
        }

      double v = ob_rand_float64 (a, b);
      if (v < a || v >= b)
        printf ("Error: ob_rand_float64(low,high) -- %f (%f) %f\n", a, v, b);
      ASSERT_GE (v, a);
      ASSERT_LT (v, b);

      v = ob_rand_state_float64 (a, b, state);
      if (v < a || v >= b)
        printf ("Error: ob_rand_state_float64(low,high,state) -- %f (%f) %f\n",
                a, v, b);
      ASSERT_GE (v, a);
      ASSERT_LT (v, b);
    }

  // test ob_rand_int32
  for (int i = 0; i < N; i++)
    {
      int a, b;
      while (true)
        {
          a = (int) ((ob_rand_float64 () - 0.5) * range);
          b = (int) ((ob_rand_float64 () - 0.5) * range);
          if (a == b)
            continue;
          if (a > b)
            {
              int t = a;
              a = b;
              b = t;
            }
          break;
        }

      int v = ob_rand_int32 (a, b);
      if (v < a || v >= b)
        printf ("Error: ob_rand_int32(low,high) -- %d (%d) %d\n", a, v, b);
      ASSERT_GE (v, a);
      ASSERT_LT (v, b);

      v = ob_rand_state_int32 (a, b, state);
      if (v < a || v >= b)
        printf ("Error: ob_rand_state_int32(low,high,state) -- %d (%d) %d\n", a,
                v, b);
      ASSERT_GE (v, a);
      ASSERT_LT (v, b);
    }

  ob_rand_free_state (state);
}

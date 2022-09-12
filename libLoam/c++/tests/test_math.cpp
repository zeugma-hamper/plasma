
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include "libLoam/c++/ob-math-utils.h"

TEST (ObMath, MinMax)
{
  EXPECT_EQ (obMin (10, 20), 10);
  EXPECT_EQ (obMin (10.5, 20.5), 10.5);
  EXPECT_EQ (obMax (10, 20), 20);
  EXPECT_EQ (obMax (10.5, 20.5), 20.5);
  EXPECT_EQ (obMin (20, 10), 10);
  EXPECT_EQ (obMin (20.5, 10.5), 10.5);
  EXPECT_EQ (obMax (20, 10), 20);
  EXPECT_EQ (obMax (20.5, 10.5), 20.5);
  // verify that side effects only occur once
  {
    int a = 10, b = 20;
    EXPECT_EQ (obMin (a++, b++), 10);
    EXPECT_EQ (a, 11);
    EXPECT_EQ (b, 21);
  }
  {
    int a = 10, b = 20;
    EXPECT_EQ (obMax (a++, b++), 20);
    EXPECT_EQ (a, 11);
    EXPECT_EQ (b, 21);
  }
}

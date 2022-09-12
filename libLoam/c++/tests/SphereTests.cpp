
/* (c)  oblong industries */

#include "gtest/gtest.h"
#include <unordered_set>
#include "libLoam/c++/Sphere.h"

using namespace oblong::loam;

TEST (SphereTests, Sphere_Constructors)
{
  const auto sphere = Sphere (Vect (1, 0, 0), 1);
  EXPECT_TRUE (sphere.IsValid ());
  EXPECT_GT (sphere.AsStr ().Length (), 0);

  const auto invalid = Sphere::Invalid ();
  EXPECT_FALSE (invalid.IsValid ());
  EXPECT_GT (invalid.AsStr ().Length (), 0);
}


/* (c)  oblong industries */

#include "gtest/gtest.h"
#include <unordered_set>
#include "libLoam/c++/Frustum.h"

using namespace oblong::loam;

TEST (FrustumTests, Frustum_Constructors)
{
  std::array<Vect, 8> frustcorners = {
    {Vect (-1, -1, 1), Vect (1, -1, 1), Vect (-1, 1, 1), Vect (1, 1, 1),
     Vect (-2, -2, -1), Vect (2, -2, -1), Vect (-2, 2, -1), Vect (2, 2, -1)}};
  Frustum frust = Frustum (frustcorners);
  EXPECT_TRUE (frust.IsValid ());
}

TEST (FrustumTests, Frustum_IsValid)
{
  EXPECT_FALSE (Frustum::Invalid ().IsValid ());
  EXPECT_TRUE (Frustum::Invalid ().AsStr ().Length () > 0);
}

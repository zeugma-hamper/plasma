
/* (c)  oblong industries */

#include "gtest/gtest.h"
#include <unordered_set>
#include "libLoam/c++/Rectangle.h"

using namespace oblong::loam;

TEST (RectangleTests, Rectangle_Constructors)
{
  const auto pgram = Rectangle (Vect (), Vect (1, 0, 0), Vect (0, 1, 0));
  EXPECT_TRUE (pgram.IsValid ());
  EXPECT_GT (pgram.AsStr ().Length (), 0);

  const auto invalid = Rectangle::Invalid ();
  EXPECT_FALSE (invalid.IsValid ());
  EXPECT_GT (invalid.AsStr ().Length (), 0);
}

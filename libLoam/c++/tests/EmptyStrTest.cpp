
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include "libLoam/c++/Str.h"

using namespace oblong::loam;


TEST (EmptyStrTest, Explicit)
{
  Str s ("");

  ASSERT_STREQ ("", s.utf8 ());
}

TEST (EmptyStrTest, Implicit)
{
  Str s;

  ASSERT_STREQ ("", s.utf8 ());
}

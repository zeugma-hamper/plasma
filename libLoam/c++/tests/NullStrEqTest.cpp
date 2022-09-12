
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include "libLoam/c++/Str.h"

using namespace oblong::loam;


TEST (NullStrEqTest, IsNull)
{
  Str s ("foo");
  Str t;
  Str *sp = &s;
  Str *tp = &t;

  ASSERT_FALSE (s.IsNull ());
  ASSERT_TRUE (t.IsNull ());
  ASSERT_FALSE (s == t);
  ASSERT_FALSE (t == s);
  ASSERT_TRUE (s != t);
  ASSERT_TRUE (t != s);

  ASSERT_FALSE (sp->IsNull ());
  ASSERT_TRUE (tp->IsNull ());
  ASSERT_FALSE (*sp == *tp);
  ASSERT_FALSE (*tp == *sp);
  ASSERT_TRUE (*sp != *tp);
  ASSERT_TRUE (*tp != *sp);
}


TEST (NullStrEqTest, RealNULL)
{
  Str s ("foo");
  Str t;
  Str *sp = &s;
  Str *tp = &t;

  ASSERT_FALSE (s.IsNull ());
  ASSERT_FALSE (s == NULL);
  ASSERT_FALSE (NULL == s);
  ASSERT_TRUE (s != NULL);
  ASSERT_TRUE (NULL != s);

  ASSERT_TRUE (t.IsNull ());
  ASSERT_TRUE (t == NULL);
  ASSERT_TRUE (NULL == t);
  ASSERT_FALSE (t != NULL);
  ASSERT_FALSE (NULL != t);

  ASSERT_FALSE (sp->IsNull ());
  ASSERT_FALSE (*sp == NULL);
  ASSERT_FALSE (NULL == *sp);
  ASSERT_TRUE (*sp != NULL);
  ASSERT_TRUE (NULL != *sp);

  ASSERT_TRUE (tp->IsNull ());
  ASSERT_TRUE (*tp == NULL);
  ASSERT_TRUE (NULL == *tp);
  ASSERT_FALSE (*tp != NULL);
  ASSERT_FALSE (NULL != *tp);
}

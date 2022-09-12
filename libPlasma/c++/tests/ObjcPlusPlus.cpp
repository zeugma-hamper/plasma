
/* (c)  oblong industries */

#import <Foundation/Foundation.h>

#include "libPlasma/c++/Plasma.h"

#include <gtest/gtest.h>
#include <libLoam/c/ob-pthread.h>
#include <libLoam/c/ob-time.h>

using namespace oblong::plasma;

TEST (ObjcPlusPlus, NilNull)
{
  Slaw s (Slaw::NilSlaw ());
  EXPECT_TRUE (s.IsNil ());
  EXPECT_FALSE (s.IsNull ());
  Slaw t;
  EXPECT_FALSE (t.IsNil ());
  EXPECT_TRUE (t.IsNull ());
}

TEST (ObjcPlusPlus, Preprocessor)
{
  bool cplus = false;
  bool objc = false;
#ifdef __cplusplus
  cplus = true;
#endif
#ifdef __OBJC__
  objc = true;
#endif
  EXPECT_TRUE (cplus);
  EXPECT_TRUE (objc);
}

TEST (ObjcPlusPlus, Mixing)
{
  NSString *string = @"999";
  Slaw s ([string intValue]);
  EXPECT_EQ (OB_CONST_U64 (999), s.Emit<unt64> ());
}

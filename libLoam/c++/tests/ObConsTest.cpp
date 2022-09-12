
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include "libLoam/c++/ObTrove.h"
#include "libLoam/c++/ObCons.h"
#include "libLoam/c++/Str.h"

using namespace oblong::loam;

typedef ObCons<Str, Str> StrPair;

TEST (ObConsTest, TroveOfPointers)
{
  ObTrove<StrPair *> t;
  t.Append (new StrPair ("one", "two"));
  t.Append (new StrPair ("three", "four"));

  StrPair *needle;
  EXPECT_EQ (-1, t.Find (needle = new StrPair ("one", "two")));
  needle->Delete ();
  EXPECT_STREQ ("one", t.Nth (0)->Car ());
  EXPECT_STREQ ("two", t.Nth (0)->Cdr ());
  EXPECT_STREQ ("three", t.Nth (1)->Car ());
  EXPECT_STREQ ("four", t.Nth (1)->Cdr ());
  EXPECT_EQ (-1, t.Find (needle = new StrPair ("three", "four")));
  needle->Delete ();
  EXPECT_EQ (-1, t.Find (needle = new StrPair ("five", "six")));
  needle->Delete ();
}


TEST (ObConsTest, TroveOfConses)
{
  ObTrove<StrPair> t;
  t.Append (StrPair ("one", "two"));
  t.Append (StrPair ("three", "four"));

  EXPECT_EQ (0, t.Find (StrPair ("one", "two")));
  EXPECT_STREQ ("one", t.Nth (0).Car ());
  EXPECT_STREQ ("two", t.Nth (0).Cdr ());
  EXPECT_STREQ ("three", t.Nth (1).Car ());
  EXPECT_STREQ ("four", t.Nth (1).Cdr ());
  EXPECT_EQ (1, t.Find (StrPair ("three", "four")));
  EXPECT_EQ (-1, t.Find (StrPair ("five", "six")));
}

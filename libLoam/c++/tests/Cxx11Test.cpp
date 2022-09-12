
/* (c)  oblong industries */

#include <gtest/gtest.h>

#include "libLoam/c++/ObAcacia.h"
#include "libLoam/c++/ObMap.h"
#include "libLoam/c++/ObTrove.h"
#include "libLoam/c++/Str.h"

#include <boost/type_traits.hpp>

using namespace oblong::loam;

static const Str colors[] = {"blue",   "brown",  "fuchsia", "green",
                             "maroon", "orange", "purple",  "red"};
static const size_t n_colors = (sizeof (colors) / sizeof (colors[0]));

typedef ObTrove<Str> StrTrove;
typedef ObAcacia<Str> StrAcacia;
typedef ObMap<Str, int64> StrMap;

TEST (Cxx11Test, ForeachTrove)
{
  StrTrove t (colors, n_colors);

  size_t i = 0;
  for (auto s : t)
    EXPECT_STREQ (colors[i++], s);

  EXPECT_EQ (n_colors, i);
}

TEST (Cxx11Test, ForeachAcacia)
{
  StrAcacia a;

  for (auto s : colors)
    a.Append (s);

  size_t i = 0;
  for (auto s : a)
    EXPECT_STREQ (colors[i++], s);

  EXPECT_EQ (n_colors, i);
}

TEST (Cxx11Test, ForeachMap)
{
  StrMap m;

  for (auto s : colors)
    m.Put (s, s.Index ("r"));

  size_t i = 0;
  for (auto c : m)
    {
      EXPECT_STREQ (colors[i], c->Car ());
      EXPECT_EQ (colors[i++].Index ("r"), c->Cdr ());
    }

  EXPECT_EQ (n_colors, i);
}

enum class Simple_Enum_Class
{
  ValueA,
  ValueB
};

Str GetValue (Simple_Enum_Class c)
{
  switch (c)
    {
      case Simple_Enum_Class::ValueA:
        return Str ("ValueA");
      case Simple_Enum_Class::ValueB:
        return Str ("ValueB");
      default:
        return Str ("default");
    }
}

TEST (Cxx11Test, SimpleEnumClassesWork)
{
  EXPECT_STREQ ("ValueA", GetValue (Simple_Enum_Class::ValueA));
  EXPECT_STREQ ("ValueB", GetValue (Simple_Enum_Class::ValueB));
}

TEST (Cxx11Test, BoostVariadic)
{
  // max template arguments for common_type is 9 if
  // BOOST_NO_CXX11_VARIADIC_TEMPLATES is defined
  boost::common_type<float32, float32, float32, float32, float32, float32,
                     float32, float32, float32, float32>::type variable;

  (void) variable;
}

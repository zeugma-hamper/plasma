
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include <libLoam/c++/Str.h>
#include <unicode/unistr.h>

using namespace oblong::loam;

namespace {

Str arabic (
  UNICODE_STRING_SIMPLE ("\\u0627\\u0628\\u0629\\u062a\\u062b").unescape ());
}

TEST (StrMarkerTest, BeginEndForEmptyStrings)
{
  Str str = Str ();
  str_marker beg = str.begin_marker ();
  str_marker end = str.end_marker ();
  EXPECT_EQ (0, beg);
  EXPECT_EQ (0, end);

  beg = str.begin_bounds_marker ();
  end = str.end_bounds_marker ();
  EXPECT_EQ (0, beg);
  EXPECT_EQ (0, end);

  UnicodeString empty_unicode = UNICODE_STRING_SIMPLE ("").unescape ();
  str = Str (empty_unicode);
  beg = str.begin_marker ();
  end = str.end_marker ();
  EXPECT_EQ (0, beg);
  EXPECT_EQ (0, end);

  beg = str.begin_bounds_marker ();
  end = str.end_bounds_marker ();
  EXPECT_EQ (0, beg);
  EXPECT_EQ (0, end);

  str = "";
  beg = str.begin_marker ();
  end = str.end_marker ();
  EXPECT_EQ (0, beg);
  EXPECT_EQ (0, end);

  beg = str.begin_bounds_marker ();
  end = str.end_bounds_marker ();
  EXPECT_EQ (0, beg);
  EXPECT_EQ (0, end);
}

TEST (StrMarkerTest, BeginEnd)
{
  Str str (arabic);
  str_marker beg = str.begin_marker ();
  str_marker end = str.end_marker ();
  EXPECT_EQ (0, beg);
  EXPECT_EQ (4, end);

  beg = str.begin_bounds_marker ();
  end = str.end_bounds_marker ();
  EXPECT_EQ (-1, beg);
  EXPECT_EQ (str.end_marker () + 1, end);

  str = Str (str.utf8 ());
  beg = str.begin_marker ();
  EXPECT_EQ (0, beg);
  end = str.end_marker ();
  EXPECT_EQ (8, end);

  beg = str.begin_bounds_marker ();
  end = str.end_bounds_marker ();
  EXPECT_EQ (-1, beg);
  EXPECT_EQ (str.end_marker () + 1, end);
}

void ForwardMarkerTest (const Str &str, const char *trace)
{
  SCOPED_TRACE (trace);
  str_marker m (str.begin_marker ());
  for (int64 index = 0, len = str.Length (); index < len; ++index)
    {
      UChar32 fetched = str.FetchAndMoveMarker (&m, 1);
      UChar32 atted = str[index];
      EXPECT_EQ (fetched, atted) << index << "th iteration failed";
    }
  EXPECT_GT (m, str.end_marker ());
  EXPECT_GE (m, str.end_bounds_marker ());
}

TEST (StrMarkerTest, ForwardIteration)
{
  ForwardMarkerTest (arabic, "Arabic Str");
  ForwardMarkerTest (Str (arabic.utf8 ()), "Arabic UTF8 Str");
}

void BackwardMarkerTest (const Str &str, const char *trace)
{
  SCOPED_TRACE (trace);
  str_marker m (str.end_marker ());
  for (int64 index = str.Length () - 1; index > 0; --index)
    {
      UChar32 fetched = str.FetchAndMoveMarker (&m, -1);
      UChar32 atted = str[index];
      EXPECT_EQ (fetched, atted) << index << "th iteration failed";
    }
  EXPECT_LT (m, str.end_marker ());
  EXPECT_LE (m, str.end_bounds_marker ());
}

TEST (StrMarkerTest, BackwardIteration)
{
  BackwardMarkerTest (arabic, "Arabic Str");
  BackwardMarkerTest (Str (arabic.utf8 ()), "Arabic UTF8 Str");
}

void ForwardIteratorTest (const Str &str, const char *trace)
{
  SCOPED_TRACE (trace);
  int64 index = 0;
  /// TODO: i != e produces an infinite loop!!!
  for (StrIterator i (str.begin ()), e (str.end ()); i < e; i++, ++index)
    {
      EXPECT_EQ (str[index], *i) << index << "th iteration failed";
    }
  EXPECT_EQ (str.Length (), index);

  index = 0;
  for (StrIterator i (str.begin ()), e (str.end ()); i < e; ++i, ++index)
    {
      EXPECT_EQ (str[index], *i) << index << "th iteration failed";
    }
  EXPECT_EQ (str.Length (), index);
}

TEST (StrIteratorTest, Forward)
{
  ForwardIteratorTest (arabic, "Arabic Str");
  ForwardIteratorTest (Str (arabic.utf8 ()), "Arabic UTF8 Str");
}

void BackwardIteratorTest (const Str &str, const char *trace)
{
  SCOPED_TRACE (trace);
  int64 index = str.Length () - 1;
  for (StrIterator i (--str.end ()), e (str.begin ()); i >= e; i--, --index)
    {
      EXPECT_EQ (str[index], *i) << index << "th iteration failed";
    }
  EXPECT_EQ (-1, index);

  index = str.Length () - 1;
  for (StrIterator i (--str.end ()), e (str.begin ()); i >= e; i--, --index)
    {
      EXPECT_EQ (str[index], *i) << index << "th iteration failed";
    }
  EXPECT_EQ (-1, index);
}

TEST (StrIteratorTest, Backward)
{
  BackwardIteratorTest (arabic, "Arabic Str");
  BackwardIteratorTest (Str (arabic.utf8 ()), "Arabic UTF8 Str");
}

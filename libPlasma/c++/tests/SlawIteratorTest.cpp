
/* (c)  oblong industries */

#include "SlawTypesTest.h"

#include "libLoam/c++/LoamStreams.h"
#include "libPlasma/c++/PlasmaStreams.h"
#include <libPlasma/c++/SlawIterator.h>

#include <algorithm>
#include <iterator>

using namespace oblong::plasma;

namespace {

class SlawIteratorTest : public SlawTypesTest
{
  void basic_iteration_test (const Callbacks &c)
  {
    std::vector<slaw> slawx;
    for (size_t i = 0; i < c.Count (); ++i)
      slawx.push_back (c.CSlawFromNthValue (i));
    Slaw s = Slaw::ListCollect (slawx.begin (), slawx.end ());

    EXPECT_EQ (SlawIterator::difference_type (c.Count ()),
               s.end () - s.begin ());

    int64 idx (0);
    int64 n (c.Count ());

    for (SlawIterator i (s.begin ()), e (s.end ()); i != e; ++idx, i++)
      EXPECT_EQ (c.SlawFromNthValue (idx), *i);
    EXPECT_EQ (n, idx);

    idx = 0;
    for (SlawIterator i (s.begin ()), e (s.end ()); i != e; ++idx, ++i)
      EXPECT_EQ (c.SlawFromNthValue (idx), *i);
    EXPECT_EQ (n, idx);

    idx = n - 1;
    for (SlawIterator i (s.end () - 1), e (s.end ()); i != e; --idx, --i)
      EXPECT_EQ (c.SlawFromNthValue (idx), *i);
    EXPECT_EQ (-1, idx);
  }

  void reverse_iterator_test (const Callbacks &c)
  {
// Versions of gcc prior to 4.2 are not able to compile this code.
#if _GNU_SOURCE && __GNUC__ >= 4 && __GNUC_MINOR__ >= 2
    std::vector<slaw> slawx;
    for (size_t i = 0; i < c.Count (); ++i)
      slawx.push_back (c.CSlawFromNthValue (i));
    Slaw s = Slaw::ListCollect (slawx.begin (), slawx.end ());

    int64 n (c.Count ());
    int64 idx (n - 1);

    typedef ::std::reverse_iterator<SlawIterator> RSlawIterator;
    for (RSlawIterator i (s.end ()), e (s.begin ()); i != e; --idx, ++i)
      EXPECT_EQ (c.SlawFromNthValue (idx), *i);
    EXPECT_EQ (-1, idx);
#endif
  }

  void wrapped_slaw_test (const Callbacks &c) override
  {
    basic_iteration_test (c);
    reverse_iterator_test (c);
  }
};

DEFINE_SLAW_TESTS_FOR_CLASS (SlawIteratorTest);

TEST_F (SlawIteratorTest, Count)
{
  Slaw s = Slaw::List (1, 2, 2, 3, 2);
  size_t cnt = ::std::count (s.begin (), s.end (), Slaw (2));
  EXPECT_EQ (size_t (3), cnt);
}

TEST_F (SlawIteratorTest, Find)
{
  Slaw s = Slaw::List (1, 2, 2, 3, 2);
  SlawIterator it = ::std::find (s.begin (), s.end (), Slaw (2));
  EXPECT_NE (s.end (), it);
  EXPECT_EQ (Slaw (2), *it);
  EXPECT_EQ (SlawIterator::difference_type (1), it - s.begin ());

  it = ::std::find (s.begin (), s.end (), Slaw (3));
  EXPECT_NE (s.end (), it);
  EXPECT_EQ (Slaw (3), *it);
  EXPECT_EQ (SlawIterator::difference_type (3), it - s.begin ());

  it = ::std::find (s.begin (), s.end (), Slaw (42));
  EXPECT_EQ (s.end (), it);
}

TEST_F (SlawIteratorTest, SlawWithNegativeCount)
{
  Slaw s =
    Slaw::FromString ("!<tag:oblong.com,2009:slaw/protein>\ndescrips:\n...");
  EXPECT_EQ (s.Count (), -1);
  EXPECT_EQ (s.begin (), s.end ());
}

}  // namespace

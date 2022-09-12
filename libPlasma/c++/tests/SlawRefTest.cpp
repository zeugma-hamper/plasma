
/* (c)  oblong industries */

// Unit tests for SlawRef

#include "SlawTypesTest.h"

#include "libLoam/c++/LoamStreams.h"
#include <libPlasma/c++/SlawRef.h>

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace oblong::plasma;
using namespace oblong::plasma::detail;
using namespace oblong::loam;

namespace {

class SlawRefTest : public SlawTypesTest
{
 protected:
  void assignment_tests (const Callbacks &c)
  {
    SCOPED_TRACE ("Assignment tests");
    for (size_t j = 0; j < c.Count (); ++j)
      {
        SlawRef s (c.CSlawFromNthValue (j));
        SlawRef t (s);
        SlawRef u (t);
        EXPECT_EQ (t, u);
        s = u;
        t = t = u;
        EXPECT_EQ (t, t);
        EXPECT_EQ (u, t);
        EXPECT_EQ (s, t);
      }
  }

  void containment_tests (const Callbacks &c)
  {
    SCOPED_TRACE ("Containment tests");
    std::vector<SlawRef> sl;
    std::map<Str, SlawRef> ml;
    std::vector<Str> keys;
    for (size_t j = 0; j < c.Count (); ++j)
      {
        std::ostringstream os ("arg");
        os << j;
        keys.push_back (Str (os.str ().c_str ()));
      }
    for (size_t j = 0; j < c.Count (); ++j)
      {
        slaw s (c.CSlawFromNthValue (j));
        SlawRef ss (s);
        ml[keys[j]] = ss;
        sl.push_back (ss);
      }
    for (size_t j = 0; j < c.Count (); ++j)
      {
        EXPECT_TRUE (c.CheckNthValue (sl[j], j)) << j << "th iteration";
        EXPECT_TRUE (c.CheckNthValue (ml[keys[j]], j)) << j << "th iteration";
      }
  }

  void wrapped_slaw_test (const Callbacks &c) override
  {
    containment_tests (c);
    assignment_tests (c);
  }
};

DEFINE_SLAW_TESTS_FOR_CLASS (SlawRefTest);

}  // namespace


/* (c)  oblong industries */

// Unit tests for SlawCons

#include "SlawTypesTest.h"

#include "libLoam/c++/LoamStreams.h"
#include "libPlasma/c++/PlasmaStreams.h"
#include "libPlasma/c++/SlawIterator.h"
#include <libPlasma/c++/Slaw.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace oblong::plasma;
using namespace oblong::loam;

namespace {

class SlawConsTest : public SlawTypesTest
{
  void assignment_test (const Callbacks &c)
  {
    for (size_t i = 0; i < c.Count (); ++i)
      for (size_t j = 0; j < c.Count (); ++j)
        {
          Slaw s = c.ConsFromValues (i, j);
          EXPECT_TRUE (s.IsCons ());
          Slaw scar = s.Car ();
          Slaw scdr = s.Cdr ();
          EXPECT_TRUE (c.CheckNthValue (scar, i));
          EXPECT_TRUE (c.CheckNthValue (scdr, j));
          Slaw t (s);
          Slaw t1 (t);
          EXPECT_EQ (t.Car (), t1.Car ());
          EXPECT_EQ (t.Cdr (), t1.Cdr ());
          s = t1;
          t = t = t1;
          EXPECT_TRUE (t == t1);
          EXPECT_TRUE (t == s);
          EXPECT_TRUE (t.Car () == s.Car ());
          EXPECT_TRUE (t.Cdr () == t1.Cdr ());
        }
  }

  void containment_test (const Callbacks &c)
  {
    std::vector<Slaw> sl;
    std::map<Str, Slaw> ml;
    std::vector<Str> keys;
    for (size_t j = 0; j < c.Count (); ++j)
      {
        std::ostringstream os ("arg");
        os << j;
        keys.push_back (Str (os.str ().c_str ()));
      }
    for (size_t j = 0; j < c.Count (); ++j)
      {
        Slaw ss (c.ConsWithNil (j));
        ml[keys[j]] = ss;
        sl.push_back (ss);
      }
    for (size_t j = 0; j < c.Count (); ++j)
      {
        Slaw slcar = sl[j].Car ();
        Slaw mlcar = ml[keys[j]].Car ();
        EXPECT_TRUE (c.CheckNthValue (slcar, j));
        EXPECT_TRUE (c.CheckNthValue (mlcar, j));
      }
  }

  void listness_test (const Callbacks &c)
  {
    for (size_t i = 0; i < c.Count (); ++i)
      for (size_t j = 0; j < c.Count (); ++j)
        {
          Slaw s = c.ConsFromValues (i, j);
          EXPECT_EQ (2, s.Count ());
          EXPECT_TRUE (s.Nth (0) == s.Car ());
          EXPECT_TRUE (s.Nth (1) == s.Cdr ());
          EXPECT_TRUE (s.Nth (-2) == s.Car ());
          EXPECT_TRUE (s.Nth (-1) == s.Cdr ());
          EXPECT_TRUE (s.Nth (-3).IsNull ());
          EXPECT_TRUE (s.Nth (-10).IsNull ());
          EXPECT_TRUE (s.Nth (2).IsNull ());
          EXPECT_TRUE (s.Nth (5).IsNull ());
          EXPECT_EQ (0, s.IndexOf (s.Car ()));
          EXPECT_EQ (s.Car () == s.Cdr () ? 0 : 1, s.IndexOf (s.Cdr ()));
          EXPECT_EQ (s, s.Slice (0, 2));
        }
  }

  void mapness_test (const Callbacks &c)
  {
    for (size_t i = 0; i < c.Count (); ++i)
      for (size_t j = 0; j < c.Count (); ++j)
        {
          Slaw s = c.ConsFromValues (i, j);
          EXPECT_EQ (1, s.MapKeys ().Count ());
          EXPECT_EQ (1, s.MapValues ().Count ());
          EXPECT_EQ (s.Car (), s.MapKeys ()[0]);
          EXPECT_EQ (s.Cdr (), s.MapValues ()[0]);
          EXPECT_EQ (s.Cdr (), s.Find (s.Car ()));
        }
  }

  void wrapped_slaw_test (const Callbacks &c) override
  {
    assignment_test (c);
    containment_test (c);
    listness_test (c);
    mapness_test (c);
  }
};

DEFINE_SLAW_TESTS_FOR_CLASS (SlawConsTest);

TEST_F (SlawConsTest, DuplicateInConstructor)
{
  slaw s = make_slaw ("foo");
  Slaw c = Slaw::Cons (s, s);
  EXPECT_EQ (c.Car (), c.Cdr ());
  EXPECT_TRUE (slawx_equal (c.Car ().SlawValue (), bslaw (s)));
  EXPECT_EQ (0, c.IndexOf (c.Cdr ()));
}

TEST_F (SlawConsTest, Scope)
{
  Slaw s (75);
  Slaw t ("hello");
  Slaw p;
  {
    Slaw p1 = Slaw::Cons (s, t);
    Slaw s1 (p1.Car ());
    Slaw t1 (p1.Cdr ());
    p = Slaw::Cons (s1, t1);
  }
  EXPECT_EQ (s, p.Car ());
  EXPECT_EQ (t, p.Cdr ());
}

}  // namespace

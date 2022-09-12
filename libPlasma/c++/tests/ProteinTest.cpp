
/* (c)  oblong industries */

// Unit tests for Protein

#include "SlawTypesTest.h"
#include "PoolTestBase.h"

#include "libLoam/c++/LoamStreams.h"
#include "libPlasma/c++/PlasmaStreams.h"
#include "libPlasma/c++/SlawIterator.h"
#include <libPlasma/c++/Hose.h>
#include <libPlasma/c++/Pool.h>
#include <libPlasma/c++/Protein.h>

#include <gtest/gtest.h>
#include <vector>

using namespace oblong::plasma;

namespace {

class ProteinTest : public SlawTypesTest
{
  void roundtrip_check (const Protein &p)
  {
    Protein pc (slaw_dup (p.ProteinValue ()));
    EXPECT_EQ (p, pc);
  }

  void basic_test (const Slaw &descrips)
  {
    Slaw ingests = Slaw::Map ("descrips", descrips);
    Protein p (descrips, ingests);

    EXPECT_TRUE (p.ToSlaw ().IsProtein ());
    EXPECT_EQ (descrips, p.Descrips ());
    EXPECT_EQ (ingests, p.Ingests ());
    roundtrip_check (p);

    Protein p2 (descrips, ingests.MapPut ("protein", p.ToSlaw ()));
    EXPECT_NE (p2, p);
    EXPECT_NE (p, p2);
    EXPECT_EQ (descrips, p2.Descrips ());
    EXPECT_EQ (ingests.MapPut ("protein", p.ToSlaw ()), p2.Ingests ());
    roundtrip_check (p2);

    Protein p3 (p2);
    Protein p4;
    p4 = p2;
    EXPECT_EQ (p4, p2);
    EXPECT_EQ (p3, p4);
  }

  void list_search_test (const Slaw &descrips)
  {
    ASSERT_TRUE (descrips.IsList ());
    Protein p (descrips, Slaw ());
    for (int64 i = 0; i < descrips.Count (); ++i)
      {
        const int64 ip = descrips.IsNil ()
                           ? p.Search (Slaw::List (Slaw::Nil ()))
                           : p.Search (descrips[i]);
        EXPECT_LE (ip, i);
        EXPECT_GT (ip, -1);
        for (int64 j = i + 1; j < descrips.Count (); ++j)
          {
            EXPECT_EQ (ip, p.Search (descrips.Slice (i, j)));
            EXPECT_EQ (ip, p.Search (Slaw::List (descrips[i], descrips[j])));
          }
      }
    EXPECT_GT (0L, p.Search (Slaw ()));
    EXPECT_GT (0L, p.Search (Slaw::Nil ()));
    EXPECT_EQ (0L, p.Search (Slaw::List ()));

    if (descrips.Count () > 0)
      {
        EXPECT_FALSE (p.Matches (descrips.ListAppend (descrips[0])));
      }
  }

  void non_list_descrips_test (const Callbacks &c)
  {
    for (size_t i = 0; i < c.Count (); ++i)
      {
        Slaw descrips (c.SlawFromNthValue (i));
        Slaw ingests = Slaw::Map ("descrips", descrips);
        Protein p (descrips, ingests);
        EXPECT_TRUE (p.ToSlaw ().IsProtein ());
        EXPECT_EQ (descrips, p.Descrips ());
        EXPECT_EQ (ingests, p.Ingests ());
        roundtrip_check (p);

        EXPECT_GT (0L, p.Search (descrips));
        EXPECT_GT (0L, p.Search (Slaw ()));
        EXPECT_GT (0L, p.Search (ingests));
      }
  }

  void non_map_ingests_test (const Callbacks &c)
  {
    for (size_t i = 0; i < c.Count (); ++i)
      {
        Slaw ingests (c.SlawFromNthValue (i));
        Slaw descrips = Slaw::Map ("descrips", ingests);
        Protein p (descrips, ingests);
        EXPECT_TRUE (p.ToSlaw ().IsProtein ());
        EXPECT_EQ (descrips, p.Descrips ());
        EXPECT_EQ (ingests, p.Ingests ());
        roundtrip_check (p);
      }
  }

  void wrapped_slaw_test (const Callbacks &c) override
  {
    Slaw lst (c.FillList (0, c.Count ()));
    basic_test (lst);
    list_search_test (lst);
    non_list_descrips_test (c);
    non_map_ingests_test (c);
  }
};

DEFINE_SLAW_TESTS_FOR_CLASS (ProteinTest);

TEST_F (ProteinTest, EmptyProtein)
{
  Protein p;
  EXPECT_FALSE (p.IsNull ());
  EXPECT_TRUE (p.Descrips ().IsNull ());
  EXPECT_TRUE (p.Ingests ().IsNull ());
  EXPECT_TRUE (slaw_is_protein (p.ProteinValue ()));
}

TEST_F (ProteinTest, NullVsEmpty)
{
  Protein p (NULL);
  EXPECT_TRUE (p.IsNull ());
  EXPECT_TRUE (p.IsEmpty ());

  EXPECT_EQ (p, Protein::Null ());
  EXPECT_NE (Protein (), p);
  EXPECT_NE (p, Protein ());

  Protein p2;
  EXPECT_FALSE (p2.IsNull ());
  EXPECT_TRUE (p2.IsEmpty ());

  EXPECT_FALSE (p2.ProteinValue () == NULL);
  EXPECT_FALSE (p == p2);
}

TEST (SlawProteinTest, ProteinFromCSlaw)
{
  /// Constructs a new instance from data in a C-style protein. Since
  /// C-level protein are also C-level slaw, this constructor checks
  /// whether @a p is actually a protein. If it is not, @a p is
  /// treated as a descrip in the newly constructed protein...
  Protein pr (slaw_string ("exterminate"));
  EXPECT_TRUE (slaw_is_protein (pr.ProteinValue ())) << pr;

  // So we would expect this Protein to have "exterminate" as a descrip:
  EXPECT_STREQ ("exterminate", pr.Descrips ().Nth (0).Emit<const char *> ());

  // and no ingests:
  EXPECT_EQ (0L, pr.Ingests ().Count ()) << pr.Ingests ();

  // ... unless the slaw is a map, in which case it becomes
  // the protein's ingests:
  pr = Protein (slaw_map_inline_cc ("a", "b", "c", "d", NULL));
  EXPECT_EQ (0L, pr.Descrips ().Count ()) << pr.Descrips ();
  Slaw ingests (pr.Ingests ());
  EXPECT_TRUE (ingests.IsMap ());
  EXPECT_EQ (Slaw ("b"), ingests.Find ("a"));
  EXPECT_EQ (Slaw ("d"), ingests.Find ("c"));
  EXPECT_EQ (2L, ingests.Count ());
}

TEST (SlawProteinTest, DepositNonProteinSlaw)
{
  // Any Slaw can be interpreted as a Protein; thus, the constructor
  // taking a Slaw must behave as the one taking a slaw.
  Slaw m = Slaw::Map ("exterminate", true);
  Protein pr = Protein (m);

  EXPECT_FALSE (pr.IsNull ()) << pr;
  EXPECT_EQ (m.Descrips (), pr.Descrips ());
  EXPECT_EQ (m.Ingests (), pr.Ingests ());

  ::std::string pname (PoolTestBase::PoolName ("Derek"));

  Hose *h (Pool::Participate (pname.c_str (), Pool::MMAP_SMALL, NULL));
  EXPECT_FALSE (h == NULL);

  EXPECT_TRUE (h->Deposit (pr).IsSplend ());
  EXPECT_EQ (OB_OK, h->LastRetort ().Code ());

  // Deposit shouldn't have failed, so a valid index is returned.
  EXPECT_NE (Protein::NO_INDEX, h->NewestIndex ());

  h->Delete ();
  Pool::Dispose (pname.c_str ());
}

// http://www.thesmokinggun.com/file/katy-perry-rider?page=4
#define KATY_PERRYS_SPELLING                                                   \
  "DO NOT STAIR AT THE BACKSEAT THRU THE REARVIEUW MIRROW"

class ProteinSearchTest : public ::testing::Test
{
 public:
  Protein haystack;
  Protein haystack_empty_list, haystack_nil, haystack_null,
    haystack_solo_string;
  Slaw solo_string_needle, nil_needle, null_needle;
  Slaw needle1, needle2, needle3, needle4, needle5, needle6, needle7;
  Slaw needle_empty_list, needle_nil, needle_null, needle_solo_string;

  ProteinSearchTest ();
};

ProteinSearchTest::ProteinSearchTest ()
    : haystack (Slaw (Str (KATY_PERRYS_SPELLING).Split (" ")), Slaw ()),
      haystack_empty_list (Slaw::List (), Slaw ()),
      haystack_nil (Slaw::Nil (), Slaw ()),
      haystack_solo_string (Slaw ("solo"), Slaw ()),
      needle1 ("MIRROW"),
      needle2 ("MIRROR"),
      needle3 (Slaw::List ("STAIR", "REARVIEUW")),
      needle4 (Slaw::List ("STARE", "REARVIEW")),
      needle5 (Slaw::List ("BACKSEAT", "THRU", "THE")),
      needle6 (Slaw::List ("DO", "AT", "THRU")),
      needle7 (Slaw::List ("THE", "REARVIEUW")),
      needle_empty_list (Slaw::List ()),
      needle_nil (Slaw::Nil ()),
      needle_null (Slaw ()),
      needle_solo_string (Slaw ("solo"))
{
}

TEST_F (ProteinSearchTest, GapSearch)
{
  EXPECT_EQ (9, haystack.Search (needle1, SEARCH_GAP));
  EXPECT_EQ (-1, haystack.Search (needle2, SEARCH_GAP));
  EXPECT_EQ (2, haystack.Search (needle3, SEARCH_GAP));
  EXPECT_EQ (-1, haystack.Search (needle4, SEARCH_GAP));
  EXPECT_EQ (5, haystack.Search (needle5, SEARCH_GAP));
  EXPECT_EQ (0, haystack.Search (needle6, SEARCH_GAP));
  EXPECT_EQ (4, haystack.Search (needle7, SEARCH_GAP));
  EXPECT_EQ (0, haystack.Search (needle_empty_list, SEARCH_GAP));
  EXPECT_EQ (-1, haystack.Search (needle_null, SEARCH_GAP));
}

TEST_F (ProteinSearchTest, ContigSearch)
{
  EXPECT_EQ (9, haystack.Search (needle1, SEARCH_CONTIG));
  EXPECT_EQ (-1, haystack.Search (needle2, SEARCH_CONTIG));
  EXPECT_EQ (-1, haystack.Search (needle3, SEARCH_CONTIG));
  EXPECT_EQ (-1, haystack.Search (needle4, SEARCH_CONTIG));
  EXPECT_EQ (5, haystack.Search (needle5, SEARCH_CONTIG));
  EXPECT_EQ (-1, haystack.Search (needle6, SEARCH_CONTIG));
  EXPECT_EQ (7, haystack.Search (needle7, SEARCH_CONTIG));
  EXPECT_EQ (0, haystack.Search (needle_empty_list, SEARCH_CONTIG));
  EXPECT_EQ (-1, haystack.Search (needle_null, SEARCH_CONTIG));
}

TEST_F (ProteinSearchTest, GapMatches)
{
  EXPECT_TRUE (haystack.Matches (needle1, SEARCH_GAP));
  EXPECT_FALSE (haystack.Matches (needle2, SEARCH_GAP));
  EXPECT_TRUE (haystack.Matches (needle3, SEARCH_GAP));
  EXPECT_FALSE (haystack.Matches (needle4, SEARCH_GAP));
  EXPECT_TRUE (haystack.Matches (needle5, SEARCH_GAP));
  EXPECT_TRUE (haystack.Matches (needle6, SEARCH_GAP));
  EXPECT_TRUE (haystack.Matches (needle7, SEARCH_GAP));
  EXPECT_TRUE (haystack.Matches (needle_empty_list, SEARCH_GAP));
  EXPECT_FALSE (haystack.Matches (needle_null, SEARCH_GAP));
}

TEST_F (ProteinSearchTest, ContigMatches)
{
  EXPECT_TRUE (haystack.Matches (needle1, SEARCH_CONTIG));
  EXPECT_FALSE (haystack.Matches (needle2, SEARCH_CONTIG));
  EXPECT_FALSE (haystack.Matches (needle3, SEARCH_CONTIG));
  EXPECT_FALSE (haystack.Matches (needle4, SEARCH_CONTIG));
  EXPECT_TRUE (haystack.Matches (needle5, SEARCH_CONTIG));
  EXPECT_FALSE (haystack.Matches (needle6, SEARCH_CONTIG));
  EXPECT_TRUE (haystack.Matches (needle7, SEARCH_CONTIG));
  EXPECT_TRUE (haystack.Matches (needle_empty_list, SEARCH_CONTIG));
  EXPECT_FALSE (haystack.Matches (needle_null, SEARCH_CONTIG));
}

TEST_F (ProteinSearchTest, DegenerateCases)
{
  // what is the matrix? here it is a 4x4 set of haystack vs. needle cases.

  for (int i = 0; i < 2; i++)
    {
      Protein_Search_Type search_type;
      if (i == 0)
        search_type = SEARCH_CONTIG;
      else
        search_type = SEARCH_GAP;

      // first check all "empty list" haystacks (matches only "empty list needle")
      EXPECT_EQ (0,
                 haystack_empty_list.Search (needle_empty_list, search_type));
      EXPECT_EQ (-1, haystack_empty_list.Search (needle_nil, search_type));
      EXPECT_EQ (-1, haystack_empty_list.Search (needle_null, search_type));
      EXPECT_EQ (-1,
                 haystack_empty_list.Search (needle_solo_string, search_type));

      // check all "nil" haystacks (matches only "empty list needle")
      EXPECT_EQ (0,
                 haystack_empty_list.Search (needle_empty_list, search_type));
      EXPECT_EQ (-1, haystack_empty_list.Search (needle_nil, search_type));
      EXPECT_EQ (-1, haystack_empty_list.Search (needle_null, search_type));
      EXPECT_EQ (-1,
                 haystack_empty_list.Search (needle_solo_string, search_type));

      // check all "null" haystacks (matches only "empty list needle")
      EXPECT_EQ (0,
                 haystack_empty_list.Search (needle_empty_list, search_type));
      EXPECT_EQ (-1, haystack_empty_list.Search (needle_nil, search_type));
      EXPECT_EQ (-1, haystack_empty_list.Search (needle_null, search_type));
      EXPECT_EQ (-1,
                 haystack_empty_list.Search (needle_solo_string, search_type));

      // check all "atomic string" haystacks (matches only "empty list needle")
      EXPECT_EQ (0,
                 haystack_empty_list.Search (needle_empty_list, search_type));
      EXPECT_EQ (-1, haystack_empty_list.Search (needle_nil, search_type));
      EXPECT_EQ (-1, haystack_empty_list.Search (needle_null, search_type));
      EXPECT_EQ (-1,
                 haystack_empty_list.Search (needle_solo_string, search_type));
    }
}

}  // namespace

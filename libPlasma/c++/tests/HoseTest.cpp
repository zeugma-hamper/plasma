
/* (c)  oblong industries */

#include "PoolTestBase.h"

#include "libLoam/c++/LoamStreams.h"
#include "libPlasma/c++/PlasmaStreams.h"
#include <libPlasma/c++/Hose.h>
#include <libPlasma/c++/HoseGang.h>
#include <libPlasma/c/pool.h>


namespace {

enum
{
  SIZE = 10
};

class HoseTest : public PoolTestBase
{
 protected:
  static Protein test_proteins[SIZE];
  static Hose *hose1, *hose2;

  void SetUp () override
  {
    EXPECT_TRUE (hose1->IsConfigured ());
    EXPECT_TRUE (hose2->IsConfigured ());
  }

  void TearDown () override
  {
    EXPECT_TRUE (hose1->Rewind ().IsSplend ());
    EXPECT_TRUE (hose2->Rewind ().IsSplend ());
  }

  static void SetUpTestCase ()
  {
    PoolTestBase::SetUpTestCase ();
    for (int i = 0; i < SIZE; ++i)
      test_proteins[i] =
        Protein (Slaw::List (i, i + SIZE), Slaw::Map ("index", i));
    hose1 = ConnectAndFill (POOL_NAME);
    hose2 = ConnectAndFill (EPOOL_NAME);
  }

  static void TearDownTestCase ()
  {
    hose1->Delete ();
    hose2->Delete ();
    PoolTestBase::TearDownTestCase ();
  }

  static Hose *ConnectAndFill (const ::std::string &name)
  {
    Hose *hose (Pool::Participate (name.c_str ()));
    EXPECT_TRUE (hose->LastRetort ().IsSplend ());
    EXPECT_TRUE (hose->Runout ().IsSplend ());
    if (hose->IsConfigured ())
      {
        for (int i = 0; i < SIZE; ++i)
          {
            EXPECT_TRUE (hose->Deposit (test_proteins[i]).IsSplend ());
            EXPECT_STREQ (name.c_str (), hose->PoolName ().utf8 ());
            EXPECT_TRUE (hose->LastRetort ().IsSplend ())
              << hose->LastRetort ();
          }
      }
    return hose;
  }

  static void check_test_protein (Hose *hose, const Protein p, int i)
  {
    EXPECT_EQ (hose->PoolName (), p.Origin ());
    EXPECT_EQ (test_proteins[i], p);
    EXPECT_TRUE (hose->LastRetort ().IsSplend ());
    EXPECT_EQ (p, hose->Previous ());
    EXPECT_TRUE (hose->SeekBy (1).IsSplend ());
    EXPECT_EQ (p, hose->ProbeBackward (test_proteins[i].Descrips ()[0]));
    EXPECT_TRUE (hose->SeekBy (1).IsSplend ());
  }

  static void sequential_access_test (const char *trace, Hose *hose,
                                      pool_timestamp wt)
  {
    SCOPED_TRACE (trace);
    for (int i = 0; i < SIZE; ++i)
      {
        EXPECT_EQ (i, hose->CurrentIndex ());
        Protein pc = hose->Current ();
        Protein p = hose->Next (wt);
        EXPECT_EQ (pc, p);
        check_test_protein (hose, p, i);
      }
    EXPECT_TRUE (hose->Next (Hose::NO_WAIT).IsNull ());
  }

  static void matching_access_test (const char *trace, Hose *hose,
                                    pool_timestamp wt)
  {
    SCOPED_TRACE (trace);
    for (int i = 0; i < SIZE; ++i)
      {
        EXPECT_EQ (i, hose->CurrentIndex ());
        Protein p = hose->ProbeForward (test_proteins[i].Descrips ()[0], wt);
        check_test_protein (hose, p, i);
        ASSERT_TRUE (hose->SeekBy (-1).IsSplend ());
        p = hose->ProbeForward (test_proteins[i].Descrips (), wt);
        check_test_protein (hose, p, i);
      }
    EXPECT_TRUE (hose->Next (Hose::NO_WAIT).IsNull ());
    ASSERT_TRUE (hose->Rewind ().IsSplend ());

    for (int i = 0; i < SIZE; i += 2)
      {
        Protein p = hose->ProbeForward (test_proteins[i].Descrips ()[0], wt);
        check_test_protein (hose, p, i);
        ASSERT_TRUE (hose->SeekBy (-1).IsSplend ());
        p = hose->ProbeForward (test_proteins[i].Descrips (), wt);
        check_test_protein (hose, p, i);
        EXPECT_EQ (i + 1, hose->CurrentIndex ());
      }
  }
};

Protein HoseTest::test_proteins[SIZE];
Hose *HoseTest::hose1;
Hose *HoseTest::hose2;


TEST_F (HoseTest, Names)
{
  EXPECT_EQ (hose1->PoolName ().ReplaceAll ("^.*/", ""), hose1->Name ());
  EXPECT_EQ (hose2->PoolName ().ReplaceAll ("^.*/", ""), hose2->Name ());
  const char *new_name ("a new hose name");
  EXPECT_TRUE (hose1->SetName (new_name).IsSplend ());
  EXPECT_STREQ (new_name, hose1->Name ());
  EXPECT_TRUE (hose1->ResetName ().IsSplend ());
  EXPECT_EQ (hose1->PoolName (), hose1->Name ());
}

TEST_F (HoseTest, SequentialAccess)
{
  ASSERT_TRUE (hose1->IsConfigured ());
  sequential_access_test ("wait", hose1, Hose::WAIT);
  ASSERT_TRUE (hose2->IsConfigured ());
  sequential_access_test ("no wait", hose2, Hose::NO_WAIT);
}

TEST_F (HoseTest, MatchingAccess)
{
  ASSERT_TRUE (hose1->IsConfigured ());
  matching_access_test ("wait", hose1, Hose::WAIT);
  ASSERT_TRUE (hose2->IsConfigured ());
  matching_access_test ("no wait", hose2, Hose::NO_WAIT);
}

TEST_F (HoseTest, SequentialAccessToCopy)
{
  Hose hc ("whatever");
  hc = *hose1;
  ASSERT_TRUE (hc.IsConfigured ());
  sequential_access_test ("wait", &hc, Hose::WAIT);
  hc = *hose2;
  ASSERT_TRUE (hc.IsConfigured ());
  sequential_access_test ("no wait", &hc, Hose::NO_WAIT);
}

TEST_F (HoseTest, SequentialAccessToDup)
{
  Hose *hc = hose1->Dup ();
  ASSERT_TRUE (hc->IsConfigured ());
  sequential_access_test ("wait", hc, Hose::WAIT);
  hc->Delete ();
  hc = hose2->Dup ();
  ASSERT_TRUE (hc->IsConfigured ());
  sequential_access_test ("no wait", hc, Hose::NO_WAIT);
  hc->Delete ();
}

void check_indexes (const std::string &trace, const ObRetort &r, Hose *h,
                    int64 c)
{
  SCOPED_TRACE (trace.c_str ());
  EXPECT_TRUE (r.IsSplend ()) << r;
  EXPECT_EQ (c, h->CurrentIndex ());
  EXPECT_EQ (SIZE - 1, h->NewestIndex ());
  EXPECT_EQ (0, h->OldestIndex ());
}

TEST_F (HoseTest, Indexes)
{
  ASSERT_TRUE (hose1->IsConfigured ());

  int64 c (hose1->CurrentIndex ());

  check_indexes ("Rewind", hose1->Rewind (), hose1, c);
  check_indexes ("Runout", hose1->Runout (), hose1, c + SIZE);
  check_indexes ("ToLast", hose1->ToLast (), hose1, c + SIZE - 1);
  check_indexes ("SeekBy 1", hose1->SeekBy (1), hose1, c + SIZE);
  check_indexes ("SeekBy -1", hose1->SeekBy (-1), hose1, c + SIZE - 1);
  check_indexes ("SeekBy N", hose1->SeekBy (1 - SIZE), hose1, c);

  int64 steps[] = {0, 2, 3};
  char buffer[128];
  for (unt64 i = 0; i < sizeof (steps) / sizeof (steps[0]); ++i)
    {
      snprintf (buffer, sizeof (buffer),
                "%" OB_FMT_64 "d (i = %" OB_FMT_64 "u)", steps[i], i);
      ::std::string t (buffer);
      c = hose1->CurrentIndex ();
      EXPECT_TRUE (hose1->IsConfigured ());
      check_indexes ("SeekBy " + t, hose1->SeekBy (steps[i]), hose1,
                     c + steps[i]);
      check_indexes ("SeekBy -2 * " + t, hose1->SeekBy (-2 * steps[i]), hose1,
                     c - steps[i]);
      check_indexes ("SeekTo c + " + t, hose1->SeekTo (c + steps[i]), hose1,
                     c + steps[i]);
    }
}

TEST_F (HoseTest, ClassNames)
{
  Hose h = *hose1;
  ;
  EXPECT_STREQ ("Hose", h.ClassName ());
  HoseGang hg;
  EXPECT_STREQ ("HoseGang", hg.ClassName ());
}

TEST (HoseTest1, DepositInfo)
{
  const Str pname (PoolTestBase::PoolName ("DepositInfo").c_str ());
  ObRetort tort;
  Pool::Configuration conf = Pool::MMAP_SMALL;
  conf.cpolicy = Pool::Create_Auto_Disposable;
  Hose *h = Pool::Participate (pname, conf, &tort);
  EXPECT_TRUE (tort.IsSplend ());
  Protein p (Slaw::List ("descrips"),
             Slaw::Map ("You are in the desert.  You see a tortoise lying "
                        "on his back in the hot sun.  You recognize his "
                        "plight, but do nothing to help.  Why?",
                        "Because you are also a tortoise."));
  for (int64 i = 0; i < 5; i++)
    {
      ObRetort_DepositInfo ordi = h->Deposit (p);
      EXPECT_TRUE (ordi.IsSplend ());
      EXPECT_EQ (i, ordi.index);
    }
  tort = h->Withdraw ();
  EXPECT_TRUE (tort.IsSplend ());
  ObRetort_DepositInfo ordi = h->Deposit (p);
  EXPECT_TRUE (ordi.IsError ());
  EXPECT_STREQ ("OB_HOSE_NOT_CONFIGURED", ordi.Description ());
  h->Delete ();
  // that pool is already gone...
  tort = Pool::Dispose (pname);
  EXPECT_EQ (POOL_NO_SUCH_POOL, tort);
}

TEST (HoseTest1, TocCapacity)
{
  const Str pname (PoolTestBase::PoolName ("TocCapacity").c_str ());

  for (unt64 i = 10; i <= 30; i += 10)
    {
      ObRetort tort;
      Pool::Configuration conf = Pool::MMAP_SMALL;
      conf.toc_capacity = i;
      Hose *h = Pool::Participate (pname, conf, &tort);
      EXPECT_TRUE (tort.IsSplend ());
      protein p = NULL;
      EXPECT_EQ (OB_OK, pool_get_info (h->RawHose (), -1, &p));
      EXPECT_EQ (i, mmap_pool_options_toc_capacity (p));
      tort = h->Withdraw ();
      EXPECT_TRUE (tort.IsSplend ());
      h->Delete ();
      tort = Pool::Dispose (pname);
      EXPECT_TRUE (tort.IsSplend ());
      protein_free (p);
    }
}

}  // namespace

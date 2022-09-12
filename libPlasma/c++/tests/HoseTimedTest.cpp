
/* (c)  oblong industries */

#include "PoolTestBase.h"

#include "libLoam/c++/LoamStreams.h"
#include "libLoam/c/ob-time.h"

#include "libPlasma/c++/PlasmaStreams.h"
#include <libPlasma/c++/Hose.h>
#include <libPlasma/c/pool_options.h>


namespace {

// See bug 840 for why you might want to tweak this.  But we ultimately
// decided to keep the original value of 1.
enum
{
  MINIMAL_MILLISECONDS = 1
};

struct DepositedProtein
{
  DepositedProtein () : info (OB_OK) {}
  Protein prot;
  ObRetort_DepositInfo info;
};

typedef ::std::vector<DepositedProtein> ProteinArray;

class HoseTimedTest : public PoolTestBase
{
 protected:
  static ProteinArray proteins;
  Hose *full_hose, *over_hose;

  static const ::std::string FULL_POOL_NAME;
  static const ::std::string OVER_POOL_NAME;
  static const unt64 PSIZE, TOCCAP, OTOCCAP;

  static int64 worst_delay;
  static pool_timestamp worst_delay_perceived;

  void SetUp () override
  {
    full_hose = ConnectSized (FULL_POOL_NAME, PSIZE, TOCCAP);
    over_hose = ConnectSized (OVER_POOL_NAME, PSIZE, OTOCCAP);
    EXPECT_TRUE (full_hose->IsConfigured ());
    EXPECT_TRUE (over_hose->IsConfigured ());
  }

  void TearDown () override
  {
    OB_DIE_ON_ERROR (full_hose->Withdraw ().Code ());
    OB_DIE_ON_ERROR (pool_dispose (FULL_POOL_NAME.c_str ()));
    OB_DIE_ON_ERROR (over_hose->Withdraw ().Code ());
    OB_DIE_ON_ERROR (pool_dispose (OVER_POOL_NAME.c_str ()));
    full_hose->Delete ();
    over_hose->Delete ();
  }

  static void SetUpTestCase ()
  {
    PoolTestBase::SetUpTestCase ();

    for (unt64 i = 0; i < TOCCAP; ++i)
      {
        DepositedProtein p;
        p.prot = Protein (Slaw::List (i, i + TOCCAP), Slaw::Map ("index", i));
        proteins.push_back (p);
      }
  }

  static void TearDownTestCase () { PoolTestBase::TearDownTestCase (); }

  static Hose *ConnectSized (const ::std::string &name, unt64 size,
                             unt64 capacity)
  {
    Protein options (toc_mmap_pool_options (size, capacity));
    EXPECT_TRUE (
      Pool::Create (name.c_str (), "mmap", true, options).IsSplend ());
    Hose *hose (Pool::Participate (name.c_str ()));
    EXPECT_TRUE (hose->LastRetort ().IsSplend ());
    EXPECT_TRUE (hose->Runout ().IsSplend ());
    return hose;
  }

  static void pause (int64 milliseconds)
  {
    if (0 == milliseconds)
      return;

    int64 orig_milliseconds = milliseconds;
    int64 total_waited = 0;
    pool_timestamp now;
    pool_timestamp orig = pool_timestamp_now ();
    for (;;)
      {
        ob_micro_sleep (1000 * milliseconds);  // milli to micro!
        total_waited += milliseconds;
        now = pool_timestamp_now ();
        if (now > orig)
          break;
        else
          milliseconds *= 2;  // exponential backoff
      }
    if (total_waited > orig_milliseconds && total_waited > worst_delay)
      {
        worst_delay = total_waited;
        worst_delay_perceived = now - orig;
      }
  }

  static void fill_timed_hose (Hose *hose, int64 msecs, unt64 upto)
  {
    if (hose->IsConfigured ())
      {
        for (unt64 i = 0; i < upto; ++i)
          {
            proteins[i].info = hose->Deposit (proteins[i % TOCCAP].prot);
            EXPECT_TRUE (proteins[i].info.IsSplend ());
            EXPECT_TRUE (hose->LastRetort ().IsSplend ())
              << hose->LastRetort ();
            pause (msecs);
          }
      }
  }

  static void check_test_protein_array (Hose *hose, ProteinArray &parray,
                                        const Protein p, int i)
  {
    EXPECT_EQ (hose->PoolName (), p.Origin ());
    EXPECT_EQ (parray[i].prot, p);
    EXPECT_TRUE (hose->LastRetort ().IsSplend ());
    EXPECT_EQ (parray[i].info.index, p.Index ());
    EXPECT_EQ (parray[i].info.timestamp, p.Timestamp ());
  }

  static void maybe_check_timed_protein (ObRetort oret, Hose *hose, int i)
  {
    EXPECT_EQ (OB_OK, oret) << "Seek failed for protein no. " << i;
    if (OB_OK == oret)
      check_test_protein_array (hose, proteins, hose->Next (), i);
  }

  void check_seekto_exact_time (Hose *hose)
  {
    const unt64 STEP (10), N (OTOCCAP * 3 / 2);
    fill_timed_hose (hose, 1 * MINIMAL_MILLISECONDS, N);
    for (unt64 s = 0; s < STEP; ++s)
      for (unt64 i = s; i < N; i += STEP)
        {
          const DepositedProtein &d = proteins[i];
          for (int k = 0; k < 3; ++k)
            {
              time_comparison b = static_cast<time_comparison> (k);
              ObRetort oret = hose->SeekToTime (d.info.timestamp, b);
              maybe_check_timed_protein (oret, hose, i);
            }
        }
  }

  void check_seekto_approx_time (Hose *hose)
  {
    const unt64 STEP (10), N (OTOCCAP * 3 / 2);
    fill_timed_hose (hose, 2 * MINIMAL_MILLISECONDS, N);
    for (unt64 s = 0; s < STEP; ++s)
      for (unt64 i = s; i < N - 1; i += STEP)
        {
          const DepositedProtein &d = proteins[i];
          const DepositedProtein &dn = proteins[i + 1];
          pool_timestamp d_stamp = d.info.timestamp;
          pool_timestamp dn_stamp = dn.info.timestamp;
          pool_timestamp post = (d_stamp + dn_stamp) / 2.0 - 0.00001;
          EXPECT_LT (d_stamp, post);
          EXPECT_LT (post, dn_stamp);

          ObRetort oret = hose->SeekToTime (post, OB_CLOSEST);
          maybe_check_timed_protein (oret, hose, i);
          oret = hose->SeekToTime (post, OB_CLOSEST_LOWER);
          maybe_check_timed_protein (oret, hose, i);
          oret = hose->SeekToTime (post, OB_CLOSEST_HIGHER);
          maybe_check_timed_protein (oret, hose, i + 1);

          post = (d_stamp + dn_stamp) / 2.0 + 0.00001;
          EXPECT_LT (d_stamp, post);
          EXPECT_LT (post, dn_stamp);

          oret = hose->SeekToTime (post, OB_CLOSEST);
          maybe_check_timed_protein (oret, hose, i + 1);
          oret = hose->SeekToTime (post, OB_CLOSEST_LOWER);
          maybe_check_timed_protein (oret, hose, i);
          oret = hose->SeekToTime (post, OB_CLOSEST_HIGHER);
          maybe_check_timed_protein (oret, hose, i + 1);
        }
    // boundaries
    const DepositedProtein &d = proteins[N - 1];
    pool_timestamp ts = d.info.timestamp + 0.0001;

    ObRetort oret = hose->SeekToTime (ts, OB_CLOSEST);
    maybe_check_timed_protein (oret, hose, N - 1);
    oret = hose->SeekToTime (ts, OB_CLOSEST_LOWER);
    maybe_check_timed_protein (oret, hose, N - 1);
    EXPECT_EQ (POOL_NO_SUCH_PROTEIN, hose->SeekToTime (ts, OB_CLOSEST_HIGHER));

    ts = d.info.timestamp - 0.0001;
    oret = hose->SeekToTime (ts, OB_CLOSEST);
    maybe_check_timed_protein (oret, hose, N - 1);
    oret = hose->SeekToTime (ts, OB_CLOSEST_LOWER);
    maybe_check_timed_protein (oret, hose, N - 2);
    oret = hose->SeekToTime (ts, OB_CLOSEST_HIGHER);
    maybe_check_timed_protein (oret, hose, N - 1);

    ts = proteins[0].info.timestamp - 0.0001;
    oret = hose->SeekToTime (ts, OB_CLOSEST);
    maybe_check_timed_protein (oret, hose, 0);
    EXPECT_EQ (POOL_NO_SUCH_PROTEIN, hose->SeekToTime (ts, OB_CLOSEST_LOWER));
    oret = hose->SeekToTime (ts, OB_CLOSEST_HIGHER);
    maybe_check_timed_protein (oret, hose, 0);
  }

  void check_seekby_time (Hose *hose)
  {
    const unt64 STEP (10), N (OTOCCAP * 3 / 2);
    fill_timed_hose (hose, 2 * MINIMAL_MILLISECONDS, N);
    for (unt64 i = 0; i < N - 2; i += STEP)
      {
        const DepositedProtein &d = proteins[i];
        const DepositedProtein &dd = proteins[i + 1];
        pool_timestamp d_stamp = d.info.timestamp;
        pool_timestamp dd_stamp = dd.info.timestamp;

        pool_timestamp delta = (dd_stamp - d_stamp) / 2.0 - 0.00001;
        EXPECT_EQ (OB_OK, hose->SeekTo (d.info.index));
        ObRetort oret = hose->SeekByTime (delta, OB_CLOSEST);
        maybe_check_timed_protein (oret, hose, i);
        EXPECT_EQ (OB_OK, hose->SeekTo (d.info.index));
        oret = hose->SeekByTime (delta, OB_CLOSEST_LOWER);
        maybe_check_timed_protein (oret, hose, i);
        EXPECT_EQ (OB_OK, hose->SeekTo (d.info.index));
        oret = hose->SeekByTime (delta, OB_CLOSEST_HIGHER);
        maybe_check_timed_protein (oret, hose, i + 1);

        delta = (dd_stamp - d_stamp) / 2.0 + 0.00001;
        EXPECT_EQ (OB_OK, hose->SeekTo (d.info.index));
        oret = hose->SeekByTime (delta, OB_CLOSEST);
        maybe_check_timed_protein (oret, hose, i + 1);
        EXPECT_EQ (OB_OK, hose->SeekTo (d.info.index));
        oret = hose->SeekByTime (delta, OB_CLOSEST_LOWER);
        maybe_check_timed_protein (oret, hose, i);
        EXPECT_EQ (OB_OK, hose->SeekTo (d.info.index));
        oret = hose->SeekByTime (delta, OB_CLOSEST_HIGHER);
        maybe_check_timed_protein (oret, hose, i + 1);

        delta = (dd_stamp - d_stamp) + 0.00001;
        EXPECT_EQ (OB_OK, hose->SeekTo (d.info.index));
        oret = hose->SeekByTime (delta, OB_CLOSEST);
        maybe_check_timed_protein (oret, hose, i + 1);
        EXPECT_EQ (OB_OK, hose->SeekTo (d.info.index));
        oret = hose->SeekByTime (delta, OB_CLOSEST_LOWER);
        maybe_check_timed_protein (oret, hose, i + 1);
        EXPECT_EQ (OB_OK, hose->SeekTo (d.info.index));
        oret = hose->SeekByTime (delta, OB_CLOSEST_HIGHER);
        maybe_check_timed_protein (oret, hose, i + 2);
      }
  }
};

ProteinArray HoseTimedTest::proteins;
int64 HoseTimedTest::worst_delay;
pool_timestamp HoseTimedTest::worst_delay_perceived;

const ::std::string HoseTimedTest::FULL_POOL_NAME (PoolName ("full-pool"));
const ::std::string HoseTimedTest::OVER_POOL_NAME (PoolName ("over-pool"));
const unt64 HoseTimedTest::PSIZE = MEGABYTE;
const unt64 HoseTimedTest::TOCCAP = MEGABYTE / 1000;
const unt64 HoseTimedTest::OTOCCAP = 80;


TEST_F (HoseTimedTest, SeekToExactTime)
{
  check_seekto_exact_time (full_hose);
  check_seekto_exact_time (over_hose);
}

TEST_F (HoseTimedTest, SeekToApproxTime)
{
  check_seekto_approx_time (full_hose);
  check_seekto_approx_time (over_hose);
}

TEST_F (HoseTimedTest, SeekByTime)
{
  check_seekby_time (full_hose);
  check_seekby_time (over_hose);
}

}  // namespace

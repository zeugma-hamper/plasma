
/* (c)  oblong industries */

#include "PoolTestBase.h"

namespace {

// See bug 840 for why you might want to tweak this.  But we ultimately
// decided to keep the original value of 1.
enum
{
  MINIMAL_MILLISECONDS = 1
};

class TocPoolTest : public PoolTestBase
{
 protected:
  void SetUp () override { super::SetUp (); }
  void TearDown () override { super::TearDown (); }

  static void SetUpTestCase () { super::SetUpTestCase (); }
  static void TearDownTestCase () { super::TearDownTestCase (); }

  void check_deposit_and_retrieval (pool_hose h)
  {
    const unt64 STEP (30), N (proteins.size ());
    fillHose (h, 0, N);
    for (unt64 s = 0; s < STEP; ++s)
      for (unt64 i = s; i < N; i += STEP)
        {
          checkProtein (i, h);
        }
  }

  void maybe_check (ob_retort ret, int k, unt64 i, pool_hose h)
  {
    EXPECT_EQ (OB_OK, ret) << "Seek failed for protein no. " << i
                           << " and crit " << k;
    if (OB_OK == ret)
      checkNextProtein (i, h);
  }

  void check_seekto_exact_time (pool_hose h)
  {
    const unt64 STEP (10), N (OICAP * 3 / 2);
    fillHose (h, 1 * MINIMAL_MILLISECONDS, N);
    for (unt64 s = 0; s < STEP; ++s)
      for (unt64 i = s; i < N; i += STEP)
        {
          const DepositedProtein &d = proteins[i];
          for (int k = 0; k < 3; ++k)
            {
              time_comparison b = static_cast<time_comparison> (k);
              ob_retort ret = pool_seekto_time (h, d.stamp, b);
              maybe_check (ret, k, i, h);
            }
        }
  }

  void check_seekto_approx_time (pool_hose h)
  {
    const unt64 STEP (10), N (OICAP * 3 / 2);
    fillHose (h, 2 * MINIMAL_MILLISECONDS, N);
    for (unt64 s = 0; s < STEP; ++s)
      for (unt64 i = s; i < N - 1; i += STEP)
        {
          const DepositedProtein &d = proteins[i];
          const DepositedProtein &dn = proteins[i + 1];
          pool_timestamp post = (d.stamp + dn.stamp) / 2.0 - 0.00001;
          EXPECT_LT (d.stamp, post);
          EXPECT_LT (post, dn.stamp);
          ob_retort ret = pool_seekto_time (h, post, OB_CLOSEST);
          maybe_check (ret, OB_CLOSEST, i, h);
          ret = pool_seekto_time (h, post, OB_CLOSEST_LOWER);
          maybe_check (ret, OB_CLOSEST_LOWER, i, h);
          ret = pool_seekto_time (h, post, OB_CLOSEST_HIGHER);
          maybe_check (ret, OB_CLOSEST_HIGHER, i + 1, h);

          post = (d.stamp + dn.stamp) / 2.0 + 0.00001;
          EXPECT_LT (d.stamp, post);
          EXPECT_LT (post, dn.stamp);
          ret = pool_seekto_time (h, post, OB_CLOSEST);
          maybe_check (ret, OB_CLOSEST, i + 1, h);
          ret = pool_seekto_time (h, post, OB_CLOSEST_LOWER);
          maybe_check (ret, OB_CLOSEST_LOWER, i, h);
          ret = pool_seekto_time (h, post, OB_CLOSEST_HIGHER);
          maybe_check (ret, OB_CLOSEST_HIGHER, i + 1, h);
        }
    // boundaries
    const DepositedProtein &d = proteins[N - 1];
    pool_timestamp ts = d.stamp + 0.0001;
    ob_retort ret = pool_seekto_time (h, ts, OB_CLOSEST);
    maybe_check (ret, OB_CLOSEST, N - 1, h);
    ret = pool_seekto_time (h, ts, OB_CLOSEST_LOWER);
    maybe_check (ret, OB_CLOSEST_LOWER, N - 1, h);
    EXPECT_EQ (POOL_NO_SUCH_PROTEIN,
               pool_seekto_time (h, ts, OB_CLOSEST_HIGHER));

    ts = d.stamp - 0.0001;
    ret = pool_seekto_time (h, ts, OB_CLOSEST);
    maybe_check (ret, OB_CLOSEST, N - 1, h);
    ret = pool_seekto_time (h, ts, OB_CLOSEST_LOWER);
    maybe_check (ret, OB_CLOSEST_LOWER, N - 2, h);
    ret = pool_seekto_time (h, ts, OB_CLOSEST_HIGHER);
    maybe_check (ret, OB_CLOSEST_HIGHER, N - 1, h);

    ts = proteins[0].stamp - 0.0001;
    ret = pool_seekto_time (h, ts, OB_CLOSEST);
    maybe_check (ret, OB_CLOSEST, 0, h);
    if (h != existing_hose_)
      {
        EXPECT_EQ (POOL_NO_SUCH_PROTEIN,
                   pool_seekto_time (h, ts, OB_CLOSEST_LOWER));
      }
    ret = pool_seekto_time (h, ts, OB_CLOSEST_HIGHER);
    maybe_check (ret, OB_CLOSEST_HIGHER, 0, h);
  }

  void check_seekby_time (pool_hose h)
  {
    const unt64 STEP (10), N (OICAP * 3 / 2);
    fillHose (h, 2 * MINIMAL_MILLISECONDS, N);
    for (unt64 i = 0; i < N - 2; i += STEP)
      {
        const DepositedProtein &d = proteins[i];
        const DepositedProtein &dd = proteins[i + 1];

        pool_timestamp delta = (dd.stamp - d.stamp) / 2.0 - 0.00001;
        EXPECT_EQ (OB_OK, pool_seekto (h, d.index));
        ob_retort ret = pool_seekby_time (h, delta, OB_CLOSEST);
        maybe_check (ret, OB_CLOSEST, i, h);
        EXPECT_EQ (OB_OK, pool_seekto (h, d.index));
        ret = pool_seekby_time (h, delta, OB_CLOSEST_LOWER);
        maybe_check (ret, OB_CLOSEST_LOWER, i, h);
        EXPECT_EQ (OB_OK, pool_seekto (h, d.index));
        ret = pool_seekby_time (h, delta, OB_CLOSEST_HIGHER);
        maybe_check (ret, OB_CLOSEST_HIGHER, i + 1, h);

        delta = (dd.stamp - d.stamp) / 2.0 + 0.00001;
        EXPECT_EQ (OB_OK, pool_seekto (h, d.index));
        ret = pool_seekby_time (h, delta, OB_CLOSEST);
        maybe_check (ret, OB_CLOSEST, i + 1, h);
        EXPECT_EQ (OB_OK, pool_seekto (h, d.index));
        ret = pool_seekby_time (h, delta, OB_CLOSEST_LOWER);
        maybe_check (ret, OB_CLOSEST_LOWER, i, h);
        EXPECT_EQ (OB_OK, pool_seekto (h, d.index));
        ret = pool_seekby_time (h, delta, OB_CLOSEST_HIGHER);
        maybe_check (ret, OB_CLOSEST_HIGHER, i + 1, h);

        delta = (dd.stamp - d.stamp) + 0.00001;
        EXPECT_EQ (OB_OK, pool_seekto (h, d.index));
        ret = pool_seekby_time (h, delta, OB_CLOSEST);
        maybe_check (ret, OB_CLOSEST, i + 1, h);
        EXPECT_EQ (OB_OK, pool_seekto (h, d.index));
        ret = pool_seekby_time (h, delta, OB_CLOSEST_LOWER);
        maybe_check (ret, OB_CLOSEST_LOWER, i + 1, h);
        EXPECT_EQ (OB_OK, pool_seekto (h, d.index));
        ret = pool_seekby_time (h, delta, OB_CLOSEST_HIGHER);
        maybe_check (ret, OB_CLOSEST_HIGHER, i + 2, h);
      }
  }
};

TEST_F (TocPoolTest, DepositAndRetrieval)
{
  check_deposit_and_retrieval (full_hose_);
  check_deposit_and_retrieval (over_hose_);
  check_deposit_and_retrieval (existing_hose_);
}

TEST_F (TocPoolTest, SeekToExactTime)
{
  check_seekto_exact_time (full_hose_);
}

TEST_F (TocPoolTest, SeekToExactTimeOverfull)
{
  check_seekto_exact_time (over_hose_);
  check_seekto_exact_time (existing_hose_);
}

TEST_F (TocPoolTest, SeekToApproxTime)
{
  check_seekto_approx_time (full_hose_);
}

TEST_F (TocPoolTest, SeekToApproxTimeOverfull)
{
  check_seekto_approx_time (over_hose_);
  check_seekto_approx_time (existing_hose_);
}

TEST_F (TocPoolTest, SeekByTime)
{
  check_seekby_time (full_hose_);
}

TEST_F (TocPoolTest, SeekByTimeOverfull)
{
  check_seekby_time (over_hose_);
  check_seekby_time (existing_hose_);
}

}  // namespace

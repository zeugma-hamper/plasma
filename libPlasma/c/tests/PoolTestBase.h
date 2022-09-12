
/* (c)  oblong industries */

#ifndef OBLONG_POOLTESTBASE_H
#define OBLONG_POOLTESTBASE_H

#include "plasma-gtest-helpers.h"

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-time.h"

#include <libPlasma/c/pool_options.h>
#include <libPlasma/c/pool.h>
#include <libPlasma/c/slaw.h>

#include <gtest/gtest.h>
#include <vector>

#include <time.h>

struct DepositedProtein
{
  protein prot;
  int64 index;
  pool_timestamp stamp;
};

typedef ::std::vector<DepositedProtein> ProteinArray;

class PoolTestBase : public ::testing::Test
{
 protected:
  typedef PoolTestBase super;

  static void SetUpTestCase ()
  {
    for (unt64 i = 0; i < ICAP; ++i)
      {
        DepositedProtein dp;
        dp.prot = makeProtein (i);
        dp.index = -1;
        dp.stamp = -1;
        proteins.push_back (dp);
      }
    existing_hose_ = createPool (EPNAME.c_str (), PSIZE, 2 * OICAP);
    fillHose (existing_hose_, 0, 4 * OICAP / 3);
  }

  static void TearDownTestCase ()
  {
    for (unt64 i = 0; i < ICAP; ++i)
      protein_free (proteins[i].prot);
    EXPECT_EQ (OB_OK, pool_withdraw (existing_hose_));
    EXPECT_EQ (OB_OK, pool_dispose (EPNAME.c_str ()));
    if (worst_delay > 0)
      {
        info_report ("Had to delay for %" OB_FMT_64 "d milliseconds, which\n"
                     "was perceived as %.3f milliseconds.\n",
                     worst_delay, 1000 * worst_delay_perceived);
      }
  }

  static pool_hose createPool (const char *name, unt64 size, unt64 capacity)
  {
    protein opts = toc_mmap_pool_options (size, capacity);
    pool_hose h = NULL;
    EXPECT_EQ (POOL_CREATED,
               pool_participate_creatingly (name, "mmap", &h, opts));
    protein_free (opts);
    return h;
  }

  static protein makeProtein (unt64 idx)
  {
    return protein_from_ff (slaw_list_inline_f (slaw_unt64 (idx), NULL), NULL);
  }


  void SetUp () override
  {
    full_hose_ = createPool (PNAME.c_str (), PSIZE, ICAP);
    over_hose_ = createPool (OPNAME.c_str (), PSIZE, OICAP);
  }

  void TearDown () override
  {
    OB_DIE_ON_ERROR (pool_withdraw (full_hose_));
    OB_DIE_ON_ERROR (pool_dispose (PNAME.c_str ()));
    OB_DIE_ON_ERROR (pool_withdraw (over_hose_));
    OB_DIE_ON_ERROR (pool_dispose (OPNAME.c_str ()));
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

  static void fillHose (pool_hose hose, int64 msecs, unt64 upto)
  {
    for (unt64 i = 0; i < upto; ++i)
      {
        DepositedProtein &p = proteins[i % ICAP];
        EXPECT_EQ (OB_OK, pool_deposit_ex (hose, p.prot, &p.index, &p.stamp));
        pause (msecs);
      }
  }

  void checkProtein (unt64 n, pool_hose hose = NULL)
  {
    if (!hose)
      hose = full_hose_;
    const DepositedProtein &d = proteins[n];
    protein p (NULL);
    pool_timestamp stamp (-1);
    EXPECT_EQ (OB_OK, pool_nth_protein (hose, d.index, &p, &stamp));
    EXPECT_TRUE (proteins_equal (d.prot, p)) << "Expected: " << d.prot
                                             << "Retrieved: " << p;
    protein_free (p);
  }

  void checkNextProtein (unt64 n, pool_hose hose = NULL)
  {
    if (!hose)
      hose = full_hose_;
    const DepositedProtein &d = proteins[n];
    protein p (NULL);
    pool_timestamp ts (-1);
    int64 idx (-1);
    EXPECT_EQ (OB_OK, pool_next (hose, &p, &ts, &idx));
    EXPECT_TRUE (proteins_equal (d.prot, p)) << "Expected: " << d.prot
                                             << "Retrieved: " << p;
    EXPECT_EQ (d.index, idx);
    EXPECT_EQ (d.stamp, ts);
    protein_free (p);
  }

  pool_hose full_hose_;
  pool_hose over_hose_;
  static pool_hose existing_hose_;

  static ProteinArray proteins;
  static const std::string BNAME, PNAME, OPNAME, EPNAME;
  static const unt64 PSIZE, ICAP, OICAP;

  static int64 worst_delay;
  static pool_timestamp worst_delay_perceived;
};


static inline const char *ob_defaultify (const char *str, const char *def)
{
  return (str ? str : def);
}


ProteinArray PoolTestBase::proteins;
pool_hose PoolTestBase::existing_hose_ (NULL);
const std::string PoolTestBase::BNAME (ob_defaultify (getenv ("TEST_POOL"),
                                                      "toc-test-pool"));
const std::string PoolTestBase::PNAME (BNAME + "-big");
const std::string PoolTestBase::OPNAME (BNAME + "-small");
const std::string PoolTestBase::EPNAME (BNAME + "-existing");
const unt64 PoolTestBase::PSIZE = MEGABYTE;
const unt64 PoolTestBase::ICAP = MEGABYTE / 1000;
const unt64 PoolTestBase::OICAP = 80;
int64 PoolTestBase::worst_delay = 0;
pool_timestamp PoolTestBase::worst_delay_perceived = 0;


#endif  // OBLONG_POOLTESTBASE_H

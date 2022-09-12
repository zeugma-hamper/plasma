
/* (c)  oblong industries */

#include "PoolTestBase.h"
#include "libLoam/c++/LoamStreams.h"
#include "libPlasma/c++/Plasma.h"

#include <gtest/gtest.h>
#include <libLoam/c/ob-pthread.h>
#include <libLoam/c/ob-time.h>

using namespace oblong::plasma;

struct thread_info
{
  volatile int64 progress;
  volatile bool done;
  const char *poolName;
};

#define A_WHILE 3000

static void *thread_main (void *v)
{
  thread_info *info = static_cast<thread_info *> (v);
  int64 oldest, idx;
  int64 i = 0;
  pool_hose hose = NULL;

  EXPECT_EQ (OB_OK, pool_participate (info->poolName, &hose, NULL));

  do
    {
      slaw ingests = slaw_map_inline_cf ("bad wolf", slaw_int64 (i), NULL);
      protein p = protein_from_ff (NULL, ingests);
      EXPECT_EQ (OB_OK, pool_deposit (hose, p, &idx));
      EXPECT_EQ (i, idx);
      EXPECT_EQ (OB_OK, pool_oldest_index (hose, &oldest));
      EXPECT_LE (oldest, i);
      protein_free (p);
      info->progress = ++i;
    }
  while (oldest < A_WHILE);  // wait until we've wrapped around for a while

  EXPECT_EQ (OB_OK, pool_withdraw (hose));

  info->done = true;
  return NULL;
}

TEST (HoseNthTest, MultiThreaded)
{
  const ::std::string PNAME (
    PoolTestBase::PoolName ("Raxacoricofallapatorius"));

  // A small 10K pool will wrap much faster
  ObRetort ret = Pool::Create (PNAME.c_str (), "mmap", false,
                               Slaw (mmap_pool_options (10 * 1024)));
  ASSERT_TRUE (ret.IsSplend ()) << ret;

  Hose *h = Pool::Participate (PNAME.c_str ());
  ASSERT_FALSE (h == NULL);

  thread_info info;
  info.progress = 0;
  info.done = false;
  info.poolName = PNAME.c_str ();

  pthread_t thr;
  EXPECT_EQ (0, pthread_create (&thr, NULL, thread_main, &info));

  int64 prev_progress = 0;
  while (!info.done)
    {
      int64 oldest_index;
      do
        oldest_index = h->OldestIndex ();
      while (oldest_index == Protein::NO_INDEX
             && OB_OK == ob_micro_sleep (923));

      int64 from = oldest_index - 10;
      int64 to = oldest_index + 10;
      if (from < 0)
        from = 0;

      for (int64 i = from; i < to; i++)
        {
          Protein pr = h->Nth (i);
          ob_retort code = h->LastRetort ().Code ();
          if (code == POOL_NO_SUCH_PROTEIN)
            continue;
          EXPECT_EQ (OB_OK, code);
          EXPECT_EQ (i, pr.Index ());
          EXPECT_EQ (pr.Index (),
                     pr.Ingests ().Find ("bad wolf").Emit<int64> ());
        }

      int64 cur_progress = info.progress;
      if (prev_progress == cur_progress)
        ob_micro_sleep (923);
      prev_progress = cur_progress;
    }

  EXPECT_EQ (0, pthread_join (thr, NULL));
  h->Withdraw ();
  h->Delete ();
  Pool::Dispose (PNAME.c_str ());
}

TEST (HoseNthTest, SingleThreaded)
{
  const ::std::string PNAME (PoolTestBase::PoolName ("Shadow Proclamation"));

  // A small 10K pool will wrap much faster
  ObRetort ret = Pool::Create (PNAME.c_str (), "mmap", false,
                               Slaw (mmap_pool_options (10 * 1024)));
  ASSERT_TRUE (ret.IsSplend ()) << ret;

  thread_info info;
  info.progress = 0;
  info.done = false;
  info.poolName = PNAME.c_str ();

  // fill the pool (but single threaded this time)
  thread_main (&info);

  Hose *h = Pool::Participate (PNAME.c_str ());
  ASSERT_FALSE (h == NULL);

  int64 oldest = h->OldestIndex ();
  int64 newest = h->NewestIndex ();
  int64 current = h->CurrentIndex ();

  EXPECT_EQ (info.progress, current);
  EXPECT_EQ (info.progress - 1, newest);
  EXPECT_EQ (A_WHILE, oldest);

  for (int64 i = oldest - 10; i < oldest + 10; i++)
    {
      Protein pr = h->Nth (i);
      ob_retort code = h->LastRetort ().Code ();
      ob_retort expected_code = (i < oldest ? POOL_NO_SUCH_PROTEIN : OB_OK);
      EXPECT_EQ (expected_code, code);
      if (code >= OB_OK)
        {
          EXPECT_EQ (i, pr.Index ());
          EXPECT_EQ (pr.Index (),
                     pr.Ingests ().Find ("bad wolf").Emit<int64> ());
        }
    }

  for (int64 i = newest - 10; i < newest + 10; i++)
    {
      Protein pr = h->Nth (i);
      ob_retort code = h->LastRetort ().Code ();
      ob_retort expected_code = (i > newest ? POOL_NO_SUCH_PROTEIN : OB_OK);
      EXPECT_EQ (expected_code, code);
      if (code >= OB_OK)
        {
          EXPECT_EQ (i, pr.Index ());
          EXPECT_EQ (pr.Index (),
                     pr.Ingests ().Find ("bad wolf").Emit<int64> ());
        }
    }

  h->Withdraw ();
  h->Delete ();
  Pool::Dispose (PNAME.c_str ());
}


/* (c)  oblong industries */

#include <gtest/gtest.h>
#include "libLoam/c/ob-atomic.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-pthread.h"
#include "libLoam/c/ob-rand.h"
#include "libLoam/c/ob-util.h"
#include "libLoam/c/ob-vers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* Note: Although Google test is threadsafe on UNIX, it is not threadsafe
 * on Windows:
 * http://code.google.com/p/googletest/wiki/V1_5_Primer#Known_Limitations
 *
 * So, since this test can run on Windows, we can't assume thread-safety.
 * Therefore, we only use Google Test assertions (EXPECT_EQ, etc.)
 * in the main thread, and we just use good old-fashioned error_exit()
 * to handle failures in any threads we spawn.
 */

#define AITERATIONS 50000
#define ATHREADS 8
#define SITERATIONS siterations
#define STHREADS 3
#define RITERATIONS 50000
#define RTHREADS 4  // but really double that

/* The swap tests go considerably slower if each thread does not have
 * its own core.
 *
 * For siterations = 100, test_swap32() takes:
 *  - 18 ms on an 8-core physical Linux machine
 *  - 4612 ms on a single-core virtual Linux machine
 *  - 28547 ms on a single-core virtual Windows machine
 *
 * Therefore, reduce the number of iterations if we don't have enough cores.
 */

const int siterations =
  (ob_get_system_info (OB_SYSINFO_NUM_CORES) < STHREADS) ? 10 : 100;

#define INTEGER_TEST(width)                                                    \
  static void *thread_main_##width (void *vloc)                                \
  {                                                                            \
    int##width *loc = (int##width *) vloc;                                     \
    int i;                                                                     \
    int##width prev = 0;                                                       \
    for (i = 0; i < AITERATIONS; i++)                                          \
      {                                                                        \
        int##width ret = ob_atomic_int##width##_add (loc, 1);                  \
        if (prev >= ret)                                                       \
          error_exit ("%" OB_FMT_64 "d >= %" OB_FMT_64 "d\n", (int64) prev,    \
                      (int64) ret);                                            \
        prev = ret;                                                            \
      }                                                                        \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  static void test_##width (void)                                              \
  {                                                                            \
    pthread_t thr[ATHREADS];                                                   \
    int##width loc;                                                            \
    int i;                                                                     \
    ob_atomic_int##width##_set (&loc, 0);                                      \
    for (i = 0; i < ATHREADS; i++)                                             \
      {                                                                        \
        int erryes =                                                           \
          pthread_create (thr + i, NULL, thread_main_##width, &loc);           \
        if (erryes != 0)                                                       \
          error_exit ("pthread_create: %s\n", strerror (erryes));              \
      }                                                                        \
    for (i = 0; i < ATHREADS; i++)                                             \
      {                                                                        \
        int erryes = pthread_join (thr[i], NULL);                              \
        if (erryes != 0)                                                       \
          error_exit ("pthread_join: %s\n", strerror (erryes));                \
      }                                                                        \
    const int##width expected = AITERATIONS * ATHREADS;                        \
    int##width actual = ob_atomic_int##width##_ref (&loc);                     \
    EXPECT_EQ (expected, actual);                                              \
  }                                                                            \
                                                                               \
  typedef struct                                                               \
  {                                                                            \
    pthread_t thr;                                                             \
    int##width *loc;                                                           \
    int ur;                                                                    \
  } stuff##width;                                                              \
                                                                               \
  static void *thread_main_swap##width (void *v)                               \
  {                                                                            \
    stuff##width *stuff = (stuff##width *) v;                                  \
    int i;                                                                     \
    for (i = 0; i < SITERATIONS; i++)                                          \
      {                                                                        \
        bool b = false;                                                        \
        int##width was = i * STHREADS + stuff->ur;                             \
        int##width shall_be = was + 1;                                         \
        do                                                                     \
          {                                                                    \
            b = ob_atomic_int##width##_compare_and_swap (stuff->loc, was,      \
                                                         shall_be);            \
          }                                                                    \
        while (!b);                                                            \
      }                                                                        \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  static void test_swap##width (void)                                          \
  {                                                                            \
    stuff##width stuff[STHREADS];                                              \
    int##width *loc = (int##width *) malloc (sizeof (int##width));             \
    int i;                                                                     \
    ob_atomic_int##width##_set (loc, 0);                                       \
    for (i = 0; i < STHREADS; i++)                                             \
      {                                                                        \
        stuff[i].loc = loc;                                                    \
        stuff[i].ur = i;                                                       \
        int erryes = pthread_create (&(stuff[i].thr), NULL,                    \
                                     thread_main_swap##width, stuff + i);      \
        if (erryes != 0)                                                       \
          error_exit ("pthread_create: %s\n", strerror (erryes));              \
      }                                                                        \
    for (i = 0; i < STHREADS; i++)                                             \
      {                                                                        \
        int erryes = pthread_join (stuff[i].thr, NULL);                        \
        if (erryes != 0)                                                       \
          error_exit ("pthread_join: %s\n", strerror (erryes));                \
      }                                                                        \
    const int##width expected = SITERATIONS * STHREADS;                        \
    int##width actual = ob_atomic_int##width##_ref (loc);                      \
    EXPECT_EQ (expected, actual);                                              \
    free (loc);                                                                \
  }

INTEGER_TEST (32);
INTEGER_TEST (64);

static void *thread_main_ref64 (void *vloc)
{
  int64 *loc = (int64 *) vloc;
  int i;
  for (i = 0; i < RITERATIONS; i++)
    {
      int64 x = ob_atomic_int64_ref (loc);
      int64 y = x;
      int n = 0;
      while (y)
        {
          n += (y < 0);
          y <<= 1;
        }
      if (n != 32)
        error_exit ("on iteration %d, I got 0x%016" OB_FMT_64 "x, which has "
                    "%d bits set, but expected 32 bits set\n",
                    i, x, n);
    }
  return NULL;
}

typedef struct
{
  int64 *loc;
  ob_rand_t *state;
} stuff_set;

static void *thread_main_set64 (void *v)
{
  stuff_set *stuff = (stuff_set *) v;
  int i;
  for (i = 0; i < RITERATIONS; i++)
    {
      unt32 x = ob_rand_state_unt32 (stuff->state);
      unt32 y = ~x;
      unt64 z = y | (((unt64) x) << 32);
      ob_atomic_int64_set (stuff->loc, z);
    }
  return NULL;
}

static void test_set_ref_64 (void)
{
  pthread_t thr[2 * RTHREADS];
  stuff_set stuff[RTHREADS];
  int i;
  int64 loc = OB_CONST_U64 (0xaaaaaaaaaaaaaaaa);

  for (i = 0; i < RTHREADS; i++)
    {
      stuff[i].loc = &loc;
      stuff[i].state =
        ob_rand_allocate_state (OB_RAND_COMPLETELY_RANDOM_PLEASE);
    }

  for (i = 0; i < RTHREADS; i++)
    {
      int erryes = pthread_create (thr + i, NULL, thread_main_set64, stuff + i);
      if (erryes != 0)
        error_exit ("pthread_create: %s\n", strerror (erryes));

      erryes =
        pthread_create (thr + i + RTHREADS, NULL, thread_main_ref64, &loc);
      if (erryes != 0)
        error_exit ("pthread_create: %s\n", strerror (erryes));
    }

  for (i = 0; i < RTHREADS * 2; i++)
    {
      int erryes = pthread_join (thr[i], NULL);
      if (erryes != 0)
        error_exit ("pthread_join: %s\n", strerror (erryes));
    }

  for (i = 0; i < RTHREADS; i++)
    ob_rand_free_state (stuff[i].state);
}

static void test_set_ref_64_reloaded (void)
{
  int i;
  int64 *u = (int64 *) calloc (1, sizeof (int64));
  for (i = 0; i < RITERATIONS; i++)
    {
      unt64 x = ob_rand_unt64 ();
      ob_atomic_int64_set (u, (int64) x);
      unt64 y = (unt64) ob_atomic_int64_ref (u);
      EXPECT_EQ (x, y) << "on iteration " << i;
    }
  free (u);
}

static void test_other_pointer_type (void)
{
  // Test ob_atomic_pointer_compare_and_swap ()
  int64 x, y, z;
  int64 *ptr1 = &x;
  int64 *ptr2 = &y;
  int64 *ptr3 = &z;
  int64 *loc;

  ob_atomic_pointer_set (&loc, ptr1);

  bool b = ob_atomic_pointer_compare_and_swap (&loc, ptr3, ptr2);
  EXPECT_FALSE (b);

  int64 *actual = ob_atomic_pointer_ref (&loc);
  EXPECT_EQ (ptr1, actual);

  b = ob_atomic_pointer_compare_and_swap (&loc, ptr1, ptr2);
  EXPECT_TRUE (b);

  actual = ob_atomic_pointer_ref (&loc);
  EXPECT_EQ (ptr2, actual);
}

static int that_thing;

TEST (TestAtomic, AtomicAdd)
{
  // Test that atomic add returns the new value.
  int64 *loc64 = (int64 *) malloc (sizeof (int64));
  ob_atomic_int64_set (loc64, OB_CONST_I64 (0xbadbadbad));
  EXPECT_EQ (OB_CONST_I64 (0xbadbadbad), *loc64);
  int64 result = ob_atomic_int64_add (loc64, OB_CONST_I64 (17468402885442));
  const int64 expected = OB_CONST_I64 (0xfeedbadbeef);
  EXPECT_EQ (expected, result);
  int64 final = ob_atomic_int64_ref (loc64);
  EXPECT_EQ (expected, final);
  free (loc64);
}

TEST (TestAtomic, CompareAndSwap)
{
  // Test ob_atomic_pointer_compare_and_swap ()
  bool foo = false;
  void *ptr1 = &foo;
  void *ptr2 = &that_thing;
  void *ptr3 = &errno;

  void **locp = (void **) malloc (sizeof (void *));
  ob_atomic_pointer_set (locp, ptr1);

  bool b = ob_atomic_pointer_compare_and_swap (locp, ptr3, ptr2);
  EXPECT_FALSE (b);

  void *actual = ob_atomic_pointer_ref (locp);
  EXPECT_EQ (ptr1, actual);

  b = ob_atomic_pointer_compare_and_swap (locp, ptr1, ptr2);
  EXPECT_TRUE (b);

  actual = ob_atomic_pointer_ref (locp);
  EXPECT_EQ (ptr2, actual);

  free (locp);
}

TEST (TestAtomic, OtherPointer)
{
  test_other_pointer_type ();
}

// These tests make progress whether scheduling is fair or not

TEST (TestAtomic, Test32)
{
  test_32 ();
}

TEST (TestAtomic, Test64)
{
  test_64 ();
}

TEST (TestAtomic, SetRef64)
{
  test_set_ref_64 ();
}

TEST (TestAtomic, SetRef64Reloaded)
{
  test_set_ref_64_reloaded ();
}

// These tests assume fair scheduling in order to make progress,
// and for some reason, scheduling under valgrind is extremely
// unfair.  These tests might never complete under valgrind, so
// don't run them!

TEST (TestAtomic, Swap32)
{
  if (!ob_running_under_valgrind ())
    test_swap32 ();
}

TEST (TestAtomic, Swap64)
{
  if (!ob_running_under_valgrind ())
    test_swap64 ();
}

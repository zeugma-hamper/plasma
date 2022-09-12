
/* (c)  oblong industries */

#include "libLoam/c/ob-rand.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-sys.h"
#include "libLoam/c/ob-time.h"
#include <stdlib.h>
#include <stdio.h>


// A lot of stuff is tested by libLoam/c++/test_rand.cpp.
// However, here we test a few more things.
// - two states with the same seed should produce the same sequence
// - two states with different seeds should produce different sequences
// - two "completely random" states should produce different sequences
// - some of the more recently added functions

#define SOME_NUMBER 0x4ffe874

/**
 * Verify that \a a and \a b produce the same sequence of
 * random numbers.
 */
static bool same_sequence (ob_rand_t *a, ob_rand_t *b)
{
  int i;

  for (i = 0; i < 25; i++)
    {
      int x = ob_rand_state_int32 (-SOME_NUMBER, SOME_NUMBER, a);
      int y = ob_rand_state_int32 (-SOME_NUMBER, SOME_NUMBER, b);
      if (x < -SOME_NUMBER || x >= SOME_NUMBER)
        error_exit ("%d should have been between %d and %d\n", x, -SOME_NUMBER,
                    SOME_NUMBER);
      if (y < -SOME_NUMBER || y >= SOME_NUMBER)
        error_exit ("%d should have been between %d and %d\n", y, -SOME_NUMBER,
                    SOME_NUMBER);
      if (x != y)
        return false;
    }

  return true;
}

static float64 dummy_xbYHNr5D[3];
static char dummy_UNPdyzTK[4];

/**
 * This just accesses all the bytes of \a u in a way that won't get
 * optimized away.  If any of the bytes are undefined, valgrind will
 * catch it.
 */
static void check_well_defined (const unt8 *u, size_t n)
{
  size_t i;
  for (i = 0; i < n; i++)
    snprintf (dummy_UNPdyzTK, sizeof (dummy_UNPdyzTK), "%02x", u[i]);
}

static void check_ob_rand_state_unt64_distribution (int too_long,
                                                    ob_rand_t *state)
{
  // Test that in a random unt64, every bit can be either 1 or 0
  unt64 ones = 0;
  unt64 zeros = UNT64_ALLBITS;
  int tries;
  for (tries = 0; tries < too_long && !(ones == UNT64_ALLBITS && zeros == 0);
       tries++)
    {
      unt64 r = ob_rand_state_unt64 (state);
      ones |= r;
      zeros &= r;
    }

  if (!(ones == UNT64_ALLBITS && zeros == 0))
    {
      error_exit ("didn't flip all unt64 bits after %d tries\n"
                  "ones was %016" OB_FMT_64 "x and "
                  "max was %016" OB_FMT_64 "x\n",
                  too_long, ones, zeros);
    }
}

static void check_ob_rand_unt32_distribution (int too_long)
{
  // Test that in a random unt64, every bit can be either 1 or 0
  unt32 ones = 0;
  unt32 zeros = UNT32_ALLBITS;
  int tries;
  for (tries = 0; tries < too_long && !(ones == UNT32_ALLBITS && zeros == 0);
       tries++)
    {
      unt32 r = ob_rand_unt32 ();
      ones |= r;
      zeros &= r;
    }

  if (!(ones == UNT32_ALLBITS && zeros == 0))
    {
      error_exit ("didn't flip all unt32 bits after %d tries\n"
                  "ones was %016ux and max was %016ux\n",
                  too_long, ones, zeros);
    }
}

static void check_ob_rand_int32_distribution (int too_long)
{
  // Test that in a random int32, every bit can be either 1 or 0. We
  // can't actually test the bits of the int32, but we can cast to
  // unt32, which is well-defined.
  unt32 ones = 0;
  unt32 zeros = UNT32_ALLBITS;
  int tries;
  for (tries = 0; tries < too_long && !(ones == UNT32_ALLBITS && zeros == 0);
       tries++)
    {
      int32 r = ob_rand_int32 (OB_INT32_MIN, OB_INT32_MAX);
      ones |= (unt32) r;
      zeros &= (unt32) r;
    }

  if (!(ones == UNT32_ALLBITS && zeros == 0))
    {
      error_exit ("didn't flip all int32 bits after %d tries\n"
                  "ones was %016ux and max was %016ux\n",
                  too_long, ones, zeros);
    }
}

static void check_ob_rand_boolean_distribution (int too_long, ob_rand_t *state)
{
  // Test we get a reasonable distribution of booleans (yes, there's
  // no guarantee this test won't fail, but we hope the probability
  // is extremely small).
  int ones = 0;
  int tries;
  for (tries = 0; tries < too_long; tries++)
    {
      int32 r = ob_rand_state_int32 (0, 2, state);
      if (r != 0 && r != 1)
        error_exit ("expected 0 or 1, but got %d\n", r);
      if (r)
        ones++;
    }

  float64 percentage = 100 * ones / (float64) too_long;
  if (percentage < 45 || percentage > 55)
    error_exit ("percentage of true booleans was %f%%\n", percentage);
}


int main (int argc, char **argv)
{
  unt64 scratch;
  const unt64 t1 = ob_monotonic_time ();
  OB_DIE_ON_ERROR (ob_truly_random (&scratch, sizeof (scratch)));
  const unt64 t2 = ob_monotonic_time ();
  OB_DIE_ON_ERROR (ob_truly_random (&scratch, sizeof (scratch)));
  const unt64 t3 = ob_monotonic_time ();

  info_report ("first ob_truly_random: %" OB_FMT_64 "u ns\n"
               "second ob_truly_random: %" OB_FMT_64 "u ns\n",
               t2 - t1, t3 - t2);

  ob_rand_t *a = ob_rand_allocate_state (1234);
  ob_rand_t *b = ob_rand_allocate_state (5678);

  if (same_sequence (a, b))
    error_exit ("sequences were the same and should not have been\n");

  ob_rand_state_seed_int32 (a, 923);
  ob_rand_state_seed_int32 (b, 923);

  if (!same_sequence (a, b))
    error_exit ("sequences were different and should have been the same\n");

  ob_rand_state_seed_int32 (a, OB_RAND_COMPLETELY_RANDOM_PLEASE);
  ob_rand_state_seed_int32 (b, OB_RAND_COMPLETELY_RANDOM_PLEASE);

  // Yes, there's a 1 in 2**128 chance this could fail anyway.
  // (Because OB_RAND_COMPLETELY_RANDOM_PLEASE seeds dSFMT with 128
  // bits from the entropy source; i. e. ob_truly_random.)
  if (same_sequence (a, b))
    error_exit ("random sequences were the same and should not have been\n");

  const int too_long = 10000;

  ob_rand_t *c = ob_rand_allocate_state (OB_RAND_COMPLETELY_RANDOM_PLEASE);
  check_ob_rand_state_unt64_distribution (too_long, c);

  ob_rand_state_seed_int32 (c, OB_RAND_COMPLETELY_RANDOM_PLEASE);
  check_ob_rand_unt32_distribution (too_long);

  ob_rand_state_seed_int32 (c, OB_RAND_COMPLETELY_RANDOM_PLEASE);
  check_ob_rand_int32_distribution (too_long);

  ob_rand_state_seed_int32 (c, OB_RAND_COMPLETELY_RANDOM_PLEASE);
  check_ob_rand_boolean_distribution (too_long, c);

  // Make sure we can call ob_rand_normal()
  dummy_xbYHNr5D[0] = ob_rand_normal (NULL);
  dummy_xbYHNr5D[1] = ob_rand_normal (&dummy_xbYHNr5D[2]);

  // Make sure we can call ob_rand_normal_state()
  dummy_xbYHNr5D[0] = ob_rand_normal_state (c, NULL);
  dummy_xbYHNr5D[1] = ob_rand_normal_state (c, &dummy_xbYHNr5D[2]);

  // Similar thing for ob_random_bytes()
  size_t i;
  for (i = 0; i < 100; i++)
    {
      unt8 *u = (unt8 *) malloc (i);
      ob_random_bytes (u, i);
      check_well_defined (u, i);
      free (u);
    }

  // ... and ob_random_bytes_state()
  for (i = 0; i < 100; i++)
    {
      unt8 *u = (unt8 *) malloc (i);
      ob_random_bytes_state (c, u, i);
      check_well_defined (u, i);
      free (u);
    }

  // For ob_truly_random(), we can also check if it returned OB_OK.
  for (i = 0; i < 100; i++)
    {
      unt8 *u = (unt8 *) malloc (i);
      OB_DIE_ON_ERROR (ob_truly_random (u, i));
      check_well_defined (u, i);
      free (u);
    }

  ob_rand_free_state (a);
  ob_rand_free_state (b);
  ob_rand_free_state (c);

  return EXIT_SUCCESS;
}

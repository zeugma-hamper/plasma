
/* (c)  oblong industries */

/* Checks that the properties of various numeric types are what we
 * expect them to be. */

#include "libLoam/c/ob-log.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/slaw-walk.h"
#include "libPlasma/c/slaw-ordering.h"
#include "libPlasma/c/slaw-string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static bool globalFail = false;

static void slaw_and_order (slaw s, const char *name, int actualSize)
{
  int baseSize, vectoricity, complexity, expected, bsize;
  bool fail;

  if (slaw_is_numeric_8 (s))
    baseSize = 1;
  else if (slaw_is_numeric_16 (s))
    baseSize = 2;
  else if (slaw_is_numeric_32 (s))
    baseSize = 4;
  else if (slaw_is_numeric_64 (s))
    baseSize = 8;
  else
    baseSize = 0; /* shouldn't happen */

  vectoricity = slaw_numeric_vector_dimension (s);

  if (slaw_is_numeric_complex (s))
    complexity = 2;
  else
    complexity = 1;

  if (slaw_is_numeric_multivector (s))
    expected = baseSize * (1 << vectoricity);
  else
    expected = baseSize * vectoricity * complexity;
  bsize = slaw_numeric_unit_bsize (s);

  fail = ((expected != bsize) || (expected != actualSize));

  printf ("%s %-20s %d * %d * %d = %2d", (fail ? "BAD! " : "good:"), name,
          baseSize, vectoricity, complexity, expected);

  if (expected != bsize)
    {
      printf (" != bsize %2d\n", bsize);
      globalFail = true;
    }
  else if (expected != actualSize)
    {
      printf (" != actual %2d\n", actualSize);
      globalFail = true;
    }
  else
    {
      printf ("\n");
    }

  // check that arrays are naturally aligned
  if (slaw_is_numeric_array (s) && slaw_numeric_array_count (s) > 0)
    {
      unt64 addr = (unt64) (unsigned long) slaw_numeric_array_emit (s);
      if (addr == 0)
        OB_FATAL_ERROR_CODE (0x20308000, "array claims to be NULL\n");
      if (addr % baseSize != 0)
        OB_FATAL_ERROR_CODE (0x20308001, "%" OB_FMT_64 "x is not %d-aligned\n",
                             addr, baseSize);
    }
}

static void check_one (slaw s, const char *name, int actualSize)
{
  slaw_and_order (s, name, actualSize);

  ob_retort err;
  slaw_fabricator *sf = slaw_fabricator_new ();
  if ((err = slaw_walk (sf, &slaw_fabrication_handler, s)) != OB_OK)
    OB_FATAL_ERROR_CODE (0x20308002, "slaw_walk returned an error: %s\n",
                         ob_error_string (err));

  if (slaw_semantic_compare (s, sf->result) != 0)
    {
      fprintf (stderr, "====================\n");
      slaw_spew_overview_to_stderr (s);
      fprintf (stderr, "\n====================\n");
      slaw_spew_overview_to_stderr (sf->result);
      fprintf (stderr, "\n====================\n");
      OB_FATAL_ERROR_CODE (0x20308003, "not equal: %s\n", name);
    }
  if (!slawx_equal (s, sf->result))
    {
      fprintf (stderr, "====================\n");
      slaw_spew_overview_to_stderr (s);
      fprintf (stderr, "\n====================\n");
      slaw_spew_overview_to_stderr (sf->result);
      fprintf (stderr, "\n====================\n");
      OB_FATAL_ERROR_CODE (0x20308004, "not equal: %s\n", name);
    }

  slaw walk_name = slaw_string_format ("%s.walk", name);
  slaw_and_order (sf->result, slaw_string_emit (walk_name), actualSize);
  slaw_free (walk_name);
  slaw_fabricator_free (sf);
}

#define CHECK(type)                                                            \
  type var_##type;                                                             \
  memset (&var_##type, 0, sizeof (var_##type));                                \
  s = slaw_##type (var_##type);                                                \
  check_one (s, #type, sizeof (var_##type));                                   \
  slaw_free (s);                                                               \
  s = slaw_##type##_array_filled (1, var_##type);                              \
  check_one (s, #type "[1]", sizeof (var_##type));                             \
  slaw_free (s);                                                               \
  s = slaw_##type##_array_filled (1993, var_##type);                           \
  check_one (s, #type "[1993]", sizeof (var_##type));                          \
  slaw_free (s);                                                               \
  s = slaw_##type##_array_filled (0, var_##type);                              \
  check_one (s, #type "[0]", sizeof (var_##type));                             \
  slaw_free (s);

int main (int argc, char **argv)
{
  slaw s;

  CHECK (int32);
  CHECK (unt32);
  CHECK (int64);
  CHECK (unt64);
  CHECK (float32);
  CHECK (float64);
  CHECK (int8);
  CHECK (unt8);
  CHECK (int16);
  CHECK (unt16);
  CHECK (int32c);
  CHECK (unt32c);
  CHECK (int64c);
  CHECK (unt64c);
  CHECK (float32c);
  CHECK (float64c);
  CHECK (int8c);
  CHECK (unt8c);
  CHECK (int16c);
  CHECK (unt16c);
  CHECK (v2int32);
  CHECK (v2unt32);
  CHECK (v2int64);
  CHECK (v2unt64);
  CHECK (v2float32);
  CHECK (v2float64);
  CHECK (v2int8);
  CHECK (v2unt8);
  CHECK (v2int16);
  CHECK (v2unt16);
  CHECK (v3int32);
  CHECK (v3unt32);
  CHECK (v3int64);
  CHECK (v3unt64);
  CHECK (v3float32);
  CHECK (v3float64);
  CHECK (v3int8);
  CHECK (v3unt8);
  CHECK (v3int16);
  CHECK (v3unt16);
  CHECK (v4int32);
  CHECK (v4unt32);
  CHECK (v4int64);
  CHECK (v4unt64);
  CHECK (v4float32);
  CHECK (v4float64);
  CHECK (v4int8);
  CHECK (v4unt8);
  CHECK (v4int16);
  CHECK (v4unt16);
  CHECK (v2int32c);
  CHECK (v2unt32c);
  CHECK (v2int64c);
  CHECK (v2unt64c);
  CHECK (v2float32c);
  CHECK (v2float64c);
  CHECK (v2int8c);
  CHECK (v2unt8c);
  CHECK (v2int16c);
  CHECK (v2unt16c);
  CHECK (v3int32c);
  CHECK (v3unt32c);
  CHECK (v3int64c);
  CHECK (v3unt64c);
  CHECK (v3float32c);
  CHECK (v3float64c);
  CHECK (v3int8c);
  CHECK (v3unt8c);
  CHECK (v3int16c);
  CHECK (v3unt16c);
  CHECK (v4int32c);
  CHECK (v4unt32c);
  CHECK (v4int64c);
  CHECK (v4unt64c);
  CHECK (v4float32c);
  CHECK (v4float64c);
  CHECK (v4int8c);
  CHECK (v4unt8c);
  CHECK (v4int16c);
  CHECK (v4unt16c);

  CHECK (m2int32);
  CHECK (m2unt32);
  CHECK (m2int64);
  CHECK (m2unt64);
  CHECK (m2float32);
  CHECK (m2float64);
  CHECK (m2int8);
  CHECK (m2unt8);
  CHECK (m2int16);
  CHECK (m2unt16);

  CHECK (m3int32);
  CHECK (m3unt32);
  CHECK (m3int64);
  CHECK (m3unt64);
  CHECK (m3float32);
  CHECK (m3float64);
  CHECK (m3int8);
  CHECK (m3unt8);
  CHECK (m3int16);
  CHECK (m3unt16);

  CHECK (m4int32);
  CHECK (m4unt32);
  CHECK (m4int64);
  CHECK (m4unt64);
  CHECK (m4float32);
  CHECK (m4float64);
  CHECK (m4int8);
  CHECK (m4unt8);
  CHECK (m4int16);
  CHECK (m4unt16);

  CHECK (m5int32);
  CHECK (m5unt32);
  CHECK (m5int64);
  CHECK (m5unt64);
  CHECK (m5float32);
  CHECK (m5float64);
  CHECK (m5int8);
  CHECK (m5unt8);
  CHECK (m5int16);
  CHECK (m5unt16);

  return (globalFail ? EXIT_FAILURE : EXIT_SUCCESS);
}

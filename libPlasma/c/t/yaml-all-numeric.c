
/* (c)  oblong industries */

#include "libLoam/c/ob-rand.h"
#include "libLoam/c/ob-log.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/slaw-io.h"
#include "libPlasma/c/slaw-ordering.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void randomly_fill (void *v, size_t s)
{
  byte *b = (byte *) v;
  ob_random_bytes (b, s);
}

static void check_one (slaw s, const char *name, int actualSize)
{
  slaw s2;
  const char *filename = "scratch/yaml-all-numeric-temp.txt";

  OB_DIE_ON_ERROR (slaw_write_to_text_file (filename, s));
  OB_DIE_ON_ERROR (slaw_read_from_file (filename, &s2));

  if (slaw_semantic_compare (s, s2) != 0)
    OB_FATAL_ERROR_CODE (0x20318000, "not equal: %s\n", name);
  if (!slawx_equal (s, s2))
    OB_FATAL_ERROR_CODE (0x20318001, "not equal: %s\n", name);

  slaw_free (s2);
}

#define CHECK(type)                                                            \
  type var_##type;                                                             \
  memset (&var_##type, 0, sizeof (var_##type));                                \
  s = slaw_##type (var_##type);                                                \
  check_one (s, #type " zero", sizeof (var_##type));                           \
  isFloat = slaw_is_numeric_float (s);                                         \
  slaw_free (s);                                                               \
  if (!isFloat)                                                                \
    {                                                                          \
      randomly_fill (&var_##type, sizeof (var_##type));                        \
      s = slaw_##type (var_##type);                                            \
      check_one (s, #type " rand", sizeof (var_##type));                       \
      slaw_free (s);                                                           \
    }                                                                          \
  s = slaw_##type##_array_filled (2, var_##type);                              \
  check_one (s, #type "[2]", sizeof (var_##type));                             \
  slaw_free (s);                                                               \
  s = slaw_##type##_array_filled (0, var_##type);                              \
  check_one (s, #type "[0]", sizeof (var_##type));                             \
  slaw_free (s);

int main (int argc, char **argv)
{
  slaw s;
  bool isFloat;

  ob_rand_seed_int32 (24601);

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

  return EXIT_SUCCESS;
}


/* (c)  oblong industries */

// A test of slaw booleans, including coercion and spew

#include "libPlasma/c/slaw.h"
#include "libPlasma/c/slaw-string.h"
#include "libPlasma/c/slaw-coerce.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main (int argc, char **argv)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  slaw t = slaw_boolean (true);
  slaw f = slaw_boolean (false);
  slaw n = slaw_nil ();

  int64 i64;
  unt64 u64;
  float64 f64;
  bool i;
  slaw s;

  const char *tmpFileName = "scratch/test-boolean-temp.txt";
  FILE *tmp;
  char buf[200];

  if (!slaw_is_boolean (t))
    OB_FATAL_ERROR_CODE (0x2030f000, "true is not a boolean\n");

  if (!slaw_is_boolean (f))
    OB_FATAL_ERROR_CODE (0x2030f001, "false is not a boolean\n");

  if (slaw_is_boolean (n))
    OB_FATAL_ERROR_CODE (0x2030f002, "nil is a boolean\n");

  if (true != *slaw_boolean_emit (t))
    OB_FATAL_ERROR_CODE (0x2030f003, "unexpected value for true\n");

  if (false != *slaw_boolean_emit (f))
    OB_FATAL_ERROR_CODE (0x2030f004, "unexpected value for false\n");

  if (NULL != slaw_boolean_emit (n))
    OB_FATAL_ERROR_CODE (0x2030f005, "unexpected value for nil\n");

  tmp = fopen (tmpFileName, "w+");
  if (tmp == 0)
    OB_FATAL_ERROR_CODE (0x2030f006, "fopen of '%s' died of '%s'\n",
                         tmpFileName, strerror (errno));
  slaw_spew_overview (t, tmp, NULL);

  rewind (tmp);
  if (!fgets (buf, sizeof (buf), tmp))
    OB_FATAL_ERROR_CODE (0x2030f007, "fgets died of '%s'\n", strerror (errno));
  if (strstr (buf, "true") == NULL)
    OB_FATAL_ERROR_CODE (0x2030f008, "overview for true: '%s'\n", buf);

  rewind (tmp);
  slaw_spew_overview (f, tmp, NULL);

  rewind (tmp);
  if (!fgets (buf, sizeof (buf), tmp))
    OB_FATAL_ERROR_CODE (0x2030f009, "fgets died of '%s'\n", strerror (errno));
  if (strstr (buf, "false") == NULL)
    OB_FATAL_ERROR_CODE (0x2030f00a, "overview for false: '%s'\n", buf);

  fclose (tmp);

  if (slaw_to_unt64 (t, &u64) != OB_OK || u64 != 1)
    OB_FATAL_ERROR_CODE (0x2030f00b, "problem with slaw_to_unt64\n");

  if (slaw_to_unt64 (f, &u64) != OB_OK || u64 != 0)
    OB_FATAL_ERROR_CODE (0x2030f00c, "problem with slaw_to_unt64\n");

  if (slaw_to_int64 (t, &i64) != OB_OK || i64 != 1)
    OB_FATAL_ERROR_CODE (0x2030f00d, "problem with slaw_to_int64\n");

  if (slaw_to_int64 (f, &i64) != OB_OK || i64 != 0)
    OB_FATAL_ERROR_CODE (0x2030f00e, "problem with slaw_to_int64\n");

  if (slaw_to_float64 (t, &f64) != OB_OK || f64 != 1)
    OB_FATAL_ERROR_CODE (0x2030f00f, "problem with slaw_to_float64\n");

  if (slaw_to_float64 (f, &f64) != OB_OK || f64 != 0)
    OB_FATAL_ERROR_CODE (0x2030f010, "problem with slaw_to_float64\n");

  if (slaw_to_boolean (t, &i) != OB_OK || i != true)
    OB_FATAL_ERROR_CODE (0x2030f011, "problem with slaw_to_boolean\n");

  if (slaw_to_boolean (f, &i) != OB_OK || i != false)
    OB_FATAL_ERROR_CODE (0x2030f012, "problem with slaw_to_boolean\n");

  s = slaw_string ("true");
  if (slaw_to_boolean (s, &i) != OB_OK || i != true)
    OB_FATAL_ERROR_CODE (0x2030f013, "string failed");
  slaw_free (s);

  s = slaw_string ("TRUE");
  if (slaw_to_boolean (s, &i) != OB_OK || i != true)
    OB_FATAL_ERROR_CODE (0x2030f014, "string failed");
  slaw_free (s);

  s = slaw_string ("false");
  if (slaw_to_boolean (s, &i) != OB_OK || i != false)
    OB_FATAL_ERROR_CODE (0x2030f015, "string failed");
  slaw_free (s);

  s = slaw_string ("FALSE");
  if (slaw_to_boolean (s, &i) != OB_OK || i != false)
    OB_FATAL_ERROR_CODE (0x2030f016, "string failed");
  slaw_free (s);

  s = slaw_int8 (-1);
  if (slaw_to_boolean (s, &i) != SLAW_RANGE_ERR)
    OB_FATAL_ERROR_CODE (0x2030f017, "should have been an error");
  slaw_free (s);

  s = slaw_int8 (0);
  if (slaw_to_boolean (s, &i) != OB_OK || i != false)
    OB_FATAL_ERROR_CODE (0x2030f018, "should have been false");
  slaw_free (s);

  s = slaw_int8 (1);
  if (slaw_to_boolean (s, &i) != OB_OK || i != true)
    OB_FATAL_ERROR_CODE (0x2030f019, "should have been true");
  slaw_free (s);

  s = slaw_int8 (2);
  if (slaw_to_boolean (s, &i) != SLAW_RANGE_ERR)
    OB_FATAL_ERROR_CODE (0x2030f01a, "should have been an error");
  slaw_free (s);

  slaw_free (t);
  slaw_free (f);
  slaw_free (n);

  return EXIT_SUCCESS;
}


/* (c)  oblong industries */

// Test that "spew" works properly for multivectors.

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"
#include "libPlasma/c/slaw.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main (int argc, char **argv)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  const char *tmpFileName = "scratch/spew-multi-temp.txt";
  const char *expected = "]: M5UNT16 = (0, 1, 2, 3, 4, 5, 12, 23, 34, 45, 51, "
                         "13, 24, 35, 41, 52, 123, 234, 345, 451, 512, 124, "
                         "235, 341, 452, 513, 1234, 2345, 3451, 4512, 5123, "
                         "12345)";
  const char *p;
  FILE *tmp;
  char buf[200];
  m5unt16 m;
  slaw s;

  memset (&m, 0xff, sizeof (m));

  m.e = 0;
  m.e1 = 1;
  m.e2 = 2;
  m.e3 = 3;
  m.e4 = 4;
  m.e5 = 5;
  m.e12 = 12;
  m.e23 = 23;
  m.e34 = 34;
  m.e45 = 45;
  m.e51 = 51;
  m.e13 = 13;
  m.e24 = 24;
  m.e35 = 35;
  m.e41 = 41;
  m.e52 = 52;
  m.e123 = 123;
  m.e234 = 234;
  m.e345 = 345;
  m.e451 = 451;
  m.e512 = 512;
  m.e124 = 124;
  m.e235 = 235;
  m.e341 = 341;
  m.e452 = 452;
  m.e513 = 513;
  m.e1234 = 1234;
  m.e2345 = 2345;
  m.e3451 = 3451;
  m.e4512 = 4512;
  m.e5123 = 5123;
  m.e12345 = 12345;

  s = slaw_m5unt16 (m);

  tmp = fopen (tmpFileName, "w+");
  if (!tmp)
    OB_FATAL_ERROR_CODE (0x2030d000, "fopen of '%s' died of '%s'\n",
                         tmpFileName, strerror (errno));
  slaw_spew_overview (s, tmp, NULL);

  rewind (tmp);
  if (!fgets (buf, sizeof (buf), tmp))
    OB_FATAL_ERROR_CODE (0x2030d001, "fgets died of '%s'\n", strerror (errno));

  fclose (tmp);
  slaw_free (s);

  p = strchr (buf, ']');
  if (!p)
    OB_FATAL_ERROR_CODE (0x2030d002, "no ']' in '%s'\n", buf);

  if (strcmp (p, expected) != 0)
    OB_FATAL_ERROR_CODE (0x2030d003, "expected '%s' but got '%s'\n", expected,
                         p);

  return EXIT_SUCCESS;
}

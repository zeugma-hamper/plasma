
/* (c)  oblong industries */

#include "libLoam/c/ob-attrs.h"
#include "libLoam/c/ob-sys.h"
#include <stdio.h>
#include <stdlib.h>

/* Verify that OB_PRE_POST can set x to MY_EXPECTED_VALUE before main starts. */

static int x = 0;

const int MY_EXPECTED_VALUE = 5;

void check_value (const char *where)
{
  if (x != MY_EXPECTED_VALUE)
    {
      printf ("FAIL %s: x is not %d, aborting abruptly\n", where,
              MY_EXPECTED_VALUE);
      fflush (stdout);
      /* avoid recursion by skipping further atexit() processing? */
      _exit (EXIT_FAILURE);
    }
  printf ("PASS %s: x is %d\n", where, MY_EXPECTED_VALUE);
}

OB_PRE_POST (x = MY_EXPECTED_VALUE, check_value ("in post"));

int main (int argc, char **argv)
{
  check_value ("in main");
  printf ("returning from main with EXIT_SUCCESS\n");
  return EXIT_SUCCESS;
}

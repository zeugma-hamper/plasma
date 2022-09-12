
/* (c)  oblong industries */

#include "libLoam/c/ob-attrs.h"
#include <stdio.h>
#include <stdlib.h>

static int x = 0;

OB_PRE_POST (x = 5, printf ("Goodbye!\n"));

int main (int argc, char **argv)
{
  printf ("x = %d\n", x);
  return EXIT_SUCCESS;
}

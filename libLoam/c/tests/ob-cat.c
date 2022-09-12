
/* (c)  oblong industries */

#include "libLoam/c/ob-file.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-util.h"

#include <stdlib.h>
#include <stdio.h>

// This is a test program (although not an automated test)
// meant to be run manually to test ob_read_file().

int main (int argc, char **argv)
{
  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s filename [...]\n", ob_get_prog_name ());
      return EXIT_FAILURE;
    }

  int i;
  for (i = 1; i < argc; i++)
    {
      char *s = ob_read_file (argv[i]);
      if (!s)
        {
          fprintf (stderr, "failed to read %s\n", argv[i]);
          return EXIT_FAILURE;
        }
      printf ("%s", s);
      free (s);
    }
  return EXIT_SUCCESS;
}

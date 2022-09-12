
/* (c)  oblong industries */

#include "pool_cmd.h"
#include "libLoam/c/ob-vers.h"

static void usage (void)
{
  ob_banner (stderr);
  fprintf (stderr, "Usage: p-rename <old name> <new name>\n");
  exit (EXIT_FAILURE);
}

int main (int argc, char **argv)
{
  OB_CHECK_ABI ();

  if (argc != 3)
    usage ();

  ob_retort tort = pool_rename (argv[1], argv[2]);
  if (tort < OB_OK)
    {
      fprintf (stderr, "Can't rename '%s' to '%s' because '%s'\n", argv[1],
               argv[2], ob_error_string (tort));
      return pool_cmd_retort_to_exit_code (tort);
    }

  return EXIT_SUCCESS;
}

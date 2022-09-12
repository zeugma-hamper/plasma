
/* (c)  oblong industries */

#include "libPlasma/c/protein.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/pool_cmd.h"
#include "libLoam/c/ob-sys.h"
#include "libLoam/c/ob-vers.h"

static void usage (void)
{
  ob_banner (stderr);
  fprintf (stderr, "Usage: p-resize <pool name> <new size>\n");
  exit (EXIT_FAILURE);
}

static ob_retort set_size (pool_hose ph, unt64 sz)
{
  slaw opts = slaw_map_inline_cf ("size", slaw_unt64 (sz), NULL);
  ob_retort tort = pool_change_options (ph, opts);
  slaw_free (opts);
  return tort;
}

int main (int argc, char **argv)
{
  OB_CHECK_ABI ();

  if (argc != 3)
    usage ();

  const char *pname = argv[1];
  unt64 newsize = pool_cmd_parse_size (argv[2]);

  pool_hose ph = NULL;
  ob_retort tort = pool_participate (pname, &ph, NULL);
  if (tort < OB_OK)
    {
      fprintf (stderr, "Can't open '%s' because '%s'\n", pname,
               ob_error_string (tort));
      return pool_cmd_retort_to_exit_code (tort);
    }

  tort = set_size (ph, newsize);
  ob_err_accum (&tort, pool_withdraw (ph));

  if (tort < OB_OK)
    fprintf (stderr, "Can't resize '%s' because '%s'\n", pname,
             ob_error_string (tort));

  return pool_cmd_retort_to_exit_code (tort);
}


/* (c)  oblong industries */

///
/// Destroy a pool.
///
#include "pool_cmd.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-string.h"
#include "libLoam/c/ob-sys.h"
#include "libLoam/c/ob-vers.h"
#include "libPlasma/c/slaw-string.h"

static void usage (void)
{
  ob_banner (stderr);
  fprintf (stderr, "Usage: p-stop [-q] [-w] <pool name> [...]\n");
  fprintf (stderr, "    -q don't complain if pool doesn't exist\n");
  fprintf (stderr, "    -w allow pool name to be a wildcard (glob pattern)\n");
  exit (EXIT_FAILURE);
}

static void dispose_pool_literally (const char *name, bool quiet, int *rval)
{
  ob_retort pret = pool_dispose (name);
  if (pret == POOL_NO_SUCH_POOL && quiet)
    {
      // silently ignore error
    }
  else if (pret < OB_OK)
    {
      fprintf (stderr, "no can stop %s: %s\n", name, ob_error_string (pret));
      *rval = pool_cmd_retort_to_exit_code (pret);
    }
}

static void dispose_pool_wildcard (const char *pat, bool quiet, int *rval)
{
  if (strchr (pat, '*') || strchr (pat, '?') || strchr (pat, '[')
      || strchr (pat, '\\'))
    {
      char *pdup = strdup (pat);
      const char *server;
      const char *pool_pat;
      char *remote = strstr (pdup, "://");
      if (remote)
        {
          remote += 3;
          remote = strchr (remote, '/');
          if (!remote)
            {
              fprintf (stderr, "seems like a malformed remote name: %s\n", pat);
              free (pdup);
              *rval = EXIT_FAILURE;
              return;
            }
          remote++;
          pool_pat = pat + (remote - pdup);
          *remote = 0;
          server = pdup;
        }
      else
        {
          server = NULL;
          pool_pat = pat;
        }
      slaw pools = NULL;
      ob_retort tort = pool_list_remote (server, &pools);
      if (tort < OB_OK)
        {
          fprintf (stderr, "%s when listing %s pools\n", ob_error_string (tort),
                   (server ? server : "local"));
          *rval = pool_cmd_retort_to_exit_code (tort);
        }
      else
        {
          bslaw pool;
          bool matched = false;
          for (pool = slaw_list_emit_first (pools); pool != NULL;
               pool = slaw_list_emit_next (pools, pool))
            {
              const char *name = slaw_string_emit (pool);
              if (ob_match_glob (name, pool_pat))
                {
                  slaw s =
                    slaw_string_format ("%s%s", (server ? server : ""), name);
                  dispose_pool_literally (slaw_string_emit (s), quiet, rval);
                  slaw_free (s);
                  matched = true;
                }
            }
          if (!matched && !quiet)
            fprintf (stderr, "No match for pattern: %s\n", pat);
        }
      slaw_free (pools);
      free (pdup);
    }
  else
    dispose_pool_literally (pat, quiet, rval);
}

int main (int argc, char **argv)
{
  OB_CHECK_ABI ();

  int i;
  bool quiet = false;
  bool wild = false;
  int c;

  while ((c = getopt (argc, argv, "qw")) != -1)
    {
      switch (c)
        {
          case 'q':
            quiet = true;
            break;
          case 'w':
            wild = true;
            break;
          default:
            usage ();
        }
    }

  argc -= optind;
  argv += optind;

  if (argc < 1)
    usage ();

  int rval = EXIT_SUCCESS;

  for (i = 0; i < argc; i++)
    (wild ? dispose_pool_wildcard : dispose_pool_literally) (argv[i], quiet,
                                                             &rval);

  return rval;
}

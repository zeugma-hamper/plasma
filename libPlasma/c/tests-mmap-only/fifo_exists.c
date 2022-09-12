
/* (c)  oblong industries */

///
/// Test that we correctly handle the case of a fifo name collision.
/// Specified pool must already exist.
///
#include "libLoam/c/ob-log.h"
#include "libPlasma/c/pool_cmd.h"
// Below necessary because we muck around inside the pool hose
#include "libPlasma/c/private/pool_impl.h"

static void usage (void)
{
  fprintf (stderr, "Usage: fifo_exists <poolname>\n");
  exit (1);
}

int main (int argc, char *argv[])
{
  pool_cmd_info cmd = {0};
  pool_cmd_info cmd2 = {0};
  pool_hose ph, ph2;
  ob_retort pret;

  if (pool_cmd_get_poolname (&cmd, argc, argv, optind))
    usage ();

  cmd2 = cmd;
  pool_cmd_open_pool (&cmd);
  ph = cmd.ph;
  pool_cmd_open_pool (&cmd2);
  ph2 = cmd2.ph;

  // Artificially create a fifo name collision
  strcpy (ph->fifo_path, ph2->fifo_path);

  // Make a gang so we can await simultaneously
  pool_gang twofer;
  pret = pool_new_gang (&twofer);
  if (pret != OB_OK)
    OB_FATAL_ERROR_CODE (0x20406000,
                         "New gang creation failed: %" OB_FMT_RETORT "d"
                         "\n",
                         pret);

  pret = pool_join_gang (twofer, ph);
  if (pret != OB_OK)
    OB_FATAL_ERROR_CODE (0x20406001,
                         "Pool %s failed to join gang: %" OB_FMT_RETORT "d"
                         "\n",
                         cmd.pool_name, pret);

  pret = pool_join_gang (twofer, ph2);
  if (pret != OB_OK)
    OB_FATAL_ERROR_CODE (0x20406002,
                         "Pool %s failed to join gang: %" OB_FMT_RETORT "d"
                         "\n",
                         cmd2.pool_name, pret);

  // Start an await (with timeout so we don't hang forever when it
  // succeeds).  The await will start successfully if the fifo name
  // collision handling is correct, and then it will timeout.
  protein prot;
  pret = pool_await_next_multi (twofer, 1, NULL, &prot, NULL, NULL);
  if (pret != POOL_AWAIT_TIMEDOUT)
    OB_FATAL_ERROR_CODE (0x20406003, "Await returned: %s\n",
                         ob_error_string (pret));

  OB_DIE_ON_ERROR (pool_disband_gang (twofer, true));

  // pool_withdraw (ph);
  // pool_withdraw (ph2);

  pool_cmd_free_options (&cmd);

  return 0;
}

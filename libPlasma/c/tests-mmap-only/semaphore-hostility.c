
/* (c)  oblong industries */

#include "pool_cmd.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-util.h"
#include "libLoam/c/ob-vers.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/protein.h"

// Below necessary because we muck around inside the pool hose
#include "libPlasma/c/private/pool_impl.h"

#include <stdlib.h>

static void usage (void)
{
  ob_banner (stderr);
  fprintf (stderr,
           "Usage: %s [-t <type>] [-s <size>] [-i <toc cap>] <pool_name>\n"
           "\t<type> defaults to \"mmap\"\n"
           "\t<size> defaults to %" OB_FMT_64 "u bytes\n",
           ob_get_prog_name (), (unt64) POOL_MMAP_DEFAULT_SIZE);
  exit (EXIT_FAILURE);
}

static void send_and_receive (pool_hose hsend, pool_hose hrecv)
{
  char uuid[37];
  int64 isend, irecv;
  protein q = NULL;

  OB_DIE_ON_ERROR (ob_generate_uuid (uuid));
  protein p =
    protein_from_ff (slaw_list_inline_c ("CoWS coWS CowS cOWS "
                                         "DiRTY DIrtY DirTy DIRtY "
                                         "COWS COWS cOwS cOWS bvpDq",
                                         NULL),
                     slaw_map_inline_cc ("you you eye dee", uuid, NULL));
  OB_DIE_ON_ERROR (pool_deposit (hsend, p, &isend));
  OB_DIE_ON_ERROR (pool_next (hrecv, &q, NULL, &irecv));
  if (isend != irecv)
    OB_FATAL_ERROR ("%" OB_FMT_64 "d != %" OB_FMT_64 "d\n", isend, irecv);
  if (!proteins_equal (p, q))
    OB_FATAL_ERROR ("p != q\n");
  Free_Protein (p);
  Free_Protein (q);

  ob_retort tort = pool_await_next (hrecv, 0.1, &q, NULL, NULL);
  if (tort != POOL_AWAIT_TIMEDOUT)
    OB_FATAL_ERROR ("Got %s but expected POOL_AWAIT_TIMEDOUT\n",
                    ob_error_string (tort));
}

int main (int argc, char **argv)
{
  OB_CHECK_ABI ();

  pool_cmd_info cmd = {0};
  int c, i;

  while ((c = getopt (argc, argv, "i:s:t:v")) != -1)
    {
      switch (c)
        {
          case 'i':
            cmd.toc_capacity = strtoll (optarg, NULL, 0);
            break;
          case 's':
            cmd.size = strtoll (optarg, NULL, 0);
            break;
          case 't':
            cmd.type = optarg;
            break;
          case 'v':
            cmd.verbose = 1;
            break;
          default:
            usage ();
        }
    }

  pool_cmd_setup_options (&cmd);
  if (pool_cmd_get_poolname (&cmd, argc, argv, optind))
    usage ();

  OB_DIE_ON_ERROR (pool_create (cmd.pool_name, cmd.type, cmd.create_options));

  // open both hoses to the same pool
  pool_hose hsend = NULL;
  pool_hose hrecv = NULL;
  OB_DIE_ON_ERROR (pool_participate (cmd.pool_name, &hsend, NULL));
  OB_DIE_ON_ERROR (pool_participate (cmd.pool_name, &hrecv, NULL));

  // check that semaphore keys and ids are the same
  if (hsend->sem_key != hrecv->sem_key)
    OB_FATAL_ERROR ("semaphore keys different\n");

  if (hsend->sem_id != hrecv->sem_id)
    OB_FATAL_ERROR ("semaphore ids different\n");

  int old_semid = hsend->sem_id;

  // send a few proteins
  for (i = 0; i < 5; i++)
    send_and_receive (hsend, hrecv);

  // destroy the semaphore
  while (semctl (hsend->sem_id, 0, IPC_RMID, 0) < 0)
    {
      if (errno == EINTR)
        continue;
      OB_FATAL_ERROR ("semaphore destroy failed: %s\n", strerror (errno));
    }

  // send a few proteins
  for (i = 0; i < 5; i++)
    send_and_receive (hsend, hrecv);

  // check that semaphore keys and ids are the same
  if (hsend->sem_key != hrecv->sem_key)
    OB_FATAL_ERROR ("semaphore keys different\n");

  if (hsend->sem_id != hrecv->sem_id)
    OB_FATAL_ERROR ("semaphore ids different\n");

  // and the id should be different than it was before destruction
  if (hsend->sem_id == old_semid)
    OB_FATAL_ERROR ("didn't expect semid to be same as before\n");

  // close hoses
  OB_DIE_ON_ERROR (pool_withdraw (hsend));
  OB_DIE_ON_ERROR (pool_withdraw (hrecv));

  // clean up
  OB_DIE_ON_ERROR (pool_dispose (cmd.pool_name));
  pool_cmd_free_options (&cmd);
  return EXIT_SUCCESS;
}

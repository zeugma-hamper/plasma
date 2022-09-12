
/* (c)  oblong industries */

///
/// Extremely simple performance test.  Given a number of threads, N,
/// tests all combinations of N total reader and writer threads.
///
#include <errno.h>
#include "libLoam/c/ob-sys.h"
#include "libLoam/c/ob-pthread.h"

#include "pool_cmd.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"
#include "libLoam/c/ob-time.h"
#include "libPlasma/c/protein.h"
#include "libPlasma/c/slaw.h"

// Total number of both readers and depositors
int nthreads = 4;
// Seconds to run test per combination of readers and depositors
int test_secs = 1;
// For testing - how many "laggard" threads to start to read the
// oldest protein in the pool.  Good for catching wraparound/overwrite
// bugs.
int laggards;

// Make this global so all threads can read it and do participate
pool_cmd_info cmd;

// Having a single "running" variable leads to an interesting race
// condition where threads that don't start by the time the test is
// over get stuck forever.  This happens, e.g., during debugging. :)
static volatile int start = 0;
static volatile float64 stoptime;

// Thread state struct - various per-thread variables needed

struct thread_info
{
  // Pthread struct
  pthread_t pthread;
  // Which thread number we are
  int thread_num;
  // Number of reads/deposits completed
  unt64 count;
  // Is this thread all set up and ready to run yet?
  volatile int ready;
  // Pthread return values are super painful, put ours here
  ob_retort pret;
};

static void wait_for_start (struct thread_info *ti)
{
  if (cmd.verbose)
    printf ("thread %d waiting for start...\n", ti->thread_num);

  // Tell main thread we're ready
  ti->ready = 1;
  // Might get EINTR
  while (start != 1)
    usleep (10000);

  if (cmd.verbose)
    printf ("thread %d starting\n", ti->thread_num);
}

static void *deposit_lots_main (void *arg)
{
  struct thread_info *ti = (struct thread_info *) arg;
  pool_hose ph;
  // ob_retort pret;
  protein prot;
  char name[100];
  int i;

  OB_DIE_ON_ERROR (pool_participate (cmd.pool_name, &ph, NULL));

  snprintf (name, sizeof (name), "thread%d", ti->thread_num);
  OB_DIE_ON_ERROR (pool_set_hose_name (ph, name));

  prot = pool_cmd_create_test_protein (name);

  wait_for_start (ti);

  // Deposit proteins as fast as we can until the test is over
  for (i = 0; i < 100 || ob_current_time () < stoptime; i++)
    {
      ti->pret = pool_deposit (ph, prot, NULL);
      if (ti->pret != OB_OK)
        break;
    }

  ti->count = i;
  protein_free (prot);
  OB_DIE_ON_ERROR (pool_withdraw (ph));
  if (cmd.verbose || ti->pret != OB_OK)
    printf ("deposit thread %d returning %" OB_FMT_64 "u, error %s\n",
            ti->thread_num, ti->count, ob_error_string (ti->pret));
  return NULL;
}

static void *read_lots_main (void *arg)
{
  struct thread_info *ti = (struct thread_info *) arg;
  // ob_retort pret;
  protein prot;
  pool_timestamp ts;
  pool_hose ph;
  int i;

  OB_DIE_ON_ERROR (pool_participate (cmd.pool_name, &ph, NULL));

  char name[100];
  snprintf (name, sizeof (name), "reader thread %d", ti->thread_num);
  OB_DIE_ON_ERROR (pool_set_hose_name (ph, name));

  wait_for_start (ti);

  // Read proteins as fast as we can until the test is over
  for (i = 0; i < 100 || ob_current_time () < stoptime; i++)
    {
      prot = NULL;
      ti->pret = pool_next (ph, &prot, &ts, NULL);
      // Did we run out?
      if (ti->pret == POOL_NO_SUCH_PROTEIN)
        {
          // Start over at the beginning
          ti->pret = pool_rewind (ph);
          // We didn't actually read a protein, so subtract from the count
          i--;
          if (ti->pret != OB_OK)
            {
              OB_LOG_ERROR_CODE (0x2040a000, "pool_rewind returned %s\n",
                                 ob_error_string (ti->pret));
              break;
            }
        }
      else if (ti->pret != OB_OK)
        {
          OB_LOG_ERROR_CODE (0x2040a001, "pool_next returns %s\n",
                             ob_error_string (ti->pret));
          break;
        }
      else if (prot == NULL)
        {
          error_report ("pret was %s but prot was NULL!\n",
                        ob_error_string (ti->pret));
          ti->pret = OB_UNKNOWN_ERR;
          break;
        }
      else
        {
          // Success!
          protein_free (prot);
        }
    }

  ti->count = i;
  OB_DIE_ON_ERROR (pool_withdraw (ph));
  if (cmd.verbose || ti->pret != OB_OK)
    printf ("read thread %d returning %" OB_FMT_64 "u, error %s\n",
            ti->thread_num, ti->count, ob_error_string (ti->pret));
  return NULL;
}

static void *laggard_main (void *arg)
{
  struct thread_info *ti = (struct thread_info *) arg;
  // ob_retort pret;
  protein prot;
  pool_timestamp ts;
  pool_hose ph;
  int64 idx;
  int i;

  OB_DIE_ON_ERROR (pool_participate (cmd.pool_name, &ph, NULL));

  char name[100];
  snprintf (name, sizeof (name), "laggard thread %d", ti->thread_num);
  OB_DIE_ON_ERROR (pool_set_hose_name (ph, name));

  // Read the oldest protein over and over again
  for (i = 0; i < 100 || ob_current_time () < stoptime; i++)
    {
      OB_DIE_ON_ERROR (pool_oldest_index (ph, &idx));
      ti->pret = pool_nth_protein (ph, idx, &prot, &ts);
      if (ti->pret != OB_OK)
        {
          if (ti->pret == POOL_NO_SUCH_PROTEIN)
            {
              // We expect the oldest protein to get overwritten a lot
              ti->pret = 0;
              continue;
            }
          else
            {
              OB_LOG_ERROR_CODE (0x2040a002, "pool_nth_protein returns %s\n",
                                 ob_error_string (ti->pret));
              break;
            }
        }
      else
        {
          // Free the protein if we actually got one
          protein_free (prot);
        }
    }

  ti->count = i;
  OB_DIE_ON_ERROR (pool_withdraw (ph));
  if (cmd.verbose || ti->pret != OB_OK)
    printf ("laggard thread %d returning %" OB_FMT_64 "u, error %s\n",
            ti->thread_num, ti->count, ob_error_string (ti->pret));
  return NULL;
}

// Create n threads to do something
static struct thread_info *create_threads (int my_nthreads,
                                           void *(*thread_main) (void *) )
{
  int i;
  struct thread_info *threads =
    (struct thread_info *) malloc (my_nthreads * sizeof (*threads));
  struct thread_info *ti;
  if (!threads)
    OB_FATAL_ERROR_CODE (0x2040a003,
                         "Can't allocate thread info array for %d threads\n",
                         my_nthreads);

  memset (threads, 0, my_nthreads * sizeof (*threads));
  for (i = 0; i < my_nthreads; i++)
    {
      ti = &threads[i];
      ti->thread_num = i;

      if (pthread_create (&ti->pthread, NULL, thread_main, (void *) ti) != 0)
        OB_FATAL_ERROR_CODE (0x2040a004, "Can't create thread %d\n", i);
    }
  return threads;
}

// Wait for n threads to be ready
static void wait_for_children (int my_nthreads, struct thread_info threads[])
{
  int i;
  struct thread_info *ti;

  for (i = 0; i < my_nthreads; i++)
    {
      ti = &threads[i];
      while (ti->ready == 0)
        usleep (10000);
      if (cmd.verbose)
        printf ("thread %d ready\n", ti->thread_num);
    }
  return;
}

// Reap threads and check for errors, protein counting version

static int reap_threads_count (int my_nthreads, struct thread_info threads[],
                               int *total_count)
{
  struct thread_info *ti;
  int err = 0;
  int i;
  for (i = 0; i < my_nthreads; i++)
    {
      ti = &threads[i];

      if (pthread_join (ti->pthread, NULL) != 0)
        OB_FATAL_ERROR_CODE (0x2040a005, "Problem joining thread %d\n", i);

      if (ti->pret != OB_OK)
        err = 1;
      else
        *total_count += ti->count;
    }
  free (threads);
  return err;
}

static void usage (void)
{
  fprintf (stderr, "Usage: perf_test [-v] [-t <type>] [-s <size>] \n"
                   "\t[-n <threads>] [-l <laggards>] [-S <seconds per run>] \n"
                   "\t[-i <toc cap>] <pool_name>\n");
  exit (1);
}

int main (int argc, char *argv[])
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  ob_retort pret;
  struct thread_info *deposit_threads;
  struct thread_info *read_threads;
  struct thread_info *laggard_threads = NULL; /* initialize to avoid warning */
  int deposit_count;
  int read_count;
  int laggard_count;
  int any_err = 0;
  int err = 0;
  int c;
  int i;

  while ((c = getopt (argc, argv, "i:l:n:s:S:t:v")) != -1)
    {
      switch (c)
        {
          case 'i':
            cmd.toc_capacity = strtoll (optarg, NULL, 0);
            break;
          case 'l':
            laggards = strtoll (optarg, NULL, 0);
            break;
          case 'n':
            nthreads = strtoll (optarg, NULL, 0);
            break;
          case 's':
            cmd.size = strtoll (optarg, NULL, 0);
            break;
          case 'S':
            test_secs = strtoll (optarg, NULL, 0);
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

  // Test all combinations of 4 threads doing deposits and reads
  for (i = 0; i <= nthreads; i++)
    {
      // Reset all loop variables
      read_count = 0;
      deposit_count = 0;
      laggard_count = 0;
      start = 0;
      err = 0;

      printf ("%d readers, %d depositors\n", nthreads - i, i);
      // Make and destroy a new pool every time for better reproducibility
      pret = pool_create (cmd.pool_name, cmd.type, cmd.create_options);
      if (pret != OB_OK)
        {
          fprintf (stderr, "no can create %s, size %" OB_FMT_64 "u: %s\n",
                   cmd.pool_name, cmd.size, ob_error_string (pret));
          exit (1);
        }

      // Fill up pool - the number of proteins in the pool heavily affect
      // the performance results.
      pool_cmd_fill_pool (&cmd);

      read_threads = create_threads (nthreads - i, read_lots_main);
      deposit_threads = create_threads (i, deposit_lots_main);
      if (laggards)
        laggard_threads = create_threads (laggards, laggard_main);

      // Wait for all the threads to finish initializing
      wait_for_children (nthreads - i, read_threads);
      wait_for_children (i, deposit_threads);
      // Don't wait for laggards, they will do their job anyway

      stoptime = ob_current_time () + test_secs;

      // Give threads the go signal
      start = 1;

      // Gather up the stats
      err |= reap_threads_count (nthreads - i, read_threads, &read_count);
      err |= reap_threads_count (i, deposit_threads, &deposit_count);
      if (laggards)
        err |= reap_threads_count (laggards, laggard_threads, &laggard_count);
      if (err)
        printf ("ERROR!!!  Do not trust the stats!\n");

      printf ("%d reads/sec (%d reads %d seconds)\n", read_count / test_secs,
              read_count, test_secs);
      printf ("%d deposits/sec (%d deposits %d seconds)\n",
              deposit_count / test_secs, deposit_count, test_secs);
      if (laggards)
        printf ("%d laggards/sec\n", laggard_count / test_secs);

      pret = pool_dispose (cmd.pool_name);
      if (pret != OB_OK)
        OB_FATAL_ERROR_CODE (0x2040a006, "no can stop %s: %s\n", cmd.pool_name,
                             ob_error_string (pret));
      any_err |= err;
    }

  pool_cmd_free_options (&cmd);

  return any_err;
}

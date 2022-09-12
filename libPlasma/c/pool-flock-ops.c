
/* (c)  oblong industries */

///
/// Semaphore-based synchronization
///
#include <sys/file.h>
#include <errno.h>
#include <stdlib.h>

#include "libLoam/c/ob-dirs.h"
#include "libLoam/c/ob-file.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-rand.h"
#include "libLoam/c/ob-string.h"

#include "libPlasma/c/pool.h"
#include "libPlasma/c/private/pool_impl.h"
#include "libPlasma/c/private/pool-ugly-callback.h"

/// We use flock()ed files to implement per-pool locks.  Each pool has
/// its own .deposit and .notify lock files, created at pool
/// creation time.  Lock files are for local pools only; remote pools
/// handle all locking on the host machine.
///
/// Lock file descriptions:
///
/// Deposit lock: While this lock is held, no one else can deposit in
/// the pool.  This is useful for (a) making a deposit, (b) eliminating
/// race windows while checking for deposits.
///
/// Notification lock: This lock is held while pools are being notified
/// via the socket-based await mechanism, and while we are adding
/// ourselves to the notification list for this mechanism.  It could be
/// combined with the deposit lock, but we want to decouple deposits
/// from reads as much as possible.
///

static ob_retort single_file_lock_dir (const char *single_file, char **path_ret)
{
  char *path = NULL;
  ob_retort tort = ob_realpath (single_file, &path);
  if (tort < OB_OK)
    return tort;
  tort = ob_insert_penultimate_directory (&path, ".plasma-locks");
  if (tort < OB_OK)
    {
      free (path);
      return tort;
    }
  *path_ret = path;
  return tort;
}

static ob_retort lock_file_dir_path (char *dir_path, pool_hose ph)
{
  ob_retort pret = pool_build_pool_dir_path (dir_path, ph->name);
  if (pret != OB_OK)
    return pret;
  if (ph->pool_directory_version == POOL_DIRECTORY_VERSION_SINGLE_FILE)
    {
      char *notpath = NULL;
      pret = single_file_lock_dir (dir_path, &notpath);
      if (pret < OB_OK)
        return pret;
      ob_safe_copy_string (dir_path, PATH_MAX, notpath);
      free (notpath);
    }
  else
    {
      pret = pool_add_path_element (dir_path, "locks");
      if (pret != OB_OK)
        return pret;
    }
  return OB_OK;
}

static ob_retort ensure_prepared_sem_key (pool_hose ph)
{
  char path[PATH_MAX];

  ob_retort tort = lock_file_dir_path (path, ph);
  if (tort < OB_OK)
    return tort;

  tort = ob_mkdir_p_with_permissions (path, 00775, -1, -1);
  if (tort < OB_OK)
    return tort;

  return OB_OK;
}

static ob_retort lock_file_path (char *path, pool_hose ph, int lt)
{
  ob_retort tort = lock_file_dir_path (path, ph);
  if (tort < OB_OK)
    return tort;

  if (lt == POOL_SEM_DEPOSIT_LOCK_IDX)
    tort = pool_add_path_element (path, "deposit.lock");
  else
    tort = pool_add_path_element (path, "notify.lock");

  return tort;
}

static ob_retort lock_file_open (pool_hose ph, int lt)
{
  char path[PATH_MAX];
  ob_retort tort = lock_file_path (path, ph, lt);
  if (tort < OB_OK)
    return tort;

  /**
   * We explicitly use ob_open_cloexec() rather than the system supplied
   * open(), as we very much want the
   * flock()s we clasp to these descriptors to die most switfly,
   * emphatically, and definitively if we exec() some other process.
   */
  int fd = ob_open_cloexec (path, O_RDWR | O_CREAT | O_TRUNC, 00664);
  if (fd < 0)
    return ob_errno_to_retort (errno);

  ph->flock_fds[lt] = fd;

  return OB_OK;
}

static ob_retort lock_file_unlink (pool_hose ph, int lt)
{
  if (ph->flock_fds[lt] > 0)
    {
      close (ph->flock_fds[lt]);
      ph->flock_fds[lt] = -1;
    }

  char path[PATH_MAX];
  ob_retort tort = lock_file_path (path, ph, lt);
  if (tort < OB_OK)
    return tort;

  if (unlink (path) < 0)
    return ob_errno_to_retort (errno);

  return OB_OK;
}

static ob_retort lock_file_close (pool_hose ph, int lt)
{
  if (ph->flock_fds[lt] >= 0)
    {
      int ret = close (ph->flock_fds[lt]);
      ph->flock_fds[lt] = -1;
      if (0 != ret)
        return ob_errno_to_retort (errno);
    }

  return OB_OK;
}

static ob_retort lock_file_lock_fd (pool_hose ph, int lt)
{
  if (flock (ph->flock_fds[lt], LOCK_EX) < 0)
    return ob_errno_to_retort (errno);

  return OB_OK;
}

static ob_retort lock_file_unlock_fd (pool_hose ph, int lt)
{
  if (flock (ph->flock_fds[lt], LOCK_UN) < 0)
    return ob_errno_to_retort (errno);

  return OB_OK;
}

static ob_retort pool_flock_destroy_semaphores (pool_hose ph,
                                                bool it_should_exist)
{
  // failure here doesn't stop our destruction
  lock_file_unlink (ph, POOL_SEM_DEPOSIT_LOCK_IDX);
  lock_file_unlink (ph, POOL_SEM_NOTIFICATION_LOCK_IDX);

  char path[PATH_MAX];
  ob_retort tort = lock_file_dir_path (path, ph);
  if (tort >= OB_OK)
    ob_rmdir_p (path); /* intentionally ignore any error */

  /* Yep, we always succeed.  Ignore any errors. */
  return OB_OK;
}


static ob_retort pool_flock_create_semaphores (pool_hose ph)
{
  ob_retort tort = ensure_prepared_sem_key (ph);
  if (tort < OB_OK)
    return tort;

  tort = lock_file_open (ph, POOL_SEM_NOTIFICATION_LOCK_IDX);
  if (tort < OB_OK)
    return tort;

  tort = lock_file_open (ph, POOL_SEM_DEPOSIT_LOCK_IDX);
  if (tort < OB_OK)
    return tort;

  return OB_OK;
}

static ob_retort pool_flock_open_semaphores (pool_hose ph)
{
  return pool_flock_create_semaphores (ph);
}

static ob_retort pool_flock_close (pool_hose ph)
{
  ob_retort tort = OB_OK;
  ob_err_accum (&tort, lock_file_close (ph, POOL_SEM_DEPOSIT_LOCK_IDX));
  ob_err_accum (&tort, lock_file_close (ph, POOL_SEM_NOTIFICATION_LOCK_IDX));

  return tort;
}

const pool_lock_ops pool_flock_ops = {pool_flock_create_semaphores,
                                      pool_flock_destroy_semaphores,
                                      pool_flock_open_semaphores,
                                      pool_flock_close,
                                      lock_file_lock_fd,
                                      lock_file_unlock_fd};

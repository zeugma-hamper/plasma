
/* (c)  oblong industries */

/* So, here's the thing:
 * libPlasma/c/fifo_ops.c - only used on non-Windows
 * libPlasma/c/win32/fifo_ops_win32.c - only used on Windows
 * libPlasma/c/pool-fifo.c - used on both!
 * libPlasma/c/private/fifo_ops.h - prototypes for all of the above .c files
 */

#include "libLoam/c/ob-rand.h"
#include "libLoam/c/ob-log.h"
#include "libPlasma/c/private/pool_impl.h"
#include "libPlasma/c/private/pool_multi.h"

#include <stdio.h>

// The length of the fifo name is chosen by making it long enough
// that we are unlikely to see accidental collisions (the penalty
// for which is running this function again to get another random
// fifo name).  For convenience, we use the number of alphanumeric
// characters (uppercase only) that can be extracted from an unt64.
#define NUM_CHARS 12

ob_retort pool_random_name (char *buf_out)
{
  // Get random data and turn it into an alpha-numeric file name
  unt64 r;
  ob_retort pret = ob_truly_random (&r, sizeof (r));
  if (pret < OB_OK)
    {
      OB_LOG_ERROR_CODE (0x2010f000, "ob_truly_random() failed with '%s'\n",
                         ob_error_string (pret));
      return pret;
    }

  char *p = buf_out;
  int i;
  for (i = 0; i < NUM_CHARS; i++)
    {
      int n = r % 36;
      r /= 36;
      n += (n < 10) ? '0' : ('A' - 10);
      *p++ = n;
    }
  *p = 0;

  return OB_OK;
}

#define NOTI_PFX "iton-"

ob_retort pool_create_fifo_path (pool_hose ph)
{
  ob_retort pret;
  // Directory the fifo will be located in
  char dir_path[PATH_MAX];
  // fifo filename itself
  char base_name[NUM_CHARS + 1];

  pret = pool_notify_dir_path (dir_path, ph->name, ph->pool_directory_version);
  if (pret != OB_OK)
    return pret;

  pret = pool_random_name (base_name);
  if (pret < OB_OK)
    return pret;

  OB_LOG_DEBUG_CODE (0x2010f001, "%s: new fifo name " NOTI_PFX "%s\n", ph->name,
                     base_name);
  // Paste it together with the notification dir to get the whole path
  char fifo_path[PATH_MAX];
  int snval =
    snprintf (fifo_path, PATH_MAX, "%s/" NOTI_PFX "%s", dir_path, base_name);
  if (snval >= PATH_MAX || snval < 0)
    {
      OB_LOG_ERROR_CODE (0x2010f002,
                         "exceeded PATH_MAX (%d): %s/" NOTI_PFX "%s\n",
                         PATH_MAX, dir_path, base_name);
      return POOL_FIFO_BADTH;
    }
  // The resulting pathname may or may not be unique, depending on the
  // quality of randomness we are provided.  We can only find out if
  // it isn't unique if we try to create it during await and get
  // EEXIST.

  // Cache the paths in the pool hose.
  strcpy (ph->notify_dir_path, dir_path);
  strcpy (ph->fifo_path, fifo_path);
  return OB_OK;
}

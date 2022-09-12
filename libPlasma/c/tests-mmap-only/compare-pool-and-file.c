
/* (c)  oblong industries */

#include <stdio.h>
#include <stdlib.h>
#include "libLoam/c/ob-util.h"
#include "libLoam/c/ob-log.h"
#include "libPlasma/c/pool.h"
#include "libPlasma/c/slaw-io.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/protein.h"

int main (int argc, char **argv)
{
  if (3 != argc)
    {
      fprintf (stderr, "Usage: %s poolname filename\n", ob_get_prog_name ());
      return EXIT_FAILURE;
    }

  const char *poolname = argv[1];
  const char *filename = argv[2];

  pool_hose ph;
  slaw_input f;

  OB_DIE_ON_ERROR (pool_participate (poolname, &ph, NULL));
  OB_DIE_ON_ERROR (pool_rewind (ph));
  OB_DIE_ON_ERROR (slaw_input_open (filename, &f));

  ob_retort ptort, ftort;
  protein pprot, fprot;
  int64 idx;
  int64 i = 0;

  while ((ptort = pool_next (ph, &pprot, NULL, &idx)),
         (ftort = slaw_input_read (f, &fprot)),
         (OB_OK == ptort && OB_OK == ftort))
    {
      if (!proteins_equal (pprot, fprot))
        error_exit ("proteins not equal\n");

      if (i != idx)
        error_exit ("index mismatch: %" OB_FMT_64 "d != %" OB_FMT_64 "d\n", i,
                    idx);

      Free_Protein (pprot);
      Free_Protein (fprot);
      i++;
    }

  if (POOL_NO_SUCH_PROTEIN != ptort)
    error_exit ("ptort was '%s' at %" OB_FMT_64 "d\n", ob_error_string (ptort),
                i);

  if (SLAW_END_OF_FILE != ftort)
    error_exit ("ftort was '%s' at %" OB_FMT_64 "d\n", ob_error_string (ftort),
                i);

  OB_DIE_ON_ERROR (pool_withdraw (ph));
  OB_DIE_ON_ERROR (slaw_input_close (f));

  return EXIT_SUCCESS;
}

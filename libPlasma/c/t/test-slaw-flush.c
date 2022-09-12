
/* (c)  oblong industries */

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-vers.h"
#include "libLoam/c/ob-sys.h"
#include "libPlasma/c/slaw-io.h"
#include "libPlasma/c/pool_cmd.h"
#include "libPlasma/c/slaw-string.h"
#include "libPlasma/c/pool.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int mainish (int argc, char **argv)
{
  ob_retort err;

  const char *filename = "scratch/test-slaw-flush.tmp";

  int i;
  for (i = 0; i < 2; i++)
    {
      slaw_output output;
      slaw_input input;
      const char *pass;

      pass = (i ? "binary" : "yaml");

      err = ((i ? slaw_output_open_binary : slaw_output_open_text) (filename,
                                                                    &output));
      OB_DIE_ON_ERROR (err);

      slaw s = slaw_string ("hello world");

      OB_DIE_ON_ERROR (slaw_output_write (output, s));

      err = ((i ? slaw_input_open_binary : slaw_input_open_text) (filename,
                                                                  &input));
      OB_DIE_ON_ERROR (err);

      slaw s2;
      err = slaw_input_read (input, &s2);
      if (err != OB_OK)
        OB_LOG_ERROR_CODE (0x20314000, "in '%s' version:\n", pass);
      OB_DIE_ON_ERROR (err);

      if (!slawx_equal_lf (s, s2))
        OB_FATAL_ERROR_CODE (0x20314001, "not equal\n");

      err = slaw_output_close (output);
      OB_DIE_ON_ERROR (err);

      err = slaw_input_close (input);
      OB_DIE_ON_ERROR (err);

      unlink (filename);

      slaw_free (s);
    }

  return EXIT_SUCCESS;
}

int main (int argc, char **argv)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  return check_for_leaked_file_descriptors_scoped (mainish, argc, argv);
}


/* (c)  oblong industries */

#include "libLoam/c/ob-retorts.h"
#include "libPlasma/c/slaw-io.h"
#include "libPlasma/c/protein.h"
#include "libLoam/c/ob-dirs.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"

#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
  OB_CHECK_ABI ();

  ob_retort err;
  protein p;
  slaw_output out;
  slaw_input yaml;
  int64 prono = 0;
  const char *srcfile;

  if (argc != 3)
    {
      ob_banner (stderr);
      fprintf (stderr, "Usage: %s infile.yaml outfile.bin\n",
               ob_basename (argv[0]));
      return EXIT_FAILURE;
    }

  err = slaw_input_open_text ((srcfile = argv[1]), &yaml);
  OB_DIE_ON_ERR_CODE (0x20206001, err);

  err = slaw_output_open_binary (argv[2], &out);
  OB_DIE_ON_ERR_CODE (0x20206002, err);

  while ((err = slaw_input_read (yaml, &p)) == OB_OK)
    {
      err = slaw_output_write (out, p);
      OB_DIE_ON_ERR_CODE (0x20206003, err);
      protein_free (p);
      prono++;
    }

  if (err != SLAW_END_OF_FILE)
    OB_FATAL_ERROR_CODE (0x20206000,
                         "Got error %s on protein #%" OB_FMT_64 "d of %s\n",
                         ob_error_string (err), prono, srcfile);

  err = slaw_output_close (out);
  OB_DIE_ON_ERR_CODE (0x20206004, err);

  err = slaw_input_close (yaml);
  OB_DIE_ON_ERR_CODE (0x20206005, err);

  return EXIT_SUCCESS;
}

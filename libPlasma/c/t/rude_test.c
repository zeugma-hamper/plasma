
/* (c)  oblong industries */

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"
#include "libPlasma/c/protein.h"
#include "libPlasma/c/slaw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  byte data[20];
  int i;

  for (i = 0; i < sizeof (data); i++)
    data[i] = i + 100;

  slaw descrips = slaw_list_f (slabu_new ());
  slaw ingests = slaw_map_inline_cc ("cold greeting", "have an ice day", NULL);

  for (i = sizeof (data) - 1; i >= 0; i--)
    {
      protein p;
      const void *rude;
      bslaw cons, car, cdr;
      const char *carStr, *cdrStr;
      int64 rudeLen;

      p = protein_from_llr (descrips, ingests, data, i);

      rude = protein_rude (p, &rudeLen);

      if (rudeLen != i)
        {
          fprintf (stderr, "rude data len is wrong for i = %d\n", i);
          fprintf (stderr,
                   "    (expected %" OB_FMT_64 "d, got %" OB_FMT_64 "d)\n",
                   (int64) i, (int64) rudeLen);
          return EXIT_FAILURE;
        }

      if (memcmp (data, rude, i) != 0)
        {
          fprintf (stderr, "rude data contents are wrong for i = %d\n", i);
          return EXIT_FAILURE;
        }

      if (slaw_list_count (protein_descrips (p)) != 0)
        {
          fprintf (stderr, "wrong number of descrips for i = %d\n", i);
          return EXIT_FAILURE;
        }

      if (slaw_list_count (protein_ingests (p)) != 1)
        {
          fprintf (stderr,
                   "wrong number of ingests (%" OB_FMT_64 "d) for i = %d\n",
                   (int64) slaw_list_count (protein_ingests (p)), i);
          return EXIT_FAILURE;
        }

      cons = slaw_list_emit_first (protein_ingests (p));
      car = slaw_cons_emit_car (cons);
      cdr = slaw_cons_emit_cdr (cons);

      if (!car || !cdr)
        {
          fprintf (stderr, "ingest is not a cons for i = %d\n", i);
          return EXIT_FAILURE;
        }

      carStr = slaw_string_emit (car);
      cdrStr = slaw_string_emit (cdr);

      if (!carStr || strcmp (carStr, "cold greeting") != 0)
        {
          fprintf (stderr, "car is wrong for i = %d\n", i);
          return EXIT_FAILURE;
        }

      if (!cdrStr || strcmp (cdrStr, "have an ice day") != 0)
        {
          fprintf (stderr, "cdr is wrong for i = %d\n", i);
          return EXIT_FAILURE;
        }


      protein_free (p);
    }

  slaw_free (descrips);
  slaw_free (ingests);

  return EXIT_SUCCESS;
}

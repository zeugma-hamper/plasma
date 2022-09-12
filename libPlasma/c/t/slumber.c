
/* (c)  oblong industries */

#include "libLoam/c/ob-log.h"
#include "libPlasma/c/slaw.h"
#include <libPlasma/c/protein.h>
#include <stdlib.h>



#define HYPERMESS(mess1, mess2) fprintf (stderr, "%s%s\n", mess1, mess2)


#define HYPERFAIL(mess1, mess2)                                                \
  do                                                                           \
    {                                                                          \
      HYPERMESS (mess1, mess2);                                                \
      exit (EXIT_FAILURE);                                                     \
    }                                                                          \
  while (0)


#define MASSIFAIL(mess) HYPERFAIL (mess, "")


#define TESTY(flavor, varr, anss)                                              \
  const flavor *varr;                                                          \
  if (!(s = slaw_map_find_c (protein_ingests (p), #flavor)))                   \
    HYPERFAIL ("drastic failure attempting extraction of ", #flavor);          \
  else if (!(varr = slaw_##flavor##_emit (s)))                                 \
    HYPERFAIL (#flavor, " could not be extracted -- type mismatch?");          \
  else if (anss != *varr)                                                      \
  HYPERFAIL ("what went in's not nearly what came out of the ", #flavor)



static void protest (protein p)
{
  bslaw s;

  TESTY (int8, i8, 51);
  TESTY (int16, i16, 52);
  TESTY (int32, i32, 53);
  TESTY (int64, i64, 54);

  TESTY (unt8, u8, 61);
  TESTY (unt16, u16, 62);
  TESTY (unt32, u32, 63);
  TESTY (unt64, u64, 64);
}



#define HASTY(flavor, varr, vall)                                              \
  flavor varr = vall;                                                          \
  slaw s##flavor = slaw_##flavor (varr);                                       \
  OB_DIE_ON_ERROR (slabu_map_put_cf (pb, #flavor, s##flavor))


int main (int ac, char **av)
{
  slabu *pb = slabu_new ();

  fprintf (stderr, "putting members of the numerical menagerie through the "
                   "wringer...\n");

  HASTY (int8, i8, 51);
  HASTY (int16, i16, 52);
  HASTY (int32, i32, 53);
  HASTY (int64, i64, 54);

  HASTY (unt8, u8, 61);
  HASTY (unt16, u16, 62);
  HASTY (unt32, u32, 63);
  HASTY (unt64, u64, 64);

  slabu *des = slabu_new ();
  OB_DIE_ON_ERROR (slabu_list_add_c (des, "it's what you've always wanted"));

  protein p, pp;

  if (!(p = protein_from_ff (slaw_list_f (des), slaw_map_f (pb))))
    MASSIFAIL ("protein_from_ff(): failed like a champ.");

  protest (p);

  if (!(pp = protein_dup (p)))
    MASSIFAIL ("protein_dup(): failed like a chimp.");

  protest (pp);

  protein_free (p);
  protein_free (pp);

  fprintf (stderr, "... and no disappointing failures to report.\n");

  return EXIT_SUCCESS;
}


/* (c)  oblong industries */

/**************************************************
 * nondeterministic pad bytes thwart slawx_equal
 *
 * slawx_equal performs a binary comparison of all the quads in a slaw:
 *
 * \code
 * int slawx_equal (slaw s1, slaw s2)
 * { if (!s1  ||  !s2)
 *     return 0;
 *   unt64 len = slaw_quadlen (s1);
 *   if (slaw_quadlen (s2)  !=  len)
 *     return 0;
 *   for ( ; len>0 ; len--)
 *     if (*s1++ != *s2++)
 *       return 0;
 *   return 1;
 * }
 * \endcode
 *
 * However, in the case of, say, an int8, the last quad only contains one byte,
 * and the remaining three bytes contain whatever garbage malloc returned. So
 * when comparing two int8 slawx, there is a good chance they will be not equal,
 * because of the different uninitialized values in the last quad.
 *
 * This test should demonstrate this issue.
 **************************************************/

#include "libLoam/c/ob-rand.h"
#include "libPlasma/c/slaw.h"

#include <stdio.h>
#include <stdlib.h>

/* uninitialized padding problems will only show up if memory has
 * already been used and it contains something other than zero. */
static void randomize_mem (void)
{
  int i;

  for (i = 8; i < 256; i += 8)
    {
      byte *foo = (byte *) malloc (i);
      ob_random_bytes (foo, i);
      free (foo);
    }
}

static slaw create_slaw_string (void)
{
  slaw s = slaw_string ("Remain seated please. ");
  return slaw_string_concat_cstrings_f (s, "Permanaced sentados por favor.",
                                        NULL);
}

int main (int argc, char **argv)
{
  slaw s1, s2;
  int bad = 0;

  ob_rand_seed_int32 (24601);

  randomize_mem ();

  printf ("strings... ");

  s1 = create_slaw_string ();
  s2 = create_slaw_string ();

  if (slawx_equal (s1, s2))
    printf ("equal\n");
  else
    printf ("not equal\n"), bad++;

  slaw_free (s1);
  slaw_free (s2);

  randomize_mem ();

  printf ("numeric singletons... ");

  s1 = slaw_unt8 (77);
  s2 = slaw_unt8 (77);

  if (slawx_equal (s1, s2))
    printf ("equal\n");
  else
    printf ("not equal\n"), bad++;

  slaw_free (s1);
  slaw_free (s2);

  randomize_mem ();

  printf ("numeric arrays... ");

  s1 = slaw_unt8_array_filled (1, 20);
  s2 = slaw_unt8_array_filled (1, 20);

  if (slawx_equal (s1, s2))
    printf ("equal\n");
  else
    printf ("not equal\n"), bad++;

  printf ("%d failures\n", bad);

  slaw_free (s1);
  slaw_free (s2);

  return bad;
}

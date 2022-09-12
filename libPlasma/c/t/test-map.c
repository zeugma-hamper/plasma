
/* (c)  oblong industries */

// Tests slaw maps.

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/slaw-string.h"
#include "libPlasma/c/pool.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM 500
#define ITER 400

// This is just to make valgrind happy!
static slaw to_be_freed[50000];
static int free_counter = 0;

static slaw f (slaw s)
{
  to_be_freed[free_counter++] = s;
  return s;
}

int main (int argc, char **argv)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  slaw m = slaw_map_inline_cc ("cat", "meow", "dog", "bark", "snake", "hiss",
                               "raven", "nevermore", "turkey", "gobble",
                               "anteater", "Mmmmm... ants are yummy!", NULL);

  bslaw v = slaw_map_find_c (m, "raven");
  if (!slawx_equal_lc (v, "nevermore"))
    OB_FATAL_ERROR_CODE (0x20311000, "raven error\n");

  v = slaw_map_find_f (m, slaw_string ("snake"));
  if (!slawx_equal_lc (v, "hiss"))
    OB_FATAL_ERROR_CODE (0x20311001, "snake error\n");

  v = slaw_map_find (m, f (slaw_string ("anteater")));
  slabu *sb = slabu_of_strings_from_split (slaw_string_emit (v), " ");
  bslaw s = slabu_list_nth (sb, 3);
  if (!slawx_equal_lc (s, "yummy!"))
    OB_FATAL_ERROR_CODE (0x20311002, "anteater error\n");

  v = slaw_map_find_c (m, "fish");
  if (v != NULL)
    OB_FATAL_ERROR_CODE (0x20311003, "fish error\n");

  slaw_free (m);

  m = slaw_map_f (sb); /* since elements are not cons, map should be empty */
  if (slaw_map_count (m) != 0)
    OB_FATAL_ERROR_CODE (0x20311004, "empty map not empty\n");

  m = slaw_map_inline_ff (slaw_string ("capybara"), slaw_nil (),
                          slaw_string ("an empty map"), m, NULL);

  v = slaw_map_find_f (m, slaw_string ("capybara"));
  if (!slaw_is_nil (v))
    {
      fprintf (stderr, "m is:\n");
      slaw_spew_overview_to_stderr (m);
      fprintf (stderr, "\nv is:\n");
      slaw_spew_overview_to_stderr (v);
      fprintf (stderr, "\n");
      OB_FATAL_ERROR_CODE (0x20311005, "capybara error\n");
    }

  v = slaw_map_find (m, f (slaw_string ("an empty map")));
  if (!slaw_is_map (v))
    OB_FATAL_ERROR_CODE (0x20311006, "not a map\n");

  slaw_free (m);

  m = slaw_map_inline (f (slaw_string ("potato")), f (slaw_string ("tomato")),
                       f (slaw_string ("pajamas")),
                       f (slaw_v3unt8c_array_empty (0)), NULL);

  v = slaw_map_find_c (m, "potato");
  if (!slawx_equal_lc (v, "tomato"))
    {
      fprintf (stderr, "m is:\n");
      slaw_spew_overview_to_stderr (m);
      fprintf (stderr, "\nv is:\n");
      slaw_spew_overview_to_stderr (v);
      fprintf (stderr, "\n");
      OB_FATAL_ERROR_CODE (0x20311007, "potato error\n");
    }

  v = slaw_map_find_c (m, "tomato");
  if (v != NULL)
    OB_FATAL_ERROR_CODE (0x20311008, "tomato error\n");

  v = slaw_map_find_c (m, "pajamas");
  if (!slaw_is_v3unt8c_array (v))
    {
      fprintf (stderr, "====================\n");
      slaw_spew_overview_to_stderr (v);
      fprintf (stderr, "\n====================\n");
      OB_FATAL_ERROR_CODE (0x20311009, "pajamas error\n");
    }

  slaw_free (m);

  sb = slabu_new ();

  bool exists[NUM];
  int i;

  memset (exists, 0, sizeof (exists));

  for (i = 0; i < ITER; i++)
    {
      int r = (rand () % NUM);
      slaw key = slaw_unt16 (r);
      slaw value = slaw_float32 (sqrt ((float) r));

      switch (rand () % 4)
        {
          case 0:
            OB_DIE_ON_ERROR (slabu_map_put (sb, f (key), f (value)));
            break;
          case 1:
            OB_DIE_ON_ERROR (slabu_map_put_ff (sb, key, value));
            break;
          case 2:
            OB_DIE_ON_ERROR (slabu_map_put_lf (sb, f (key), value));
            break;
          case 3:
            OB_DIE_ON_ERROR (slabu_map_put_fl (sb, key, f (value)));
            break;
        }

      exists[r] = true;
    }

  m = slaw_map (sb);
  slabu_free (sb);

  for (i = 0; i < ITER; i++)
    {
      int r = (rand () % NUM);
      slaw key = slaw_unt16 (r);
      bslaw result;

      if ((rand () & 1) == 0)
        result = slaw_map_find (m, f (key));
      else
        result = slaw_map_find_f (m, key);

      if (exists[r])
        {
          if (result == NULL)
            OB_FATAL_ERROR_CODE (0x2031100a, "expected to find %d but didn't\n",
                                 r);
          else if (!slaw_is_float32 (result))
            OB_FATAL_ERROR_CODE (0x2031100b,
                                 "expected a float32 and it wasn't\n");
        }
      else
        {
          if (result != NULL)
            OB_FATAL_ERROR_CODE (0x2031100c, "found %d but didn't expect to\n",
                                 r);
        }
    }

  slaw_free (m);

  // free stuff to please valgrind
  for (i = 0; i < free_counter; i++)
    slaw_free (to_be_freed[i]);

  return EXIT_SUCCESS;
}

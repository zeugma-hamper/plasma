
/* (c)  oblong industries */

// Test inserting and replacing in slaw lists.

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/slaw-string.h"
#include "libPlasma/c/pool.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main (int argc, char **argv)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  slaw row = slaw_string ("row");
  slabu *sb = slabu_new ();
  int64 got, expected;

  if (!slabu_is_map (sb))
    OB_FATAL_ERROR_CODE (0x20303000, "empty slabu should be a valid map\n");

  got = slabu_list_insert (sb, -1, row);
  expected = 0;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x20303001,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  if (slabu_is_map (sb))
    OB_FATAL_ERROR_CODE (0x20303002,
                         "should not be a map: has %" OB_FMT_64 "d entries\n",
                         slabu_count (sb));

  got = slabu_list_insert_f (sb, 1, slaw_string ("boat"));
  expected = 1;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x20303003,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  got = slabu_list_insert_c (sb, 1, "your");
  expected = 1;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x20303004,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  got = slabu_list_insert_z (sb, 0, row);
  expected = 0;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x20303005,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  got = slabu_list_insert_x (sb, 0, slaw_string ("row"));
  expected = 0;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x20303006,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  got = slabu_list_insert_z (sb, 6, row);
  expected = OB_BAD_INDEX;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x20303007,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  slaw str = slaw_strings_join_slabu (sb, " ");
  const char *expected_str = "row row row your boat";
  if (!slawx_equal_lc (str, expected_str))
    OB_FATAL_ERROR_CODE (0x20303008, "got '%s' but expected '%s'\n",
                         slaw_string_emit (str), expected_str);

  slaw rho = slaw_string ("rho");

  got = slabu_list_replace_nth (sb, slabu_list_find_c (sb, "row"), rho);
  expected = 0;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x20303009,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  got = slabu_list_replace_nth_c (sb, slabu_list_find_c (sb, "row"), "rho");
  expected = 1;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x2030300a,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  got = slabu_list_replace_nth_z (sb, slabu_list_find_c (sb, "row"), rho);
  expected = 2;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x2030300b,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  slaw cons = slaw_cons_cf ("song", str);
  str = slaw_strings_join_slabu (sb, " ");
  expected_str = "rho rho rho your boat";
  if (!slawx_equal_lc (str, expected_str))
    OB_FATAL_ERROR_CODE (0x2030300c, "got '%s' but expected '%s'\n",
                         slaw_string_emit (str), expected_str);

  got = slabu_list_replace_nth_x (sb, 4, cons);
  expected = 4;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x2030300d,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  cons = slaw_cons_cc ("foo", "bar");
  got = slabu_list_replace_nth_f (sb, 3, cons);
  expected = 3;
  if (got != expected)
    OB_FATAL_ERROR_CODE (0x2030300e,
                         "got %" OB_FMT_64 "d but expected %" OB_FMT_64 "d\n",
                         got, expected);

  if (slabu_list_remove_nth (sb, -3) != OB_OK)
    OB_FATAL_ERROR_CODE (0x2030300f, "not OK\n");

  if (slabu_list_remove_nth (sb, 0) != OB_OK)
    OB_FATAL_ERROR_CODE (0x20303010, "not OK\n");

  if (slabu_list_remove_c (sb, "rho") != OB_OK)
    OB_FATAL_ERROR_CODE (0x20303011, "not OK\n");

  if (!slabu_is_map (sb))
    OB_FATAL_ERROR_CODE (0x20303012, "expected it to be a map now\n");

  slabu_free (sb);
  slaw_free (str);
  slaw_free (rho);
  slaw_free (row);

  return EXIT_SUCCESS;
}

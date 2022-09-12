
/* (c)  oblong industries */

/* Tests:
 * - cons
 * - slaw_strings_concat
 * - coercion of floats and vectors
 * - slaw_numeric_vector_dimension
 * - slaw_numeric_unit_bsize
 * - slaw_strings_join
 * - slabu_dup
 * - slabu_of_strings_from_split
 * - slaw_map_find_c
 */

#include "libPlasma/c/slaw.h"
#include "libPlasma/c/slaw-string.h"
#include "libPlasma/c/slaw-coerce.h"
#include "libPlasma/c/slaw-ordering.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"

#include <stdlib.h>

static void car_cdr_equal (slaw s)
{
  bslaw car, cdr;

  if (!slaw_is_cons (s))
    OB_FATAL_ERROR_CODE (0x20312000, "not cons\n");

  car = slaw_cons_emit_car (s);
  cdr = slaw_cons_emit_cdr (s);

  if (!car || !cdr)
    OB_FATAL_ERROR_CODE (0x20312001, "null");

  if (!slawx_equal (car, cdr))
    {
      slaw_spew_overview_to_stderr (s);
      OB_FATAL_ERROR_CODE (0x20312002, "not equal");
    }
}

static void fun_with_cons (bslaw s1, bslaw s2) /* must be strings */
{
  slaw c1 = slaw_cons (s1, s2);
  slaw c2 = slaw_cons_cl (slaw_string_emit (s1), s2);

  slaw c3 = slaw_cons_cf (slaw_string_emit (s1), slaw_dup (s2));
  slaw c4 = slaw_cons_lc (s1, slaw_string_emit (s2));

  slaw c5 = slaw_cons_fc (slaw_dup (s1), slaw_string_emit (s2));
  slaw c6 = slaw_cons_cc (slaw_string_emit (s1), slaw_string_emit (s2));

  slaw x1 = slaw_cons_lf (c1, c2);
  slaw x2 = slaw_cons_fl (c3, c4);
  slaw x3 = slaw_cons_ff (c5, c6);

  slaw y1, y2, z;

  car_cdr_equal (x1);
  car_cdr_equal (x2);
  car_cdr_equal (x3);

  y1 = slaw_cons_fl (x1, x2);
  y2 = slaw_cons_ff (x2, x3);

  car_cdr_equal (y1);
  car_cdr_equal (y2);

  z = slaw_cons_lf (y1, y2);

  car_cdr_equal (z);

  if (slaw_numeric_vector_dimension (z) != -1)
    OB_FATAL_ERROR_CODE (0x20312003, "not -1");

  if (slaw_numeric_unit_bsize (y1) != 0)
    OB_FATAL_ERROR_CODE (0x20312004, "not 0");

  if (slaw_numeric_vector_dimension (s1) != -1)
    OB_FATAL_ERROR_CODE (0x20312005, "not -1");

  if (slaw_numeric_unit_bsize (s2) != 0)
    OB_FATAL_ERROR_CODE (0x20312006, "not 0");

  slaw_free (c1);
  slaw_free (c4);
  slaw_free (y1);
  slaw_free (z);
}

int main (int argc, char **argv)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  const char *yovo = "yovo, ";
  slaw str1, str2, str3, lst;
  slaw v, s, m;
  float64 v1;
  v2float64 v2;
  v3float64 v3;
  v4float64 v4;
  int64 i, j;
  slabu *sb = slabu_new ();
  slabu *sb2;
  ob_retort tort;

  str1 = slaw_strings_concat_c (yovo, yovo, "an oblong's life for me", NULL);
  OB_DIE_ON_ERROR (slabu_list_add_c (sb, "yo"));
  OB_DIE_ON_ERROR (slabu_list_add_c (sb, "vo, yovo, an oblong'"));
  OB_DIE_ON_ERROR (slabu_list_add_c (sb, "s life for me"));
  str2 = slaw_strings_join_f (slaw_list_f (sb), NULL);

  if (!slawx_equal (str1, str2))
    OB_FATAL_ERROR_CODE (0x20312007, "not equal: '%s' and '%s'\n",
                         slaw_string_emit (str1), slaw_string_emit (str2));

  fun_with_cons (str1, str2);

  sb = slabu_of_strings_from_split (slaw_string_emit (str1), ",");

  for (i = 0; i < slabu_count (sb); i++)
    for (j = 0; j < slabu_count (sb); j++)
      fun_with_cons (slabu_list_nth (sb, i), slabu_list_nth (sb, j));

  lst = slaw_list_f (sb);
  str3 = slaw_strings_join (lst, ",");

  if (!slawx_equal_lc (str3, slaw_string_emit (str2)))
    OB_FATAL_ERROR_CODE (0x20312008, "not equal: '%s' and '%s'\n",
                         slaw_string_emit (str3), slaw_string_emit (str2));

  slaw_free (str1);
  slaw_free (str2);
  slaw_free (str3);
  slaw_free (lst);

  str1 = slaw_string ("1");
  str2 = slaw_string ("2, 3");
  str3 = slaw_string ("4, 5, 6");
  lst = slaw_list_inline_c ("7", "8", "9", "10", NULL);

  if (OB_OK != slaw_to_float64 (str1, &v1))
    OB_FATAL_ERROR_CODE (0x20312009, "error\n");

  if (OB_OK != slaw_to_v2 (str2, &v2))
    OB_FATAL_ERROR_CODE (0x2031200a, "error\n");

  if (OB_OK != slaw_to_v3 (str3, &v3))
    OB_FATAL_ERROR_CODE (0x2031200b, "error\n");

  if (OB_OK != slaw_to_v4 (lst, &v4))
    OB_FATAL_ERROR_CODE (0x2031200c, "error\n");

  slaw_free (str1);
  slaw_free (str2);
  slaw_free (str3);
  slaw_free (lst);

  v = slaw_float64 (v1);
  if (1 != slaw_numeric_vector_dimension (v))
    OB_FATAL_ERROR_CODE (0x2031200d, "not 1\n");
  if (8 != slaw_numeric_unit_bsize (v))
    OB_FATAL_ERROR_CODE (0x2031200e, "not 8\n");
  slaw_free (v);

  v = slaw_v2float64 (v2);
  if (2 != slaw_numeric_vector_dimension (v))
    OB_FATAL_ERROR_CODE (0x2031200f, "not 2\n");
  if (16 != slaw_numeric_unit_bsize (v))
    OB_FATAL_ERROR_CODE (0x20312010, "not 16\n");
  slaw_free (v);

  v = slaw_v3float64 (v3);
  if (3 != slaw_numeric_vector_dimension (v))
    OB_FATAL_ERROR_CODE (0x20312011, "not 3\n");
  if (24 != slaw_numeric_unit_bsize (v))
    OB_FATAL_ERROR_CODE (0x20312012, "not 24\n");
  slaw_free (v);

  v = slaw_v4float64 (v4);
  if (4 != slaw_numeric_vector_dimension (v))
    OB_FATAL_ERROR_CODE (0x20312013, "not 4\n");
  if (32 != slaw_numeric_unit_bsize (v))
    OB_FATAL_ERROR_CODE (0x20312014, "not 32\n");
  slaw_free (v);

  s = slaw_v3unt8c_array_empty (7);
  if (3 != slaw_numeric_vector_dimension (s))
    OB_FATAL_ERROR_CODE (0x20312015, "not 3\n");
  if (6 != slaw_numeric_unit_bsize (s))
    OB_FATAL_ERROR_CODE (0x20312016, "not 6\n");
  slaw_free (s);

  // This is like a separate test; it could be moved into its own file

  sb = slabu_of_strings_from_split ("a man a plan a canal panama", " ");
  sb2 = slabu_dup (sb);

  for (i = 0; i < 2; i++)
    if ((tort = slabu_list_remove_c (sb2, "a")) < 0)
      OB_FATAL_ERROR_CODE (0x20312017, "tort = %" OB_FMT_RETORT "d\n", tort);

  if ((tort = slabu_list_remove_nth (sb2, 0)) < 0)
    OB_FATAL_ERROR_CODE (0x20312018, "tort = %" OB_FMT_RETORT "d\n", tort);

  if ((tort = slabu_list_remove_c (sb, "panama")) < 0)
    OB_FATAL_ERROR_CODE (0x20312019, "tort = %" OB_FMT_RETORT "d\n", tort);
  OB_DIE_ON_ERROR (slabu_list_add_c (sb, "banana"));

  s = slaw_list_f (sb);
  str1 = slaw_strings_join_f (s, " ");
  s = slaw_list_f (sb2);
  str2 = slaw_strings_join (s, " ");

  if (!slawx_equal_lc (str1, "a man a plan a canal banana"))
    OB_FATAL_ERROR_CODE (0x2031201a, "banana error: '%s'\n",
                         slaw_string_emit (str1));

  if (!slawx_equal_lc (str2, "plan a canal panama"))
    OB_FATAL_ERROR_CODE (0x2031201b, "plan error: '%s'\n",
                         slaw_string_emit (str2));

  sb = slabu_from_slaw (s);
  slaw_free (s);
  slaw_free (str1);
  slaw_free (str2);

  OB_DIE_ON_ERROR (slabu_list_add_c (sb, "hat"));
  if ((tort = slabu_list_remove_c (sb, "canal")) < 0)
    OB_FATAL_ERROR_CODE (0x2031201c, "tort = %" OB_FMT_RETORT "d\n", tort);

  s = slaw_list_f (sb);
  str1 = slaw_strings_join_f (s, " ");

  if (!slawx_equal_lc (str1, "plan a panama hat"))
    OB_FATAL_ERROR_CODE (0x2031201d, "hat error\n");

  slaw_free (str1);

  // and this is like a third test; I'm getting lazy

  s = slaw_list_f (slabu_new ());
  m = slaw_map_f (slabu_new ());

  if (slawx_equal (s, m))
    OB_FATAL_ERROR_CODE (0x2031201e, "slaw and map are equal\n");
  if (slaw_semantic_compare (s, m) == 0)
    OB_FATAL_ERROR_CODE (0x2031201f, "slaw and map are equal\n");

  sb = slabu_from_slaw (s);
  sb2 = slabu_from_slaw_f (m);

  slaw_free (s);

  if (slabu_count (sb) != 0 || slabu_count (sb2) != 0)
    OB_FATAL_ERROR_CODE (0x20312020, "slabu problem\n");

  slabu_free (sb);
  slabu_free (sb2);

  // fourth little mini-test

  s = slaw_v3unt16_array_empty (7);
  if (!slawx_equal_lf (s, slaw_v3unt16_array_empty (7)))
    OB_FATAL_ERROR_CODE (0x20312021, "slawx_equal_lf should have been true\n");
  if (slawx_equal_lf (s, slaw_v3unt16_array_empty (6)))
    OB_FATAL_ERROR_CODE (0x20312022, "slawx_equal_lf should have been false\n");
  if (slawx_equal_lf (s, slaw_v3int16_array_empty (7)))
    OB_FATAL_ERROR_CODE (0x20312023, "slawx_equal_lf should have been false\n");
  slaw_free (s);

  // make sure slaw_map_find_c works on empty lists

  s = slaw_list_f (slabu_new ());
  if (slaw_map_find_c (s, "you won't find me") != NULL)
    OB_FATAL_ERROR_CODE (0x20312024, "that was unexpected\n");
  slaw_free (s);

  return EXIT_SUCCESS;
}

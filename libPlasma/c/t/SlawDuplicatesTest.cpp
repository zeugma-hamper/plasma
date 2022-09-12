
/* (c)  oblong industries */

#include "libPlasma/c/slaw-string.h"
#include "libPlasma/c/slaw.h"

#include <gtest/gtest.h>

namespace {

TEST (SlawDuplicatesTest, SlawCons)
{
  slaw s = slaw_int8 (3);
  EXPECT_TRUE (NULL == slaw_cons (NULL, s));
  EXPECT_TRUE (NULL == slaw_cons (s, NULL));
  EXPECT_TRUE (NULL == slaw_cons_lf (NULL, s));
  s = slaw_int8 (4);
  EXPECT_TRUE (NULL == slaw_cons_fl (NULL, s));
  EXPECT_TRUE (NULL == slaw_cons_cf (NULL, s));
  s = slaw_int16 (32);
  EXPECT_TRUE (NULL == slaw_cons_fc (s, NULL));
  s = slaw_string ("hello");
  slaw c = slaw_cons_ff (s, s);
  EXPECT_FALSE (NULL == c);
  slaw_free (c);
}

TEST (SlawDuplicatesTest, ListInline)
{
  slaw s = slaw_string ("foo");
  slaw ls = slaw_list_inline_f (s, s, s, s, NULL);
  EXPECT_EQ (4, slaw_list_count (ls));
  for (int i = 1; i < 4; ++i)
    EXPECT_TRUE (
      slawx_equal (slaw_list_emit_nth (ls, 0), slaw_list_emit_nth (ls, i)));
  slaw_free (ls);
}

TEST (SlawDuplicatesTest, ListContigSearch)
{
  slaw s = slaw_string ("foo");
  slaw k = slaw_int8 (3);
  slaw lst = slaw_list_inline (s, k, k, s, NULL);
  EXPECT_EQ (1, slaw_list_contigsearch_inline_f (lst, k, k, NULL));
  k = slaw_boolean (false);
  EXPECT_GT (0, slaw_list_contigsearch_inline_f (lst, k, s, k, NULL));
  slaw_free (lst);
}

TEST (SlawDuplicatesTest, ListGapSearch)
{
  slaw s = slaw_string ("foo");
  slaw k = slaw_int8 (3);
  slaw lst = slaw_list_inline (s, k, s, k, NULL);
  EXPECT_EQ (1, slaw_list_gapsearch_inline_f (lst, k, k, NULL));
  k = slaw_boolean (false);
  EXPECT_GT (0, slaw_list_gapsearch_inline_f (lst, k, k, s, NULL));
  slaw_free (lst);
}

TEST (SlawDuplicatesTest, ListConcat)
{
  slaw ls = slaw_list_inline_c ("a", "b", "c", NULL);
  slaw lls =
    slaw_lists_concat_f (ls, ls, slaw_list_inline_c ("d", "e", NULL), ls, NULL);
  EXPECT_EQ (11, slaw_list_count (lls));
  slaw_free (lls);
}

TEST (SlawDuplicatesTest, StringConcat)
{
  slaw s = slaw_string ("as");
  slaw s2 = slaw_strings_concat_f (s, s, s, NULL);
  EXPECT_STREQ ("asasas", slaw_string_emit (s2));
  slaw_free (s2);
}

TEST (SlawDuplicatesTest, MapPut)
{
  slabu *sb = slabu_new ();
  slaw s = slaw_string ("something");
  EXPECT_LE (0, slabu_map_put_ff (sb, s, s));
  slaw m = slaw_map_f (sb);
  s = slaw_string ("something");
  EXPECT_TRUE (slawx_equal (slaw_map_find_c (m, "something"), s));
  slaw_free (m);
  slaw_free (s);
}

}  // namespace

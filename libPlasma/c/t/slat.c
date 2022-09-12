
/* (c)  oblong industries */

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"
#include <libPlasma/c/slaw.h>

int main (int ac, char **av)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  slaw s1 = slaw_string ("abaft");
  slaw s2 = slaw_string ("athwart");
  slaw s3 = slaw_string ("askance");
  slaw s4 = slaw_string ("aloft");

  slaw sa = slaw_int32 (1832);
  slaw sb = slaw_int32 (-5);
  slaw sc = slaw_int32 (7734);
  slaw sd = slaw_int32 (0);

  slaw sl = slaw_list_inline (s1, sa, NULL);
  slaw sll = slaw_list_inline (s2, sb, NULL);
  slaw slll = slaw_list_inline (s3, sc, NULL);
  slaw sllll = slaw_list_inline (s4, sd, NULL);

  slaw ss;

  //
  // strings, Chet.
  //
  ss = slaw_strings_concat_f (slaw_dup (s1), slaw_dup (s2), slaw_dup (s3),
                              slaw_dup (s4), NULL);
  OBSERT (0 == strcmp ("abaftathwartaskancealoft", slaw_string_emit (ss)));
  Free_Slaw (ss);

  ss = slaw_strings_concat (s1, s2, s3, s4, NULL);
  OBSERT (0 == strcmp ("abaftathwartaskancealoft", slaw_string_emit (ss)));
  Free_Slaw (ss);


  ss =
    slaw_strings_concat_f (slaw_dup (s1), slaw_dup (s2), slaw_dup (sb),
                           slaw_dup (s3), slaw_dup (s4), slaw_dup (sd), NULL);
  OBSERT (0 == strcmp ("abaftathwartaskancealoft", slaw_string_emit (ss)));
  Free_Slaw (ss);

  ss = slaw_strings_concat (s1, s2, sb, s3, s4, sd, NULL);
  OBSERT (0 == strcmp ("abaftathwartaskancealoft", slaw_string_emit (ss)));
  Free_Slaw (ss);


  ss = slaw_string_concat_cstrings_f (slaw_dup (s1), " the", " thwarty",
                                      " loft", NULL);
  OBSERT (0 == strcmp ("abaft the thwarty loft", slaw_string_emit (ss)));
  Free_Slaw (ss);

  ss = slaw_string_concat_cstrings (s1, " the", " thwarty", " loft", NULL);
  OBSERT (0 == strcmp ("abaft the thwarty loft", slaw_string_emit (ss)));
  Free_Slaw (ss);


  ss = slaw_strings_concat_f (slaw_dup (s2), NULL);
  OBSERT (slawx_equal (s2, ss));
  OBSERT (0 == strcmp ("athwart", slaw_string_emit (ss)));
  Free_Slaw (ss);

  ss = slaw_strings_concat (s2, NULL);
  OBSERT (slawx_equal (s2, ss));
  OBSERT (0 == strcmp ("athwart", slaw_string_emit (ss)));
  Free_Slaw (ss);

  ss = slaw_string_concat_cstrings_f (slaw_dup (s3), NULL, NULL);
  OBSERT (slawx_equal (s3, ss));
  OBSERT (0 == strcmp ("askance", slaw_string_emit (ss)));
  Free_Slaw (ss);

  ss = slaw_string_concat_cstrings (s3, NULL, NULL);
  OBSERT (slawx_equal (s3, ss));
  OBSERT (0 == strcmp ("askance", slaw_string_emit (ss)));
  Free_Slaw (ss);


  //
  // lists, Gwen.
  //
  slabu *slb = slabu_new ();
  OB_DIE_ON_ERROR (slabu_list_add_x (slb, s1));
  OB_DIE_ON_ERROR (slabu_list_add_x (slb, sa));
  OB_DIE_ON_ERROR (slabu_list_add_x (slb, s2));
  OB_DIE_ON_ERROR (slabu_list_add_x (slb, sb));
  OB_DIE_ON_ERROR (slabu_list_add_x (slb, s3));
  OB_DIE_ON_ERROR (slabu_list_add_x (slb, sc));
  OB_DIE_ON_ERROR (slabu_list_add_x (slb, s4));
  OB_DIE_ON_ERROR (slabu_list_add_x (slb, sd));
  slaw sL = slaw_list (slb);

  ss = slaw_lists_concat_f (slaw_dup (sl), slaw_dup (sll), slaw_dup (slll),
                            slaw_dup (sllll), NULL);
  OBSERT (slawx_equal (sL, ss));
  Free_Slaw (ss);

  ss = slaw_lists_concat (sl, sll, slll, sllll, NULL);
  OBSERT (slawx_equal (sL, ss));
  Free_Slaw (ss);

  ss = slaw_lists_concat_f (slaw_dup (sl), slaw_dup (sll), slaw_dup (sa),
                            slaw_dup (slll), slaw_dup (sllll), slaw_dup (s1),
                            NULL);
  OBSERT (slawx_equal (sL, ss));
  Free_Slaw (ss);

  ss = slaw_lists_concat (sl, sll, sb, slll, sllll, s3, s4, NULL);
  OBSERT (slawx_equal (sL, ss));
  Free_Slaw (ss);

  slaw sss = slaw_lists_concat (sl, sll, NULL);
  ss = slaw_lists_concat_f (slaw_dup (s2), slaw_dup (sl), slaw_dup (sll), NULL);
  OBSERT (slawx_equal (sss, ss));
  Free_Slaw (ss);

  ss = slaw_lists_concat_f (slaw_dup (sll), NULL);
  OBSERT (slawx_equal (sll, ss));
  Free_Slaw (ss);

  ss = slaw_lists_concat (sllll, NULL);
  OBSERT (slawx_equal (sllll, ss));
  Free_Slaw (ss);

  // free stuff to please valgrind
  slaw_free (sl);
  slaw_free (sll);
  slaw_free (slll);
  slaw_free (sllll);
  slabu_free (slb);
  slaw_free (sL);
  slaw_free (sss);

  return 0;
}


/* (c)  oblong industries */

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/slaw-string.h"
#include "libPlasma/c/pool.h"
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  int i;
  const char *expected;

  for (i = 0; i < 2; i++)
    {
      slaw m1 = slaw_map_inline_cc ("color", "blue", "texture", "rough",
                                    "smell", "wet dog", NULL);

      slaw m2 = slaw_map_inline_cc ("color", "green", NULL);

      slaw m3 =
        slaw_map_inline_cc ("texture", "smooth", "viscosity", "high", NULL);

      slaw m;
      if (i)
        m = slaw_maps_merge (m1, m2, m3, NULL);
      else
        m = slaw_maps_merge_f (m3, m2, m1, NULL);

      if (slaw_map_count (m) != 4)
        OB_FATAL_ERROR_CODE (0x20306000, "[%d] map count was wrong\n", i);

      bslaw v = slaw_map_find_c (m, "smell");
      if (!slawx_equal_lc (v, "wet dog"))
        OB_FATAL_ERROR_CODE (0x20306001, "[%d] wrong smell\n", i);

      v = slaw_map_find_c (m, "viscosity");
      if (!slawx_equal_lc (v, "high"))
        OB_FATAL_ERROR_CODE (0x20306002, "[%d] wrong viscosity\n", i);

      v = slaw_map_find_c (m, "color");
      if (!slawx_equal_lc (v, (expected = (i ? "green" : "blue"))))
        OB_FATAL_ERROR_CODE (0x20306003,
                             "[%d] wrong color: got '%s', expected '%s'\n", i,
                             slaw_string_emit (v), expected);

      v = slaw_map_find_c (m, "texture");
      if (!slawx_equal_lc (v, (expected = (i ? "smooth" : "rough"))))
        OB_FATAL_ERROR_CODE (0x20306004,
                             "[%d] wrong texture: got '%s', expected '%s'\n", i,
                             slaw_string_emit (v), expected);

      slaw_free (m);

      if (i)
        {
          slaw_free (m1);
          slaw_free (m2);
          slaw_free (m3);
        }
    }
  return EXIT_SUCCESS;
}

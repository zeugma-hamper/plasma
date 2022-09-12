
/* (c)  oblong industries */

#include "libLoam/c/ob-log.h"
#include "libPlasma/c/slaw-coerce.h"
#include "libPlasma/c/pool.h"
#include <stdio.h>
#include <stdlib.h>

static void trycoerce (int trial, slaw s)
{
  v2float64 v2;
  v3float64 v3;
  v4float64 v4;
  ob_retort err;

  printf ("\n===== test %d =====\n\n", trial);

  err = slaw_to_v2 (s, &v2);
  if (err == OB_OK)
    {
      printf ("slaw_to_v2: %f, %f\n", v2.x, v2.y);
    }
  else
    {
      printf ("slaw_to_v2: error %" OB_FMT_RETORT "d"
              "\n",
              err);
    }

  err = slaw_to_v3 (s, &v3);
  if (err == OB_OK)
    {
      printf ("slaw_to_v3: %f, %f, %f\n", v3.x, v3.y, v3.z);
    }
  else
    {
      printf ("slaw_to_v3: error %" OB_FMT_RETORT "d"
              "\n",
              err);
    }

  err = slaw_to_v4 (s, &v4);
  if (err == OB_OK)
    {
      printf ("slaw_to_v4: %f, %f, %f, %f\n", v4.x, v4.y, v4.z, v4.w);
    }
  else
    {
      printf ("slaw_to_v4: error %" OB_FMT_RETORT "d"
              "\n",
              err);
    }

  slaw_free (s);
}

int main (int argc, char **argv)
{
  v2int8 q1;
  v3float64 q2;
  v4float32 q3;
  v4unt64c q4;

  slabu *sb = slabu_new ();

  q1.x = 1;
  q1.y = 2;

  q2.x = 3;
  q2.y = 4;
  q2.z = 5;

  q3.x = 6;
  q3.y = 7;
  q3.z = 8;
  q3.w = 9;

  q4.x.re = 10;
  q4.x.im = 11;
  q4.y.re = 12;
  q4.y.im = 13;
  q4.z.re = 14;
  q4.z.im = 15;
  q4.w.re = 16;
  q4.w.im = 17;

  OB_DIE_ON_ERROR (slabu_list_add_x (sb, slaw_unt64 (1)));
  OB_DIE_ON_ERROR (slabu_list_add_x (sb, slaw_int8 (2)));
  OB_DIE_ON_ERROR (slabu_list_add_x (sb, slaw_float32 (3)));
  OB_DIE_ON_ERROR (slabu_list_add_x (sb, slaw_string ("4")));

  trycoerce (1, slaw_string ("1.2, -4.5, 7.8"));
  trycoerce (2, slaw_int32_array_filled (0, 36));
  trycoerce (3, slaw_int32_array_filled (1, 36));
  trycoerce (4, slaw_int32_array_filled (2, 36));
  trycoerce (5, slaw_int32_array_filled (3, 36));
  trycoerce (6, slaw_int32_array_filled (4, 36));
  trycoerce (7, slaw_int32_array_filled (5, 36));
  trycoerce (8, slaw_unt8_array_filled (0, 72));
  trycoerce (9, slaw_unt8_array_filled (1, 72));
  trycoerce (10, slaw_unt8_array_filled (2, 72));
  trycoerce (11, slaw_unt8_array_filled (3, 72));
  trycoerce (12, slaw_unt8_array_filled (4, 72));
  trycoerce (13, slaw_unt8_array_filled (5, 72));
  trycoerce (14, slaw_float32_array_filled (0, 3.14f));
  trycoerce (15, slaw_float32_array_filled (1, 3.14f));
  trycoerce (16, slaw_float32_array_filled (2, 3.14f));
  trycoerce (17, slaw_float32_array_filled (3, 3.14f));
  trycoerce (18, slaw_float32_array_filled (4, 3.14f));
  trycoerce (19, slaw_float32_array_filled (5, 3.14f));
  trycoerce (20, slaw_float64_array_filled (0, 3.14159));
  trycoerce (21, slaw_float64_array_filled (1, 3.14159));
  trycoerce (22, slaw_float64_array_filled (2, 3.14159));
  trycoerce (23, slaw_float64_array_filled (3, 3.14159));
  trycoerce (24, slaw_float64_array_filled (4, 3.14159));
  trycoerce (25, slaw_float64_array_filled (5, 3.14159));
  trycoerce (26, slaw_string ("8,9,10,11"));
  trycoerce (27, slaw_string ("8,9,10,11,12"));
  trycoerce (28, slaw_string ("-1, 0.0001"));
  trycoerce (29, slaw_nil ());
  trycoerce (30, slaw_string ("blech, foo, barf"));
  trycoerce (31, slaw_v2int8 (q1));
  trycoerce (32, slaw_v3float64 (q2));
  trycoerce (33, slaw_v4float32 (q3));
  trycoerce (34, slaw_v4unt64c (q4));
  trycoerce (35, slaw_list_f (sb));

  return EXIT_SUCCESS;
}

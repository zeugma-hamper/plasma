
/* (c)  oblong industries */

#include <stdlib.h>
#include <stdio.h>


#include "libPlasma/c/c-plasma.h"
#include "rubySlaw-ilk-macros.h"


const float64 basic_num = 23.4;

#define CNT 7

#define NUMERIC_BASIC_DEPOSIT(x, T, y)                                         \
  do                                                                           \
    {                                                                          \
      slaw s = slaw_##T (basic_num);                                           \
      deposit_or_die (ph, s, #T);                                              \
    }                                                                          \
  while (0);

#define VECT_DEPOSIT(vT, T, NUM_ARGS)                                          \
  do                                                                           \
    {                                                                          \
      union                                                                    \
      {                                                                        \
        vT val;                                                                \
        T accessor[NUM_ARGS];                                                  \
      } u;                                                                     \
      T num = basic_num;                                                       \
      int q;                                                                   \
      for (q = 0; q < NUM_ARGS; q++)                                           \
        u.accessor[q] = num++;                                                 \
      slaw s = slaw_##vT (u.val);                                              \
      deposit_or_die (ph, s, #vT);                                             \
    }                                                                          \
  while (0);

#define NUMERIC_BASIC_ARRAY_DEPOSIT(aT, T, y)                                  \
  do                                                                           \
    {                                                                          \
      T arr[CNT];                                                              \
      T num = basic_num;                                                       \
      int q;                                                                   \
      for (q = 0; q < CNT; q++)                                                \
        arr[q] = num++;                                                        \
      slaw s = slaw_##aT (arr, CNT);                                           \
      deposit_or_die (ph, s, #aT);                                             \
    }                                                                          \
  while (0);

#define VECT_BASIC_ARRAY_DEPOSIT(aT, T, NUM_ARGS)                              \
  do                                                                           \
    {                                                                          \
      v##NUM_ARGS##T arr[CNT];                                                 \
      T num = basic_num;                                                       \
      int q;                                                                   \
      for (q = 0; q < CNT; q++)                                                \
        {                                                                      \
          union                                                                \
          {                                                                    \
            v##NUM_ARGS##T val;                                                \
            T accessor[NUM_ARGS];                                              \
          } u;                                                                 \
          int qq;                                                              \
          for (qq = 0; qq < NUM_ARGS; qq++)                                    \
            u.accessor[qq] = num++;                                            \
          arr[q] = u.val;                                                      \
        }                                                                      \
      slaw s = slaw_##aT (arr, CNT);                                           \
      deposit_or_die (ph, s, #aT);                                             \
    }                                                                          \
  while (0);


void deposit_or_die (pool_hose ph, slaw s, const char *type_tag)
{
  protein p = protein_from_ff (slaw_list_inline_c ("unit-test", type_tag, NULL),
                               slaw_map_inline_cf ("slaw", s, NULL));

  ob_retort ret = pool_deposit (ph, p, NULL);
  if (ret != OB_OK)
    {
      printf ("couldn't deposit: %s\n", ob_error_string (ret));
      exit (1);
    }
}


int main (int ac, char **av)
{
  if (ac != 2)
    {
      fprintf (stderr, "Usage: c-test-feeder poolname\n");
      return EXIT_FAILURE;
    }
  const char *pname = av[1];

  protein options =
    protein_from_ff (slaw_list_inline_c ("dummy", NULL),
                     slaw_map_inline_cf ("size", slaw_int64 (1048576), NULL));

  ob_retort ret = pool_create (pname, "mmap", options);
  if (ret == POOL_EXISTS)
    {
      printf ("pool %s already exists; "
              "we'll dispose and recreate\n",
              pname);
      ob_retort disp_ret = pool_dispose (pname);
      if (disp_ret != OB_OK)
        {
          printf ("couldn't dispose: %s\n", ob_error_string (ret));
          exit (1);
        }
      ret = pool_create (pname, "mmap", options);
    }
  if (ret != OB_OK)
    {
      printf ("create failed: %s\n", ob_error_string (ret));
      exit (1);
    }

  printf ("filling pool with standard proteins\n");

  pool_hose ph;
  ret = pool_participate (pname, &ph, NULL);
  if (ret != OB_OK)
    {
      printf ("couldn't participate: %s\n", ob_error_string (ret));
      exit (1);
    }

  FOR_ALL_NUMERIC_BASICS (NUMERIC_BASIC_DEPOSIT, , , );
  FOR_ALL_NUMERIC_234VECTS (VECT_DEPOSIT);
  FOR_ALL_NUMERIC_BASIC_ARRAYS (NUMERIC_BASIC_ARRAY_DEPOSIT, );
  FOR_ALL_NUMERIC_V234_ARRAYS (VECT_BASIC_ARRAY_DEPOSIT);

  {
    slaw s = slaw_nil ();
    deposit_or_die (ph, s, "nil");
  }

  {
    slaw s = slaw_boolean (true);
    deposit_or_die (ph, s, "boolean");
    s = slaw_boolean (false);
    deposit_or_die (ph, s, "boolean");
  }

  return 0;
}

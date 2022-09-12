
/* (c)  oblong industries */

// A very basic test of slaw lists.

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"
#include "libPlasma/c/slaw.h"

#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  slaw l = slaw_list_inline_f (slaw_nil (), slaw_boolean (false),
                               slaw_boolean (true), NULL);

  int64 actual, expected;
  if ((actual = slaw_list_count (l)) != (expected = 3))
    OB_FATAL_ERROR_CODE (0x20309000,
                         "expected slaw_list_count to be %" OB_FMT_64
                         "d, but it was %" OB_FMT_64 "d\n",
                         expected, actual);

  bslaw got;
  if (!slaw_is_nil (got = slaw_list_emit_nth (l, 0)))
    {
      slaw_spew_overview_to_stderr (got);
      fputc ('\n', stderr);
      OB_FATAL_ERROR_CODE (0x20309001, "[0] was not nil\n");
    }

  if (!slaw_is_boolean (got = slaw_list_emit_nth (l, 1)))
    {
      slaw_spew_overview_to_stderr (got);
      fputc ('\n', stderr);
      OB_FATAL_ERROR_CODE (0x20309002, "[1] was not boolean\n");
    }

  if (*slaw_boolean_emit (got) != false)
    {
      slaw_spew_overview_to_stderr (got);
      fputc ('\n', stderr);
      OB_FATAL_ERROR_CODE (0x20309003, "[1] was not false\n");
    }

  if (!slaw_is_boolean (got = slaw_list_emit_nth (l, 2)))
    {
      slaw_spew_overview_to_stderr (got);
      fputc ('\n', stderr);
      OB_FATAL_ERROR_CODE (0x20309004, "[2] was not boolean\n");
    }

  if (*slaw_boolean_emit (got) != true)
    {
      slaw_spew_overview_to_stderr (got);
      fputc ('\n', stderr);
      OB_FATAL_ERROR_CODE (0x20309005, "[2] was not true\n");
    }

  if (NULL != (got = slaw_list_emit_nth (l, 3)))
    {
      slaw_spew_overview_to_stderr (got);
      fputc ('\n', stderr);
      OB_FATAL_ERROR_CODE (0x20309006, "[3] was not NULL\n");
    }

  slaw_free (l);

  return EXIT_SUCCESS;
}

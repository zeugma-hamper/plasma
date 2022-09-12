
/* (c)  oblong industries */

// test ob_generate_uuid()

#include "libLoam/c/ob-util.h"
#include "libLoam/c/ob-log.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static void validate_uuid (const char uu[37], const char *name)
{
  int i;

  for (i = 0; i < 37; i++)
    {
      bool ok;
      const char *expected;
      const char c = uu[i];

      if (i == 36)
        {
          expected = "NUL";
          ok = (c == 0);
        }
      else if (i == 8 || i == 13 || i == 18 || i == 23)
        {
          expected = "-";
          ok = (c == '-');
        }
      else
        {
          expected = "hex digit";
          ok = isxdigit (c);
        }

      if (!ok)
        error_exit ("%s: index %d of '%s' was '%c' but expected %s\n", name, i,
                    uu, c, expected);
    }
}

int main (int argc, char **argv)
{
  char uuid1[37];
  char uuid2[37];

  OB_DIE_ON_ERROR (ob_generate_uuid (uuid1));
  OB_DIE_ON_ERROR (ob_generate_uuid (uuid2));

  validate_uuid (uuid1, "uuid1");
  validate_uuid (uuid1, "uuid2");

  if (strcmp (uuid1, uuid2) == 0)
    error_exit ("collision: both are %s\n", uuid1);

  return EXIT_SUCCESS;
}

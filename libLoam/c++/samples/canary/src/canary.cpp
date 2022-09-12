
/* (c)  oblong industries */

/* app that simply tests a few g-speak helper functions, then exits.
 * This is not an interactive application.
 */

#include <libLoam/c++/Str.h>
#include <unicode/unistr.h>
#include <stdio.h>

using namespace oblong::loam;

int my_failures = 0;
#define MY_FAIL() printf ("FAIL: %s\n", __FUNCTION__), my_failures++
#define MY_PASS() printf ("PASS: %s\n", __FUNCTION__)
void MY_FINISH ()
{
  printf ("%s: %d failures\n", __FILE__, my_failures);
  exit (my_failures ? 1 : 0);
}

void check_string_at ()
{
  // Verify that we can fetch one character of a unicode / utf8 string
  const Str german (
    UNICODE_STRING_SIMPLE ("Sch\\u00f6nes Auto: \\u20ac 11240.").unescape ());

  printf ("The character at position %d of %s is %x\n", 14, german.utf8 (),
          german.At (14));
  if (german.At (14) != 0x20ac)
    MY_FAIL ();
  else
    MY_PASS ();
}

#include <unicode/ucol.h>
void check_icu_data ()
{
  // Verify that ICU can open its internal data file
  UErrorCode status = U_ZERO_ERROR;
  UCollator *myCollator = ucol_open ("en_US", &status);
  if (U_FAILURE (status))
    MY_FAIL ();
  else
    {
      MY_PASS ();
      ucol_close (myCollator);
    }
}

int main (int, char **)
{
  check_string_at ();
  check_icu_data ();
  MY_FINISH ();
}

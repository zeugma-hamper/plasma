
/* (c)  oblong industries */

#include "libLoam/c/ob-vers.h"
#include <stdio.h>
#include <stdlib.h>

static void print_version (ob_version_of_what what, const char *name)
{
  char *s = ob_get_version (what);
  printf ("%s => q{%s},\n", name, s);
  free (s);
}

int main (int argc, char **argv)
{
  OB_CHECK_ABI ();
  print_version (OB_VERSION_OF_GSPEAK, "gspeak");
  print_version (OB_VERSION_OF_COMPILER, "compiler");
  print_version (OB_VERSION_OF_OS, "os");
  print_version (OB_VERSION_OF_KERNEL, "kernel");
  print_version (OB_VERSION_OF_LIBC, "libc");
  print_version (OB_VERSION_OF_CPU, "cpu");
  print_version (OB_VERSION_OF_YOBUILD, "yobuild");
  return EXIT_SUCCESS;
}

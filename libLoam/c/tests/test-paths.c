
/* (c)  oblong industries */

#include "libLoam/c/ob-sys.h"
#include "libLoam/c/ob-dirs.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-util.h"
#include "libLoam/c/ob-string.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ob_retort my_search_path (const char *path, const char *filename,
                                 const char *searchspec, ob_path_callback func,
                                 ...)
{
  va_list ap;
  va_start (ap, func);
  ob_retort tort = ob_search_path (path, filename, searchspec, func, ap);
  va_end (ap);
  return tort;
}

static ob_retort appender (const char *name, va_list vargies)
{
  char *buf = va_arg (vargies, char *);
  size_t len = va_arg (vargies, size_t);
  int limit = va_arg (vargies, int);
  int *count = va_arg (vargies, int *);
  if (*buf)
    ob_safe_append_string (buf, len, "+");
  ob_safe_append_string (buf, len, name);
  (*count)++;
  if (*count >= limit)
    return OB_STOP;
  return OB_OK;
}

static void czech (const char *path, const char *searchspec,
                   const char *expected, int limit)
{
  size_t len = 16 + 2 * strlen (expected);
  char *buf = (char *) malloc (len);
  *buf = 0;
  int count = 0;
  ob_retort tort =
    my_search_path (path, "x", searchspec, appender, buf, len, limit, &count);
  if (tort < OB_OK)
    error_exit ("Got %s for spec %s in path %s\n", ob_error_string (tort),
                searchspec, path);
  if (0 != strcmp (buf, expected))
    error_exit ("For spec %s in path %s, expected %s but got %s\n", searchspec,
                path, expected, buf);
  free (buf);
}

#ifdef _MSC_VER
static char *subst (const char *s, char bad, char good)
{
  char *r = strdup (s);
  for (char *p = r; *p; p++)
    if (*p == bad)
      *p = good;
  return r;
}

#define check(a, b, c, d)                                                      \
  do                                                                           \
    {                                                                          \
      char *path = subst (a, ':', ';');                                        \
      char *expected = subst (c, '/', '\\');                                   \
      czech (path, b, expected, d);                                            \
      free (expected);                                                         \
      free (path);                                                             \
    }                                                                          \
  while (0)
#else
#define check czech
#define subst(x, y, z) strdup (x)
#endif

static void check2 (const char *searchspec, const char *filename, int nmatches,
                    ...)
{
  char *fn = subst (filename, '/', '\\');
  const char *srcdir = ABS_TOP_SRCDIR "/libLoam/c/tests";
  size_t cap = 80 + 2 * strlen (srcdir);
  char *path = (char *) alloca (cap);
  snprintf (path, cap, "%s%csniff%cdir1%c%s%csniff%cdir2", srcdir, OB_DIR_CHAR,
            OB_DIR_CHAR, OB_PATH_CHAR, srcdir, OB_DIR_CHAR, OB_DIR_CHAR);
  char *buf = (char *) alloca (cap);
  *buf = 0;
  int count = 0;
  ob_retort tort =
    my_search_path (path, fn, searchspec, appender, buf, cap, 100, &count);
  if (tort < OB_OK)
    error_exit ("Got %s for spec %s in path %s\n", ob_error_string (tort),
                searchspec, path);

  if (count != nmatches)
    error_exit ("Got %d but expected %d for \"%s\", \"%s\"\n", count, nmatches,
                searchspec, filename);

  int i;
  va_list vargs;
  va_start (vargs, nmatches);
  for (i = 0; i < nmatches; i++)
    {
      const char *arg = va_arg (vargs, const char *);
      char *a = subst (arg, '/', '\\');
      if (!strstr (buf, a))
        error_exit ("Couldn't find '%s' in '%s'\n", a, buf);
      free (a);
    }
  va_end (vargs);
  free (fn);
}

int main (int argc, char **argv)
{
  check ("foo:bar:baz", "1", "foo/x+bar/x+baz/x", 100);
  check ("foo:bar:baz", "1", "foo/x+bar/x+baz/x", 3);
  check ("foo:bar:baz", "1", "foo/x+bar/x", 2);
  check ("foo:bar:baz", "1", "foo/x", 1);
  check ("foo:bar:baz", "0", "", 100);
  check ("foo:bar:baz", "00", "", 100);
  check ("foo:bar:baz", "01", "", 100);
  check ("foo:bar:baz", "10", "", 100);
  check ("foo:bar:baz", "0|0", "", 100);
  check ("foo:bar:baz", "0|1", "foo/x+bar/x+baz/x", 100);
  check ("foo:bar:baz", "1|0", "foo/x+bar/x+baz/x", 100);
  check ("foo:bar:baz", "1|1", "foo/x+bar/x+baz/x", 100);
  check ("foo:bar:baz", "01|01", "", 100);
  check ("foo:bar:baz", "10|10", "", 100);
  check ("foo:^bar:baz", "^", "^bar/x", 100);
  check ("foo:^bar:baz", "0|^", "^bar/x", 100);
  check ("foo:^bar:baz", "^0", "", 100);
  check ("foo:^bar:baz", "^1", "^bar/x", 100);
  check ("foo:^bar:baz", "^1|10", "^bar/x", 100);
  check ("foo:^bar:baz", "^|1", "foo/x+^bar/x+baz/x", 100);
  check ("foo:^bar:baz", "^,1", "^bar/x+foo/x+^bar/x+baz/x", 100);
  check ("foo:^bar:baz", "^,1", "^bar/x+foo/x+^bar/x+baz/x", 4);
  check ("foo:^bar:baz", "^,1", "^bar/x+foo/x+^bar/x", 3);
  check ("foo:^bar:baz", "^,1", "^bar/x+foo/x", 2);
  check ("foo:^bar:baz", "^,1", "^bar/x", 1);
  check ("foo:^bar:baz", "0,^", "^bar/x", 100);
  check ("foo:^bar:baz", "^,0", "^bar/x", 100);
  check ("foo:^bar:baz", "1,^", "foo/x+^bar/x+baz/x+^bar/x", 100);
  check2 ("FR", "111", 1, "dir1/111");
  check2 ("FR", "112", 1, "dir1/112");
  check2 ("FR", "113", 0);
  check2 ("DR", "923", 1, "dir1/923");
  check2 ("FR", "923", 1, "dir2/923");
  check2 ("R", "923", 2, "dir1/923", "dir2/923");
  check2 ("FR", "hello", 1, "dir2/hello");
  check2 ("DR", "hello", 0);
  check2 ("R", "goodbye", 1, "dir2/goodbye");
  check2 ("FDR", "goodbye", 0);
  check2 ("FR", "subdir/ectory", 1, "dir2/subdir/ectory");
  check2 ("E", "923", 2, "dir1/923", "dir2/923");
  check2 ("E", "goodbye", 1, "dir2/goodbye");

  const char *expected;
#ifdef _MSC_VER
  expected = "C:\\some\\absolute\\path";
#else
  expected = "/absolute/path/should/inhibit/search";
#endif
  char *actual = ob_resolve_standard_path (ob_etc_path, expected, "R");
  if (0 != strcmp (actual, expected))
    error_exit ("Expected %s but got %s\n", expected, actual);
  free (actual);

  return EXIT_SUCCESS;
}

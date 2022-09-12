
/* (c)  oblong industries */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

/* So, the test MiscPoolTest.CloseOnExec wants to see what file descriptors
 * are being inherited across an exec.  So, to do that, we need to exec()
 * some other program, and have it report back to MiscPoolTest which file
 * descriptors are open from its point of view.  The command to do this
 * is almost just "ls /dev/fd/" (the trailing slash is important,
 * because /dev/fd is a symlink on Linux, and we want to follow the link).
 * But, the problem with that is that listing the contents of /dev/fd
 * inherently requires that you create a file descriptor.  And given the
 * way file descriptor numbers are reused, there's a good chance that it
 * will reuse one of the file descriptors that was closed on exec.  So,
 * from MiscPoolTest's point of view, it will look like that file descriptor
 * was inherited, even though it was actually closed and then reopened as
 * something completely unrelated.
 *
 * To avoid that problem, this program does pretty much exactly the same
 * thing that "ls /dev/fd/" does, except that it omits the one file
 * descriptor which it is actually using to obtain the listing with.
 */

int main (int argc, char **argv)
{
  struct dirent *e;
  DIR *d = opendir ("/dev/fd/");
  if (!d)
    {
      perror ("opendir");
      return EXIT_FAILURE;
    }

  const int dfd = dirfd (d);

  while (NULL != (errno = 0, e = readdir (d)))
    {
      const char *name = e->d_name;
      char *x = NULL;
      int n = strtol (name, &x, 10);
      if (*x) /* skip non-numbers; especially "." and ".." */
        continue;
      if (n != dfd)
        printf ("%s\n", name);
    }

  if (errno != 0)
    {
      perror ("readdir");
      closedir (d);
      return EXIT_FAILURE;
    }

  if (0 != closedir (d))
    {
      perror ("closedir");
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


/* (c)  oblong industries */

#include "libLoam/c/ob-attrs.h"
#include "libLoam/c/ob-dirs.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-sys.h"
#include "libLoam/c/ob-util.h"
#include "libLoam/c/ob-vers.h"
#include <stdlib.h>
#include <string.h>

static OB_NORETURN void usage (void)
{
  ob_banner (stderr);
  fprintf (
    stderr,
    "Usage: %s <dir-type>\n"
    "       %s [-a] <path-type> <search-spec> <filename>\n"
    "    -a print all matches, one per line (default: print only one)\n"
    "    <dir-type> is one of 'tmp', 'pools', or 'yobuild'\n"
    "    <path-type> is one of 'share', 'etc', or 'var'\n"
    "    <search-spec> may contain the following characters:\n"
    "      'd' directory specified by path exists\n"
    "      'r' directory specified by path is readable\n"
    "      'w' directory specified by path is writable\n"
    "      'c' directory specified by path can be created\n"
    "      'l' directory specified by path is on a local (non-NFS) filesystem\n"
    "      'F' filename exists as a regular file\n"
    "      'D' filename exists as a directory\n"
    "      'R' filename is readable\n"
    "      'W' filename is writable\n"
    "      \n"
    "      Concatenating multiple letters together is an \"and\", meaning\n"
    "      that a single component of the path must fulfill all the specified\n"
    "      conditions in order to match.  So, \"Rw\" would mean the file is \n"
    "      readable, and exists in a writable directory.\n"
    "      \n"
    "      The character '|' can be used to mean \"or\".  \"and\" binds more\n"
    "      tightly than \"or\".  So, \"RF|w\" means either the specified\n"
    "      filename is readable and is a regular file, or the directory in\n"
    "      the path is writable.\n"
    "      \n"
    "      The character ',' means \"start over and go through all the path\n"
    "      components again\".  '|' binds more tightly than ','.\n"
    "      So, \"RF,w,c\" means first check all the directories in the path\n"
    "      to see if any of them contain a readable, regular file named\n"
    "      <filename>.  Then, go through all the directories in the path and\n"
    "      see if any of them are writable. Then, go through all the\n"
    "      directories in the path and see if any of them can be created.\n"
    "    <filename> is the file to search for in the path\n",
    ob_get_prog_name (), ob_get_prog_name ());
  exit (EXIT_FAILURE);
}

static ob_retort callback (const char *file, va_list vargies)
{
  ob_retort next = va_arg (vargies, ob_retort);
  printf ("%s\n", file);
  return next;
}

// This is used for things which are single directories, not paths
static OB_NORETURN void show_dir (ob_standard_dir osd)
{
  const char *s = ob_get_standard_path (osd);
  if (s)
    printf ("%s\n", s);
  exit (EXIT_SUCCESS);
}

int main (int argc, char **argv)
{
  int c;
  bool all = false;

  while ((c = getopt (argc, argv, "a")) != -1)
    {
      switch (c)
        {
          case 'a':
            all = true;
            break;
          default:
            usage ();
        }
    }

  argc -= optind;
  argv += optind;

  if (argc < 1 || argc > 3)
    usage ();

  const char *path_type = argv[0];

  ob_standard_dir dir;
  if (0 == strcmp (path_type, "share"))
    dir = ob_share_path;
  else if (0 == strcmp (path_type, "etc"))
    dir = ob_etc_path;
  else if (0 == strcmp (path_type, "var"))
    dir = ob_var_path;
  else if (0 == strcmp (path_type, "tmp"))
    show_dir (ob_tmp_dir);
  else if (0 == strcmp (path_type, "pools"))
    show_dir (ob_pools_dir);
  else if (0 == strcmp (path_type, "yobuild"))
    show_dir (ob_yobuild_dir);
  else
    usage ();

  if (argc != 3)
    usage ();

  const char *search_spec = argv[1];
  const char *filename = argv[2];

  ob_retort next = (all ? OB_OK : OB_STOP);
  OB_DIE_ON_ERROR (
    ob_search_standard_path (dir, filename, search_spec, callback, next));
  return EXIT_SUCCESS;
}

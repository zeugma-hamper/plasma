
/* (c)  oblong industries */

#include "libLoam/c/ob-util.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-sys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Below we use atoll instead of atoi because "number" can be greater
// than 2^31-1 and in those cases, in 32-bit builds, atoi limits it to
// 2^31-1 whereas we want it to remain undistorted by the string to
// integer conversion and then have it wrap to a negative integer when
// it is assigned to "expected".  E.g., with atoi:
//
//     greenbean$ make check/test-userid.sh-single
//     PASS: ../../../bld/faux_install.sh
//     error: test-userid.c:52: For user nfsnobody, expected 2147483647 but got -2
//
// Because
//
//     greenbean$ id -u nfsnobody
//     4294967294
//
// Perhaps this also points to the need to convert our representation
// of uid from int to unsigned int, uint32, or uid_t, but that seems
// too invasive for me to do at the moment.
//
// An alternate approach would be to use strtoul (string to unsigned
// long) instead of atoll.  One interesting feature (bug?) of atoll is
// it would allow negative numbers to be used, as is done in some
// /etc/passwd files, e.g. -2 for user "nobody":
//
//     hyacinth:~ bdenckla$ grep -e -2 /etc/passwd
//     nobody:*:-2:-2:Unprivileged User:/var/empty:/usr/bin/false
//     _update_sharing:*:95:-2:Update Sharing:/var/empty:/usr/bin/false
//     _installer:*:96:-2:Installer:/var/empty:/usr/bin/false


// Below we use strsep instead of strtok to allow for empty password
// fields in /etc/passwd.  For example, the line created by "mock" for
// the user "mockbuild" looks like this:
//
//     mockbuild::501:104::/builddir:/bin/bash


// Both of the issues discussed above (big uids and empty passwords)
// raise the question in my mind: should we, in some cases, test
// tests?  In particular, it may be worth testing a test in cases like
// these that are exercised only rarely.  For frequent cases, the test
// "tests itself" in the sense that a test bug leading to a test
// failure will get noticed quickly.  This still leaves open the
// possibility of a test bug that causes a test to pass when it should
// have failed.
//
// Update: now a simple self-test has been added

void do_test (const char *what, ob_retort (*func) (const char *name, int *id),
              const char *file)
{
  FILE *f = fopen (file, "r");
  if (!f)
    error_exit ("%s: %s\n", file, strerror (errno));

  char buf[4096];
  while (fgets (buf, sizeof (buf), f))
    {
      char *name;
      char *number;
      if (buf[0] == '#')
        continue;
      char *mbuf = buf;
      name = strsep (&mbuf, ":");
      strsep (&mbuf, ":");  // skip password
      number = strsep (&mbuf, ":");
      if (!name || !number)
        error_exit ("failed to parse %s\n", file);
      int expected = atoll (number);
      int actual;
      ob_retort tort = func (name, &actual);
      if (tort < OB_OK)
        error_exit ("Looking up %s %s (which should be %d) "
                    "but failed because %s\n",
                    what, name, expected, ob_error_string (tort));
      if (actual != expected)
        error_exit ("For %s %s, expected %d but got %d\n", what, name, expected,
                    actual);
    }

  CHECK_POSIX_ERROR (fclose (f));
}

static char selftest_contents[] =
  // cat libLoam/c/tests/self-test.txt | runhaskell libLoam/c/tests/quote.hs
  "nobody:*:-2:-2:Unprivileged User:/var/empty:/usr/bin/false\n"
  "_update_sharing:*:95:-2:Update Sharing:/var/empty:/usr/bin/false\n"
  "_installer:*:96:-2:Installer:/var/empty:/usr/bin/false\n"
  "mockbuild::501:104::/builddir:/bin/bash\n"
  "root:x:0:0:root:/root:/bin/bash\n"
  "bin:x:1:1:bin:/bin:/sbin/nologin\n"
  "daemon:x:2:2:daemon:/sbin:/sbin/nologin\n"
  "adm:x:3:4:adm:/var/adm:/sbin/nologin\n"
  "lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin\n"
  "sync:x:5:0:sync:/sbin:/bin/sync\n"
  "shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown\n"
  "halt:x:7:0:halt:/sbin:/sbin/halt\n"
  "mail:x:8:12:mail:/var/spool/mail:/sbin/nologin\n"
  "news:x:9:13:news:/etc/news:\n"
  "uucp:x:10:14:uucp:/var/spool/uucp:/sbin/nologin\n"
  "operator:x:11:0:operator:/root:/sbin/nologin\n"
  "games:x:12:100:games:/usr/games:/sbin/nologin\n"
  "gopher:x:13:30:gopher:/var/gopher:/sbin/nologin\n"
  "ftp:x:14:50:FTP User:/var/ftp:/sbin/nologin\n"
  "nscd:x:28:28:NSCD Daemon:/:/sbin/nologin\n"
  "vcsa:x:69:69:virtual console memory owner:/dev:/sbin/nologin\n"
  "pcap:x:77:77::/var/arpwatch:/sbin/nologin\n"
  "rpc:x:32:32:Portmapper RPC user:/:/sbin/nologin\n"
  "mailnull:x:47:47::/var/spool/mqueue:/sbin/nologin\n"
  "smmsp:x:51:51::/var/spool/mqueue:/sbin/nologin\n"
  "rpcuser:x:29:29:RPC Service User:/var/lib/nfs:/sbin/nologin\n"
  "nfsnobody:x:4294967294:4294967294:Anonymous NFS "
  "User:/var/lib/nfs:/sbin/nologin\n"
  "sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin\n"
  "dbus:x:81:81:System message bus:/:/sbin/nologin\n"
  "avahi:x:70:70:Avahi daemon:/:/sbin/nologin\n"
  "haldaemon:x:68:68:HAL daemon:/:/sbin/nologin\n"
  "avahi-autoipd:x:100:101:avahi-autoipd:/var/lib/avahi-autoipd:/sbin/nologin\n"
  "apache:x:48:48:Apache:/var/www:/sbin/nologin\n"
  "ntp:x:38:38::/etc/ntp:/sbin/nologin\n"
  "hsqldb:x:96:96::/var/lib/hsqldb:/sbin/nologin\n"
  "xfs:x:43:43:X Font Server:/etc/X11/fs:/sbin/nologin\n"
  "gdm:x:42:42::/var/gdm:/sbin/nologin\n"
  "sabayon:x:86:86:Sabayon user:/home/sabayon:/sbin/nologin\n"
  "demo:x:500:500:Demo:/home/demo:/bin/bash\n"
  "bdenckla:x:501:501:bdenckla:/home/bdenckla:/bin/bash\n"
  "ppelletier:x:502:502:ppelletier:/home/ppelletier:/bin/bash\n"
  "jao:x:503:503:jao:/home/jao:/bin/bash\n"
  "navjot:x:504:504:navjot:/home/navjot:/bin/bash\n"
  "oprofile:x:16:16:Special user account to be used by "
  "OProfile:/home/oprofile:/sbin/nologin\n"
  "buildbot-slave:x:505:505::/home/buildbot-slave:/bin/bash\n";

static int64 selftest_userid (const char *s)
{
  // cat libLoam/c/tests/self-test.txt | runhaskell libLoam/c/tests/passwd.hs
  if (0 == strcmp (s, "nobody"))
    return OB_CONST_I64 (-2);
  if (0 == strcmp (s, "_update_sharing"))
    return OB_CONST_I64 (95);
  if (0 == strcmp (s, "_installer"))
    return OB_CONST_I64 (96);
  if (0 == strcmp (s, "mockbuild"))
    return OB_CONST_I64 (501);
  if (0 == strcmp (s, "root"))
    return OB_CONST_I64 (0);
  if (0 == strcmp (s, "bin"))
    return OB_CONST_I64 (1);
  if (0 == strcmp (s, "daemon"))
    return OB_CONST_I64 (2);
  if (0 == strcmp (s, "adm"))
    return OB_CONST_I64 (3);
  if (0 == strcmp (s, "lp"))
    return OB_CONST_I64 (4);
  if (0 == strcmp (s, "sync"))
    return OB_CONST_I64 (5);
  if (0 == strcmp (s, "shutdown"))
    return OB_CONST_I64 (6);
  if (0 == strcmp (s, "halt"))
    return OB_CONST_I64 (7);
  if (0 == strcmp (s, "mail"))
    return OB_CONST_I64 (8);
  if (0 == strcmp (s, "news"))
    return OB_CONST_I64 (9);
  if (0 == strcmp (s, "uucp"))
    return OB_CONST_I64 (10);
  if (0 == strcmp (s, "operator"))
    return OB_CONST_I64 (11);
  if (0 == strcmp (s, "games"))
    return OB_CONST_I64 (12);
  if (0 == strcmp (s, "gopher"))
    return OB_CONST_I64 (13);
  if (0 == strcmp (s, "ftp"))
    return OB_CONST_I64 (14);
  if (0 == strcmp (s, "nscd"))
    return OB_CONST_I64 (28);
  if (0 == strcmp (s, "vcsa"))
    return OB_CONST_I64 (69);
  if (0 == strcmp (s, "pcap"))
    return OB_CONST_I64 (77);
  if (0 == strcmp (s, "rpc"))
    return OB_CONST_I64 (32);
  if (0 == strcmp (s, "mailnull"))
    return OB_CONST_I64 (47);
  if (0 == strcmp (s, "smmsp"))
    return OB_CONST_I64 (51);
  if (0 == strcmp (s, "rpcuser"))
    return OB_CONST_I64 (29);
  if (0 == strcmp (s, "nfsnobody"))
    return OB_CONST_I64 (4294967294);
  if (0 == strcmp (s, "sshd"))
    return OB_CONST_I64 (74);
  if (0 == strcmp (s, "dbus"))
    return OB_CONST_I64 (81);
  if (0 == strcmp (s, "avahi"))
    return OB_CONST_I64 (70);
  if (0 == strcmp (s, "haldaemon"))
    return OB_CONST_I64 (68);
  if (0 == strcmp (s, "avahi-autoipd"))
    return OB_CONST_I64 (100);
  if (0 == strcmp (s, "apache"))
    return OB_CONST_I64 (48);
  if (0 == strcmp (s, "ntp"))
    return OB_CONST_I64 (38);
  if (0 == strcmp (s, "hsqldb"))
    return OB_CONST_I64 (96);
  if (0 == strcmp (s, "xfs"))
    return OB_CONST_I64 (43);
  if (0 == strcmp (s, "gdm"))
    return OB_CONST_I64 (42);
  if (0 == strcmp (s, "sabayon"))
    return OB_CONST_I64 (86);
  if (0 == strcmp (s, "demo"))
    return OB_CONST_I64 (500);
  if (0 == strcmp (s, "bdenckla"))
    return OB_CONST_I64 (501);
  if (0 == strcmp (s, "ppelletier"))
    return OB_CONST_I64 (502);
  if (0 == strcmp (s, "jao"))
    return OB_CONST_I64 (503);
  if (0 == strcmp (s, "navjot"))
    return OB_CONST_I64 (504);
  if (0 == strcmp (s, "oprofile"))
    return OB_CONST_I64 (16);
  if (0 == strcmp (s, "buildbot-slave"))
    return OB_CONST_I64 (505);
  return OB_CONST_I64 (0xbadbaddeadbeef);
}

static ob_retort selftest_check (const char *name, int *id)
{
  int64 i64 = selftest_userid (name);
  if (i64 == OB_CONST_I64 (0xbadbaddeadbeef))
    return OB_NOT_FOUND;
  *id = (int) i64;
  return OB_OK;
}

static void test_get_user_name (const char *expected)
{
  const char *got = ob_get_user_name ();
  if (!got || strcmp (got, expected))
    error_exit ("get_user_check: expected %s, got %s\n", expected, got);
}


int main (int argc, char **argv)
{
  if (argc != 4)
    {
      fprintf (stderr, "Usage: test-userid `id -un` `id -u` tempfile\n");
      return EXIT_FAILURE;
    }
  char *name = argv[1];
  char *number = argv[2];

  test_get_user_name (name);

  // ----- self-test of the test
  const char *selftest_filename = argv[3];
  FILE *f = fopen (selftest_filename, "w");
  if (!f)
    error_exit ("%s: %s\n", selftest_filename, strerror (errno));
  if (fputs (selftest_contents, f) < 0)
    error_exit ("fputs: %s\n", strerror (errno));
  CHECK_POSIX_ERROR (fclose (f));

  do_test ("selftest", selftest_check, selftest_filename);

// ----- actual test
#if !defined(_WIN32)

#ifndef __APPLE__
  // OS X uses Open Directory instead of /etc/passwd, so /etc/passwd
  // might be out of sync (and indeed it is on Ben's machine) so skip it
  do_test ("user", ob_uid_from_name, "/etc/passwd");
  do_test ("group", ob_gid_from_name, "/etc/group");
#endif
  // When using LDAP or Open Directory, the current user might not be in
  // /etc/passwd, so try looking up the current user in addition.
  int expected = atoll (number);
  int actual;
  OB_DIE_ON_ERROR (ob_uid_from_name (name, &actual));
  if (actual != expected)
    error_exit ("For user %s, expected %d but got %d\n", name, expected,
                actual);
  // Now exercise some error cases
  ob_retort err;
  if (OB_NOT_FOUND != (err = ob_uid_from_name ("nosuchuserihope", &actual)))
    error_exit ("Got %s instead of OB_NOT_FOUND for non-existent user\n",
                ob_error_string (err));
  if (OB_NOT_FOUND != (err = ob_gid_from_name ("nosuchgroupihope", &actual)))
    error_exit ("Got %s instead of OB_NOT_FOUND for non-existent group\n",
                ob_error_string (err));
#endif
  return EXIT_SUCCESS;
}

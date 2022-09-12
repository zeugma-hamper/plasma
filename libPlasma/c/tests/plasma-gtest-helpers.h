
/* (c)  oblong industries */

// Useful for Google Test-based tests of libPlasma/c

#ifndef PLASMA_GTEST_HELPERS_ASSISTANCE
#define PLASMA_GTEST_HELPERS_ASSISTANCE

#include "libLoam/c/ob-sys.h" // avoid death & dismemberment in winsock.h
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/pool_cmd.h"
#include "libPlasma/c/private/plasma-testing.h"
#include <set>
#include <ostream>
#include <gtest/gtest.h>

inline ::std::ostream&
operator << (::std::ostream &os, bslaw s)
{ slaw ss = slaw_spew_overview_to_string (s);
  os << slaw_string_emit (ss);
  slaw_free (ss);
  os << std::endl;
  return os;
}

static inline ::testing::AssertionResult
AssertTortsEq (const char *s_expected,
               const char *s_actual,
               ob_retort expected,
               ob_retort actual)
{ if (expected == actual)
    return ::testing::AssertionSuccess ();

  ::testing::Message msg;
  msg
    << "Value of: " << s_actual << std::endl
    << "  Actual: " << actual << " (" << ob_error_string (actual)
    << ")" << std::endl
    << "Expected: " << expected << " (" << ob_error_string (expected) << ")";
  return ::testing::AssertionFailure (msg);
}

#define EXPECT_TORTEQ(a, b) EXPECT_PRED_FORMAT2 (AssertTortsEq, a, b)
#define ASSERT_TORTEQ(a, b) ASSERT_PRED_FORMAT2 (AssertTortsEq, a, b)

/**
 * Like pool_cmd_open_pool(), but also name the hose after the
 * current test.
 */
#define POOL_CMD_OPEN_POOL(x)                                           \
  do {                                                                  \
    pool_cmd_open_pool (x);                                             \
    const ::testing::TestInfo* const test_info =                        \
      ::testing::UnitTest::GetInstance()->current_test_info();          \
    EXPECT_EQ (OB_OK, pool_set_hose_name ((x)->ph, test_info->name())); \
  } while (0)

#ifndef _MSC_VER                // CLOEXEC not relevant on Windows
/* Checks whether any of the file descriptors passed in (ignoring any
 * that are -1) are inherited across an exec.  This is tricky to test,
 * because we need to made sure the child process that we exec doesn't
 * open file descriptors of its own.  (Since it might be reusing the
 * numbers of file descriptors that were closed by the exec, which would
 * appear to us like they hadn't been closed, since we're only checking
 * the presence or absence of descriptors, not what they point to.)
 * Possible things that could be opening new file descriptors after the
 * exec:
 *
 * 1) The ob-log code, if OB_LOG var tells it to log to a file.
 * 2) Malloc logging on Mac
 * 3) Valgrind
 * 4) The file descriptor used to open /dev/fd
 *
 * We avoid (1) by making ob-lsfd a stand-alone C program that doesn't link
 * against any Oblong libraries.  We avoid (2) by unsetting the environment
 * variables that cause malloc logging.  Avoiding (3) is very difficult,
 * if not impossible, so we skip this test when running under valgrind.
 * And (4) is unavoidable, since we have to open a new file descriptor in
 * order to list the contents of /dev/fd, but we work around this by
 * making ob-lsfd exclude that file descriptor from the list it generates.
 * (This is why we need a custom program like ob-lsfd, instead of just
 * plain old "ls /dev/fd/".)
 */
static inline void refute_inheritance (const fd_and_description *fd, int nfd)
{ // Sadly, valgrind uses file descriptors that confuse this check
  if (ob_running_under_valgrind ())
    return;

  // Assumes the ob-lsfd program has been built and is in the parent directory.
  // Need to unset these environment variables because bld/test.pl sets them
  // up to do malloc testing on Mac OS X, but we can't have that in ob-lsfd,
  // because the malloc log file uses an additional file descriptor.
  FILE *p = popen ("unset MallocLogFile MallocPreScribble MallocScribble MallocErrorAbort ; ../ob-lsfd", "r");
  EXPECT_TRUE (p != NULL);
  char buf[80];
  std::set<int> actual;
  while (fgets (buf, sizeof (buf), p))
    actual.insert (atoi (buf));
  EXPECT_EQ (0, pclose (p));
  for (int i = 0  ;  i < nfd  ;  i++)
    if (fd[i].fd >= 0  &&  actual.end () != actual.find (fd[i].fd))
      ADD_FAILURE () << "inherited fd " << fd[i].fd
                     << ", which is " << fd[i].description;
}
#endif  // _MSC_VER

static inline void shrink_size_for_valgrind (pool_cmd_info *cmd)
{ if (ob_running_under_valgrind ())
    { pool_cmd_free_options (cmd);
      cmd->size >>= 4;
      pool_cmd_setup_options (cmd);
    }
}

#endif  /* PLASMA_GTEST_HELPERS_ASSISTANCE */

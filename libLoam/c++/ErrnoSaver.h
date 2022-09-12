
/* (c)  oblong industries */

#ifndef ERRNO_SAVER_LIFE
#define ERRNO_SAVER_LIFE

#include "libLoam/c++/Preterite.h"
#include <errno.h>

/**
 * An RAII class that saves the value of errno and restores it when
 * it goes out of scope.
 *
 * In particular, this is useful to instantiate as the very first
 * thing in a signal handler function, so that any changes to errno
 * that occur in the signal handler do not affect the code that was
 * interrupted.
 *
 * Forgive the absurdly long URL:
 * https://www.securecoding.cert.org/confluence/display/seccode/ERR32-C.+Do+not+rely+on+indeterminate+values+of+errno#ERR32-C.Donotrelyonindeterminatevaluesoferrno-NoncompliantCodeExample%28POSIX%29
 */
class ErrnoSaver
{
 OB_PRIVATE:
  int saved_errno;

 public:
  ErrnoSaver () { saved_errno = errno; }
  ~ErrnoSaver () { errno = saved_errno; }
};

#endif /* ERRNO_SAVER_LIFE */

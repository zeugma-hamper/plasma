
/* (c)  oblong industries */

#include "TimeKeeper.h"

#include <unistd.h>

#ifndef CLOCK_REALTIME
#include <sys/time.h>
#endif

namespace oblong {
namespace plasma {
namespace bench {

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
static int clock_gettime (int, timespec *ts)
{
  timeval tv;
  int r = gettimeofday (&tv, NULL);
  if (0 == r)
    {
      ts->tv_sec = tv.tv_sec;
      ts->tv_nsec = tv.tv_usec * 1000;
    }
  return r;
}
#endif  // CLOCK_REALTIME

TimeKeeper::TimeKeeper (unsigned int c) : running_ (false)
{
  if (c > 0)
    lapses_.reserve (c);
}

void TimeKeeper::LapseStart ()
{
  LapseEnd ();
  running_ = (0 == clock_gettime (CLOCK_REALTIME, &start_));
}

void TimeKeeper::LapseEnd ()
{
  if (running_)
    {
      timespec end;
      if (0 == clock_gettime (CLOCK_REALTIME, &end))
        {
          float64 lapse =
            1e9 * (end.tv_sec - start_.tv_sec) + end.tv_nsec - start_.tv_nsec;
          lapses_.push_back (lapse);
        }
      running_ = false;
    }
}
}
}
}  // namespace oblong::plasma::bench

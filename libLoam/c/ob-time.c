
/* (c)  oblong industries */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE  // necessary to get strptime() on Linux
#endif

#include "libLoam/c/ob-time.h"
#include "libLoam/c/ob-sys.h"
#include "libLoam/c/ob-log.h"

#include <time.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <mach/mach_time.h>
#endif

float64 ob_current_time (void)
{
  struct timeval now;
  gettimeofday (&now, NULL);
  return now.tv_sec + 0.000001 * now.tv_usec;
}

#ifdef _MSC_VER
ob_retort ob_micro_sleep (unt32 micro_seconds)
{
  // Round microseconds up to next millisecond
  Sleep ((micro_seconds + 999) / 1000);
  return OB_OK;  // Sleep() cannot fail on Windows
}
#else
ob_retort ob_micro_sleep (unt32 micro_seconds)
{
  int retval;
  // XXX: this was calling both sched_yield and usleep, but that seems
  // redundant; let's just call one or the other.
  if (micro_seconds == 0)
    retval = sched_yield ();
  else
    // XXX: usleep() is allowed to fail with EINVAL for durations greater
    // than one second, so maybe we should call nanosleep() instead?
    // Doesn't actually seem to be a problem in practice, though.
    retval = usleep (micro_seconds);

  if (retval < 0)
    return ob_errno_to_retort (errno);
  else
    return OB_OK;
}
#endif

void ob_format_time (char *buf, size_t bufsiz, const struct timeval *tv)
{
  time_t now;
  struct tm stm;
  now = tv->tv_sec;
  localtime_r (&now, &stm);
  strftime (buf, bufsiz,
            "%b " OB_STRFTIME_DAY_OF_MONTH_NO_LEADING_ZERO ", %Y %H:%M:%S",
            &stm);
  // collapse repeated whitespace, like "Dec  4" to "Dec 4"
  char *src, *dest;
  char prev = ' ';
  for (src = dest = buf; prev;)
    {
      char c = *src++;
      if (c != ' ' || prev != ' ')
        *dest++ = c;
      prev = c;
    }

  dest--;  // back up dest so it points to NUL terminator

  size_t used = dest - buf;

  if (used + 5 < bufsiz)
    {
      int hundredths = tv->tv_usec / 10000;
      snprintf (dest, bufsiz - used, ".%02d ", hundredths);
    }
}

void ob_format_time_f (char *buf, size_t bufsize, const float64 seconds)
{
  struct timeval tv;
  tv.tv_sec = floor (seconds);
  tv.tv_usec = (seconds - tv.tv_sec) * 1000000;
  ob_format_time (buf, bufsize, &tv);
}

ob_retort ob_strptime (const char *s, float64 *seconds)
{
  if (!s)
    return OB_INVALID_ARGUMENT;

  // since strptime (struct tm) does not support milliseconds, we must split
  // the string into datetime and milliseconds
  char *s_cpy = (char *) malloc (strlen (s) + 1);
  strcpy (s_cpy, s);
  char *delim;
  size_t d_len, s_len = strlen (s) + 1;
  for (d_len = 0, delim = s_cpy; d_len < s_len; d_len++, delim++)
    {
      if (delim[0] == '.')
        break;
    }

  if (d_len == 0)
    {
      free (s_cpy);
      return OB_INVALID_ARGUMENT;
    }

  char *datetime = (char *) malloc (d_len + 1);
  strncpy (datetime, s_cpy, d_len);
  float64 ms = atof (s_cpy + d_len);  // atof handily return 0.0 on error

  struct tm fill;
  time_t tv;
  strptime (datetime,
            "%b " OB_STRFTIME_DAY_OF_MONTH_NO_LEADING_ZERO ", %Y %H:%M:%S",
            &fill);
  fill.tm_isdst = -1; /* Not set by strptime.  Tells mktime() to check DST. */
  tv = mktime (&fill);
  free (s_cpy);
  free (datetime);

  if (tv == -1)
    return ob_errno_to_retort (errno);

  *seconds = (float64) tv + ms;

  return OB_OK;
}


#ifdef _MSC_VER
/*
 * The authoritative source for how to get good time info on windows is:
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx
 * It strongly recommends QueryPerformanceCounter(), and says it's completely
 * reliable, safe, and monotonic on Vista and higher, even on SMP systems;
 * all threads will be within one tick of each other.
 */

static void *qpc_critical;

static LARGE_INTEGER qpc_previous, qpf;
static unt64 nano_count;

static unt64 windows_nanos (void)
{
  CRITICAL_SECTION *crit = ob_fetch_critical (&qpc_critical);
  EnterCriticalSection (crit);
  if (qpf.QuadPart == 0)
    {
      if (!QueryPerformanceFrequency (&qpf)
          || !QueryPerformanceCounter (&qpc_previous))
        {
          ob_retort tort = ob_win32err_to_retort (GetLastError ());
          OB_FATAL_ERROR_CODE (0x10070002,
                               "QueryPerformanceCounter/QueryPerformanceFreq"
                               "uency\nfailed: %s\n",
                               ob_error_string (tort));
        }
      nano_count = 0; /* should be 0 anyway, of course... */
    }
  else
    {
      LARGE_INTEGER large;
      if (!QueryPerformanceCounter (&large))
        {
          ob_retort tort = ob_win32err_to_retort (GetLastError ());
          OB_FATAL_ERROR_CODE (0x10070003, "QueryPerformanceCounter failed:\n"
                                           "%s\n",
                               ob_error_string (tort));
        }
      unt64 diff = large.QuadPart - qpc_previous.QuadPart;
      qpc_previous = large;
      float64 floaty = diff;
      floaty /= qpf.QuadPart;
      floaty *= 1e9;
      unt64 nanodiff = (unt64) floaty;
      nano_count += nanodiff;
    }
  unt64 nanos = nano_count;
  LeaveCriticalSection (crit);
  return nanos;
}
#endif

unt64 ob_monotonic_time (void)
{
  unt64 nanos;
#ifdef _MSC_VER
  nanos = windows_nanos ();
#elif defined(__APPLE__)
  // Blech.  No clock_gettime() on Mac, despite its alleged UNIXiness.
  // http://www.wand.net.nz/~smr26/wordpress/2009/01/19/monotonic-time-in-mac-os-x/
  // http://developer.apple.com/qa/qa2004/qa1398.html
  // http://lists.apple.com/archives/perfoptimization-dev/2006/Jul/msg00024.html
  unt64 now = mach_absolute_time ();
  mach_timebase_info_data_t mtidt;
  kern_return_t kr = mach_timebase_info (&mtidt);
  if (kr != KERN_SUCCESS)
    // Treating this error so harshly because it shouldn't ever happen.
    OB_FATAL_ERROR_CODE (0x10070001,
                         "mach_timebase_info returned %" OB_FMT_64 "d\n",
                         (int64) kr);
  nanos = (now * mtidt.numer) / mtidt.denom;
#else
  struct timespec ts;
  if (clock_gettime (CLOCK_MONOTONIC, &ts) < 0)
    // Treating this error so harshly because it shouldn't ever happen.
    OB_FATAL_ERROR_CODE (0x10070000, "clock_gettime failed: %s\n",
                         strerror (errno));
  // If CLOCK_MONOTONIC is measured since the epoch, we'll have a
  // Y2554 problem, but that doesn't bother me too much, and most likely
  // CLOCK_MONOTONIC isn't measured since the epoch anyway.
  nanos = ts.tv_sec * OB_CONST_U64 (1000000000);
  nanos += ts.tv_nsec;
#endif
  return nanos;
}

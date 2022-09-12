
/* (c)  oblong industries */

#include <libLoam/c++/FatherTime.h>
#include "libLoam/c/ob-sys.h"


using namespace oblong::loam;

#ifdef __APPLE__  // see bug 4593
#define USE_MONOTONIC_TIME 0
#else
#define USE_MONOTONIC_TIME 1
#endif


#if USE_MONOTONIC_TIME
#define CURRENT_NANOSECONDS ob_monotonic_time
#else
static unt64 CURRENT_NANOSECONDS (void)
{
  // Representing time as 64 bits of nanoseconds since the epoch
  // will result in a Y2554 problem.  I'm not going to worry; I'll
  // be dead by then.
  struct timeval tv;
  gettimeofday (&tv, NULL);
  unt64 nanos = tv.tv_sec * OB_CONST_U64 (1000000000);
  nanos += tv.tv_usec * OB_CONST_U64 (1000);
  return nanos;
}
#endif



FatherTime::FatherTime ()
{
  ZeroTime ();
  time_is_paused = false;
  seconds_per_second = 1.0;
}


FatherTime::FatherTime (const FatherTime &otro)
    : things_past (otro.things_past),
      cur_time (otro.cur_time),
      time_is_paused (otro.time_is_paused),
      seconds_per_second (otro.seconds_per_second)
{
}


FatherTime &FatherTime::CopyFrom (const FatherTime &otro)
{
  time_is_paused = otro.time_is_paused;
  seconds_per_second = otro.seconds_per_second;
  things_past = otro.things_past;
  cur_time = otro.cur_time;
  return *this;
}



/**
 * Gets the current time, sets "things_past" to it,
 * and returns the difference between the current time and
 * the previous value of "things_past", multiplied by
 * seconds_per_second.
 *
 * Mmmmmm... delicious abstraction!  This avoids duplicating
 * the same code in four different methods.
 */
inline float64 FatherTime::_ComputeDelta ()
{
  unt64 now = CURRENT_NANOSECONDS ();
  float64 ret = seconds_per_second * 1e-9 * (now - things_past);
  things_past = now;
  return ret;
}

inline float64 FatherTime::_ComputeDeltaPeekingly () const
{
  unt64 now = CURRENT_NANOSECONDS ();
  float64 ret = seconds_per_second * 1e-9 * (now - things_past);
  return ret;
}


void FatherTime::SetSecondsPerSecond (float32 sps)
{
  if (!time_is_paused)
    cur_time += _ComputeDelta ();

  seconds_per_second = sps;
}


float32 FatherTime::SecondsPerSecond () const
{
  return seconds_per_second;
}


void FatherTime::ZeroTime ()
{
  cur_time = 0.0;
  things_past = CURRENT_NANOSECONDS ();
}



float64 FatherTime::SetTime (float64 t)
{
  cur_time = (t);
  things_past = CURRENT_NANOSECONDS ();
  return cur_time;
}

float64 FatherTime::AdjustTime (float64 t)
{
  cur_time += t;
  return CurTime ();
}


float64 FatherTime::CurTime ()
{
  if (time_is_paused)
    return cur_time;

  cur_time += _ComputeDelta ();
  return cur_time;
}

float64 FatherTime::CurTimePeek () const
{
  if (time_is_paused)
    return cur_time;

  return (cur_time + _ComputeDeltaPeekingly ());
}


float64 FatherTime::DeltaTime ()
{
  if (time_is_paused)
    return 0.0;

  float64 diffTime = _ComputeDelta ();
  cur_time += diffTime;
  return diffTime;
}

float64 FatherTime::DeltaTimePeek () const
{
  if (time_is_paused)
    return 0.0;

  return _ComputeDeltaPeekingly ();
}



bool FatherTime::IsTimePaused () const
{
  return time_is_paused;
}



void FatherTime::PauseTime ()
{
  if (time_is_paused)
    return;

  cur_time += _ComputeDelta ();
  time_is_paused = true;
}


void FatherTime::UnPauseTime ()
{
  if (time_is_paused)
    things_past = CURRENT_NANOSECONDS ();
  time_is_paused = false;
}


void FatherTime::TogglePauseTime ()
{
  if (time_is_paused)
    UnPauseTime ();
  else
    PauseTime ();
}

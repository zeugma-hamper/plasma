
/* (c)  oblong industries */

#ifndef FATHER_TIME_FUGIT
#define FATHER_TIME_FUGIT


#include <libLoam/c++/patella-macros.h>

#include <libLoam/c/ob-types.h>
#include <libLoam/c/ob-api.h>
#include <libLoam/c/ob-time.h>

#include <libLoam/c++/Preterite.h>


namespace oblong {
namespace loam {


/**
 * a stateful time object -- a simple kind of clock with a settable
 * hand and adjustable run-speed -- FatherTime instances provide
 * facilities for doing all kinds of useful and exciting stuff with
 * time.
 */
class OB_LOAMXX_API FatherTime
{
  PATELLA_CLASS (FatherTime);

 OB_PROTECTED:
  /**
   * \cond INTERNAL
   */
  unt64 things_past;
  float64 cur_time;
  bool time_is_paused;
  float32 seconds_per_second;
  /** \endcond */

 public:
  /**
   * the default constructor sets the current time to zero (0.0) and
   * gets time running at a forward rate of 1.0 seconds per
   * second. The returned instance is distinct from all other
   * instances.
   */
  FatherTime ();
  FatherTime (const FatherTime &otro);
  virtual ~FatherTime () {}

  void Delete () { delete this; }

  /**
   * slurps the guts out of another FatherTime, mimicking it
   * completely -- see any of the various Invasion of the Body
   * Snatchers movies/remakes.
   */
  FatherTime &CopyFrom (const FatherTime &otherFT);


  /**
   * a static method (no instance required! act now!): returns time in
   * its 'raw' form, viz. seconds since the grandly named 'epoch'
   * (0:0:0 UTC, 1 Jan 1970).
   */
  static float64 AbsoluteTime () { return ob_current_time (); }

  /**
   * allows the intrepid programmer to set the speed at which time
   * runs: an argument of 2.0 advances clock time at twice the forward
   * rate of real-world time; -0.2 runs the clock backward at one
   * second for every five seconds of real time. Providing a rate of
   * 0.0 is subtly different from calling PauseTime() [below], so just
   * do the latter if that's what you mostly mean.
   */
  void SetSecondsPerSecond (float32 sps);

  /**
   * returns the rate at which the clock's time runs (as a ratio of
   * clock time to real-world time).
   */
  float32 SecondsPerSecond () const;

  /**
   * make the 'current time' be zero (same as <b>SetTime (0.0);</b>)
   */
  void ZeroTime ();

  /**
   * set cur_time to \a t, so that an instantaneously following call
   * to CurTime() would return \a t.  Returns \a t.
   */
  float64 SetTime (float64 t);

  /**
   * add \a t seconds to the time, atomically. returns the new time.
   */
  float64 AdjustTime (float64 t);

  /**
   * returns the clock's current time. Subsequent calls to DeltaTime()
   * will be measured relative to the time this call was made...
   */
  float64 CurTime ();

  /**
   * returns the clock's current time. Has no effect on subsequent calls
   * to DeltaTime().
   */
  float64 CurTimePeek () const;

  /**
   * returns the interval of time (respecting the clock's run-rate and
   * pause state, natch) since the last call to SetTime(), ZeroTime(),
   * CurTime(), or DeltaTime() itself.
   */
  float64 DeltaTime ();

  /**
   * returns the interval of time (respecting the clock's run-rate and
   * pause state, natch) since the last call to SetTime(), ZeroTime(),
   * CurTime(), or DeltaTime(), but -- unlike this last -- does not
   * "re-start" the delta timer...
   */
  float64 DeltaTimePeek () const;

  /**
   * well? is it?
   */
  bool IsTimePaused () const;
  /**
   * pauses the onward (or backward, if your rate's set negative)
   * march of time.
   */
  void PauseTime ();
  /**
   * if the clock's time'd been paused, unpauses things, allowing 'em
   * to run free.
   */
  void UnPauseTime ();
  /**
   * pauses if running; runs if paused.
   */
  void TogglePauseTime ();

 OB_PRIVATE:
  float64 _ComputeDelta ();
  float64 _ComputeDeltaPeekingly () const;
};
}
}  // here lie the rotting carcasses of namespaces loam and oblong


#endif

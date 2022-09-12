
/* (c)  oblong industries */

#ifndef OBVIOUS_INFORMATION_FOR_OBVIOUS_REASONS
#define OBVIOUS_INFORMATION_FOR_OBVIOUS_REASONS

#include <libLoam/c/ob-coretypes.h>
#include <libLoam/c++/LoamForward.h>
#include <libLoam/c++/ObTrove.h>

namespace oblong {
namespace loam {


/**
 * C++ native interface to ob-vers.h and ob-info.h
 */
class OB_LOAMXX_API ObInfo
{
 public:
  /**
   * Things that one might need to find the version, for a loose definition of
   * version.
   *
   * These may be static and apply to the system that compiled g-speak or they
   * may be dynamic and be the system where the app is running.
   */
  enum class Version_Of
  {
    /** Version of g-speak platform (static) */
    Gspeak,
    /** Compiler name and version (static) */
    Compiler,
    /** Os name and version (dynamic) */
    Os,
    /** OS Kernal version (dynamic) */
    Kernel,
    /** Version of LIBC (???) */
    Libc,
    /** Version of the CPU being used (dynamic) */
    Cpu,
    /** Version of Yobuild used to build g-speak (static) */
    Yobuild,
    /** Hardware platform (dynamic) */
    Machine,
    /** g-speak ABI (static) */
    Abi,
    /** Build version (static) */
    Build,
  };

  /**
   * System things which are numbers you might need.
   */
  enum class Number_Of
  {
    Cpu_Mhz,
    Cpu_Cores,
    Ram,
    Bits_In_Word,
    Swap,
  };


  /**
   * Locations that g-speak knows about.
   */
  enum class Location_Of
  {
    Prefix,
    Share,
    Etc,
    Var,
    Tmp,
    Pools,
    Yobuild,
  };

  /**
   * Get information about the g-speak system.
   *
   * Get the information associated with \a what you're looking for, like a version, a
   * number, or a Trove of locations.
   */
  //@{
  static Str GetInfo (Version_Of what);
  static int64 GetInfo (Number_Of what);
  static ObTrove<Str> GetInfo (Location_Of what);
  //@}

  /**
   * Get a list of all supported feature flags on the current CPU.
   */
  static ObTrove<Str> GetCPUFeatureFlags ();

  /**
   * Return the YAML representation of system info.
   *
   * Return *all* system info as a YAML formatted string.  We use a subset of
   * YAML that is valid JSON, so this can also be used to export a JSON string.
   *
   * The information returned is the same as running `ob-version -y`
   */
  static Str ObInfoToYAML ();

 private:
  ObInfo ();  // Prevent instantiation, even if ENCAPSULATION_IS_BAD
};
}
}  // no loam no oblong

#endif /*  OBVIOUS_INFORMATION_FOR_OBVIOUS_REASONS */


/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_POOL_H
#define OBLONG_PLASMA_POOL_H


#include <stddef.h>  // for NULL

#include <libLoam/c/ob-api.h>

#include <libLoam/c++/LoamForward.h>
#include <libPlasma/c++/PlasmaForward.h>
#include <libLoam/c++/Str.h>

namespace oblong {
namespace plasma {
using namespace oblong::loam;


/**
 * Pool types are denoted as strings, to allow dynamically
 * loadable, user-defined types.  (In some theoretical sense,
 * although we have never done so, and "mmap" is still the
 * only pool type at present and in the foreseeable future.)
 * @ingroup PlasmaPools
 */
typedef Str PoolType;


/**
 * Utility class providing pool manipulation functions and Hose
 * factories. When any of these functions refers to a pool by name, it
 * uses the same conventions for pool naming as libPlasma/c.
 *
 * @ingroup PlasmaPools
 */
class OB_PLASMAXX_API Pool
{
 public:
  /* Configuration has always been defined internally, so we take care
     of those types here first */

  /**
   * Type descriptor denoting an mmaped pool.
   */
  static const PoolType MMAP;

  enum Creation_Policy
  {
    Do_Not_Create,
    Create_Auto_Disposable,
    Create_Persistent,
  };

  // 4 handy pre-defined pool sizes (which match pool_options.h)
  static const unt64 SIZE_SMALL;
  static const unt64 SIZE_MEDIUM;
  static const unt64 SIZE_LARGE;
  static const unt64 SIZE_HUGE;

  class Configuration
  {
   public:
    PoolType ptype;
    Creation_Policy cpolicy;
    unt64 size;
    union  // hack to give one field two names
    {
      unt64 indexCapacity;  // deprecated name; do not use!
      unt64 toc_capacity;   // this is the name you should use
    };
    // constructor simply sets defaults
    Configuration ()
    {
      ptype = MMAP;
      cpolicy = Do_Not_Create;
      size = SIZE_SMALL;
      toc_capacity = 0;
    }
    // for historical conversions
    static Configuration Historical (unt64 csize)
    {
      Configuration c;
      // override these two
      c.cpolicy = Create_Persistent;
      c.size = csize;
      return c;
    }
  };

  /**
   * Historical predefined pool configurations. Each configuration embodies a
   * pool type and a set of default configuration parameters (most
   * notably, the pool size).
   */
  static const Configuration MMAP_SMALL;
  static const Configuration MMAP_MEDIUM;
  static const Configuration MMAP_LARGE;
  static const Configuration MMAP_HUGE;

  /**
   * Participates in an existing pool. On success, a non-null hose is
   * returned. Otherwise, this function returns NULL and stores the
   * error code in the output parameter @a ret (provided it's not
   * null).
   */
  static Hose *Participate (const char *pool_name, ObRetort *ret = NULL);

  /**
   * Participates in a pool, creating it if needed. If the pool denoted
   * by @a name does not already exist, a new one is created using the
   * given configuration and creation policy. Note that if the pool did
   * already exist, the creation policy and configuration are effectively
   * ignored, and a hose to the pool is returned.
   *
   * A conf.cpolicy of Create_Auto_Disposable is mechanically different
   * and preferable to using Create with auto_dispose = true. By using
   * it here, the flag is set at the libPlasma level and the disposal
   * occurs at the moment the last hose is withdrawn, independent of
   * which process triggers the final withdrawl. If done in the Create
   * instead, then an explicit call to DisposeOfAutoDisposables () is
   * required, which in fact will fail if this or other processes
   * currently have connected hoses.
   *
   * (Setting the @a old_crufty_auto_dispose flag here is equivalent
   * to using Create with auto_dispose = true, and different than
   * Create_Auto_Disposable as described above.  Using
   * @a old_crufty_auto_dispose is highly discouraged and is only
   * supported for compatibility.)
   *
   * If the pool does not exist and could not be created, NULL is
   * returned. The result of the operation is stored in @a ret, if it's
   * not null.
   */
  static Hose *Participate (const char *name, Configuration conf,
                            ObRetort *ret = NULL,
                            bool old_crufty_auto_dispose = false);

  /**
   * Creates a new pool, provided it doesn't exist, with the given type
   * and options. The @a auto_dispose flag, when set to @c true, adds
   * the newly created pool to the list of pools disposed by
   * #DisposeOfAutoDisposables. Note that "auto" here doesn't imply any
   * automatic disposal: you still need to call
   * DisposeOfAutoDisposables explicitly for anything to happen to
   * pools created with @a auto_dispose set to @a true.
   *
   * It is preferable to use PoolParticipate with the creation policy
   * set to Create_Auto_Disposable if in fact you plan on connecting
   * a hose to this pool since that happens at the libPlasma level
   * and independent of which process is the last to withdraw.
   */
  static ObRetort Create (const char *name, PoolType type, bool auto_dispose,
                          Protein options);
  /**
   * Same as above, except that @a options is Slaw as regarded by C++
   * but a protein under the covers.
   */
  static ObRetort Create (const char *name, PoolType type, bool auto_dispose,
                          Slaw options);

  /**
   * Creates a new pool, using one of the default configurations. See
   * #DisposeOfAutoDisposables for information on the @a auto_dispose
   * flag.
   */
  static ObRetort Create (const char *name, Configuration conf,
                          bool auto_dispose = false);

  /**
   * Deletes the given pool from the server. As a result, any existing
   * hose connected to it will lose its connection.
   */
  static ObRetort Dispose (const char *name);

  /**
   * Disposes all pools created with the @c auto_dispose flag set to @c
   * true. The disposal is unconditional: no check is performed on
   * possible active hoses before disposing of the pools.
   *
   * This is only relevant for pools created by calling the above
   * Create () call with auto_dispose=true. The recommended pattern
   * is not to use this method but to instread call PoolParticipate
   * with the creation policy set to Create_Auto_Disposable. See
   * notes above. This method is kept for backwards compatibility
   * and for rare instances where you want to create an pool
   * that will be disposed *without* connecting a hose to it.
   */
  static void DisposeOfAutoDisposables ();

  /**
   * helper method for converting from a "Configuration" to a slaw map
   * suitable for create or participate
   */
  static Slaw OptionsSlawFromConfiguration (Configuration conf);

 private:
  Pool ();
  ~Pool ();
  Pool (const Pool &other);
  Pool &operator= (const Pool &other);
};
}
}  // namespace oblong::plasma


#endif  // OBLONG_PLASMA_POOL_H

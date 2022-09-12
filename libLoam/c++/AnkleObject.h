
/* (c)  oblong industries */

#ifndef ANKLE_OBJECT_JUST_OFF_THE_GROUND
#define ANKLE_OBJECT_JUST_OFF_THE_GROUND


#include <libLoam/c++/patella-macros.h>

#include <libLoam/c++/ObRef.h>
#include <libLoam/c++/ObWeakRef.h>

#include <libLoam/c/ob-api.h>


namespace oblong {
namespace loam {

/**
 * The base of our object hierarchy.  Knows how to be memory managed
 * (e. g. reference counted, deleted, etc.)  Due to the issues
 * discussed in (TODO: some higher-level documentation), AnkleObjects
 * cannot be deleted directly with the C++ delete operator, but
 * instead should be queued for future deletion by calling the
 * Delete() method.
 */
class OB_LOAMXX_API AnkleObject
{
  PATELLA_CLASS (AnkleObject);

 public:
  /**
   * \cond INTERNAL
   */
  GripMinder *grip_mnd;
  /** \endcond */

  AnkleObject () : grip_mnd (NULL) {}

  /**
   * in the destructor following we don't delete grip_mnd, because if
   * we've gotten here then by rights it's been whacked (and
   * generously set to NULL) by the refcounting-or-equivalent
   * mechanism -- see AnkleObject_ObRefGuts.h .
   */
  virtual ~AnkleObject () {}

  /**
   * Don't call me.  Call Delete() instead.
   */
  void operator delete (void *);

 public:
  /**
   * Deletes this object, possibly at some point in the future.
   * \note don't override this one unless you know exactly what you're doing
   * ... here we make the basic decision about whether this object is
   * no longer in use.
   */
  virtual void Delete ();

  /**
   * override CleanUp if your class needs to control when and how it
   * is deleted. Please do eventually call AnkleObject::CleanUp() to
   * perform final, c++-level object deletion.
   */
  virtual void CleanUp ();
};
}
}  // why hast thou forsaken us, o namespace loam, o namespace oblong?


#endif

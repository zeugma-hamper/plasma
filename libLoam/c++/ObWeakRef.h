
/* (c)  oblong industries */

#ifndef OB_WEAK_REF_HEADER_GUARD
#define OB_WEAK_REF_HEADER_GUARD


#include <libLoam/c++/ObRef.h>


namespace oblong {
namespace loam {

class AnkleObject;

template <typename T>
class ObWeakRef
{
 private:
  ObWeakRef ();
  ObWeakRef (T);
};


/**
 * ObWeakRef - implementation of weak references. Given an ObRef, you
 * can do the following:
 *
 *   ObWeakRef<Foo*> weak_ref (some_ob_ref_wrappin_a_foo);
 *   Foo *f = ~weak_ref;
 *   if (f)
 *     { ... }
 *
 * Please note that the mechanisms here are very much not threadsafe,
 * so don't use ObWeakRef to hold onto pointers across thread
 * boundaries.
 */
template <typename T>
class ObWeakRef<T *>
{
 public:
  /**
   * \cond INTERNAL
   */
  GripMinder::WeakGripMinder *weak_grip_mnd;
  T *pointee;
  /** \endcond */

  ObWeakRef (const ObRef<T *> &r) : pointee (~r)
  {
    if (pointee)
      weak_grip_mnd = r.AssociatedWeakGripMinder ();
    else
      weak_grip_mnd = NULL;
  }

  ~ObWeakRef ()
  {
    if (weak_grip_mnd)  // fix: can't do followng ... weakref might be shared
      weak_grip_mnd->Ungrip ();
  }

  void Delete () { delete this; }


  ObWeakRef (const ObWeakRef<T *> &wr)
      : weak_grip_mnd (wr.weak_grip_mnd), pointee (wr.pointee)
  {
    if (weak_grip_mnd)
      weak_grip_mnd->Grip ();
  }

  ObWeakRef (ObWeakRef &&wr) noexcept : weak_grip_mnd (NULL), pointee (NULL)
  {
    std::swap (weak_grip_mnd, wr.weak_grip_mnd);
    std::swap (pointee, wr.pointee);
  }

  ObWeakRef<T *> &operator= (const ObWeakRef<T *> &r)
  {
    if (weak_grip_mnd /* &&  weak_grip_mnd -> IsValid () */)
      weak_grip_mnd->Ungrip ();
    pointee = r.pointee;
    if ((weak_grip_mnd = r.weak_grip_mnd))
      weak_grip_mnd->Grip ();
    return *this;
  }

  template <typename SUBT, template <class> class R_WRP>
  ObWeakRef<T *> &operator= (const ObRef<SUBT, R_WRP> &r)
  {
    // if (weak_grip_mnd  &&  weak_grip_mnd -> IsValid ())
    if (weak_grip_mnd)
      weak_grip_mnd->Ungrip ();
    pointee = ~r;
    if (pointee)
      weak_grip_mnd = r.AssociatedWeakGripMinder ();
    else
      weak_grip_mnd = NULL;
    return *this;
  }

  template <typename SUBT>
  ObWeakRef<T *> &operator= (const ObWeakRef<SUBT> &r)
  {
    // if (weak_grip_mnd  &&  weak_grip_mnd -> IsValid ())
    if (weak_grip_mnd)
      weak_grip_mnd->Ungrip ();
    pointee = r.pointee;
    if ((weak_grip_mnd = r.weak_grip_mnd))
      weak_grip_mnd->Grip ();
    return *this;
  }

  ObWeakRef &operator= (ObWeakRef &&wr)
  {
    if (this == &wr)
      return *this;

    ObWeakRef tmp (std::move (*this));

    std::swap (weak_grip_mnd, wr.weak_grip_mnd);
    std::swap (pointee, wr.pointee);

    return *this;
  }


  template <typename SUBT, template <class> class R_WRP>
  bool operator== (const ObRef<SUBT, R_WRP> &r) const
  {
    return (this->pointee == ~r);
  }

  template <typename SUBT>
  bool operator== (const ObWeakRef<SUBT> &r) const
  {
    return (this->pointee == r.pointee);
  }

  /**
   * constructor for AnkleObject derivatives. Makes an ObRef
   * if necessary, and sets it up as a shell holding onto
   * a WeakRef only
   */
  template <typename U,
            typename = typename std::enable_if<std::is_base_of<AnkleObject,
                                                               U>::value>::type>
  ObWeakRef (U *ank_obj)
  {
    if (ank_obj)
      {
        if (!ank_obj->grip_mnd)
          {
            ObRef<AnkleObject *> temp_ref (ank_obj);
            temp_ref.MangleGutsToBecomeWeakRefShill ();
          }
        weak_grip_mnd = ank_obj->grip_mnd->FurnishWeakGripMinder ();
      }
    else
      weak_grip_mnd = NULL;
    pointee = ank_obj;
  }

  T *operator~ () const
  {
    if (!weak_grip_mnd || !weak_grip_mnd->IsValid ())
      return NULL;
    return pointee;
  }

  T *operator-> () const
  {
    if (!weak_grip_mnd || !weak_grip_mnd->IsValid ())
      return NULL;
    return pointee;
  }

  T *RawPointer () const
  {
    if (!weak_grip_mnd)
      return NULL;
    return pointee;
  }

  /**
   * A return value of true expresses that the object originally referenced
   * by this ObWeakRef has since been deleted by dint of all strong
   * references having gone out of scope; this means, equivalently,
   * that calls to 'operator ~' will return NULL.
   */
  bool HasBeenExpunged () const
  {
    if (weak_grip_mnd && !weak_grip_mnd->IsValid ())
      return true;
    return false;
  }

  ObWeakRef () noexcept : weak_grip_mnd (NULL), pointee (NULL) {}

  /**
   * XXX: Is this internal, or is the user meant to call it?
   */
  ObWeakRef<T *> &Nullify ()
  {
    if (weak_grip_mnd && weak_grip_mnd->IsValid ())
      weak_grip_mnd->Ungrip ();
    weak_grip_mnd = NULL;
    pointee = NULL;
    return *this;
  }
};



#if 0
// what's that you say? can't actually define this in the appropriate
// place? c++ makes you put it somewhere else? surely not...

template <typename T, template <class DUM> class MEM_MGR_TAG>
ObRef<T *, MEM_MGR_TAG>
  &ObRef<T *, MEM_MGR_TAG>::operator = (const ObWeakRef <T *> &wr)
{ if ((~(*this)) == ~wr)
    return *this;
//  some sort of 'Become()'... but what?
  return *this;
}
#endif
}
}  // end namespaces: first loam, then oblong...


#endif

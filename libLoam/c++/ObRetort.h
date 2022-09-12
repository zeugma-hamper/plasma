
/* (c)  oblong industries */

#ifndef OBLONG_LOAM_OBRETORT_H
#define OBLONG_LOAM_OBRETORT_H


#include <libLoam/c++/LoamForward.h>
#include <libLoam/c++/ObRetortPod.h>

#include <libLoam/c/ob-retorts.h>


namespace oblong {
namespace loam {


/**
 * Lightweight wrapper around the integer type used by the C-level
 * loam library to report status and errors (@c ob_retort).
 * Instances of this class are implicitly convertible to and from @c
 * ob_retort.
 *
 * Each code has an associated description, which can be returned
 * as a Str.  The ObRetort can optionally be augmented by an
 * ObRetortPod, but the details of that are left as an exercise
 * for the reader.
 */
class OB_LOAMXX_API ObRetort
{
 OB_PRIVATE:
  ob_retort c_retort;
  ObRef<ObRetortPod *> *ret_pod;

 public:
  /**
   * Constructs a success indicator (@c OB_OK).
   */
  ObRetort () : c_retort (OB_OK), ret_pod (NULL) {}
  /**
   * Implicit construction, to allow no-fuss handling.
   */
  ObRetort (ob_retort tort) : c_retort (tort), ret_pod (NULL) {}
  /**
   * handy one-line constructor for numerical code plus pod
   */
  ObRetort (ob_retort tort, ObRetortPod *orp)
      : c_retort (tort), ret_pod (orp ? new ObRef<ObRetortPod *> (orp) : NULL)
  {
  }
  /**
   * copy constructor, to deal with all the poo
   */
  ObRetort (const ObRetort &ander)
      : c_retort (ander.c_retort),
        ret_pod (ander.ret_pod ? new ObRef<ObRetortPod *> (~(*ander.ret_pod))
                               : NULL)
  {
  }
  /**
   * move constructor, to deal with c++11 poo
   */
  ObRetort (ObRetort &&ander) noexcept
      : c_retort (ander.c_retort), ret_pod (ander.ret_pod)
  {
    ander.ret_pod = NULL;
  }

  /**
   * Delete the bool ctor to avoid implicit, incorrect conversions
   */
  ObRetort (bool) = delete;

  /**
   * And, way up here in Olympus, the copy assignment thingy.
   */
  ObRetort &operator= (const ObRetort &otha)
  {
    if (this == &otha)
      return *this;

    c_retort = otha.c_retort;
    if (!ret_pod)
      {
        if (otha.ret_pod)
          ret_pod = new ObRef<ObRetortPod *> (~(*otha.ret_pod));
      }
    else
      *ret_pod = otha.ret_pod ? ~(*otha.ret_pod) : NULL;
    return *this;
  }

  /**
   * From the peak, Bjarne bestows the move assignment thingy.
   */
  ObRetort &operator= (ObRetort &&otha) noexcept
  {
    if (this == &otha)
      return *this;

    ObRetort tmp (std::move (*this));

    std::swap (c_retort, otha.c_retort);
    std::swap (ret_pod, otha.ret_pod);

    return *this;
  }

  ~ObRetort ()
  {
    if (ret_pod)
      delete ret_pod;
  }

  void Delete () { delete this; }

  /**
   * Gets back the numeric retort code associated with this retort.
   */
  //@{
  ob_retort Code () const { return c_retort; }
  ob_retort NumericRetort () const { return c_retort; }
  //@}

  /**
   * Returns a string which describes this retort.
   */
  Str Description () const;

  /**
   * Conventionally, codes represent errors when negative.
   */
  bool IsError () const { return (c_retort < 0); }

  /**
   * For convenience, the negation of IsError.
   */
  bool IsSplend () const { return !IsError (); }

  ObRetortPod *RetortPod () const { return (ret_pod ? (~(*ret_pod)) : NULL); }

  template <class RPOD_SUBT>
  RPOD_SUBT *RetortPod () const
  {
    return (ret_pod ? dynamic_cast<RPOD_SUBT *> (~(*ret_pod)) : NULL);
  }

  void SetRetortPod (ObRetortPod *orp)
  {
    if (!ret_pod)
      ret_pod = new ObRef<ObRetortPod *> ();
    (*ret_pod) = orp;
    if (orp)
      orp->SetNumericRetort (NumericRetort ());
  }

  ObRetort AccreteRetortPod (ObRetortPod *orp);


  ObRetortPod *FirstRetortPodOfClass (const char *cls) const
  {
    if (ObRetortPod *pod = RetortPod ())
      return pod->FirstRetortPodOfClass (cls);
    return NULL;
  }

  template <typename POD_TYPE>
  POD_TYPE *FirstRetortPodOfClass () const
  {
    ObRetortPod *p = RetortPod ();
    return (p ? p->FirstRetortPodOfClass<POD_TYPE> () : NULL);
  }

  ObRetort &operator+ (ObRetortPod *orp)
  {
    AccreteRetortPod (orp);
    return *this;
  }

  ObRetort &operator+= (ObRetortPod *orp)
  {
    AccreteRetortPod (orp);
    return *this;
  }

  /**
   * Utility to register new codes. Returns @c false (and doesn't
   * register the code) if @a code is already registered.  By
   * convention, a negative @c code denotes an error.
   */
  static bool Register (ob_retort code, const Str &desc);

  inline bool operator== (const ob_retort ret) const
  {
    return (c_retort == ret);
  }

  inline friend bool operator== (const ob_retort ret, const ObRetort &obr)
  {
    return (ret == obr.c_retort);
  }

  inline bool operator!= (const ob_retort ret) const
  {
    return (c_retort != ret);
  }

  inline friend bool operator!= (const ob_retort ret, const ObRetort &obr)
  {
    return (ret != obr.c_retort);
  }

  inline bool operator< (const ob_retort ret) const { return (c_retort < ret); }

  inline friend bool operator< (const ob_retort ret, const ObRetort &obr)
  {
    return (ret < obr.c_retort);
  }

  inline bool operator<= (const ob_retort ret) const
  {
    return (c_retort <= ret);
  }

  inline friend bool operator<= (const ob_retort ret, const ObRetort &obr)
  {
    return (ret <= obr.c_retort);
  }

  inline bool operator> (const ob_retort ret) const { return (c_retort > ret); }

  inline friend bool operator> (const ob_retort ret, const ObRetort &obr)
  {
    return (ret > obr.c_retort);
  }

  inline bool operator>= (const ob_retort ret) const
  {
    return (c_retort >= ret);
  }

  inline friend bool operator>= (const ob_retort ret, const ObRetort &obr)
  {
    return (ret >= obr.c_retort);
  }

  inline bool operator== (const ObRetort &other) const
  {
    return (c_retort == other.c_retort);
  }

  inline bool operator!= (const ObRetort &other) const
  {
    return (c_retort != other.c_retort);
  }

  inline bool operator< (const ObRetort &other) const
  {
    return (c_retort < other.c_retort);
  }

  inline bool operator<= (const ObRetort &other) const
  {
    return (c_retort <= other.c_retort);
  }

  inline bool operator> (const ObRetort &other) const
  {
    return (c_retort > other.c_retort);
  }

  inline bool operator>= (const ObRetort &other) const
  {
    return (c_retort >= other.c_retort);
  }

  /**
   * Returns a hash of the ObRetort.
   */
  unt64 Hash () const;
};


// http://www.boost.org/doc/libs/1_51_0/doc/html/hash/custom.html
inline size_t hash_value (const ObRetort &r)
{
  return static_cast<size_t> (r.Hash ());
}

/**
 * A hash function wrapper that makes ObRetort easy to use in Boost or TR1
 * unordered maps.
 */
struct ObRetort_hash
{
  std::size_t operator() (ObRetort const &key) const { return key.Hash (); };
};
}
}  // namespace oblong::loam

#include <functional>

namespace std {

template <>
struct hash<oblong::loam::ObRetort>
{
  typedef oblong::loam::ObRetort argument_type;
  typedef std::size_t result_type;

  result_type operator() (const argument_type &r) const
  {
    return static_cast<size_t> (r.Hash ());
  }
};
}

#endif  // OBLONG_LOAM_OBRETORT_H

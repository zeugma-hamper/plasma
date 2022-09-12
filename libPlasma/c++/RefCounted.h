
/* (c)  oblong industries */

#ifndef OBLONG_REFCOUNTED_H
#define OBLONG_REFCOUNTED_H


#include "libLoam/c/ob-atomic.h"

#include <algorithm>

namespace oblong {
namespace plasma {
namespace detail {

template <typename T>
class RCPtrTraits
{
 public:
  static T Default () { return NULL; }
  static void Destroy (T p) { delete p; }
};


/**
 * Reference counting template. @a Traits must declare public
 * @c Default() and @c Destroy(T) static members.
 */
template <typename T, typename Traits = detail::RCPtrTraits<T>>
class RefCounted
{
 public:
  ~RefCounted () { DecRef (); }

  RefCounted () : ref_ (NULL) {}
  RefCounted (T value) : ref_ (new Data (value)) {}
  RefCounted (const RefCounted &other) : ref_ (other.ref_) { IncRef (); }

  void Swap (RefCounted &other) { ::std::swap (ref_, other.ref_); }

  bool AtomicCompareAndSwap (const RefCounted &old, RefCounted &other)
  {
    Data **r = &ref_;
    if (ob_atomic_pointer_compare_and_swap ((void **) r, old.ref_, other.ref_))
      {
        other.ref_ = old.ref_;
        return true;
      }
    return false;
  }

  bool SwapIfNull (RefCounted &other)
  {
    Data **r = &ref_;
    if (ob_atomic_pointer_compare_and_swap ((void **) r, NULL, other.ref_))
      {
        other.ref_ = NULL;
        return true;
      }
    return false;
  }

  RefCounted &operator= (const RefCounted &other)
  {
    if (this != &other && this->ref_ != other.ref_)
      {
        RefCounted r (other);
        Swap (r);
      }
    return *this;
  }

  RefCounted &operator= (const T &value)
  {
    return operator= (RefCounted (value));
  }

  T Value () { return (ref_ ? ref_->object : Traits::Default ()); }

  const T Value () const { return (ref_ ? ref_->object : Traits::Default ()); }

  operator T () { return Value (); }

  operator const T () const { return Value (); }

  T operator-> () { return Value (); }

  const T operator-> () const { return Value (); }

  int32 Count () const { return (ref_ ? ref_->count : 0); }

 private:
  struct Data
  {
    T object;
    int32 count;
    Data (T o) : object (o), count (1) {}
  } * ref_;

  void IncRef ()
  {
    if (ref_)
      (void) (ob_atomic_int32_add (&ref_->count, 1));
  }

  void DecRef ()
  {
    if (ref_ && ob_atomic_int32_add (&ref_->count, -1) == 0)
      {
        Traits::Destroy (ref_->object);
        delete ref_;
        ref_ = NULL;
      }
  }
};
}
}
}  // namespace oblong::plasma::detail


#endif  // OBLONG_REFCOUNTED_H


/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_SLAWITERATOR_H
#define OBLONG_PLASMA_SLAWITERATOR_H


#include "Slaw.h"

#include <iterator>


namespace oblong {
namespace plasma {


/**
 * Random access iterator. Since Slawx are immutable, this type is a
 * const_iterator that, when de-referenced, yields Slaw values.
 *
 * You can use this iterator in any STL context that does not modify
 * the contents of the container Slaw. For instance, in typical loops:
 * @code
 *   Slaw s (get_a_slaw ());
 *   void do_something (Slaw s);
 *   for (SlawIterator i (s.begin ()), e (s.end ()); i != e; ++i)
 *     {
 *       do_something (*i);
 *       do_something (i->Nth (2));
 *     }
 * @endcode
 * or, more interestingly, use standard algorithms:
 * @code
 *   Slaw s = Slaw::List (1, 2, 2, 3, 2);
 *   size_t cnt = ::std::count (s.begin (), s.end (), Slaw (2));
 *   assert (3 == cnt);
 * @endcode
 *
 * @ingroup PlasmaSlaw
 */
class OB_PLASMAXX_API SlawIterator
{
 public:
  class pointer
  {
   public:
    explicit pointer (const Slaw &s) : slaw_ (s) {}
    pointer (Slaw *s) : slaw_ (*s) {}
    const Slaw *operator-> () const { return &slaw_; }
    const Slaw &operator* () const { return slaw_; }
   private:
    Slaw slaw_;
  };

  typedef ptrdiff_t difference_type;
  typedef Slaw value_type;
  typedef Slaw reference;
  typedef ::std::random_access_iterator_tag iterator_category;

 public:
  SlawIterator ();
  explicit SlawIterator (const Slaw &s);
  SlawIterator (const SlawIterator &other);

#ifndef _WIN32
  /* Since we have a copy constructor, c++11 says we also need these,
   * see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58407
   * Visual C++ barfs on this, though.
   */
  SlawIterator(SlawIterator &&) = default;
  SlawIterator &operator= (const SlawIterator &) = default;
  SlawIterator &operator= (SlawIterator &) = default;
#endif

  reference operator* () const;
  pointer operator-> () const;
  SlawIterator &operator++ ();
  SlawIterator operator++ (int);
  SlawIterator &operator-- ();
  SlawIterator operator-- (int);
  SlawIterator operator+ (difference_type n) const;
  SlawIterator &operator+= (difference_type n);
  SlawIterator operator- (difference_type n) const;
  SlawIterator &operator-= (difference_type n);
  difference_type operator- (const SlawIterator &other) const;
  reference operator[] (difference_type n) const;
  bool operator== (const SlawIterator &o) const;
  bool operator!= (const SlawIterator &o) const;
  bool operator< (const SlawIterator &o) const;
  bool operator<= (const SlawIterator &o) const;
  bool operator> (const SlawIterator &o) const;
  bool operator>= (const SlawIterator &o) const;

  void Spew (::std::ostream &os) const;

 private:
  SlawIterator (const Slaw &s, unt64 idx);
  bool IsEnd () const;
  void SetIndex (unt64);

  Slaw slaw_;
  unt64 idx_;
};
}
}  // namespace oblong::plasma


#endif  // OBLONG_PLASMA_SLAWITERATOR_H

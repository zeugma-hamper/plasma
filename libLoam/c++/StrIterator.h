
/* (c)  oblong industries */

#ifndef BASEMENT_STR_ITERATOR_H
#define BASEMENT_STR_ITERATOR_H


#include <libLoam/c/ob-types.h>
#include <libLoam/c/ob-api.h>

#include <libLoam/c++/Preterite.h>

#include <unicode/uversion.h>


namespace oblong {
namespace loam {


class Str;
typedef int64 str_marker;  // duplicate of Str.h's str_marker typedef


class OB_LOAMXX_API StrIterator
{
 OB_PRIVATE:
  const Str *strp;
  str_marker marker;

 public:
  StrIterator (const Str *str, str_marker marker_);

  StrIterator ();
  StrIterator (const StrIterator &other);
  StrIterator &operator= (const StrIterator &other);

  void Delete () { delete this; }


  UChar32 operator* () const;

  StrIterator &operator++ ();
  StrIterator operator++ (int);
  StrIterator &operator-- ();
  StrIterator operator-- (int);

  bool operator== (const StrIterator &other);
  bool operator!= (const StrIterator &other) { return !(*this == other); }
  bool operator< (const StrIterator &other);
  bool operator<= (const StrIterator &other) const;
  bool operator> (const StrIterator &other) const;
  bool operator>= (const StrIterator &other) const;
};
}
}  // good night, sweet namespaces oblong and loam


#endif

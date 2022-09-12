
/* (c)  oblong industries */

#include <libLoam/c++/StrIterator.h>
#include <libLoam/c++/Str.h>
#include <libLoam/c++/LoamStreams.h>

#include <ostream>


using namespace oblong::loam;


StrIterator::StrIterator (const Str *str, str_marker marker_)
{
  strp = str;
  marker = marker_;
}

StrIterator::StrIterator ()
{
  strp = NULL;
  marker = 0;
}

StrIterator::StrIterator (const StrIterator &other)
{
  strp = other.strp;
  marker = other.marker;
}

StrIterator &StrIterator::operator= (const StrIterator &other)
{
  strp = other.strp;
  marker = other.marker;
  return *this;
}


UChar32 StrIterator::operator* () const
{
  if (!strp)
    return -1;
  return strp->Fetch (marker);
}

StrIterator &StrIterator::operator++ ()
{
  if (strp)
    marker = strp->MoveMarker (marker, 1);
  return *this;
}

StrIterator StrIterator::operator++ (int)
{
  StrIterator ret (*this);
  if (strp)
    marker = strp->MoveMarker (marker, 1);
  return ret;
}

StrIterator &StrIterator::operator-- ()
{
  if (strp)
    marker = strp->MoveMarker (marker, -1);
  return *this;
}

StrIterator StrIterator::operator-- (int)
{
  StrIterator ret (*this);
  if (strp)
    marker = strp->MoveMarker (marker, -1);
  return ret;
}

bool StrIterator::operator== (const StrIterator &other)
{
  return (marker == other.marker);
}

bool StrIterator::operator< (const StrIterator &other)
{
  return (marker < other.marker);
}

bool StrIterator::operator<= (const StrIterator &other) const
{
  return (marker <= other.marker);
}

bool StrIterator::operator> (const StrIterator &other) const
{
  return (marker > other.marker);
}

bool StrIterator::operator>= (const StrIterator &other) const
{
  return (marker >= other.marker);
}

namespace oblong {
namespace loam {

::std::ostream &operator<< (::std::ostream &os,
                            const oblong::loam::StrIterator &)
{
  return os << "<StrIterator>";
}
}
}  // end oblong::loam

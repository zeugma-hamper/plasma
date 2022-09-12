
/* (c)  oblong industries */

#include "libLoam/c/ob-attrs.h"
#include "Callbacks.h"
#include "libPlasma/c++/SlawIterator.h"
#include "libPlasma/c++/PlasmaStreams.h"

#include <gtest/gtest.h>
#include <vector>

namespace {

template <typename T>
inline bool equalp (const T *lhm, const T *rhm)
{
  // shallow equality test -- tests checking for full breadth needed for arrays.
  return lhm == rhm || (lhm != NULL && rhm != NULL && *lhm == *rhm);
}

template <typename T>
inline bool equalp (T lhm, T rhm)
{
  return lhm == rhm;
}

template <>
inline bool equalp (const char *lhm, const char *rhm)
{
  return oblong::loam::Str (rhm) == oblong::loam::Str (lhm);
}

template <>
inline OB_UNUSED bool equalp (slaw lhm, slaw rhm)
{
  return (lhm == rhm) || slawx_equal (rhm, lhm);
}

template <>
inline OB_UNUSED bool equalp (bslaw lhm, bslaw rhm)
{
  return (lhm == rhm) || slawx_equal (rhm, lhm);
}

template <typename T>
Slaw fill_list (const T *values, size_t N)
{
  std::vector<slaw> slawx;
  for (size_t i = 0; i < N; ++i)
    slawx.push_back (make_slaw<T> (values[i]));
  return Slaw::ListCollect (slawx.begin (), slawx.end ());
}

template <typename T>
void check_elements (T *values, size_t n, const Slaw &l)
{
  for (size_t i = 0; i < n; ++i)
    {
      EXPECT_TRUE (equalp (l[i].Emit<T> (), values[i])) << l[i];
      Slaw snth = l.Nth (i);
      EXPECT_TRUE (equalp (snth.Emit<T> (), values[i])) << snth;
      EXPECT_EQ (Slaw (values[i]), l.Nth (l.IndexOf (values[i])));
    }
}

}  // namespace

template <typename T, size_t N>
CallbacksImpl<T, N>::CallbacksImpl (T (&in_values)[N]) : values (in_values)
{
}

template <typename T, size_t N>
size_t CallbacksImpl<T, N>::Count () const
{
  return N;
}

template <typename T, size_t N>
Slaw CallbacksImpl<T, N>::SlawFromNthValue (int64 n) const
{
  return Slaw (values[n]);
}

template <typename T, size_t N>
int64 CallbacksImpl<T, N>::IndexOfNthValue (const Slaw &lst, int64 n) const
{
  return lst.IndexOf (values[n]);
}

template <typename T, size_t N>
Slaw CallbacksImpl<T, N>::ConsFromValues (int64 i, int64 j) const
{
  return Slaw::Cons (values[i], values[j]);
}

template <typename T, size_t N>
Slaw CallbacksImpl<T, N>::ConsWithNil (int64 i) const
{
  return Slaw::Cons (values[i], Slaw::Nil ());
}

template <typename T, size_t N>
Slaw CallbacksImpl<T, N>::MapWithValues (const char *key) const
{
  Slaw m;
  for (size_t i = 0; i < N; ++i)
    m = m.MapPut (key, values[i]);
  return m;
}

template <typename T, size_t N>
Slaw CallbacksImpl<T, N>::MapWithKeys (const char *value) const
{
  Slaw m;
  for (size_t i = 0; i < N; ++i)
    m = m.MapPut (values[i], value);
  return m;
}

template <typename T, size_t N>
bool CallbacksImpl<T, N>::CheckNthValue (const Slaw &s, int64 i) const
{
  return equalp (s.Emit<T> (), values[i]);
}

template <typename T, size_t N>
bool CallbacksImpl<T, N>::CheckNthValue (bslaw s, int64 i) const
{
  return equalp (slaw_cast<T> (s), values[i]);
}

template <typename T, size_t N>
Slaw CallbacksImpl<T, N>::FillList (int64 start, int64 len) const
{
  return fill_list (values + start, len);
}

template <typename T, size_t N>
Slaw CallbacksImpl<T, N>::MakeSlawList (int64 i, int64 j, int64 k) const
{
  return Slaw::List (values[i], values[j], values[k]);
}

template <typename T, size_t N>
void CallbacksImpl<T, N>::CheckElements (const Slaw &lst) const
{
  check_elements (values, N, lst);
}

template <typename T, size_t N>
void CallbacksImpl<T, N>::CheckElements (const Slaw &lst, size_t n) const
{
  check_elements (values, n, lst);
}

template <typename T, size_t N>
slaw CallbacksImpl<T, N>::CSlawFromNthValue (int64 n) const
{
  return make_slaw<T> (values[n]);
}

template <typename T, size_t N>
bool CallbacksImpl<T, N>::HasType (const Slaw &s) const
{
  return s.Is<T> ();
}

template <typename T, size_t N>
bool CallbacksImpl<T, N>::CanEmit (const Slaw &s) const
{
  return s.CanEmit<T> ();
}

template class CallbacksImpl<oblong::loam::Str, 5ul>;
template class CallbacksImpl<oblong::loam::Vect, 3ul>;
template class CallbacksImpl<oblong::loam::Vect4, 3ul>;
template class CallbacksImpl<oblong::loam::Quat, 3ul>;
template class CallbacksImpl<bool, 2ul>;
template class CallbacksImpl<char const *, 4ul>;
template class CallbacksImpl<double, 5ul>;
template class CallbacksImpl<float, 5ul>;
template class CallbacksImpl<int, 5ul>;
#ifndef OB_INT64_IS_LONG
template class CallbacksImpl<long long, 5ul>;
#endif
template class CallbacksImpl<long, 5ul>;
template class CallbacksImpl<short, 5ul>;
template class CallbacksImpl<signed char, 5ul>;
template class CallbacksImpl<unsigned char, 5ul>;
template class CallbacksImpl<unsigned int, 5ul>;
#ifndef OB_INT64_IS_LONG
template class CallbacksImpl<unsigned long long, 5ul>;
#endif
template class CallbacksImpl<unsigned long, 5ul>;
template class CallbacksImpl<unsigned short, 5ul>;
template class CallbacksImpl<v2float32, 2ul>;
template class CallbacksImpl<v2float64, 2ul>;
template class CallbacksImpl<v2int16, 2ul>;
template class CallbacksImpl<v2int32, 2ul>;
template class CallbacksImpl<v2int64, 2ul>;
template class CallbacksImpl<v2int8, 2ul>;
template class CallbacksImpl<v2unt16, 2ul>;
template class CallbacksImpl<v2unt32, 2ul>;
template class CallbacksImpl<v2unt64, 2ul>;
template class CallbacksImpl<v2unt8, 2ul>;
template class CallbacksImpl<v3float32, 2ul>;
template class CallbacksImpl<v3float64, 2ul>;
template class CallbacksImpl<v3int16, 2ul>;
template class CallbacksImpl<v3int32, 2ul>;
template class CallbacksImpl<v3int64, 2ul>;
template class CallbacksImpl<v3int8, 2ul>;
template class CallbacksImpl<v3unt16, 2ul>;
template class CallbacksImpl<v3unt32, 2ul>;
template class CallbacksImpl<v3unt64, 2ul>;
template class CallbacksImpl<v3unt8, 2ul>;
template class CallbacksImpl<v4float32, 2ul>;
template class CallbacksImpl<v4float64, 2ul>;
template class CallbacksImpl<v4int16, 2ul>;
template class CallbacksImpl<v4int32, 2ul>;
template class CallbacksImpl<v4int64, 2ul>;
template class CallbacksImpl<v4int8, 2ul>;
template class CallbacksImpl<v4unt16, 2ul>;
template class CallbacksImpl<v4unt32, 2ul>;
template class CallbacksImpl<v4unt64, 2ul>;
template class CallbacksImpl<v4unt8, 2ul>;

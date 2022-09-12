
/* (c)  oblong industries */

#ifndef OB_HASHER_HASH
#define OB_HASHER_HASH


#include <libLoam/c/ob-coretypes.h>
#include <libLoam/c/ob-types.h>
#include <libLoam/c/ob-hash.h>
#include <libLoam/c++/ObRef.h>
#include <libLoam/c++/ObWeakRef.h>
#include <libLoam/c++/ObTrove.h>
#include <libLoam/c++/ObMap.h>
#include <libLoam/c++/ObAcacia.h>
#include <libLoam/c++/ob-coretypes-hash.h>
#include <functional>


namespace oblong {
namespace loam {

/**
 * In general, we've adopted the boost convention of calling hash_value
 * (const T &) and counting on argument dependent look up to find the
 * right function for us.  On the other hand, boost returns the same
 * integral value it's passed (cast to std::size_t of course), so for
 * those values and floats and double and pointers, we'll instead use a
 * hash function that mixes the bits around some.
 *
 * Types provided for:
 * bool
 * char
 * int8
 * unt8
 * int16
 * unt16
 * int32
 * unt32
 * int64
 * unt64
 * float32
 * float64
 * long double
 * wchar_t
 * T *
 * ObRef <T*>
 * ObWeakRef <T*>
 * ObTrove \<T\>
 * ObAcacia \<T\>
 * ObMap <K, V>
 *
 * Types provided for by ADL:
 * Slaw
 * Protein
 * ObRetort
 * Str
 * complex and vector and multivector types from ob-types.h
 */

template <typename T>
struct ObHasher
{
  inline std::size_t operator() (const T &thing) const
  {
    return hash_value (thing);
  }
};

#define OB_HASHER_32_BIT_OBTYPE(type)                                          \
  template <>                                                                  \
  struct ObHasher<type>                                                        \
  {                                                                            \
    inline std::size_t operator() (type const &thing) const                    \
    {                                                                          \
      return ob_hash_unt32_to_size_t ((unt32) thing);                          \
    }                                                                          \
  }

#define OB_HASHER_64_BIT_OBTYPE(type)                                          \
  template <>                                                                  \
  struct ObHasher<type>                                                        \
  {                                                                            \
    inline std::size_t operator() (type const &thing) const                    \
    {                                                                          \
      return ob_hash_unt64_to_size_t ((unt64) thing);                          \
    }                                                                          \
  }

#define OB_HASHER_OBTYPE(type)                                                 \
  template <>                                                                  \
  struct ObHasher<type>                                                        \
  {                                                                            \
    inline std::size_t operator() (type const &thing) const                    \
    {                                                                          \
      return (                                                                 \
        std::size_t) ob_city_hash64 (reinterpret_cast<const void *> (&thing),  \
                                     sizeof (type));                           \
    }                                                                          \
  }


template <typename T>
struct ObHasher<T *>
{
  inline std::size_t operator() (const T *thing) const
  {
    return ob_hash_size_t (reinterpret_cast<std::size_t> (thing));
  }
};


template <typename T, template <class> class MEM_MGR_TAG,
          typename GUTS_OVERRIDE_T>
struct ObHasher<oblong::loam::ObRef<T *, MEM_MGR_TAG, GUTS_OVERRIDE_T>>
{
  inline std::size_t operator() (
    oblong::loam::ObRef<T *, MEM_MGR_TAG, GUTS_OVERRIDE_T> const &thing) const
  {
    return ob_hash_size_t (reinterpret_cast<std::size_t> (~thing));
  }
};


template <typename T>
struct ObHasher<oblong::loam::ObWeakRef<T *>>
{
  inline std::size_t
  operator() (oblong::loam::ObWeakRef<T *> const &thing) const
  {
    return ob_hash_size_t (reinterpret_cast<std::size_t> (~thing));
  }
};


template <typename E, template <typename D> class M>
struct ObHasher<oblong::loam::ObTrove<E, M>>
{
  ObHasher<E> h;
  inline std::size_t operator() (const oblong::loam::ObTrove<E, M> &tr) const
  {
    const int64 count = tr.Count ();
    if (tr.Count () == 0)
      return 0;

    std::size_t hash = 0;
    hash = h (tr.Nth (0));
    for (int64 i = 1; i < count; ++i)
      hash = ob_hash_2xsize_t_to_size_t (hash, h (tr.Nth (i)));

    return hash;
  }
};


template <typename K, typename V, template <typename D1> class K_M,
          template <typename D2> class V_M>
struct ObHasher<oblong::loam::ObMap<K, V, K_M, V_M>>
{
  typedef oblong::loam::ObMap<K, V, K_M, V_M> Mappy;
  typedef typename Mappy::MapCons MappyCons;

  ObHasher<K> key_hasher;
  ObHasher<V> val_hasher;
  inline std::size_t operator() (const Mappy &map) const
  {
    const int64 count = map.Count ();
    if (count == 0)
      return 0;

    std::size_t hash = 0;
    MappyCons *mc = map.NthCons (0);
    if (mc)
      {
        hash = key_hasher (mc->Car ());
        hash = ob_hash_2xsize_t_to_size_t (hash, val_hasher (mc->Cdr ()));
      }
    for (int64 i = 1; i < count; ++i)
      {
        mc = map.NthCons (i);
        if (mc)
          {
            hash = ob_hash_2xsize_t_to_size_t (hash, key_hasher (mc->Car ()));
            hash = ob_hash_2xsize_t_to_size_t (hash, val_hasher (mc->Cdr ()));
          }
      }

    return hash;
  }
};


template <typename T, template <typename D> class M>
struct ObHasher<oblong::loam::ObAcacia<T, M>>
{
  ObHasher<T> h;
  inline std::size_t operator() (const oblong::loam::ObAcacia<T, M> &ac) const
  {
    oblong::loam::ObCrawl<T> cr = ac.Crawl ();
    if (cr.isempty ())
      return 0;

    std::size_t hash = 0;
    hash = h (cr.popfore ());

    while (!cr.isempty ())
      hash = ob_hash_2xsize_t_to_size_t (hash, h (cr.popfore ()));

    return hash;
  }
};


/* no real c++ objects in here, ok? */
namespace detail {
template <typename RetType, typename ArgType>
RetType union_cast (ArgType const &t)
{
  union cast_t
  {
    ArgType arg;
    RetType ret;
  } uc;
  uc.arg = t;
  return uc.ret;
}
}

template <>
struct ObHasher<float32>
{
  inline std::size_t operator() (float32 const &thing) const
  {
    return ob_hash_unt32_to_size_t (detail::union_cast<unt32> (thing));
  }
};

template <>
struct ObHasher<float64>
{
  inline std::size_t operator() (float64 const &thing) const
  {
    return ob_hash_unt64_to_size_t (detail::union_cast<unt64> (thing));
  }
};


// built-in types
OB_HASHER_32_BIT_OBTYPE (bool);
OB_HASHER_32_BIT_OBTYPE (char);
// OB_HASHER_OBTYPE (unsigned char); //the commented types are defined below
// OB_HASHER_OBTYPE (short);
// OB_HASHER_OBTYPE (unsigned short);
// OB_HASHER_OBTYPE (int);
// OB_HASHER_OBTYPE (unsigned int);
// OB_HASHER_OBTYPE (long);
// OB_HASHER_OBTYPE (unsigned long);
// OB_HASHER_OBTYPE (long long);
// OB_HASHER_OBTYPE (unsigned long long);
// OB_HASHER_OBTYPE (float);
// OB_HASHER_OBTYPE (double);

/* long double is an odd fellow.  sizeof (long double) is 16 on my
   64-bit machine using gcc 4.4.  the wikipedia suggests that the
   precision of long doubles on 64-bit machines is actually 80 bits.
   what's the proper way to hash this if I don't know how many and which
   of the bits don't actually count towards the value of the long
   double.
*/
OB_HASHER_OBTYPE (long double);
OB_HASHER_32_BIT_OBTYPE (wchar_t);
// OB_HASHER_OBTYPE (size_t);
// OB_HASHER_OBTYPE (ssize_t);

// core types
OB_HASHER_32_BIT_OBTYPE (int8);
OB_HASHER_32_BIT_OBTYPE (unt8);
OB_HASHER_32_BIT_OBTYPE (int16);
OB_HASHER_32_BIT_OBTYPE (unt16);
OB_HASHER_32_BIT_OBTYPE (int32);
OB_HASHER_32_BIT_OBTYPE (unt32);
OB_HASHER_64_BIT_OBTYPE (int64);
OB_HASHER_64_BIT_OBTYPE (unt64);

#undef OB_HASHER_32_BIT_OBTYPE
#undef OB_HASHER_64_BIT_OBTYPE
#undef OB_HASHER_OBTYPE
}
}


#endif

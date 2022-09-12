
/* (c)  oblong industries */

#ifndef OB_TYPEOF_HEADER_GUARD
#define OB_TYPEOF_HEADER_GUARD

#if defined(_MSC_VER) || defined(__GXX_EXPERIMENTAL_CXX0X__)                   \
  || (defined(__cplusplus) && (__cplusplus > 199711L))
#define OB_TYPEOF(x) decltype (x)
#else
#define OB_TYPEOF(x) typeof(x)
#endif

#endif  // ! OB_TYPEOF_HEADER_GUARD

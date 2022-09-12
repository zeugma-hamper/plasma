
/* (c)  oblong industries */

#ifndef PRETERITE_SMORASBORD
#define PRETERITE_SMORASBORD


#include <libLoam/c/ob-types.h>

#include <stdlib.h>
#include <string.h>


/** delete \a ptr (using its Delete method) and set it to NULL */
#define Del_Ankle_Ptr(ptr)                                                     \
  do                                                                           \
    {                                                                          \
      if (ptr)                                                                 \
        {                                                                      \
          (ptr)->Delete ();                                                    \
          (ptr) = NULL;                                                        \
        }                                                                      \
    }                                                                          \
  while (0)

/** delete \a ptr and set it to NULL */
#define Del_Ptr(ptr)                                                           \
  do                                                                           \
    {                                                                          \
      if (ptr)                                                                 \
        {                                                                      \
          delete (ptr);                                                        \
          (ptr) = NULL;                                                        \
        }                                                                      \
    }                                                                          \
  while (0)

/** delete[] \a ptr and set it to NULL */
#define Del_Ptr_Array(ptr)                                                     \
  do                                                                           \
    {                                                                          \
      if (ptr)                                                                 \
        {                                                                      \
          delete[](ptr);                                                       \
          (ptr) = NULL;                                                        \
        }                                                                      \
    }                                                                          \
  while (0)

/** free \a ptr and set it to NULL */
#define Free_Ptr(ptr)                                                          \
  do                                                                           \
    {                                                                          \
      if (ptr)                                                                 \
        {                                                                      \
          free (ptr);                                                          \
          (ptr) = NULL;                                                        \
        }                                                                      \
    }                                                                          \
  while (0)


#if !defined(ENCAPSULATION_IS_BAD)
#define OB_PUBLIC public
#define OB_PROTECTED protected
#define OB_PRIVATE private
#else
#define OB_PUBLIC public
#define OB_PROTECTED public
#define OB_PRIVATE public
#endif  // !ENCAPSULATION_IS_BAD


#endif  // PRETERITE_SMORASBORD


/* (c)  oblong industries */

#ifndef OBLONG_LOAM_MUTEX_H
#define OBLONG_LOAM_MUTEX_H


#include "libLoam/c/ob-api.h"
#include <libLoam/c/ob-pthread.h>

#include <libLoam/c++/Preterite.h>


namespace oblong {
namespace loam {


/**
 * Lightweight pthreads mutex wrapper, with support for recursivity.
 */
class OB_LOAMXX_API Mutex
{
 public:
  Mutex (bool recursive);
  ~Mutex ();
  void Delete () { delete this; }

  void Lock ();
  void Unlock ();

 OB_PRIVATE:
#ifndef _MSC_VER
  pthread_mutex_t mutex_;
#else
  bool recursive;              //we have to store this and use different objects
  CRITICAL_SECTION c_section;  //this is for recursive behavior
  HANDLE semaphore;            //this is for non-recursive behavior
#endif
};

/**
 * Your regular RAII lock.
 */
template <class T>
class Lock
{
 public:
  Lock (T &m) : lockable_ (m) { lockable_.Lock (); }
  ~Lock () { lockable_.Unlock (); }
  void Delete () { delete this; }

 private:
  Lock (const Lock &other);
  Lock &operator= (const Lock &other);

 OB_PRIVATE:
  T &lockable_;
};

typedef Lock<Mutex> MutexLock;
}
}  // namespace oblong::loam


#endif  // OBLONG_LOAM_MUTEX_H

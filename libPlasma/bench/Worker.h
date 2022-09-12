
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_BENCH_WORKER_H
#define OBLONG_PLASMA_BENCH_WORKER_H

#include "TimeKeeper.h"
#include "libPlasma/c/pool.h"

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>

namespace oblong {
namespace plasma {
namespace bench {

typedef TimedDataKeeper<::std::pair<int64, unt64>> TimedBytes;

class Worker
{
 public:
  struct Config
  {
    ::std::string pool;
    ::std::string out;
    unt64 batches;
    unt64 batch_size;
  };

 public:
  virtual ~Worker ();

  bool Fork ();
  bool Wait ();
  bool Run ();

  virtual ob_retort DoRun (pool_hose, TimedBytes &) = 0;
  virtual ::std::string Description () const = 0;

 protected:
  Worker (const Config &);

  const unt64 batches_;
  const unt64 bsize_;

 private:
  bool CheckRetort (ob_retort) const;
  void Report (const TimedBytes &, ::std::ostream &) const;

  ::std::string pool_;
  ::std::string out_;
  pid_t pid_;
  pool_hose hose_;
};
}
}
}  // namespace oblong::plasma::bench


#endif  // OBLONG_PLASMA_BENCH_WORKER_H

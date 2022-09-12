
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_BENCH_SERIALWORKER_H
#define OBLONG_PLASMA_BENCH_SERIALWORKER_H

#include "Reader.h"
#include "Writer.h"
#include "Worker.h"

namespace oblong {
namespace plasma {
namespace bench {

class SerialWorker : public Worker
{
 public:
  ~SerialWorker ();

  explicit SerialWorker (const Config &cfg);

  void Add (Worker *);
  void Add (const Reader::Config &);
  void Add (const Writer::Config &);

 private:
  ob_retort DoRun (pool_hose h, TimedBytes &tb) override;
  ::std::string Description () const override;

  typedef ::std::pair<bool, Worker *> WInfo;
  typedef ::std::vector<WInfo> Workers;
  typedef Workers::size_type WSize;

  Workers workers_;
};
}
}
}  // namespace oblong::plasma::bench


#endif  // OBLONG_PLASMA_BENCH_SERIALWORKER_H

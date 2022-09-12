
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_BENCH_PING_H
#define OBLONG_PLASMA_BENCH_PING_H

#include "LoneWorker.h"
#include "utils.h"

namespace oblong {
namespace plasma {
namespace bench {

class PingPonger : public LoneWorker
{
 public:
  struct Config : Worker::Config
  {
    Proteins proteins;
    pool_timestamp timeout;
    bool ping;
    Config (const Config &other);
    Config ();
  };

 public:
  ~PingPonger ();
  explicit PingPonger (const Config &cfg);

 private:
  ob_retort DoRun (pool_hose h, TimedBytes &tb) override;
  ::std::string Description () const override;

  unt64 Read (pool_hose h, unt64 b, unt64 i, unt64 &ec);
  unt64 Write (pool_hose h, unt64 b, unt64 i, unt64 &ec);

  const Proteins proteins_;
  const pool_timestamp timeout_;
  const bool ping_;
  const Sizes psizes_;
};
}
}
}  // namespace oblong::plasma::bench


#endif  // OBLONG_PLASMA_BENCH_PING_H


/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_BENCH_WRITER_H
#define OBLONG_PLASMA_BENCH_WRITER_H

#include "LoneWorker.h"
#include "utils.h"

namespace oblong {
namespace plasma {
namespace bench {

class Writer : public LoneWorker
{
 public:
  class Config : public Worker::Config
  {
   public:
    Proteins proteins;
    bool skip;
    void AddProtein (unt64 size, const char *descrip);
    bool AddProteins (const char *str);
  };

 public:
  ~Writer ();
  explicit Writer (const Config &cfg);

 private:
  ob_retort DoRun (pool_hose h, TimedBytes &tb) override;
  ::std::string Description () const override;

  const Proteins proteins_;
  const bool skip_;
};
}
}
}  // namespace oblong::plasma::bench


#endif  // OBLONG_PLASMA_BENCH_WRITER_H

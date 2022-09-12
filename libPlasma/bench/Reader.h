
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_BENCH_READER_H
#define OBLONG_PLASMA_BENCH_READER_H

#include "LoneWorker.h"

namespace oblong {
namespace plasma {
namespace bench {

class Reader : public LoneWorker
{
 public:
  enum Mode
  {
    OLDEST,
    NEWEST,
    FORWARD,
    TAIL,
    BACKWARD,
    RANDOM
  };

  class Config : public Worker::Config
  {
   public:
    Mode mode;
    pool_timestamp timeout;
    slaw matcher;
    bool ModeFromString (const char *str);
  };

 public:
  ~Reader ();
  explicit Reader (const Config &cfg);

 private:
  ::std::string Description () const override;
  ob_retort DoRun (pool_hose h, TimedBytes &tb) override;
  int64 CleanProteins ();

  bool CheckError (ob_retort r, unt64 b, unt64 i, unt64 &ec)
  {
    if (r != OB_OK)
      {
        AddError (b, i, r);
        ps_[i] = NULL;
        ec++;
      }
    return POOL_NO_SUCH_PROTEIN == r;
  }

  template <class F>
  void RunBatches (F f, pool_hose h, TimedBytes &tb)
  {
    for (unt64 b = 0; b < batches_; ++b)
      {
        unt64 ec = 0;
        tb.LapseStart ();
        if (matcher_)
          {
            for (unt64 i = 0; i < bsize_; ++i)
              if (CheckError (f (h, matcher_, ps_ + i), b, i, ec))
                f.Wrap (h);
          }
        else
          {
            for (unt64 i = 0; i < bsize_; ++i)
              if (CheckError (f (h, ps_ + i), b, i, ec))
                f.Wrap (h);
          }
        tb.LapseEnd ();
        tb.AddData (::std::make_pair (bsize_ - ec, CleanProteins ()));
      }
  }

  Mode mode_;
  pool_timestamp timeout_;
  slaw matcher_;
  protein *ps_;
};
}
}
}  // namespace oblong::plasma::bench


#endif  // OBLONG_PLASMA_BENCH_READER_H

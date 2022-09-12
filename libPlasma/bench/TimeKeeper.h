
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_BENCH_TIMEKEEPER_H
#define OBLONG_PLASMA_BENCH_TIMEKEEPER_H

#include <libLoam/c/ob-coretypes.h>

#include <vector>
#include <time.h>

namespace oblong {
namespace plasma {
namespace bench {

typedef ::std::vector<float64> Lapses;

/**
 * Keeping time statistics
 */
class TimeKeeper
{
 public:
  explicit TimeKeeper (unsigned int capacity = 0);

  void LapseStart ();
  void LapseEnd ();

  const Lapses &GetLapses () const { return lapses_; }

 private:
  Lapses lapses_;
  timespec start_;
  bool running_;
};

template <typename T>
class TimedDataKeeper : public TimeKeeper
{
 public:
  typedef ::std::vector<T> Data;

 public:
  explicit TimedDataKeeper (unsigned int c = 0) : TimeKeeper (c)
  {
    if (c > 0)
      data_.reserve (c);
  }

  void LapseEndWith (const T &d)
  {
    LapseEnd ();
    data_.push_back (d);
  }
  void AddData (const T &d) { data_.push_back (d); }

  const Data &GetData () const { return data_; }

 private:
  Data data_;
};
}
}
}  // namespace oblong::plasma::bench

#endif  // OBLONG_PLASMA_BENCH_TIMEKEEPER_H

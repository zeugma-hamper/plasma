
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_BENCH_LONEWORKER_H
#define OBLONG_PLASMA_BENCH_LONEWORKER_H

#include "Worker.h"

namespace oblong {
namespace plasma {
namespace bench {

class LoneWorker : public Worker
{
 public:
  explicit LoneWorker (const Config &);

 protected:
  void ClearErrors ();
  void AddError (unt64 b, unt64 i, ob_retort r);
  void ReportErrors (const char *);

 private:
  typedef ::std::vector<::std::pair<unt64, ob_retort>> ErrorList;

  ErrorList errs_;
};
}
}
}  // namespace oblong::plasma::bench


#endif  // OBLONG_PLASMA_BENCH_LONEWORKER_H

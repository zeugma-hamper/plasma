
/* (c)  oblong industries */

#include "Writer.h"
#include "utils.h"

#include <algorithm>
#include <string>
#include <sstream>

#include "libPlasma/c/slaw.h"
#include "libPlasma/c/protein.h"

namespace oblong {
namespace plasma {
namespace bench {

void Writer::Config::AddProtein (unt64 size, const char *descrip)
{
  ::oblong::plasma::bench::AddProtein (proteins, size, descrip);
}

bool Writer::Config::AddProteins (const char *str)
{
  return ::oblong::plasma::bench::AddProteins (proteins, str);
}

Writer::Writer (const Config &c)
    : LoneWorker (c), proteins_ (c.proteins), skip_ (c.skip)
{
}

Writer::~Writer ()
{
  FreeProteins (proteins_);
}

::std::string Writer::Description () const
{
  ::std::ostringstream os;
  os << "Writer: " << batches_ << (batches_ > 1 ? " batches" : " batch")
     << " of " << bsize_ << (bsize_ > 1 ? " proteins" : " protein")
     << (proteins_.size () > 1 ? " (sizes " : " (size ")
     << slaw_len (proteins_[0]);
  for (unt64 i = 1, n = proteins_.size (); i < n; ++i)
    os << ", " << slaw_len (proteins_[i]);
  os << ")";
  return os.str ();
}

ob_retort Writer::DoRun (pool_hose h, TimedBytes &tb)
{
  const Proteins::size_type N (proteins_.size ());
  ClearErrors ();
  int64 idx = -1;
  Sizes s (ProteinSizes (proteins_));
  for (unt64 b = 0; b < batches_; ++b)
    {
      const unt64 M = b * bsize_;
      unt64 ec = 0;
      unt64 bs = 0;
      tb.LapseStart ();
      for (unt64 i = 0; i < bsize_; ++i)
        {
          unt64 n = (M + i) % N;
          ob_retort r = pool_deposit (h, proteins_[n], &idx);
          if (OB_OK == r)
            bs += s[n];
          else
            {
              ec++;
              AddError (b, i, r);
            }
        }
      tb.LapseEndWith (::std::make_pair (-int64 (bsize_ - ec), bs));
    }
  ReportErrors ("Writer");
  if (skip_ && idx > -1)
    pool_seekto (h, idx + 1);
  return OB_OK;
}
}
}
}  // namespace oblong::plasma::bench

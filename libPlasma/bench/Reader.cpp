
/* (c)  oblong industries */

#include "TimeKeeper.h"
#include "Reader.h"
#include "utils.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#include "libPlasma/c/slaw.h"
#include "libPlasma/c/protein.h"

namespace oblong {
namespace plasma {
namespace bench {

namespace {

::std::string ModeToStr (Reader::Mode m, const char *suffix = "")
{
  static const ::std::string modes[] = {"oldest", "newest",   "forward",
                                        "tail",   "backward", "random"};
  return modes[m] + suffix;
}

class ReadNext
{
 public:
  explicit ReadNext (pool_timestamp w) : wait (w) {}
  ob_retort operator() (pool_hose h, protein *p) const
  {
    return pool_await_next (h, wait, p, NULL, NULL);
  }

  ob_retort operator() (pool_hose h, bslaw m, protein *p) const
  {
    return pool_await_probe_frwd (h, m, wait, p, NULL, NULL);
  }

  void Wrap (pool_hose h) const { pool_rewind (h); }

  const pool_timestamp wait;
};

class ReadPrev
{
 public:
  ob_retort operator() (pool_hose h, protein *p) const
  {
    return pool_prev (h, p, NULL, NULL);
  }

  ob_retort operator() (pool_hose h, bslaw m, protein *p) const
  {
    return pool_probe_back (h, m, p, NULL, NULL);
  }

  void Wrap (pool_hose h) const { pool_runout (h); }
};

class ReadRandom
{
 public:
  explicit ReadRandom (pool_hose h)
  {
    srand (clock ());
    Wrap (h);
  }

  ob_retort operator() (pool_hose h, protein *p) const
  {
    return pool_nth_protein (h, first + rand () % range, p, NULL);
  }

  ob_retort operator() (pool_hose h, bslaw, protein *p) const
  {
    return operator() (h, p);
  }

  void Wrap (pool_hose h)
  {
    first = 0;
    range = 1;
    int64 from (0), to (0);
    if (pool_oldest_index (h, &from) == OB_OK
        && pool_newest_index (h, &to) == OB_OK)
      {
        first = from;
        range = ::std::max (int64 (1), to - from);
      }
  }

  int64 first, range;
};

}  // namespace

Reader::~Reader ()
{
  delete[] ps_;
  slaw_free (matcher_);
}

Reader::Reader (const Config &cfg)
    : LoneWorker (cfg),
      mode_ (cfg.mode),
      timeout_ (cfg.timeout),
      matcher_ (cfg.matcher),
      ps_ (new protein[bsize_])
{
  for (unt64 i = 0; i < bsize_; ++i)
    ps_[i] = NULL;
}

int64 Reader::CleanProteins ()
{
  int64 bs = 0;
  for (unt64 j = 0; j < bsize_; ++j)
    {
      if (ps_[j])
        {
          bs += slaw_len (ps_[j]);
          protein_free (ps_[j]);
          ps_[j] = NULL;
        }
    }
  return bs;
}

ob_retort Reader::DoRun (pool_hose h, TimedBytes &tb)
{
  ClearErrors ();
  ob_retort r = OB_OK;
  switch (mode_)
    {
      case RANDOM:
        RunBatches (ReadRandom (h), h, tb);
        break;
      case TAIL:
        if ((r = pool_runout (h)) != OB_OK)
          return r;
      case BACKWARD:
        RunBatches (ReadPrev (), h, tb);
        break;
      case OLDEST:
        if ((r = pool_rewind (h)) != OB_OK)
          return r;
        RunBatches (ReadNext (timeout_), h, tb);
        break;
      case NEWEST:
        if ((r = pool_runout (h)) != OB_OK)
          return r;
      case FORWARD:
        RunBatches (ReadNext (timeout_), h, tb);
        break;
      default:
        return OB_UNKNOWN_ERR;
    }
  ReportErrors (ModeToStr (mode_, " reader").c_str ());
  return r;
}

::std::string Reader::Description () const
{
  ::std::ostringstream os;
  os << "Reader: " << batches_ << (batches_ > 1 ? " batches" : " batch")
     << " of " << bsize_ << (bsize_ > 1 ? " proteins" : " protein") << " ("
     << ModeToStr (mode_) << " mode)";
  return os.str ();
}

namespace {

typedef ::std::map<::std::string, Reader::Mode> ModeMap;

void add_mode (ModeMap &m, const char *key, Reader::Mode mode)
{
  ::std::string k (key);
  for (::std::size_t i = 1, n = k.size (); i <= n; ++i)
    m[k.substr (0, i)] = mode;
}

bool parse_stamp (const ::std::string &a, pool_timestamp &t)
{
  const char *a1 (a.c_str ());
  char *r (NULL);
  errno = 0;
  t = strtold (a1, &r);
  return (0 == errno && r != a1 && '\0' == *r);
}

bool parse_mode_args (Reader::Mode m, const Strings &as, pool_timestamp &t,
                      slaw &s)
{
  t = 0;
  s = NULL;
  const unt64 N (as.size ());
  if (1 == N)
    return true;
  if (N > 3)
    return false;
  if (Reader::RANDOM == m)
    return false;
  if (Reader::BACKWARD == m || Reader::TAIL == m)
    {
      if (N > 2)
        return false;
      s = slaw_string (as[1].c_str ());
      return true;
    }
  if (!parse_stamp (as[1], t))
    return false;
  if (N > 2)
    s = slaw_string (as[2].c_str ());
  return true;
}

}  // namespace

bool Reader::Config::ModeFromString (const char *str)
{
  static ModeMap modes;
  if (0 == modes.size ())
    {
      add_mode (modes, "oldest", Reader::OLDEST);
      add_mode (modes, "newest", Reader::NEWEST);
      add_mode (modes, "forward", Reader::FORWARD);
      add_mode (modes, "backward", Reader::BACKWARD);
      add_mode (modes, "tail", Reader::TAIL);
      add_mode (modes, "random", Reader::RANDOM);
    }
  Strings tks (Tokenize (str, "/"));
  if (tks.empty ())
    return false;
  ModeMap::const_iterator it (modes.find (tks[0]));
  if (modes.end () == it)
    return false;
  mode = it->second;
  return parse_mode_args (mode, tks, timeout, matcher);
  return true;
}
}
}
}  // namespace oblong::plasma::bench

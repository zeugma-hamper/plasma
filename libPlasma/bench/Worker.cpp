
/* (c)  oblong industries */

#include "Worker.h"

#include <sys/wait.h>
#include <fstream>

namespace oblong {
namespace plasma {
namespace bench {

Worker::Worker (const Config &cfg)
    : batches_ (cfg.batches),
      bsize_ (cfg.batch_size),
      pool_ (cfg.pool),
      out_ (cfg.out),
      pid_ (-1),
      hose_ (NULL)
{
}

Worker::~Worker ()
{
  pool_withdraw (hose_);
}

bool Worker::Run ()
{
  ::std::cerr << "# " << Description () << "\n## Pool: " << pool_ << "\n";
  bool r (pool_.empty () || hose_
          || CheckRetort (pool_participate (pool_.c_str (), &hose_, NULL)));
  if (r)
    {
      TimedBytes tb;
      r = CheckRetort (DoRun (hose_, tb));
      if (!out_.empty ())
        {
          ::std::ofstream outf (out_.c_str (),
                                ::std::ios::out | ::std::ios::app);
          Report (tb, outf);
        }
      else
        Report (tb, ::std::cout);
    }

  /* Used to be:
   *   if (r >= OB_OK) ::std::cerr << "## Sucessfully completed\n";
   * but since r is a boolean, it is either 0 or 1, and thus always
   * >= 0.  Seems like it should only print the message if r is true,
   * since true seems to indicate OK in this case.  (This is why I
   * hate using booleans to indicate success or failure, since unless
   * the variable has a name like "isOk", it's not clear which is which.)
   * Coverity caught this.  (defect 10039 in our trial database)
   */

  if (r)
    ::std::cerr << "## Sucessfully completed\n";
  return r;
}

bool Worker::Fork ()
{
  pid_ = fork ();
  if (pid_ < 0)
    return false;
  if (0 == pid_)
    return true;
  _exit (Run () ? 0 : 1);
  return true;
}

bool Worker::Wait ()
{
  if (pid_ <= 0)
    return false;
  int status = 0;
  do
    {
      if (waitpid (pid_, &status, WUNTRACED | WCONTINUED) == -1)
        return false;
      if (WIFEXITED (status))
        return (0 == WEXITSTATUS (status));
    }
  while (!WIFSIGNALED (status));
  return false;
}

bool Worker::CheckRetort (ob_retort r) const
{
  if (r < OB_OK)
    ::std::cerr << "## Stopped with code " << r << " (" << ob_error_string (r)
                << ")\n";
  return OB_OK == r;
}

void Worker::Report (const TimedBytes &tk, ::std::ostream &o) const
{
  static const ::std::string H (
    "# protein no., bytes, time (nsec), bytes/s, proteins/s\n");
  const Lapses &l = tk.GetLapses ();
  const TimedBytes::Data &data = tk.GetData ();
  const Lapses::size_type N (l.size ());
  if (N > 0)
    {
      o << H;
      int64 samples = 0;
      unt64 bytes = 0;
      float64 tm = 0;
      int64 proteins = 0;
      float64 bpersec = 0;
      float64 ppersec = 0;
      for (Lapses::size_type i = 0; i < N; ++i)
        {
          if (l[i] > 0)
            {
              float64 bs ((1e9 * data[i].second) / l[i]);
              float64 ps ((1e9 * data[i].first) / l[i]);
              o << data[i].first << " " << data[i].second << " " << l[i] << " "
                << bs << "  " << ps << ::std::endl;
              if (data[i].first != 0)
                {
                  ++samples;
                  proteins += data[i].first;
                  bytes += data[i].second;
                  tm += l[i];
                  bpersec += bs;
                  ppersec += ps;
                }
            }
        }
      if (proteins != 0 && samples > 0)
        {
          ::std::cerr << "## Averages:\n## " << H;
          ::std::cerr << float64 (proteins) / samples << " "
                      << float64 (bytes) / samples << " " << tm / samples << " "
                      << bpersec / samples << " " << ppersec / samples
                      << ::std::endl;
        }
    }
}
}
}
}  // namespace oblong::plasma::bench

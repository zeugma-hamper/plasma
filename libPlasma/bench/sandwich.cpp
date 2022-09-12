
/* (c)  oblong industries */

#include "SerialWorker.h"

#include <string>
#include <iostream>
#include <stdlib.h>

namespace pb = ::oblong::plasma::bench;

void usage (const char *msg)
{
  ::std::cerr << "Error: " << msg << ::std::endl
              << "Usage: sandwich"
              << " <pool-name> <iterations>"
              << " [r <batches> <bsize> <mode> | "
              << "u <batches> <bsize> <psizes>]+" << ::std::endl;
  exit (1);
}

pb::Worker::Config read_config (int &argc, const char **&argv)
{
  if (argc < 7)
    usage ("invalid argument count");
  pb::Worker::Config cfg;
  cfg.pool = argv[1];
  int n = atoi (argv[2]);
  if (n <= 0)
    usage ("invalid iteration number");
  cfg.batches = n;
  cfg.batch_size = 1;

  argv += 3;
  argc -= 3;

  if (0 == argc)
    usage ("missing workers specification");
  if (0 != argc % 4)
    usage ("invalid workers specification");

  return cfg;
}

bool read_kind (const char *a, char &k, ::std::string &p)
{
  ::std::string sp (a);
  unt64 len (sp.length ());
  if (sp.empty () || len == 2 || (len > 1 && sp[1] != '/'))
    return false;
  char kind (sp[0]);
  if ('r' != kind && 'u' != kind && 's' != kind)
    return false;
  k = kind;
  p = len > 2 ? sp.substr (2) : "";
  return true;
}

bool read_worker (pb::SerialWorker &w, int &argc, const char **&argv)
{
  if (0 == argc)
    return false;
  ::std::string pool;
  char kind = 0;
  if (!read_kind (argv[0], kind, pool))
    usage ("invalid worker kind");
  int batches = atoi (argv[1]);
  if (batches <= 0)
    usage ("invalid batches number");
  int bsize = atoi (argv[2]);
  if (bsize <= 0)
    usage ("invalid batch size");
  if ('r' == kind)
    {
      pb::Reader::Config cfg;
      if (!cfg.ModeFromString (argv[3]))
        usage ("invalid reader mode");
      cfg.batches = batches;
      cfg.batch_size = bsize;
      cfg.pool = pool;
      w.Add (cfg);
    }
  else
    {
      pb::Writer::Config cfg;
      if (!cfg.AddProteins (argv[3]))
        usage ("invalid protein sizes");
      cfg.skip = 's' == kind;
      cfg.batches = batches;
      cfg.batch_size = bsize;
      cfg.pool = pool;
      w.Add (cfg);
    }
  argv += 4;
  argc -= 4;
  return argc > 0;
}

int main (int argc, const char **argv)
{
  pb::SerialWorker w (read_config (argc, argv));
  while (read_worker (w, argc, argv))
    {
    };
  return w.Run () ? EXIT_SUCCESS : EXIT_FAILURE;
}

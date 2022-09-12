
/* (c)  oblong industries */

#include "Reader.h"

#include <string>
#include <iostream>
#include <stdlib.h>

namespace pb = ::oblong::plasma::bench;

void usage (const char *name, const char *msg)
{
  ::std::cerr << "Error: " << msg << ::std::endl
              << "Usage: " << name << " <pool-name> <batch-number> <batch-size>"
              << " {forward, backward, random}" << ::std::endl;
  exit (1);
}

pb::Reader::Config read_args (int argc, const char **argv)
{
  if (argc < 5)
    usage (argv[0], "invalid argument count");
  pb::Reader::Config cfg;
  cfg.pool = argv[1];
  int n = atoi (argv[2]);
  if (n <= 0)
    usage (argv[0], "invalid batch number");
  cfg.batches = n;
  n = atoi (argv[3]);
  if (n <= 0)
    usage (argv[0], "invalid batch size");
  cfg.batch_size = n;
  if (!cfg.ModeFromString (argv[4]))
    usage (argv[0], "invalid read mode");
  return cfg;
}

int main (int argc, const char **argv)
{
  pb::Reader r (read_args (argc, argv));
  return r.Run () ? EXIT_SUCCESS : EXIT_FAILURE;
}

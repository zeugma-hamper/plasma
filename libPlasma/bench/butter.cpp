
/* (c)  oblong industries */

#include "Writer.h"

#include <string>
#include <iostream>
#include <stdlib.h>

namespace pb = ::oblong::plasma::bench;

void usage (const char *name, const char *msg)
{
  ::std::cerr << "Error: " << msg << ::std::endl
              << "Usage: " << name << " <pool-name> <batch-number> <batch-size>"
              << " <ps1,ps2,...,psn>" << ::std::endl;
  exit (1);
}

pb::Writer::Config read_args (int argc, const char **argv)
{
  if (argc < 5)
    usage (argv[0], "invalid argument count");
  pb::Writer::Config cfg;
  cfg.pool = argv[1];
  int n = atoi (argv[2]);
  if (n <= 0)
    usage (argv[0], "invalid batch number");
  cfg.batches = n;
  n = atoi (argv[3]);
  if (n <= 0)
    usage (argv[0], "invalid batch size");
  cfg.batch_size = n;
  if (!cfg.AddProteins (argv[4]))
    usage (argv[0], "invalid protein sizes");
  cfg.skip = false;
  return cfg;
}

int main (int argc, const char **argv)
{
  pb::Writer r (read_args (argc, argv));
  return r.Run () ? EXIT_SUCCESS : EXIT_FAILURE;
}

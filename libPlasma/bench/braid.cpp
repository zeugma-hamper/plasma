
/* (c)  oblong industries */

#include "PingPonger.h"

#include <string>
#include <iostream>
#include <stdlib.h>

namespace pb = ::oblong::plasma::bench;

void usage (const char *msg)
{
  ::std::cerr << "Error: " << msg << ::std::endl
              << "Usage: braid"
              << " <pool-name> <batch-number> <batch-size>"
              << " <ps1,ps2,...,psn> <timeout> <pingout> <pongout>"
              << ::std::endl;
  exit (1);
}

int main (int argc, const char **argv)
{
  if (argc < 8)
    usage ("invalid argument count");

  pb::PingPonger::Config pic;
  pic.pool = argv[1];

  int n = atoi (argv[2]);
  if (n <= 0)
    usage ("invalid batch number");

  pic.batches = n;
  n = atoi (argv[3]);
  if (n <= 0)
    usage ("invalid batch size");
  pic.batch_size = n;

  if (!pb::AddProteins (pic.proteins, argv[4]))
    usage ("invalid protein sizes");
  pic.timeout = strtold (argv[5], NULL);

  if (pic.timeout <= 0)
    usage ("invalid read timeout");

  pic.ping = true;
  pic.out = argv[6];

  pb::PingPonger::Config poc (pic);
  poc.out = argv[7];
  poc.ping = false;

  pb::PingPonger ping (pic);
  pb::PingPonger pong (poc);

  if (!pong.Fork () || !ping.Fork ())
    return EXIT_FAILURE;
  if (!ping.Wait () || !pong.Wait ())
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}


/* (c)  ANIMIST contributors */

#include "libPlasma/c/slaw.h"
#include "libPlasma/c/plasma-info.h"
#include "libPlasma/c/private/plasma-private.h"

#include <stdlib.h>

slaw plasma_info (void)
{
  slaw  yaml_slaw    = NULL;
  slaw  ssl_slaw     = NULL;
  char *yaml_version = plasma_yaml_version();
  char *ssl_version  = plasma_ssl_version();

  if (yaml_version)
    yaml_slaw = slaw_string (yaml_version);
  else
    yaml_slaw = slaw_boolean (false);

  if (ssl_version)
    ssl_slaw = slaw_string (ssl_version);
  else
    ssl_slaw = slaw_boolean (false);

  free (yaml_version);
  free (ssl_version);

  return slaw_map_inline_cf ("YAML", yaml_slaw,
                             "TLS",  ssl_slaw,
                             NULL);
}

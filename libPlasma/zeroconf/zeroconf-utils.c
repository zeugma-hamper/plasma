
/* (c)  oblong industries */

#include "zeroconf-services.h"
#include "libPlasma/c/pool_cmd.h"

void zeroconf_enable_debug_messages (void)
{
  pool_cmd_enable_debug_messages (ZEROCONF_LOG_CODE, 64 - 12);
}

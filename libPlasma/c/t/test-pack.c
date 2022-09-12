
/* (c)  oblong industries */

/* Tests for pool_net_pack_op and pool_net_unpack_op. */

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-vers.h"

#include "libPlasma/c/slaw.h"
#include "libPlasma/c/protein.h"
#include "libPlasma/c/pool.h"
#include "libPlasma/c/private/pool_net.h"

int main (int argc, char **argv)
{
  slaw args;
  protein op;
  char *str1, *str2;
  ob_retort pret;
  args = op = NULL;
  str1 = str2 = NULL;
  pret = OB_OK;

  OB_DIE_ON_ERROR (OB_CHECK_ABI ());

  OB_LOG_INFO (
    "This test causes warnings saying 'This was not a string', and that's ok");

  /* Test that unpacking a pool TCP protein with a missing string
   * argument doesn't crash. (bug 17941) */
  /* Has one string argument; try to unpack two. */
  args = slaw_list_inline_c ("foo", NULL);
  op = protein_from_ff (NULL, slaw_map_inline_cf (ARGS_KEY, args, NULL));
  pret = pool_net_unpack_op_f (op, 1, "ss", &str1, &str2);
  if (pret != POOL_PROTOCOL_ERROR)
    OB_FATAL_ERROR_CODE (0x2031a000, "string was missing but protocol error "
                                     "not signaled\n");
  free (str1);
  free (str2);

  /* Test unpacking a slaw which exists but is the wrong type (not a string). */
  /* Has one int64 argument; try to unpack a string. */
  args = slaw_list_inline_f (slaw_int64 (420), NULL);
  op = protein_from_ff (NULL, slaw_map_inline_cf (ARGS_KEY, args, NULL));
  str1 = NULL;
  pret = pool_net_unpack_op_f (op, 1, "s", &str1);
  if (pret != POOL_PROTOCOL_ERROR)
    OB_FATAL_ERROR_CODE (0x2031a001,
                         "string arg was wrong type but protocol error "
                         "not signaled\n");
  free (str1);

  return 0;
}

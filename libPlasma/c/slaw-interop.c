
/* (c)  oblong industries */

#include "libPlasma/c/slaw-interop.h"
#include "libPlasma/c/private/private-versioning.h"
#include "libPlasma/c/slaw-io.h" /* for SLAW_WRONG_VERSION */

const slaw_vfuncs *get_vfuncs (slaw_version v)
{
  switch (v)
    {
#ifndef DROP_SUPPORT_FOR_SLAW_V1
      case 1:
        return &slaw_v1funcs;
#endif
      case 2:
        return &slaw_v2funcs;
      default:
        return NULL;
    }
}

static ob_retort internal_convert (slaw *s, slaw_endian e, slaw_version from,
                                   slaw_version to, int64 *len)
{
  const slaw_vfuncs *vfrom = get_vfuncs (from);
  const slaw_vfuncs *vto = get_vfuncs (to);
  ob_retort tort = OB_OK;

  if (!vfrom || !vto)
    return SLAW_WRONG_VERSION;

  if (e == SLAW_ENDIAN_OPPOSITE)
    tort = vfrom->vslaw_swap (*s, (slaw) (size_t)
                              // XXX: ugly hack; need better bound
                              OB_CONST_U64 (0xfffffffffffffff0));
  else if (e == SLAW_ENDIAN_UNKNOWN)
    tort = vfrom->vprotein_fix_endian (*s);

  if (tort >= OB_OK && from != to)
    tort = vto->vbuild (s, vfrom);

  if (len && tort >= OB_OK)
    *len = vto->vprotein_len (*s);

  return tort;
}

ob_retort slaw_convert_from (slaw *s, slaw_endian e, slaw_version v)
{
  return internal_convert (s, e, v, SLAW_VERSION_CURRENT, NULL);
}

ob_retort slaw_convert_to (slaw *s, slaw_version v, int64 *len)
{
  return internal_convert (s, SLAW_ENDIAN_CURRENT, SLAW_VERSION_CURRENT, v,
                           len);
}

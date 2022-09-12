
/* (c)  oblong industries */

#include "libPlasma/c/slaw-path.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/protein.h"
#include "libPlasma/c/slaw-string.h"
#include "libPlasma/c/slaw-coerce.h"
#include <string.h>

const char *slaw_path_get_string (bslaw s, const char *path, const char *dflt)
{
  const char *result = slaw_string_emit (slaw_path_get_slaw (s, path));
  return (result ? result : dflt);
}

int64 slaw_path_get_int64 (bslaw s, const char *path, int64 dflt)
{
  int64 result;
  return (slaw_to_int64 (slaw_path_get_slaw (s, path), &result) == OB_OK
            ? result
            : dflt);
}

unt64 slaw_path_get_unt64 (bslaw s, const char *path, unt64 dflt)
{
  unt64 result;
  return (slaw_to_unt64 (slaw_path_get_slaw (s, path), &result) == OB_OK
            ? result
            : dflt);
}

float64 slaw_path_get_float64 (bslaw s, const char *path, float64 dflt)
{
  float64 result;
  return (slaw_to_float64 (slaw_path_get_slaw (s, path), &result) == OB_OK
            ? result
            : dflt);
}

bool slaw_path_get_bool (bslaw s, const char *path, bool dflt)
{
  bool result;
  return (slaw_to_boolean (slaw_path_get_slaw (s, path), &result) == OB_OK
            ? result
            : dflt);
}

bslaw slaw_path_get_slaw (bslaw s, const char *path)
{
  int64 i;

  if (slaw_is_protein (s))
    s = protein_ingests (s);

  if (!slaw_is_list_or_map (s))
    return NULL;

  slabu *sb = slabu_of_strings_from_split (path, "/");

  if (!sb)
    return NULL;

  for (i = 0; s != NULL && i < slabu_count (sb); i++)
    {
      bslaw key = slabu_list_nth (sb, i);
      s = slaw_map_find (s, key);
    }

  slabu_free (sb);

  return s;
}

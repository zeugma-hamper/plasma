
/* (c)  oblong industries */

#include "slaw.h"
#include "slaw-string.h"
#include "libPlasma/c/private/plasma-private.h"

#include <stdlib.h>
#include <string.h>


slaw slaw_strings_join_slabu (slabu *sb, const char *sep)
{
  int64 q, num, len = 0;
  bslaw s;
  slaw sout;
  char *st;
  const char *str;
  size_t sepLen;

  if (!sb)
    return NULL;

  /**************************************************
   * If sb contains zero elements, we return the
   * empty string, unlike the previous version of
   * this function, which would return NULL:
   * https://gitlab.oblong.com/platform/docs/-/wikis/_slaw_string_from_slabu_of_strings-return-value
   **************************************************/

  num = slabu_count (sb);
  sepLen = ((sep && num) ? strlen (sep) : 0);

  for (q = 0; q < num; q++)
    {
      s = slabu_list_nth (sb, q);
      if (!s || !slaw_is_string (s))
        continue;
      str = slaw_string_emit (s);
      if (!str || !*str)
        continue;
      len += strlen (str);
    }

  len += sepLen * (num - 1);

  if (!(sout = slaw_string_raw (len))
      || !(st = SLAW_STRING_EMIT_WRITABLE (sout)))
    return NULL;

  for (q = 0; q < num; q++)
    {
      if (sepLen && q)
        {
          memcpy (st, sep, sepLen);
          st += sepLen;
        }
      s = slabu_list_nth (sb, q);
      if (!s || !slaw_is_string (s))
        continue;
      str = slaw_string_emit (s);
      if (!str || !*str)
        continue;
      len = strlen (str);
      memcpy (st, str, len);
      st += len;
    }

  *st = '\0';
  return sout;
}


slaw slaw_strings_concat_f (slaw s, ...)
{
  if (!s)
    return slaw_string ("");

  slabu *sb = slabu_new ();
  ob_retort err = sb ? OB_OK : OB_NO_MEM;

#define MAX_ARGS 256
  slaw args[MAX_ARGS];
  int n = 0;
  args[0] = s;

  va_list vargies;
  va_start (vargies, s);
  while (args[n] && n < MAX_ARGS)
    {
      bool dup = false;
      int i = n - 1;
      for (; i >= 0 && !dup; --i)
        dup = (args[i] == args[n]);
      if (err != OB_OK)
        {
          if (!dup)
            slaw_free (args[n]);
        }
      else
        {
          if (dup)
            ob_err_accum (&err, slabu_list_add (sb, args[n]));
          else
            ob_err_accum (&err, slabu_list_add_x (sb, args[n]));
        }
      args[++n] = va_arg (vargies, slaw);
    }
  va_end (vargies);

  slaw result = (err < OB_OK) ? NULL : slaw_strings_join_slabu (sb, NULL);
  slabu_free (sb);

  return result;
}


slaw slaw_strings_concat (bslaw s, ...)
{
  if (!s)
    return slaw_string ("");

  slabu *sb = slabu_new ();
  if (!sb)
    return NULL;

  ob_retort err = OB_OK;
  va_list vargies;
  va_start (vargies, s);
  while (s && OB_OK == err)
    {
      ob_err_accum (&err, slabu_list_add_z (sb, s));
      s = va_arg (vargies, slaw);
    }
  va_end (vargies);

  if (err < OB_OK)
    {
      slabu_free (sb);
      return NULL;
    }

  return slaw_strings_join_slabu_f (sb, NULL);
}

static slaw concat_cstrings (slaw s, const char *str, va_list vargies)
{
  if (!s)
    return slaw_string ("");

  slabu *sb = slabu_new ();
  if (!sb)
    return NULL;

  ob_retort err = OB_OK;
  if (slaw_is_string (s))
    ob_err_accum (&err, slabu_list_add_x (sb, s));
  else
    slaw_free (s);

  while (str && OB_OK == err)
    {
      if (*str)
        ob_err_accum (&err, slabu_list_add_x (sb, slaw_string (str)));
      str = va_arg (vargies, const char *);
    }

  slaw result = (err < OB_OK) ? NULL : slaw_strings_join_slabu (sb, NULL);
  slabu_free (sb);

  return result;
}

slaw slaw_string_concat_cstrings_f (slaw s, const char *str, ...)
{
  va_list vargies;
  va_start (vargies, str);
  slaw ret = concat_cstrings (s, str, vargies);
  va_end (vargies);
  return ret;
}


slaw slaw_string_concat_cstrings (bslaw s, const char *str, ...)
{
  va_list vargies;
  va_start (vargies, str);
  slaw ret = concat_cstrings (slaw_dup (s), str, vargies);
  va_end (vargies);
  return ret;
}


slaw slaw_lists_concat_f (slaw sl, ...)
{
  if (!sl)
    return slaw_list_empty ();

  slabu *sb = slabu_new ();

  slaw args[MAX_ARGS];
  int arg_count = 0;
  ob_retort err = OB_OK;

  va_list vargies;
  va_start (vargies, sl);
  while (sl && arg_count < MAX_ARGS)
    {
      if (sb && slaw_is_list (sl))
        {
          int64 q, num;
          bslaw s = NULL;
          for (num = slaw_list_count (sl), q = 0; q < num; q++)
            if ((s = slaw_list_emit_next (sl, s)))
              ob_err_accum (&err, slabu_list_add_z (sb, s));
        }
      args[arg_count++] = sl;
      sl = va_arg (vargies, slaw);
    }
  va_end (vargies);

  slaw result = NULL;
  if (err < OB_OK)
    slabu_free (sb);
  else if (sb)
    result = slaw_list_f (sb);

  int i, j;
  for (i = 0; i < arg_count; ++i)
    {
      if (args[i])
        {
          for (j = i + 1; j < arg_count; ++j)
            if (args[j] == args[i])
              args[j] = NULL;
          slaw_free (args[i]);
        }
    }

  return result;
}


slaw slaw_lists_concat (bslaw sl, ...)
{
  if (!sl)
    return slaw_list_empty ();

  slabu *sb = slabu_new ();
  if (!sb)
    return NULL;

  ob_retort err = OB_OK;
  va_list vargies;
  va_start (vargies, sl);
  while (sl && OB_OK == err)
    {
      if (slaw_is_list (sl))
        {
          int64 q, num;
          bslaw s = NULL;
          for (num = slaw_list_count (sl), q = 0; q < num; q++)
            if ((s = slaw_list_emit_next (sl, s)))
              ob_err_accum (&err, slabu_list_add_z (sb, s));
        }
      sl = va_arg (vargies, slaw);
    }
  va_end (vargies);

  if (err < OB_OK)
    {
      slabu_free (sb);
      return NULL;
    }

  return slaw_list_f (sb);
}

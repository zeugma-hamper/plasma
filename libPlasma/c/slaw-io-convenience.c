
/* (c)  oblong industries */

/* This file implements the functions from slaw-io.h which are
 * merely convenience functions; i. e. they can be implemented
 * entirely in terms of other public API functions, and don't
 * need any special implementation knowledge.  Keeping them here
 * keeps the other files from getting cluttered up.
 *
 * (i. e. if you change the implementation, you know you don't
 * have to touch this file, since it doesn't know about the
 * implementation.)
 */

#include "libLoam/c/ob-file.h"
#include "libLoam/c/ob-sys.h"
#include "libPlasma/c/slaw-io.h"

#include <errno.h>

ob_retort slaw_input_open_binary (const char *filename, slaw_input *f)
{
  ob_retort err;
  FILE *ff = ob_fopen_cloexec (filename, "rb");

  if (ff == NULL)
    return ob_errno_to_retort (errno);

  err = slaw_input_open_binary_x (ff, f);

  if (err < OB_OK)
    fclose (ff);

  return err;
}

ob_retort slaw_output_open_binary (const char *filename, slaw_output *f)
{
  ob_retort err;

  FILE *ff = ob_fopen_cloexec (filename, "wb");
  if (ff == NULL)
    return ob_errno_to_retort (errno);

  err = slaw_output_open_binary_x (ff, f);
  if (err < OB_OK)
    fclose (ff);

  return err;
}

ob_retort slaw_input_open_text (const char *filename, slaw_input *f)
{
  ob_retort err;
  int fd = ob_open_cloexec (filename, O_RDONLY | OB_O_BINARY, 0);
  if (fd < 0)
    return ob_errno_to_retort (errno);

  err = slaw_input_open_text_fdx (fd, f);
  if (err < OB_OK)
    close (fd);

  return err;
}

ob_retort slaw_output_open_text (const char *filename, slaw_output *f)
{
  return slaw_output_open_text_options (filename, f, NULL);
}

ob_retort slaw_output_open_text_x (FILE *file, slaw_output *f)
{
  return slaw_output_open_text_options_x (file, f, NULL);
}

ob_retort slaw_output_open_text_z (FILE *file, slaw_output *f)
{
  return slaw_output_open_text_options_z (file, f, NULL);
}

ob_retort slaw_output_open_text_options (const char *filename, slaw_output *f,
                                         bslaw options)
{
  FILE *ff = ob_fopen_cloexec (filename, "wb");

  if (!ff)
    return ob_errno_to_retort (errno);

  ob_retort err = slaw_output_open_text_options_x (ff, f, options);
  if (err < OB_OK)
    fclose (ff);

  return err;
}

ob_retort slaw_output_open_text_options_f (const char *filename, slaw_output *f,
                                           slaw options)
{
  ob_retort err = slaw_output_open_text_options (filename, f, options);
  slaw_free (options);
  return err;
}

ob_retort slaw_read_from_binary_file (const char *filename, slaw *s)
{
  ob_retort err, err2;
  slaw_input f;

  err = slaw_input_open_binary (filename, &f);
  if (err != OB_OK)
    return err;

  err = slaw_input_read (f, s);
  err2 = slaw_input_close (f);

  return (err ? err : err2);
}

ob_retort slaw_write_to_binary_file (const char *filename, bslaw s)
{
  if (!s)
    return OB_ARGUMENT_WAS_NULL;

  ob_retort err, err2;
  slaw_output f;

  err = slaw_output_open_binary (filename, &f);
  if (err != OB_OK)
    return err;

  err = slaw_output_write (f, s);
  err2 = slaw_output_close (f);

  return (err ? err : err2);
}

ob_retort slaw_read_from_text_file (const char *filename, slaw *s)
{
  ob_retort err, err2;
  slaw_input f;

  err = slaw_input_open_text (filename, &f);
  if (err != OB_OK)
    return err;

  err = slaw_input_read (f, s);
  err2 = slaw_input_close (f);

  return (err ? err : err2);
}

ob_retort slaw_read_from_file (const char *filename, slaw *s)
{
  ob_retort err, err2;
  slaw_input f;

  err = slaw_input_open (filename, &f);
  if (err != OB_OK)
    return err;

  err = slaw_input_read (f, s);
  err2 = slaw_input_close (f);

  return (err ? err : err2);
}

ob_retort slaw_write_to_text_file (const char *filename, bslaw s)
{
  if (!s)
    return OB_ARGUMENT_WAS_NULL;

  ob_retort err, err2;
  slaw_output f;

  err = slaw_output_open_text (filename, &f);
  if (err != OB_OK)
    return err;

  err = slaw_output_write (f, s);
  err2 = slaw_output_close (f);

  return (err ? err : err2);
}

ob_retort slaw_to_string (bslaw sl, slaw *str)
{
  return slaw_to_string_options (sl, str, NULL);
}

ob_retort slaw_to_string_options_f (bslaw s, slaw *str, slaw options)
{
  ob_retort err = slaw_to_string_options (s, str, options);
  slaw_free (options);
  return err;
}

static ob_retort my_rewind (FILE *f)
{
  errno = 0;
  rewind (f);
  const int e = errno;
  if (0 == e)
    return OB_OK;
  else
    return ob_errno_to_retort (e);
}

ob_retort slaw_input_open (const char *filename, slaw_input *f)
{
  ob_retort err;
  FILE *ff = ob_fopen_cloexec (filename, "rb");
  if (!ff)
    return ob_errno_to_retort (errno);

  // a slaw binary file always starts with two bytes of 0xff,
  // but a yaml file never can.  (see comment at top of slaw-io.c)
  unt16 ffff = 0;
  if (1 == fread (&ffff, 2, 1, ff) && ffff == 0xffff)
    {
      err = my_rewind (ff);
      if (err >= OB_OK)
        err = slaw_input_open_binary_x (ff, f);
      if (err < OB_OK)
        fclose (ff);
      return err;
    }

  err = my_rewind (ff);
  if (err >= OB_OK)
    err = slaw_input_open_text_x (ff, f);
  if (err < OB_OK)
    fclose (ff);
  return err;
}

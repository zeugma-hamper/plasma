
/* (c)  oblong industries */

#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-file.h"
#include "libLoam/c/ob-util.h"
#include "libLoam/c/ob-sys.h"
#include "libPlasma/c/protein.h"
#include "libPlasma/c/slaw-io.h"
#include "libPlasma/c/slaw-string.h"

static ob_retort read_callback (void *cookie, byte *buffer, size_t size,
                                size_t *size_read)
{
  FILE *f = (FILE *) cookie;
  *size_read = fread (buffer, 1, size, f);
  if (ferror (f))
    return ob_errno_to_retort (errno);
  else
    return OB_OK;
}

static ob_retort write_callback (void *cookie, const byte *buffer, size_t size)
{
  FILE *f = (FILE *) cookie;
  if (size != fwrite (buffer, 1, size, f))
    return ob_errno_to_retort (errno);
  else
    return OB_OK;
}

static ob_retort flush_callback (void *cookie)
{
  FILE *f = (FILE *) cookie;
  if (0 != fflush (f))
    return ob_errno_to_retort (errno);
  else
    return OB_OK;
}

static ob_retort close_callback (void *cookie)
{
  FILE *f = (FILE *) cookie;
  if (0 != fclose (f))
    return ob_errno_to_retort (errno);
  else
    return OB_OK;
}

static ob_retort nop_callback (OB_UNUSED void *cookie)
{
  return OB_OK;
}

typedef struct
{
  int fd;
  bool close_fd;
  byte yaml_hack_state;
  byte yaml_hack_cached_byte;
} fd_info_t;

/* So what is this horrible hack, and why did I have to do it?
 *
 * This hack takes the first read (and only the first read) from a
 * file descriptor, and splits it in two.  So if the first read reads
 * n bytes, it will be synthetically split to look like a read of
 * n-1 bytes, followed by a read of 1 byte.
 *
 * The reason is because of a very minor and subtle bug in libyaml,
 * which mostly never causes any trouble, but which does cause
 * trouble for us if we're trying to fix bug 2525.
 *
 * Basically, before libyaml will return the first yaml document
 * (i. e. protein), it insists on doing two reads.  It doesn't matter
 * how many bytes were returned by the first read; it still wants to
 * do a second one.  (It has to do with a bug in how the way it detects
 * character encoding interacts with the way it refills its buffer, in
 * case you care.)
 *
 * Suppose 100 bytes are currently available to be read from the file
 * descriptor, and the first yaml document is contained entirely within
 * those 100 bytes.  Libyaml will ask us to do a read, we'll return 100
 * bytes (everything currently available), and even though libyaml now
 * has the complete first document, because of the bug, it will insist
 * on doing another read, and since no more bytes are available, the
 * read will block.  Which is not what we want; we want the first
 * document to be returned, since it was fully available on the input.
 *
 * So, to fake libyaml out, we only return 99 bytes of the 100 we read,
 * and we save the last byte.  Then, when libyaml does the second read,
 * we say we read one byte (without actually calling read again), and
 * return the one byte we cached.
 *
 * It is only necessary to do this once per file descriptor, because it
 * has to do with how character encoding is detected, which is only
 * done once per stream.  So after we do our little hack the first time,
 * the rest of the reads happen normally.
 */

enum
{
  YAML_HACK_STATE_NORMAL,
  YAML_HACK_STATE_CACHED_BYTE,
  YAML_HACK_STATE_SHOULD_DO_HACK
};

static ob_retort fd_read_callback (void *cookie, byte *buffer, size_t size,
                                   size_t *size_read)
{
  fd_info_t *fdi = (fd_info_t *) cookie;
  if (fdi->yaml_hack_state == YAML_HACK_STATE_CACHED_BYTE)
    {
      if (size < 1)  // don't think that can happen, but what if it did?
        {
          *size_read = 0;
          return OB_OK;
        }
      *buffer = fdi->yaml_hack_cached_byte;
      OB_INVALIDATE (fdi->yaml_hack_cached_byte);
      *size_read = 1;
      fdi->yaml_hack_state = YAML_HACK_STATE_NORMAL;
      return OB_OK;
    }
  ssize_t ss = read (fdi->fd, buffer, size);
  if (ss < 0)
    return ob_errno_to_retort (errno);
  if (ss > 1 && fdi->yaml_hack_state == YAML_HACK_STATE_SHOULD_DO_HACK)
    {
      ss--;
      fdi->yaml_hack_cached_byte = buffer[ss];
      fdi->yaml_hack_state = YAML_HACK_STATE_CACHED_BYTE;
    }
  *size_read = ss;
  return OB_OK;
}

static OB_UNUSED ob_retort fd_write_callback (void *cookie, const byte *buffer,
                                              size_t size)
{
  int fd = ((fd_info_t *) cookie)->fd;
  ssize_t ss = write (fd, buffer, size);
  if (ss < 0)
    return ob_errno_to_retort (errno);
  else if (ss != size) /* on files in blocking mode, shouldn't happen? */
    return OB_UNKNOWN_ERR;
  else
    return OB_OK;
}

static ob_retort fd_close_callback (void *cookie)
{
  fd_info_t *fdi = (fd_info_t *) cookie;
  ob_retort tort = OB_OK;
  if (fdi->close_fd && close (fdi->fd) < 0)
    tort = ob_errno_to_retort (errno);
  free (cookie);
  return tort;
}

typedef ob_retort (*close_func_t) (void *cookie);

static inline close_func_t get_close_func (bool close_file)
{
  return (close_file ? close_callback : nop_callback);
}

static inline slaw_read_handler get_read_handler (FILE *ff, bool close_file)
{
  slaw_read_handler rh;
  rh.read = read_callback;
  rh.close = get_close_func (close_file);
  rh.cookie = ff;
  return rh;
}

static inline slaw_write_handler get_write_handler (FILE *ff, bool close_file)
{
  slaw_write_handler wh;
  wh.write = write_callback;
  wh.flush = flush_callback;
  wh.close = get_close_func (close_file);
  wh.cookie = ff;
  return wh;
}

static slaw_read_handler get_fd_read_handler (int fd, bool close_file)
{
  slaw_read_handler rh;
  rh.read = fd_read_callback;
  rh.close = fd_close_callback;
  rh.cookie = calloc (1, sizeof (fd_info_t));
  fd_info_t *fdi = (fd_info_t *) rh.cookie;
  fdi->fd = fd;
  fdi->close_fd = close_file;
  fdi->yaml_hack_state = YAML_HACK_STATE_SHOULD_DO_HACK;
  return rh;
}

static inline ob_retort slaw_input_open_binary_xz (FILE *ff, slaw_input *f,
                                                   bool close_file)
{
  return slaw_input_open_binary_handler (get_read_handler (ff, close_file), f);
}

ob_retort slaw_input_open_binary_x (FILE *file, slaw_input *f)
{
  return slaw_input_open_binary_xz (file, f, true);
}

ob_retort slaw_input_open_binary_z (FILE *file, slaw_input *f)
{
  return slaw_input_open_binary_xz (file, f, false);
}

/* So here's the thing.  The binary file code doesn't expect partial reads,
 * so using fd_read_callback could confuse it.  Also, even if it wasn't
 * for that problem, the binary file code doesn't do its own buffering,
 * so using read() instead of fread() could be inefficient.  For these
 * reasons, we always use fread() for binary slaw files.  Therefore, if
 * given an fd, we use fdopen() to get a FILE* and then use the FILE*
 * code path.  Since fclose() closes the underlying file descriptor
 * (and not doing fclose() would leak the FILE*), if the user doesn't
 * want us to close their descriptor, we have to dup it before fdopening it.
 *
 * The YAML parser, on the other hand, does its own buffering and doesn't
 * mind partial reads, so for YAML files we use fd_read_callback when we
 * are given a file descriptor.
 */

ob_retort slaw_input_open_binary_fdx (int fd, slaw_input *f)
{
  FILE *ff = fdopen (fd, "rb");
  if (!ff)
    return ob_errno_to_retort (errno);
  return slaw_input_open_binary_x (ff, f);
}

ob_retort slaw_input_open_binary_fdz (int fd, slaw_input *f)
{
  int x = dup (fd);
  if (x < 0)
    return ob_errno_to_retort (errno);
  return slaw_input_open_binary_fdx (x, f);
}

static inline ob_retort slaw_output_open_binary_xz (FILE *file, slaw_output *f,
                                                    bool close_file)
{
  return slaw_output_open_binary_handler (get_write_handler (file, close_file),
                                          NULL, f);
}

ob_retort slaw_output_open_binary_x (FILE *file, slaw_output *f)
{
  return slaw_output_open_binary_xz (file, f, true);
}

ob_retort slaw_output_open_binary_z (FILE *file, slaw_output *f)
{
  return slaw_output_open_binary_xz (file, f, false);
}

static inline ob_retort slaw_input_open_text_xz (FILE *ff, slaw_input *f,
                                                 bool close_file)
{
  return slaw_input_open_text_handler (get_read_handler (ff, close_file), f);
}

ob_retort slaw_input_open_text_x (FILE *file, slaw_input *f)
{
  return slaw_input_open_text_xz (file, f, true);
}

ob_retort slaw_input_open_text_z (FILE *file, slaw_input *f)
{
  return slaw_input_open_text_xz (file, f, false);
}

static inline ob_retort slaw_output_open_text_options_xz (FILE *file,
                                                          slaw_output *f,
                                                          bslaw options,
                                                          bool close_file)
{
  return slaw_output_open_text_handler (get_write_handler (file, close_file),
                                        options, f);
}

static ob_retort slaw_input_open_text_fdxz (int fd, slaw_input *f,
                                            bool close_file)
{
  return slaw_input_open_text_handler (get_fd_read_handler (fd, close_file), f);
}

ob_retort slaw_input_open_text_fdx (int fd, slaw_input *f)
{
  return slaw_input_open_text_fdxz (fd, f, true);
}

ob_retort slaw_input_open_text_fdz (int fd, slaw_input *f)
{
  return slaw_input_open_text_fdxz (fd, f, false);
}

ob_retort slaw_output_open_text_options_x (FILE *file, slaw_output *f,
                                           bslaw options)
{
  return slaw_output_open_text_options_xz (file, f, options, true);
}

ob_retort slaw_output_open_text_options_z (FILE *file, slaw_output *f,
                                           bslaw options)
{
  return slaw_output_open_text_options_xz (file, f, options, false);
}


typedef struct
{
  const char *str;
  size_t pos;
  size_t len;
} string_input_t;

static ob_retort str_read_callback (void *cookie, byte *buffer, size_t size,
                                    size_t *size_read)
{
  string_input_t *sit = (string_input_t *) cookie;
  size_t len = sit->len - sit->pos;
  if (len > size)
    len = size;
  memcpy (buffer, sit->str + sit->pos, len);
  sit->pos += len;
  *size_read = len;
  return OB_OK;
}

ob_retort slaw_from_string (const char *str, slaw *ret)
{
  if (!str || !ret)
    return OB_ARGUMENT_WAS_NULL;

  string_input_t sit;
  sit.str = str;
  sit.pos = 0;
  sit.len = strlen (str);

  slaw_read_handler rh;
  rh.read = str_read_callback;
  rh.close = nop_callback;
  rh.cookie = &sit;

  slaw_input f = NULL;
  ob_retort tort = slaw_input_open_text_handler (rh, &f);
  if (tort < OB_OK)
    return tort;

  tort = slaw_input_read (f, ret);
  if (tort < OB_OK)
    {
      slaw_input_close (f);
      return tort;
    }

  tort = slaw_input_close (f);
  if (tort < OB_OK)
    {
      // don't think this should happen, but not 100% sure what to do
      // if it does...
      Free_Slaw (*ret);
    }

  return tort;
}

static ob_retort str_write_callback (void *cookie, const byte *buffer,
                                     size_t size)
{
  slabu *sb = (slabu *) cookie;
  slaw s = slaw_string_from_substring ((const char *) buffer, size);
  return (s && slabu_list_add_x (sb, s) >= 0) ? OB_OK : OB_NO_MEM;
}

ob_retort slaw_to_string_options (bslaw s, slaw *ret, bslaw options)
{
  if (!s || !ret)
    return OB_ARGUMENT_WAS_NULL;

  slabu *sb = slabu_new ();
  if (!sb)
    return OB_NO_MEM;

  slaw_write_handler wh;
  wh.write = str_write_callback;
  wh.flush = nop_callback;
  wh.close = nop_callback;
  wh.cookie = sb;

  // make "comment" default to false, but allow the user to override it
  // in their options map
  if (slaw_is_protein (options))
    options = protein_ingests (options);
  slaw no_comment = slaw_map_inline_cf ("comment", slaw_boolean (false), NULL);
  slaw opts = slaw_maps_merge (no_comment, options, NULL);
  Free_Slaw (no_comment);

  slaw_output f = NULL;
  ob_retort tort = slaw_output_open_text_handler (wh, opts, &f);
  Free_Slaw (opts);
  if (tort < OB_OK)
    {
      slabu_free (sb);
      return tort;
    }

  tort = slaw_output_write (f, s);
  ob_err_accum (&tort, slaw_output_close (f));

  if (tort >= OB_OK)
    {
      *ret = slaw_strings_join_slabu (sb, NULL);
      if (!*ret)
        tort = OB_NO_MEM;
    }

  slabu_free (sb);
  return tort;
}

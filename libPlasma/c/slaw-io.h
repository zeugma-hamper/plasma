
/* (c)  oblong industries */

#ifndef SLAW_IO_ERROR
#define SLAW_IO_ERROR

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-retorts.h"

#include "libPlasma/c/slaw.h"
#include "libPlasma/c/plasma-retorts.h"

#include <stdio.h> /* for FILE* */

#ifdef __cplusplus
extern "C" {
#endif

struct slaw_input_struct;
/**
 * handle to a slaw input stream
 */
typedef struct slaw_input_struct *slaw_input;

struct slaw_output_struct;
/**
 * handle to a slaw output stream
 */
typedef struct slaw_output_struct *slaw_output;

/**
 * The _x and _z versions of the open functions have similar semantics
 * to _x and _z for slaw_list_add, except the memory management semantics
 * apply to the FILE* rather than a slaw.  Specifically, if you open with
 * _x, your FILE* will be fclosed when you call slaw_input_close, but
 * if you open with _z, your FILE* will be left open by slaw_input_close.
 */

/* Read slawx from a file */
/**
 *                  Opens a binary slaw file for reading.
 *
 * \param[in]       filename is the name of the file to open.
 *
 * \param[out]      f is the location that receives the newly opened
 *                  "slaw input handle".
 *
 * \return          OB_OK if successful, or else another error code
 */
OB_PLASMA_API ob_retort slaw_input_open_binary (const char *filename,
                                                slaw_input *f);
OB_PLASMA_API ob_retort slaw_input_open_binary_x (FILE *file, slaw_input *f);
OB_PLASMA_API ob_retort slaw_input_open_binary_z (FILE *file, slaw_input *f);
OB_PLASMA_API ob_retort slaw_input_open_text (const char *filename,
                                              slaw_input *f);
OB_PLASMA_API ob_retort slaw_input_open_text_x (FILE *file, slaw_input *f);
OB_PLASMA_API ob_retort slaw_input_open_text_z (FILE *file, slaw_input *f);

// Like above, but take file descriptor instead of FILE*.
// TODO: Doxygenate
OB_PLASMA_API ob_retort slaw_input_open_binary_fdx (int fd, slaw_input *f);
OB_PLASMA_API ob_retort slaw_input_open_binary_fdz (int fd, slaw_input *f);
OB_PLASMA_API ob_retort slaw_input_open_text_fdx (int fd, slaw_input *f);
OB_PLASMA_API ob_retort slaw_input_open_text_fdz (int fd, slaw_input *f);

/**
 * Auto-detects text or binary.  No _x or _z versions of this, because it needs
 * to be able to rewind the stream, thus needs to be an actual file on disk.
 */
OB_PLASMA_API ob_retort slaw_input_open (const char *filename, slaw_input *f);

/**                 Reads a slaw from a "slaw input handle" which
 *                  was previously opened with slaw_input_open_binary
 *                  or slaw_input_open_text.
 *
 * \param[in]       f is the handle to read from.
 *
 * \param[out]      s is the location that receives the newly allocated
 *                  slaw read from the file.
 *
 * \return          OB_OK if successful, or else another error code
 */
OB_PLASMA_API ob_retort slaw_input_read (slaw_input f, slaw *s);

/**                 Closes a "slaw input handle" which was
 *                  previously opened with slaw_input_open_binary
 *                  or slaw_input_open_text.
 *
 * \param[in]       f is the handle to close.
 *
 * \return          OB_OK if successful, or else another error code
 */
OB_PLASMA_API ob_retort slaw_input_close (slaw_input f);

/* Write slawx to a file */
/**                 Opens a binary file into which slawx can be written.
 *                  The file has an 8-byte header that contains a magic
 *                  number to identify it as a binary slaw file.
 *
 * \param[in]       filename is the name of the file to open.
 *
 * \param[out]      f is the location that receives the newly opened
 *                  "slaw output handle".
 *
 * \return          OB_OK if successful, or else another error code
 */
OB_PLASMA_API ob_retort slaw_output_open_binary (const char *filename,
                                                 slaw_output *f);
OB_PLASMA_API ob_retort slaw_output_open_binary_x (FILE *file, slaw_output *f);
OB_PLASMA_API ob_retort slaw_output_open_binary_z (FILE *file, slaw_output *f);
OB_PLASMA_API ob_retort slaw_output_open_text (const char *filename,
                                               slaw_output *f);
OB_PLASMA_API ob_retort slaw_output_open_text_x (FILE *file, slaw_output *f);
OB_PLASMA_API ob_retort slaw_output_open_text_z (FILE *file, slaw_output *f);

/**                 Writes a slaw to a "slaw output handle" which
 *                  was previously opened with slaw_output_open_binary
 *                  or slaw_output_open_text.
 *
 * \param[in]       f is the handle to write to.
 *
 * \param[in]       s is the slaw to write.
 *
 * \return          OB_OK if successful, or else another error code
 */
OB_PLASMA_API ob_retort slaw_output_write (slaw_output f, bslaw s);

/**
 *                  Closes a "slaw output handle" which was
 *                  previously opened with slaw_output_open_binary
 *                  or slaw_output_open_text.
 *
 * \param[in]       f is the handle to close.
 *
 * \return          OB_OK if successful, or else another error code
 */
OB_PLASMA_API ob_retort slaw_output_close (slaw_output f);

/* Read single-slaw files */
/**
 *                  Reads a slaw from a file that was written with
 *                  slaw_write_to_binary_file.  Automatically converts
 *                  endianness if necessary.
 *
 * \param[in]       filename is the name of the file to read from
 *
 * \param[out]      s is assigned the newly allocated slaw, if successful
 *
 * \return          OB_OK if successful, or else an error code
 */
OB_PLASMA_API ob_retort slaw_read_from_binary_file (const char *filename,
                                                    slaw *s);
OB_PLASMA_API ob_retort slaw_read_from_text_file (const char *filename,
                                                  slaw *s);
OB_PLASMA_API ob_retort slaw_read_from_file (const char *filename, slaw *s);

/**
 *                  Given a string containing a YAML representation of
 *                  a slaw, construct the slaw that it represents.
 *                  If the string contains more than one YAML document,
 *                  returns the slaw represented by the first YAML document.
 *
 * \param[in]       str is the NUL-terminated UTF-8 string of YAML to parse
 *
 * \param[out]      s is the location that receives the new slaw
 *
 * \return          OB_OK if successful, or else another error code
 */
OB_PLASMA_API ob_retort slaw_from_string (const char *str, slaw *s);

/** Write YAML to a string (allocated as a slaw string) */
OB_PLASMA_API ob_retort slaw_to_string (bslaw s, slaw *str);

/* Write single-slaw files */
/**
 *                  Creates a binary slaw file containing a single slaw.
 *
 * \param[in]       filename is the name of the file to write to
 *
 * \param[in]       s is the slaw to write to the file
 *
 * \return          OB_OK if successful, or else an error code
 */
OB_PLASMA_API ob_retort slaw_write_to_binary_file (const char *filename,
                                                   bslaw s);
OB_PLASMA_API ob_retort slaw_write_to_text_file (const char *filename, bslaw s);

/**
 * Versions of the above that take an options map.
 * Currently, the keys are:
 * - "tag_numbers", which takes a boolean,
 *   and defaults to true.  Set it to false if you don't need
 *   full fidelity, and would rather not have all your numbers tagged.
 * - "directives", which takes a boolean, and defaults to true.  Set it
 *   to false if you don't want to emit the %YAML and %TAG directives.
 *   This makes the tag name more verbose, and the YAML version unspecified.
 * - "ordered_maps", which takes a boolean, and defaults to true.
 *   If true, encodes slaw maps as Yaml's !!omap.  If false, encodes slaw
 *   maps as Yaml's !!map (which means the ordering might be lost).
 * - "comment", which takes a boolean, and defaults to true for the
 *   file-based functions, but false for the string-based functions.
 *   If true, starts the file with a comment which includes the g-speak
 *   and libYaml version numbers, and a mode comment to prevent emacs
 *   from going into idlwave-mode for files with a .pro extension.
 * - "max-array-elements", which takes an int64, and defaults to -1.
 *   If not -1, arrays will be truncated after this many elements are
 *   printed.  This makes the file non-round-trippable.
 */
OB_PLASMA_API ob_retort slaw_output_open_text_options (const char *filename,
                                                       slaw_output *f,
                                                       bslaw options);
OB_PLASMA_API ob_retort slaw_output_open_text_options_f (const char *filename,
                                                         slaw_output *f,
                                                         slaw options);
/**
 * To avoid explosion of functions, the _x and _z don't have a version
 * that frees the options; you'll need to free them yourself.
 * XXX: It could be confusing that the _x and _z apply to "file", but
 * the _f applies to "options".
 */
OB_PLASMA_API ob_retort slaw_output_open_text_options_x (FILE *file,
                                                         slaw_output *f,
                                                         bslaw options);
OB_PLASMA_API ob_retort slaw_output_open_text_options_z (FILE *file,
                                                         slaw_output *f,
                                                         bslaw options);
OB_PLASMA_API ob_retort slaw_to_string_options (bslaw s, slaw *str,
                                                bslaw options);
OB_PLASMA_API ob_retort slaw_to_string_options_f (bslaw s, slaw *str,
                                                  slaw options);

// advanced stuff - specify a handler callback instead of a FILE*

typedef struct
{
  ob_retort (*read) (void *cookie, byte *buffer, size_t size,
                     size_t *size_read);
  ob_retort (*close) (void *cookie);
  void *cookie;
} slaw_read_handler;

typedef struct
{
  ob_retort (*write) (void *cookie, const byte *buffer, size_t size);
  ob_retort (*flush) (void *cookie);
  ob_retort (*close) (void *cookie);
  void *cookie;
} slaw_write_handler;

OB_PLASMA_API ob_retort slaw_input_open_binary_handler (slaw_read_handler h,
                                                        slaw_input *f);
OB_PLASMA_API ob_retort slaw_input_open_text_handler (slaw_read_handler h,
                                                      slaw_input *f);
OB_PLASMA_API ob_retort slaw_output_open_binary_handler (slaw_write_handler h,
                                                         bslaw options,
                                                         slaw_output *f);
OB_PLASMA_API ob_retort slaw_output_open_text_handler (slaw_write_handler h,
                                                       bslaw options,
                                                       slaw_output *f);

#ifdef __cplusplus
}
#endif

#endif /* SLAW_IO_ERROR */

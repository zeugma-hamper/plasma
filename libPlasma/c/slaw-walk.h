
/* (c)  oblong industries */

#ifndef SLAW_WALK_DOG
#define SLAW_WALK_DOG

/*
  There are a lot of different slaw types, and a lot of macros to test
  for those different types.  If you get a slaw of unknown type and want
  to write code that does something meaningful with it, it can take a
  lot of code.

  The first thing that occurred to me was that the Visitor pattern would
  be the perfect way to walk through a slaw.  However, implementing the
  Visitor pattern in a non-object-oriented language can be cumbersome.
  Also, some of the things you would want to visit (like an individual
  array element) are not full-fledged slaw in their own right.
  Therefore, I took a slightly different approach, which still has some
  of the feel of a Visitor, but which would probably be more accurately
  described as an event interface.  You call slaw_walk()
  to walk through all of the nested structure of any slaw
  or protein, and as it visits each element, it will call function
  pointers that you supply.

  This then makes it very convenient to write code to print out an
  arbitrary slaw in whatever format you want.  (For example,
  slaw_write_to_text_file() uses slaw_walk().)  It also might
  be helpful when importing proteins into another environment, like
  Ruby.

  The opposite service is also provided.  slaw_fabrication_handler is a
  slaw_handler which contructs a new slaw, based on the exact
  same function calls that slaw_walk uses.  This makes it possible to
  build a pluggable library to translate slaw to and from various
  formats.  (Currently only YAML is supported, but it's easy to imagine
  others.)
 */

#include "libPlasma/c/slaw.h"
#include "libLoam/c/ob-api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The length hint is either the size of the list or array (or whatever)
 * if known, or -1 if not known. */
typedef ob_retort (*slaw_callback_with_length_hint) (void *, int64);

typedef ob_retort (*slaw_callback_with_array_hint) (void *, int64, int);

typedef struct slaw_handler
{
  // slaw callbacks
  ob_retort (*begin_cons) (void *cookie);
  ob_retort (*end_cons) (void *cookie);
  ob_retort (*begin_map) (void *cookie, int64 length_hint);
  ob_retort (*end_map) (void *cookie);
  ob_retort (*begin_list) (void *cookie, int64 length_hint);
  ob_retort (*end_list) (void *cookie);
  /* bits_hint is the number of bits per array element, if they are
   * plain old numbers, or -1 if the array elements are vectors, multivectors,
   * or complex, or if this information is not known. */
  ob_retort (*begin_array) (void *cookie, int64 length_hint, int bits_hint);
  ob_retort (*end_array) (void *cookie);
  ob_retort (*begin_multivector) (void *cookie, int64 length_hint);
  ob_retort (*end_multivector) (void *cookie);
  ob_retort (*begin_vector) (void *cookie, int64 length_hint);
  ob_retort (*end_vector) (void *cookie);
  ob_retort (*begin_complex) (void *cookie);
  ob_retort (*end_complex) (void *cookie);
  ob_retort (*handle_nil) (void *cookie);
  // s is UTF-8 string.  len is length of string in *bytes*, not codepoints,
  // and not counting any terminating NUL.
  ob_retort (*handle_string) (void *cookie, const char *s, int64 len);
  ob_retort (*handle_int) (void *cookie, int64 val, int bits);
  /* boolean is treated as a 1-bit unsigned integer */
  ob_retort (*handle_unt) (void *cookie, unt64 val, int bits);
  ob_retort (*handle_float) (void *cookie, float64 val, int bits);
  ob_retort (*handle_empty_array) (void *cookie, int vecsize, bool isMVec,
                                   bool isComplex, bool isUnsigned,
                                   bool isFloat, int bits);

  // protein callbacks
  ob_retort (*begin_protein) (void *cookie);
  ob_retort (*end_protein) (void *cookie);
  ob_retort (*handle_nonstd_protein) (void *cookie, const void *p, int64 len);
  ob_retort (*begin_descrips) (void *cookie);
  ob_retort (*end_descrips) (void *cookie);
  ob_retort (*begin_ingests) (void *cookie);
  ob_retort (*end_ingests) (void *cookie);
  ob_retort (*handle_rude_data) (void *cookie, const void *d, int64 len);

} slaw_handler;

struct slabu_chain;
typedef struct slabu_chain slabu_chain;

typedef struct slaw_fabricator
{
  slaw result;
  slabu_chain *stack;
} slaw_fabricator;

OB_PLASMA_API extern const slaw_handler slaw_fabrication_handler;

/**                 Recursively walks through a slaw data structure, calling
 *                  one of the specified handler functions for each thing
 *                  encountered in the slaw.  This is inspired by, and
 *                  has some similarities to, a Visitor pattern that one
 *                  would use in an object-oriented language, although
 *                  there are some notable differences, too.
 *
 * \param[in]       cookie is an arbitrary user-supplied pointer that is
 *                  passed to the handler functions
 *
 * \param[in]       handler is a structure containing function pointers
 *                  to handle the various events which can occur while
 *                  walking the slaw.  The handler function can return
 *                  OB_OK to continue the walk, or any other value to
 *                  stop and propagate the error code up to the caller.
 *
 * \param[in]       victim is the slaw to walk
 *
 * \return          OB_OK if successful, or else an error code
 *                  (which might come from a handler function, or might
 *                  be internally generated).
 */
OB_PLASMA_API ob_retort slaw_walk (void *cookie, const slaw_handler *handler,
                                   bslaw victim);

/**                 Allocates a new slaw_fabricator, which should be passed
 *                  as the cookie when using slaw_fabrication_handler.
 *                  Most of the fields of slaw_fabricator should be treated
 *                  as private, but the "result" field
 *                  is where the user should get their slaw that
 *                  was built up by the walk.
 *
 * \return          the new slaw_fabricator, or NULL on failure
 */
OB_PLASMA_API slaw_fabricator *slaw_fabricator_new (void);

/**                 Frees a slaw_fabricator that was allocated with
 *                  slaw_fabricator_new, including everything it points to.
 *                  (This means if you don't want it to free result,
 *                  you need to NULL it out.)
 *
 * \param[in]       sf is the slaw_fabricator to free
 *
 * \return          None
 */
OB_PLASMA_API void slaw_fabricator_free (slaw_fabricator *sf);

#ifdef __cplusplus
}
#endif

#endif /* SLAW_WALK_DOG */

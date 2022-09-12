
/* (c)  oblong industries */

#ifndef PRIVATE_VERSIONING_ELECTROPHOTOMICROGRAPHICALLY
#define PRIVATE_VERSIONING_ELECTROPHOTOMICROGRAPHICALLY

#include "libPlasma/c/slaw.h"
#include "libPlasma/c/slaw-walk.h"
#include "libPlasma/c/slaw-interop.h"
#include "libPlasma/c/slaw-io.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum slaw_type {
  SLAW_TYPE_NULL,  // a NULL pointer; not a slaw at all
  SLAW_TYPE_NIL,
  SLAW_TYPE_BOOLEAN,
  SLAW_TYPE_STRING,
  SLAW_TYPE_NUMERIC,  // arrays, non-arrays, vectors, you name it
  SLAW_TYPE_CONS,
  SLAW_TYPE_LIST,  // includes maps, too
  SLAW_TYPE_PROTEIN,
  SLAW_TYPE_UNKNOWN
} slaw_type;

typedef struct slaw_vfuncs
{
  slaw_type (*vslaw_gettype) (bslaw s);
  bslaw (*vslaw_list_emit_first) (bslaw s);
  bslaw (*vslaw_list_emit_next) (bslaw s_list, bslaw s_prev);
  bool (*vslaw_is_map) (bslaw s);
  bool (*vslaw_is_numeric_array) (bslaw s);
  bool (*vslaw_is_numeric_multivector) (bslaw s);
  bool (*vslaw_is_numeric_vector) (bslaw s);
  bool (*vslaw_is_numeric_complex) (bslaw s);
  bool (*vslaw_is_numeric_float) (bslaw s);
  bool (*vslaw_is_numeric_int) (bslaw s);
  bool (*vslaw_is_numeric_unt) (bslaw s);
  bool (*vslaw_is_numeric_8) (bslaw s);
  bool (*vslaw_is_numeric_16) (bslaw s);
  bool (*vslaw_is_numeric_32) (bslaw s);
  bool (*vslaw_is_numeric_64) (bslaw s);
  int (*vslaw_numeric_vector_dimension) (bslaw s);
  int (*vslaw_numeric_unit_bsize) (bslaw s);
  int64 (*vslaw_numeric_array_count) (bslaw s);
  const void *(*vslaw_numeric_array_emit) (bslaw s);
  const void *(*vslaw_numeric_nonarray_emit) (bslaw s);
  bslaw (*vslaw_cons_emit_car) (bslaw s);
  bslaw (*vslaw_cons_emit_cdr) (bslaw s);
  const bool *(*vslaw_boolean_emit) (bslaw s);
  const char *(*vslaw_string_emit) (bslaw s);
  int64 (*vslaw_string_emit_length) (bslaw s);
  bool (*vprotein_is_nonstandard) (bprotein p);
  int64 (*vprotein_len) (bprotein p);  // or slaw len; same function
  bslaw (*vprotein_descrips) (bprotein p);
  bslaw (*vprotein_ingests) (bprotein p);
  const void *(*vprotein_rude) (bprotein prot, int64 *len);
  void (*vslaw_free) (slaw s);
  ob_retort (*vprotein_fix_endian) (protein p);
  ob_retort (*vslaw_swap) (slaw s, slaw stop);
  ob_retort (*vbuild) (slaw *s, const struct slaw_vfuncs *v);
  ob_retort (*vbinary_input_read) (slaw_read_handler h, bool needToSwap,
                                   slaw *s);
  int64 (*vslaw_list_length) (bslaw s);
} slaw_vfuncs;

ob_retort slaw_walk_versioned (void *cookie, const slaw_handler *handler,
                               bslaw victim, const slaw_vfuncs *v) OB_HIDDEN;

const slaw_vfuncs *get_vfuncs (slaw_version v) OB_HIDDEN;

extern const slaw_vfuncs slaw_v1funcs OB_HIDDEN;
extern const slaw_vfuncs slaw_v2funcs OB_HIDDEN;

#ifdef __cplusplus
}
#endif

#endif /* PRIVATE_VERSIONING_ELECTROPHOTOMICROGRAPHICALLY */

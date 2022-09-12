
/* (c)  oblong industries */

#ifndef POOL_OPTION_HELPERS
#define POOL_OPTION_HELPERS

#include "libPlasma/c/protein.h"
#define KILOBYTE OB_KILOBYTE
#define MEGABYTE OB_MEGABYTE
#define GIGABYTE OB_GIGABYTE
#define TERABYTE OB_TERABYTE

#define POOL_SIZE_SMALL (MEGABYTE)
#define POOL_SIZE_MEDIUM (10 * MEGABYTE)
#define POOL_SIZE_LARGE (100 * MEGABYTE)
#define POOL_SIZE_HUGE (2 * GIGABYTE)

#define OB_RESIZABLE_BY_DEFAULT true

#ifdef __cplusplus
extern "C" {
#endif


OB_PLASMA_API protein mmap_pool_options (unt64 size);
OB_PLASMA_API protein toc_mmap_pool_options (unt64 size, unt64 toc_capacity);
OB_PLASMA_API protein small_mmap_pool_options (void);
OB_PLASMA_API protein medium_mmap_pool_options (void);
OB_PLASMA_API protein large_mmap_pool_options (void);
OB_PLASMA_API protein obscene_mmap_pool_options (void);


OB_PLASMA_API unt64 mmap_pool_options_file_size (bprotein options);
OB_PLASMA_API unt64 mmap_pool_options_toc_capacity (bprotein options);

// For backwards compatibility (remove at some point in the future)
#define indexed_mmap_pool_options(s, c) toc_mmap_pool_options (s, c)
#define mmap_pool_options_index_capacity(o) mmap_pool_options_toc_capacity (o)

#ifdef __cplusplus
}
#endif

#endif

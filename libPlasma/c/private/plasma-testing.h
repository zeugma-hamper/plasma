
/* (c)  oblong industries */

// These functions represent a halfway-ness of private-ness, in that
// they are used by the tests, because the tests need some additional
// bit of visibility or tweakability, but they are private as far as
// normal applications are concerned.
//
// (But because they are called from the test applications which
// link against the plasma DLL, they need to be declared OB_PLASMA_API.)

#ifndef PLASMA_TESTING_ANIMAL
#define PLASMA_TESTING_ANIMAL

#include "libLoam/c/ob-api.h"
#include "libPlasma/c/pool.h"
#include "libPlasma/c/slaw-io.h"

#ifdef __cplusplus
extern "C" {
#endif

OB_PLASMA_API void private_test_yaml_hash (void);

typedef struct
{
  int fd;
  const char *description;
} fd_and_description;

// For testing purposes only, fill fd[] with various file descriptors
// used by the open hose.  Extra ones up to nfd are filled with -1.
OB_PLASMA_API void
private_get_file_descriptors (pool_hose ph, fd_and_description *fd, int nfd);

#ifdef __cplusplus
}
#endif

#endif /* PLASMA_TESTING_ANIMAL */


/* (c)  oblong industries */

#ifndef PLASMAXX_RETORTS_WITTY
#define PLASMAXX_RETORTS_WITTY


#include <libLoam/c/ob-retorts.h>


// We have 100000 retorts to play with, from OB_RETORTS_PLASMAXX_FIRST
// to OB_RETORTS_PLASMAXX_LAST.

// ---------- failure codes (negative) ----------

/**
 * Returned when calling Hose methods on a Hose which has not
 * been configured.
 */
#define OB_HOSE_NOT_CONFIGURED -(OB_RETORTS_PLASMAXX_FIRST + 0)

// ---------- success codes (positive) ----------

// (none)

#endif /* PLASMAXX_RETORTS_WITTY */

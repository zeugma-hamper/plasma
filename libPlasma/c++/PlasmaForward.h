
/* (c)  oblong industries */

#ifndef PLASMA_FORWARD_THINKING
#define PLASMA_FORWARD_THINKING

// This macro is needed in slaw-traits.h, PlasmaStreams.h, and Slaw.cpp,
// so this is a convenient place to put it.
#define OB_FOR_ALL_NUMERIC_TYPES(M)                                            \
  M (int8);                                                                    \
  M (int16);                                                                   \
  M (int32);                                                                   \
  M (int64);                                                                   \
  M (unt8);                                                                    \
  M (unt16);                                                                   \
  M (unt32);                                                                   \
  M (unt64);                                                                   \
  M (float32);                                                                 \
  M (float64)

namespace oblong {
namespace plasma {
class Hose;
class HoseGang;
class Pool;
class ObRetort_DepositInfo;
class Slaw;
class SlawIterator;
class Protein;
struct OStreamReference;
namespace detail {
class CompositeSlaw;
class SlawRef;
}
}
}  // ending oblong::plasma::detail

#endif /* PLASMA_FORWARD_THINKING */

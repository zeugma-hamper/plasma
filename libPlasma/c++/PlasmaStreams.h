
/* (c)  oblong industries */

#ifndef PLASMA_STREAMS_BED
#define PLASMA_STREAMS_BED

#include <libLoam/c/ob-api.h>
#include <libLoam/c/ob-types.h>
#include "libPlasma/c++/PlasmaForward.h"
#include <iosfwd>

namespace oblong {
namespace plasma {
namespace detail {
OB_PLASMAXX_API::std::ostream &
operator<< (::std::ostream &os,
            const oblong::plasma::detail::CompositeSlaw &cs);

OB_PLASMAXX_API::std::ostream &
operator<< (::std::ostream &os, const oblong::plasma::detail::SlawRef &ref);

}  // ending detail

OB_PLASMAXX_API::std::ostream &operator<< (::std::ostream &os,
                                           const oblong::plasma::Protein &p);

OB_PLASMAXX_API::std::ostream &
operator<< (::std::ostream &os, const oblong::plasma::SlawIterator &it);

OB_PLASMAXX_API::std::ostream &operator<< (::std::ostream &os,
                                           const oblong::plasma::Slaw &s);
}
}  // ending oblong::plasma

#define DECLARE_VOSTREAM(T)                                                    \
  OB_PLASMAXX_API::std::ostream &operator<< (::std::ostream &os,               \
                                             const v2##T &v);                  \
  OB_PLASMAXX_API::std::ostream &operator<< (::std::ostream &os,               \
                                             const v3##T &v);                  \
  OB_PLASMAXX_API::std::ostream &operator<< (::std::ostream &os,               \
                                             const v4##T &v);

OB_FOR_ALL_NUMERIC_TYPES (DECLARE_VOSTREAM);

#undef DECLARE_VOSTREAM

namespace oblong {
namespace plasma {

/**
 * It is forbidden (with echo sound effect like in Superman) to
 * include any C++ I/O headers, even harmless little \<iosfwd\>,
 * in "normal" Oblong headers like Slaw.h.  So, we take a
 * Plessy v. Ferguson approach and segregate all mention of
 * std::ostream into this header, PlasmaStreams.h.
 *
 * However, the problem is that Jao long ago gave Slaw this method:
 *
 * \code
 *   void Spew (\::std::ostream &os)  const;
 * \endcode
 *
 * So, in order to remain backwards-compatible with that method,
 * while retroactively purging \<iosfwd\> from Slaw.h, we have to
 * resort to a little trick.  We change the method to instead be:
 *
 * \code
 *   void Spew (OStreamReference os)  const;
 * \endcode
 *
 * Where OStreamReference is forward-declared in PlasmaForward.h.
 * Then, if you actually want to call that method, you include
 * PlasmaStream.h, which defines OStreamReference to be a simple
 * wrapper around a reference to \::std::ostream, and most importantly,
 * with a constructor that will automatically wrap the ostream for you.
 * Therefore, due to the wonders of C++, you can continue to call
 * the method just like you used to, even though the signature has
 * changed.  This double-indirection-of-forward-declarations seems
 * silly, but then the lengths to which Oblong goes to avoid the
 * standard C++ library are often silly.
 */
struct OStreamReference
{
  ::std::ostream &os;
  OStreamReference (::std::ostream &osin) : os (osin) {}
};
}
}  // namespace oblong::plasma

#endif /* PLASMA_STREAMS_BED */

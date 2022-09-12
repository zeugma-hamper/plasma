
/* (c)  oblong industries */

#ifndef LOAM_STREAMS_BANK
#define LOAM_STREAMS_BANK


#include <libLoam/c/ob-api.h>
#include "libLoam/c++/LoamForward.h"
#include <iosfwd>


namespace oblong {
namespace loam {


OB_LOAMXX_API::std::ostream &operator<< (::std::ostream &os,
                                         const oblong::loam::Str &s);

OB_LOAMXX_API::std::ostream &operator<< (::std::ostream &os,
                                         const oblong::loam::StrIterator &);

OB_LOAMXX_API::std::ostream &operator<< (::std::ostream &os,
                                         const oblong::loam::ObRetort &ret);

OB_LOAMXX_API::std::ostream &operator<< (::std::ostream &os,
                                         const oblong::loam::Vect &v);

OB_LOAMXX_API::std::ostream &operator<< (::std::ostream &os,
                                         const oblong::loam::Vect4 &v);

OB_LOAMXX_API::std::ostream &operator<< (::std::ostream &os,
                                         const oblong::loam::Quat &q);

OB_LOAMXX_API::std::ostream &operator<< (::std::ostream &os,
                                         const oblong::loam::Matrix44 &m);

OB_LOAMXX_API::std::ostream &operator<< (::std::ostream &,
                                         const oblong::loam::ObColor &);
}
}  // the end? well, yes, if you're namespace loam and then namespace oblong


#endif /* LOAM_STREAMS_BANK */

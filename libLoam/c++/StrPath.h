
/* (c) oblong industries */

#ifndef I_TOOK_THE_PATH_LESS_FOLLOWED_NAMELY_UTF16
#define I_TOOK_THE_PATH_LESS_FOLLOWED_NAMELY_UTF16

#include <libLoam/c++/Str.h>

#include <boost/filesystem.hpp>

namespace oblong {
namespace loam {

OB_LOAMXX_API Str ConvertToStr (const boost::filesystem::path &path);

OB_LOAMXX_API boost::filesystem::path ConvertToPath (const Str &str);
}
}


#endif  //I_TOOK_THE_PATH_LESS_FOLLOWED_NAMELY_UTF16

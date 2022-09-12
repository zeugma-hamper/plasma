#include <libLoam/c++/StrPath.h>

#include <unicode/unistr.h>

namespace oblong {
namespace loam {

Str ConvertToStr (const boost::filesystem::path &path)
{
  return oblong::loam::Str{
    icu::UnicodeString (path.native ().c_str (), path.size ())};
}

boost::filesystem::path ConvertToPath (const Str &str)
{
#if defined _MSC_VER
  const wchar_t *buf =
    reinterpret_cast<const wchar_t *> (str.ICUUnicodeString ().getBuffer ());
  int32_t len = str.ICUUnicodeString ().length ();
  return {buf, buf + len};
#else
  return {str.utf8 ()};
#endif
}
}
}

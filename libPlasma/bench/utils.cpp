
/* (c)  oblong industries */

#include "utils.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/protein.h"
#include "libPlasma/c/pool_cmd.h"

#include <algorithm>

namespace oblong {
namespace plasma {
namespace bench {

Strings Tokenize (const ::std::string &str, const ::std::string &delimiters)
{
  Strings result;
  ::std::string::size_type last = str.find_first_not_of (delimiters, 0);
  ::std::string::size_type pos = str.find_first_of (delimiters, last);

  while (::std::string::npos != pos || ::std::string::npos != last)
    {
      result.push_back (str.substr (last, pos - last));
      last = str.find_first_not_of (delimiters, pos);
      pos = str.find_first_of (delimiters, last);
    }
  return result;
}

void AddProtein (Proteins &p, unt64 size, const char *descrip)
{
  slaw descrips = slaw_list_inline_f (slaw_string (descrip), NULL);
  ::std::string s (size, 'p');
  slaw ingests = slaw_map_inline_cc ("payload", s.c_str (), NULL);
  p.push_back (protein_from_ff (descrips, ingests));
}

bool AddProteins (Proteins &p, const char *str)
{
  Strings strs (Tokenize (str, ","));
  for (unt64 i = 0; i < strs.size (); ++i)
    {
      Strings sd (Tokenize (strs[i], "/"));
      const unt64 n = pool_cmd_parse_size (sd[0].c_str ());
      if (n == 0)
        return false;
      const char *desc = sd.size () > 1 ? sd[1].c_str () : NULL;
      AddProtein (p, n, desc);
    }
  return strs.size () > 0;
}

Sizes ProteinSizes (const Proteins &p)
{
  Sizes s;
  for (unt64 i = 0; i < p.size (); ++i)
    s.push_back (slaw_len (p[i]));
  return s;
}

void FreeProteins (const Proteins &p)
{
  ::std::for_each (p.begin (), p.end (), protein_free);
}
}
}
}  // namespace oblong::plasma::bench

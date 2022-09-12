
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_BENCH_UTILS_H
#define OBLONG_PLASMA_BENCH_UTILS_H

#include <string>
#include <vector>

#include "libPlasma/c/protein.h"

namespace oblong {
namespace plasma {
namespace bench {

typedef ::std::vector<::std::string> Strings;
typedef ::std::vector<protein> Proteins;
typedef ::std::vector<unt64> Sizes;

Strings Tokenize (const ::std::string &str, const ::std::string &delims);


void AddProtein (Proteins &, unt64 size, const char *descrip);
bool AddProteins (Proteins &, const char *str);
Sizes ProteinSizes (const Proteins &);
void FreeProteins (const Proteins &);
}
}
}  // namespace oblong::plasma::bench


#endif  // OBLONG_PLASMA_BENCH_UTILS_H

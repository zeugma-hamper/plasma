
/* (c)  oblong industries */

#include <iostream>
#include "libLoam/c++/ObInfo.h"
#include "libLoam/c++/Str.h"

using namespace oblong::loam;

int main (void)
{
  std::cout << ObInfo::ObInfoToYAML ().utf8 () << std::endl;
  return 0;
}

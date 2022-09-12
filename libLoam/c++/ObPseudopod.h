
/* (c)  oblong industries */

#ifndef OB_PSEUDOPOD_POLYPLIKENESS
#define OB_PSEUDOPOD_POLYPLIKENESS


#include <libLoam/c++/AnkleObject.h>


namespace oblong {
namespace loam {


class OB_LOAMXX_API ObPseudopod : public AnkleObject
{
  PATELLA_SUBCLASS (ObPseudopod, AnkleObject);

 public:
  ObPseudopod ();
  ~ObPseudopod () override;
};
}
}  // bagpipes & Amazing Grace for namespaces loam and oblong


#endif

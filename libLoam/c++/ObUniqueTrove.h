
/* (c)  oblong industries */

#ifndef OB_UNIQUE_TROVE_CASKET
#define OB_UNIQUE_TROVE_CASKET


#include <libLoam/c++/ObTrove.h>


namespace oblong {
namespace loam {


/**
 * This is a trove in which all elements must be unique. (Other folks
 * might call it an ordered set.)
 */
template <typename ELEMType,
          template <typename DUMMY> class MEM_MGR_CLASS = UnspecifiedMemMgmt>
class ObUniqueTrove : public ObTrove<ELEMType, MEM_MGR_CLASS>
{
//
// again, a boost over the wall...
//
#define WITH_MEM_MANAGEMENT_SENSITIVITY
#include "OT_Helpy.h"
#undef WITH_MEM_MANAGEMENT_SENSITIVITY

 public:  // after this, things are public, publicly speaking.
  ObUniqueTrove () : ObTrove<ELEMType, MEM_MGR_CLASS> () {}

  ObUniqueTrove (const ObTrove<ELEMType, MEM_MGR_CLASS> &otha)
      : ObTrove<ELEMType, MEM_MGR_CLASS> (otha)
  {
  }

  ObUniqueTrove (const ObUniqueTrove &otha)
      : ObTrove<ELEMType, MEM_MGR_CLASS> (otha)
  {
  }

  ObUniqueTrove (ObUniqueTrove &&otha)
      : ObTrove<ELEMType, MEM_MGR_CLASS> (std::move (otha))
  {
  }

  ObUniqueTrove &operator= (const ObUniqueTrove &otha)
  {
    ObTrove<ELEMType, MEM_MGR_CLASS>::operator= (otha);
    return *this;
  }

  ObUniqueTrove &operator= (ObUniqueTrove &&otha) noexcept
  {
    ObTrove<ELEMType, MEM_MGR_CLASS>::operator= (std::move (otha));
    return *this;
  }

  /* Note: the "this ->" is required by Standard C++, and is
   * enforced by gcc 4.7 and up, and by clang.  See bug 2438, or:
   * http://blog.llvm.org/2009/12/dreaded-two-phase-name-lookup.html
   */
  ObRetort Append (ARGType elem) override
  {
    if (this->Find (elem) >= 0)
      return OB_ALREADY_PRESENT;
    return ObTrove<ELEMType, MEM_MGR_CLASS>::Append (elem);
  }

  // Avoid wrath of -Werror=overloaded-virtual by letting compiler find
  // other Append signatures in parent
  using ObTrove<ELEMType, MEM_MGR_CLASS>::Append;

  ObRetort Insert (ARGType elem, int64 ind) override
  {
    if (this->Find (elem) >= 0)
      return OB_ALREADY_PRESENT;
    return ObTrove<ELEMType, MEM_MGR_CLASS>::Insert (elem, ind);
  }

  ObRetort ReplaceNth (int64 ind, ARGType elem) override
  {
    if (this->Find (elem) >= 0)
      return OB_ALREADY_PRESENT;
    return ObTrove<ELEMType, MEM_MGR_CLASS>::ReplaceNth (ind, elem);
  }

  ObRetort ExpandUsingDefaultValTo (OB_UNUSED int64 num) override
  {
    return OB_INVALID_OPERATION;
  }
};
}
}  // an end to namespace loam, and that scurrilous namespace oblong


#endif

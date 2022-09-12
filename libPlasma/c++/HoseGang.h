
/* (c)  oblong industries */

#ifndef HOSE_GANG_HEADER_GUARD
#define HOSE_GANG_HEADER_GUARD


#include "Hose.h"

#include <libLoam/c/ob-api.h>

#include <libLoam/c++/ObTrove.h>


namespace oblong {
namespace plasma {
using namespace oblong::loam;


/**
 * A multiplexed collection of input Hoses. Instances of this class
 * mangage a collection of Hoses, called @e tributaries, and allows
 * waiting for the next protein on any of them (similarly to select
 * calls in unix environments).
 *
 * Tributaries can be appended and removed on the fly, but they should
 * be used outside the gang with care, since Hose instances are not
 * thread safe.
 *
 * @ingroup PlasmaPools
 */
class OB_PLASMAXX_API HoseGang : public AnkleObject
{
  PATELLA_SUBCLASS (HoseGang, AnkleObject);

 protected:
  ObTrove<Hose *> hoses;
  pool_gang pg;
  ObRetort last_retort;
  Str name;

 public:
  HoseGang ();
  ~HoseGang () override;

  /**
   * Retrieve the next protein available from one of the hoses in the
   * gang
   */
  virtual Protein Next (pool_timestamp timeout = Hose::WAIT);

  virtual const Str &Name () { return name; }
  virtual ObRetort SetName (const Str &nm)
  {
    name = nm;
    return OB_OK;
  }

  /**
   * Add an existing hose to a gang
   */
  virtual ObRetort AppendTributary (Hose *hose);
  /**
   * Create a hose connected to a named pool and, if successful,
   * add it to the gang
   */
  virtual ObRetort AppendTributary (const Str &pool_name);

  /**
   * Remove an existing hose from a gang
   */
  virtual ObRetort RemoveTributary (Hose *hose);
  /**
   * Find by name a hose in the gang and remove it if it exists
   * Returns OB_NOT_FOUND if no such hose is a member of the gang
   */
  virtual ObRetort RemoveTributary (const Str &pool_name);

  /**
   * The number of hoses currently in the gang
   */
  virtual int64 NumTributaries ();
  /**
   * The nth hose in the gang
   */
  virtual Hose *NthTributary (int64 index);
  /**
   * Find a hose in the gang by name
   */
  virtual Hose *FindTributary (const Str &pool_name);

  // virtual ObCrawl <nHose *> CrawlTributaries ();

  /**
   * A signal-safe and thread-safe function to interrupt any call
   * to Next() on this gang.
   */

  /**
   * For each time that this function is called, one call to Next()
   * will return with a pool_retort of POOL_AWAIT_WOKEN.  Unlike
   * bare pool hoses, all gangs have wakeup enabled.  Thus there is
   * no need for an equivalent of Hose::EnableWakeup() for gangs.
   */
  virtual ObRetort WakeUp ();
};
}
}  // end of namespaces plasma and oblong


#endif

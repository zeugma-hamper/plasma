
/* (c)  oblong industries */

#include "HoseGang.h"


using namespace oblong::plasma;


HoseGang::HoseGang () : last_retort (OB_OK), name ("")
{
  pool_new_gang (&pg);
  // fix: can this fail; what should we do?
}

HoseGang::~HoseGang ()
{
  pool_disband_gang (pg, false);
}


// note: don't do too much here or you will not be threadsafe wrt WakeUp()
// (ie: don't use pg for anything outside of pool_await_next_multi)
Protein HoseGang::Next (pool_timestamp timeout)
{
  // protein prot;
  // pool_hose ph;
  // pool_timestamp ts;
  // int64 idx;

  // last_retort = pool_await_next_multi (pg, timeout, &ph, &prot, &ts, &idx);
  // if (last_retort < 0)
  //   return Protein ();

  Protein result (Protein::Null ());
  pool_timestamp stamp (Protein::NO_TIME);
  int64 idx (Protein::NO_INDEX);
  protein p (NULL);
  pool_hose hose (NULL);

  ObRetort ret = pool_await_next_multi (pg, timeout, &hose, &p, &stamp, &idx);
  if (ret.IsSplend ())
    result = Protein (Slaw (p), stamp, idx, hose);
  return result;
}


ObRetort HoseGang::AppendTributary (Hose *hose)
{
  // Don't know what this comment was babbling about, but dynamic casting
  // a Hose * to a Hose * is clearly going to give you the same thing you
  // started with, and is thus pointless.
  //
  // // we do some runtime type checking to see whether we know how to
  // // add this hose to this gang. child classes can do this
  // // differently, if they are so inclined. see the ruby implementation
  // // for a more extensible approach (which could be implemented here).
  // Hose *hoss = dynamic_cast <Hose *> (hose);

  if (!hose)
    return OB_ARGUMENT_WAS_NULL;

  ob_retort ret = pool_join_gang (pg, hose->RawHose ());
  if (ret < 0)
    return ret;

  hoses.Append (hose);
  return OB_OK;
}

ObRetort HoseGang::AppendTributary (const Str &pool_name)
{
  Hose *h = new Hose (pool_name);
  if (!h->IsConfigured ())
    {
      ObRetort ret = h->LastRetort ();
      h->Delete ();
      return ret;
    }

  ObRetort ret = AppendTributary (h);
  if (ret.IsError ())
    h->Delete ();
  return ret;
}

ObRetort HoseGang::RemoveTributary (Hose *hose)
{
  if (!hose)
    return OB_ARGUMENT_WAS_NULL;

  ObRetort ret = hoses.Remove (hose);
  if (ret == OB_NOT_FOUND)
    return ret;

  return pool_leave_gang (pg, hose->RawHose ());
}

ObRetort HoseGang::RemoveTributary (const Str &hose_name)
{
  Hose *h = FindTributary (hose_name);
  if (!h)
    return OB_NOT_FOUND;
  return RemoveTributary (h);
}


int64 HoseGang::NumTributaries ()
{
  return hoses.Count ();
}

Hose *HoseGang::NthTributary (int64 index)
{
  return hoses.Nth (index);
}

Hose *HoseGang::FindTributary (const Str &hose_name)
{
  // fix: refactor this ... naming should be part and parcel of all
  // hose interfaces, yes?
  Hose *h = NULL;
  ObCrawl<Hose *> cr = hoses.Crawl ();
  while (!cr.isempty ())
    {
      Hose *hoss = cr.popfore ();
      if (hoss && (hoss->Name () == hose_name))
        {
          h = hoss;
          break;
        }
    }
  return h;
}

ObRetort HoseGang::WakeUp ()
{
  return pool_gang_wake_up (pg);
}

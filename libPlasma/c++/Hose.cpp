
/* (c)  oblong industries */

#include "Hose.h"


using namespace oblong::plasma;


const pool_timestamp Hose::WAIT = POOL_WAIT_FOREVER;
const pool_timestamp Hose::NO_WAIT = POOL_NO_WAIT;


Hose::Hose (const Str &pool_name) : ph (NULL)
{
  is_configured = false;

  last_retort = pool_participate (pool_name.utf8 (), &ph, NULL);
  if (last_retort.IsError ())
    return;

  is_configured = true;
}

Hose::Hose (const Hose &other) : ph (NULL)
{
  last_retort = pool_hose_clone (other.ph, &ph);
  if (last_retort.IsError ())
    is_configured = false;
  else
    is_configured = true;
}

Hose::Hose (pool_hose hose)
{
  ph = hose;
  if (!hose)
    is_configured = false;
  else
    is_configured = true;
}


Hose::~Hose ()
{
  if (is_configured)
    pool_withdraw (ph);
}


Hose &Hose::operator= (const Hose &other)
{
  if (this != &other)
    {
      Hose tmp (other);
      swap (tmp);
    }
  return *this;
}

void Hose::swap (Hose &other)
{
  if (this != &other)
    {
      ::std::swap (ph, other.ph);
      ::std::swap (is_configured, other.is_configured);
      ::std::swap (last_retort, other.last_retort);
    }
}


ObRetort Hose::Withdraw ()
{
  if (is_configured)
    {
      is_configured = false;
      last_retort = pool_withdraw (ph);
      return last_retort;
    }
  else
    return OB_HOSE_NOT_CONFIGURED;
}


ObRetort_DepositInfo Hose::Deposit (const Protein &prot)
{
  if (!is_configured)
    return ObRetort_DepositInfo (OB_HOSE_NOT_CONFIGURED);

  int64 idx;
  pool_timestamp stamp;
  ob_retort ret = pool_deposit_ex (ph, prot.ProteinValue (), &idx, &stamp);
  last_retort = ret;
  if (ret < 0)
    return ObRetort_DepositInfo (ret);
  return ObRetort_DepositInfo (ret, idx, stamp);
}


Protein Hose::Current ()
{
  if (!is_configured)
    return Protein ();

  Protein result (Protein::Null ());
  protein p (NULL);
  pool_timestamp stamp (Protein::NO_TIME);

  last_retort = pool_curr (ph, &p, &stamp);
  if (last_retort.IsSplend ())
    result = Protein (Slaw (p), stamp, CurrentIndex (), ph);
  return result;
}


Protein Hose::Next (pool_timestamp timeout)
{
  if (!is_configured)
    return Protein ();

  Protein result (Protein::Null ());
  protein p (NULL);
  pool_timestamp stamp (Protein::NO_TIME);
  int64 idx (Protein::NO_INDEX);

  last_retort = pool_await_next (ph, timeout, &p, &stamp, &idx);
  if (last_retort.IsSplend ())
    result = Protein (Slaw (p), stamp, idx, ph);
  return result;
}


Protein Hose::Previous ()
{
  if (!is_configured)
    return Protein ();

  Protein result (Protein::Null ());
  protein p (NULL);
  pool_timestamp stamp (Protein::NO_TIME);
  int64 idx (Protein::NO_INDEX);

  last_retort = pool_prev (ph, &p, &stamp, &idx);
  if (last_retort.IsSplend ())
    result = Protein (Slaw (p), stamp, idx, ph);
  return result;
}


Protein Hose::Nth (int64 idx)
{
  if (!is_configured)
    return Protein ();

  Protein result (Protein::Null ());
  protein p (NULL);
  pool_timestamp stamp (Protein::NO_TIME);

  last_retort = pool_nth_protein (ph, idx, &p, &stamp);
  if (last_retort.IsSplend ())
    result = Protein (Slaw (p), stamp, idx, ph);
  return result;
}


Protein Hose::ProbeForward (const Slaw &search, pool_timestamp timeout)
{
  if (!is_configured)
    return Protein ();

  Protein result (Protein::Null ());
  bslaw s = search.SlawValue ();
  protein p (NULL);
  pool_timestamp stamp (Protein::NO_TIME);
  int64 idx (Protein::NO_INDEX);

  last_retort = pool_await_probe_frwd (ph, s, timeout, &p, &stamp, &idx);
  if (last_retort.IsSplend ())
    result = Protein (Slaw (p), stamp, idx, ph);
  return result;
}

Protein Hose::ProbeBackward (const Slaw &search)
{
  if (!is_configured)
    return Protein ();

  Protein result (Protein::Null ());
  bslaw s = search.SlawValue ();
  protein p (NULL);
  pool_timestamp stamp (Protein::NO_TIME);
  int64 idx (Protein::NO_INDEX);

  last_retort = pool_probe_back (ph, s, &p, &stamp, &idx);
  if (last_retort.IsSplend ())
    result = Protein (Slaw (p), stamp, idx, ph);
  return result;
}


ObRetort Hose::EnableWakeup ()
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;
  return pool_hose_enable_wakeup (ph);
}

ObRetort Hose::WakeUp ()
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;
  return pool_hose_wake_up (ph);
}


int64 Hose::CurrentIndex ()
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  int64 index;
  last_retort = pool_index (ph, &index);
  if (last_retort >= 0)
    return index;
  return -1;
}

int64 Hose::OldestIndex ()
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  int64 index;
  last_retort = pool_oldest_index (ph, &index);
  if (last_retort >= 0)
    return index;
  return -1;
}

int64 Hose::NewestIndex ()
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  int64 index;
  last_retort = pool_newest_index (ph, &index);
  if (last_retort >= 0)
    return index;
  return -1;
}


ObRetort Hose::SeekTo (int64 index)
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  last_retort = pool_seekto (ph, index);
  return last_retort;
}

ObRetort Hose::SeekToTime (pool_timestamp timestamp, time_comparison bound)
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  last_retort = pool_seekto_time (ph, timestamp, bound);
  return last_retort;
}

ObRetort Hose::SeekBy (int64 offset)
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  last_retort = pool_seekby (ph, offset);
  return last_retort;
}

ObRetort Hose::SeekByTime (pool_timestamp lapse, time_comparison bound)
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  last_retort = pool_seekby_time (ph, lapse, bound);
  return last_retort;
}

ObRetort Hose::ToLast ()
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  last_retort = pool_tolast (ph);
  return last_retort;
}

ObRetort Hose::Runout ()
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  last_retort = pool_runout (ph);
  return last_retort;
}

ObRetort Hose::Rewind ()
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;

  last_retort = pool_rewind (ph);
  return last_retort;
}


Str Hose::PoolName () const
{
  if (!is_configured)
    return Str ();
  return pool_name (ph);
}


Str Hose::Name () const
{
  if (!is_configured)
    return Str ();
  return pool_get_hose_name (ph);
}

ObRetort Hose::SetName (const Str &name)
{
  if (!is_configured)
    return OB_HOSE_NOT_CONFIGURED;
  return pool_set_hose_name (ph, name.utf8 ());
}

ObRetort Hose::ResetName ()
{
  return SetName (PoolName ());
}


pool_hose Hose::RawHose ()
{
  return ph;
}

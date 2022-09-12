
/* (c)  oblong industries */

#ifndef HOSE_HEADER_GUARD
#define HOSE_HEADER_GUARD


#include "libLoam/c++/AnkleObject.h"
#include "libLoam/c++/ObRetort.h"

#include "libPlasma/c++/Protein.h"


namespace oblong {
namespace plasma {
using namespace oblong::loam;


/*
 * A retort returned by Hose::Deposit (). If the deposit succeeds, the
 * index and the timestamp fields are filled in. If not, they hold -1
 * and 0.0, respectively.
 *
 * @ingroup PlasmaPools
 */
class ObRetort_DepositInfo : public ObRetort
{
 public:
  int64 index;
  pool_timestamp timestamp;

  ObRetort_DepositInfo (ob_retort code)
      : ObRetort (code), index (-1), timestamp (0.0)
  {
  }

  ObRetort_DepositInfo (ob_retort code, int64 idx, pool_timestamp ts)
      : ObRetort (code), index (idx), timestamp (ts)
  {
  }

  ObRetort_DepositInfo (const ObRetort_DepositInfo &odi)
      : ObRetort (odi), index (odi.index), timestamp (odi.timestamp)
  {
  }

  ObRetort_DepositInfo (ObRetort_DepositInfo &&odi) noexcept
      : ObRetort (std::move (odi)), index (odi.index), timestamp (odi.timestamp)
  {
  }

  ObRetort_DepositInfo &operator= (const ObRetort_DepositInfo &odi)
  {
    if (this == &odi)
      return *this;

    this->ObRetort::operator= (odi);
    index = odi.index;
    timestamp = odi.timestamp;

    return *this;
  }

  ObRetort_DepositInfo &operator= (ObRetort_DepositInfo &&odi) noexcept
  {
    if (this == &odi)
      return *this;

    this->ObRetort::operator= (std::move (odi));
    index = odi.index;
    timestamp = odi.timestamp;

    return *this;
  }
};


/**
 * A Hose provides a read-write connection to a pool. Hoses are
 * AnkleObjects, and are therefore generally used as pointers, managed
 * by ObRef, ObTrove and friends.
 *
 * Typically a hose is obtained via a Pool factory method:
 *
 *   Hose *h = Pool::Participate ("some-pool");
 *   Protein prot = h->next ();
 *
 * @ingroup PlasmaPools
 */
class OB_PLASMAXX_API Hose : public AnkleObject
{
  PATELLA_SUBCLASS (Hose, AnkleObject);

 protected:
  pool_hose ph;
  bool is_configured;
  ObRetort last_retort;

 public:
  ~Hose () override;


  /**
   * Time value denoting no timeout when waiting for a Protein.
   */
  static const pool_timestamp WAIT;
  /**
   * Time value denoting immediate timeout when waiting for a Protein.
   */
  static const pool_timestamp NO_WAIT;


  /**
   * Hose constructors. If it's preferable to obtain a pool on the
   * stack rather than as a pointer on the heap (or if there's some
   * other reason not to use the Pool::Participate functions to
   * obtain a pool), the constructor taking a pool name is available.
   * The copy constructor and the explicit constructor taking a
   * c-level pool_hose may also prove useful.
   *
   * Each constructor attempts to create a Hose connected to an
   * appropriate pool (named pool lookup in the Str case, a clone of
   * the relevant pool_hose in the other two cases). Each sets
   * LastRetort () -- and sets IsConfigured () to false on failure,
   * true on success.
   */
  explicit Hose (const Str &pool_name);
  /** Takes ownership of \a hose; this is a new behavior. */
  explicit Hose (pool_hose hose);
  Hose (const Hose &other);

  /**
   * Convenience function cloning this Hose, with the same semantics as
   * the copy constructor.
   */
  Hose *Dup () const { return new Hose (*this); }

  /**
   * Assignment operator giving Hose value semantics. As in the case of
   * copy construction, a clone of the passed-in hose is created
   * (spawning a new connection), while the old one is closed.
   */
  Hose &operator= (const Hose &other);

  /**
   * The standard swap operator.
   */
  void swap (Hose &other);

  /**
   * A boolean giving an indication -- a best guess -- of whether
   * this Hose is properly set up and participating. If pool
   * construction fails, IsConfigured() returns false. Withdraw ()
   * sets IsConfigured () to false.
   *
   * Note that in the common case of using the Pool::Participate
   * functions, which return a NULL pointer on failure, there's never
   * any need to call IsConfigured ().
   */
  virtual bool IsConfigured () const { return is_configured; }

  /**
   * The result of the most recent operation that generates an
   * ObRetort.
   */
  virtual const ObRetort &LastRetort () const { return last_retort; }

  /**
   * Withdraws from the pool and sets IsConfigured () to false. Also
   * sets LastRetort () and returns that value. If the Hose is not
   * configured, returns OB_HOSE_NOT_CONFIGURED.
   */
  virtual ObRetort Withdraw ();

  /**
   * Deposits a protein. Sets LastRetort (). Returns an ObRetort
   * subclass.
   *
   *  ObRetort_DepositInfo ret = h->Deposit (foo);
   *    if (ret.IsError ())
   *      printf ("we couldn't deposit: %s\n", ret.Description().utf8());
   *    else
   *      printf ("ts/index: %d, %f\n", (int)ret.index, ret.timestamp);
   *
   *
   */
  virtual ObRetort_DepositInfo Deposit (const Protein &protein);

  /**
   * Fetches the next available protein in the pool, incrementing the
   * index pointer at the same time, so that you can Next (), Next (),
   * Next () nice and sequentially. The timeout argument specifies
   * how long, in fractional seconds, to wait before returning
   * without a Protein in hand. Two symbolic constants are available:
   * WAIT (which means to wait forever), and NO_WAIT (which
   * configures the call to return right away if no protein is
   * immediately available).
   *
   * If no protein was available, Next () returns a null
   * Protein. LastRetort () will provide further information about
   * what happened to prevent the fetch.
   *
   * If a Protein is returned, it will have a valid Index (),
   * Timestamp (), and Origin () data.
   *
   *  Protein prot = h->Next (1.0);
   *  if (prot.IsNull ())
   *    printf ("no protein available: %s\n",
   *            h->LastRetort().Description().utf8());
   *  else
   *    printf ("ts/index: %d, %f\n", (int)prot.Index (), prot.Timestamp ());
   *
   */
  virtual Protein Next (pool_timestamp timeout = WAIT);

  /**
   * Fetches the next available protein without incrementing the index
   * pointer. Successive calls to Current () will return the same
   * protein over and over unless and until the pool fills up and
   * wraps around. See Next () for a description of the returned
   * protein.
   *
   *  prot = h->Current ();
   *  if (prot.IsNull ())
   *    printf ("no protein available: %s\n",
   *            h->LastRetort().Description().utf8());
   *  else
   *    printf ("ts/index: %d, %f\n", (int)prot.Index (), prot.Timestamp ());
   *
   */
  virtual Protein Current ();

  /**
   * Fetches the previous protein, decrementing the index pointer such
   * that successive calls to Previous () walk backwards through the
   * pool. See Next () for a description of the returned protein.
   *
   *  prot = h->Previous ();
   *  if (prot.IsNull ())
   *    printf ("no protein available: %s\n",
   *            h->LastRetort().Description().utf8());
   *  else
   *    printf ("ts/index: %d, %f\n", (int)prot.Index (), prot.Timestamp ());
   *
   */
  virtual Protein Previous ();

  /**
   * Fetches the protein at the given index. See Next () for a description
   * of the returned protein and (analogous) usage patterns.
   */
  virtual Protein Nth (int64 idx);

  /**
   * Like Next (), except that it returns the next matching protein
   * instead of the next protein.  A protein matches if its descrips
   * are a list containing the needle slaw, `search'.  If `search' is
   * itself a list, the check succeeds if all of its elements are in a
   * protein's descrips, in order.  See the documentation for
   * protein_match (), for more information.
   *
   * When accessing remote pools, this method can be more efficient
   * than fetching all of the proteins and examining them locally, as
   * it can perform the match remotely.
   *
   * A timeout may be specified, as for Next ().
   *
   * If no protein was available, ProbeForward () returns a null
   * Protein. LastRetort () will provide further information about
   * what happened to prevent the fetch.
   *
   * If a Protein is returned, it will have a valid Index (),
   * Timestamp (), and Origin () data.
   *
   */
  virtual Protein ProbeForward (const Slaw &search,
                                pool_timestamp timeout = WAIT);

  /**
   * Like Previous (), except that it returns the previous matching
   * protein instead of the previous protein.  Matching is performed as
   * in ProbeForward ().  Unlike ProbeForward (), this method has no
   * timeout.
   *
   */
  virtual Protein ProbeBackward (const Slaw &search);

  /**
   * Allows the WakeUp() method to be called on this hose in the
   * future.  (It allocates a few additional file descriptors, which
   * is why it needs to be asked for explicitly.)  Unlike WakeUp()
   * itself, this method is not threadsafe, so you must call it before
   * you begin your multithreaded fun and games.  (Probably wisest to
   * just call it immediately after you create the hose, for any hoses
   * where you anticipate needing this feature.)
   */
  virtual ObRetort EnableWakeup ();

  /**
   * A threadsafe way to interrupt another thread which is waiting
   * in this hose's Next() method.  WakeUp() is the only method
   * which is safe to call when another thread is using the same hose.
   * In order to call WakeUp(), you must have previously called
   * the EnableWakeup() method on this hose.
   */
  virtual ObRetort WakeUp ();

  /**
   * Query the index pointer for this hose, and the index number of
   * the oldest and newest proteins currently found in the pool. Upon
   * connection to an empty pool, all of these methods would return
   * 0. The index sequence increases monotonically, so OldestIndex ()
   * will always be less than or equal to NewestIndex ().
   */
  virtual int64 CurrentIndex ();
  virtual int64 OldestIndex ();
  virtual int64 NewestIndex ();

  /**
   * Move around in the pool. SeekTo () and SeekToTime () move the
   * index pointer absolutely. By contrast, SeekBy () and SeekByTime ()
   * move it relatively. ToLast () sets the index pointer so that
   * Current () will return the very newest protein in the pool. Runout ()
   * sets the index pointer just past the end of the pool, so that a
   * call to Next () will wait, returning the next protein deposited by
   * another participant. Rewind () sets the index pointer so that
   * Current () will return the oldest protein in the pool.
   *
   */
  virtual ObRetort SeekTo (int64 index);
  virtual ObRetort SeekToTime (pool_timestamp timestamp, time_comparison bound);
  virtual ObRetort SeekBy (int64 offset);
  virtual ObRetort SeekByTime (pool_timestamp lapse, time_comparison bound);
  virtual ObRetort ToLast ();
  virtual ObRetort Runout ();
  virtual ObRetort Rewind ();

  /**
   * The name of the pool in which we are a participant. If this Hose
   * is not configured, the null Str is returned.
   */
  virtual Str PoolName () const;

  /**
   * The name of this individual hose, which defaults to the name of
   * the pool in which we are a participant, or the null Str if not
   * configured.
   */
  virtual Str Name () const;
  virtual ObRetort SetName (const Str &name);
  virtual ObRetort ResetName ();

  /**
   * The underlying, c-level pool_hose.
   */
  virtual pool_hose RawHose ();
};
}
}  //  end of namespaces plasma, oblong


#endif

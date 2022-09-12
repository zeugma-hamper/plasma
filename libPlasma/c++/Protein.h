
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_PROTEIN_H
#define OBLONG_PLASMA_PROTEIN_H


#include "Slaw.h"

#include <libLoam/c/ob-api.h>
#include <libPlasma/c/pool.h>
#include <libPlasma/c/protein.h>


namespace oblong {
namespace plasma {


/**
 * Proteins as a Slaw descrips plus a Slaw ingests. Access to the
 * protein's structure is therefore granted via the Slaw interfaces,
 * which keeps this class definition short and sweet. This class adds
 * to Slaw's protein interface access to metadata related to the hoses
 * originating the given protein.
 *
 * As with other Slaw containers, Protein instances can be constructed
 * from either a C-style object (protein) or by explicitly providing
 * its components in the form of separate Slaw values. In the former
 * case, this class takes care of disposing C-level resources when no
 * longer needed, and of maximising sharing without copying unless
 * needed.
 *
 * Protein instances are immutable objects with value semantics.
 *
 * @ingroup PlasmaSlaw
 */
class OB_PLASMAXX_API Protein
{
 public:
  ~Protein ();
  Protein (const Protein &);
  Protein &operator= (const Protein &);

  /**
   * @name Constructing new proteins.
   */
  //@{
  /**
   * An empty protein. Note that this is different from a null one,
   * which you can obtain via Null.
   */
  Protein ();

  /**
   * Constructs a new instance from data in a C-style protein. Since
   * C-level protein are also C-level slaw, this constructor checks
   * whether @a p is actually a protein. If it is not, @a p is
   * treated as either a descrip or, if it is a map, a series of
   * keyed ingests in the newly constructed protein. Remember that
   * the new Protein instance takes ownership of all @a p resources,
   * so you shouldn't use it for constructing any other Slaw or Slaw
   * container object, nor free its memory in any other way.
   * Thus, this will construct a protein with a single descrip:
   * @code
   *   Protein pr (slaw_string ("descrip"));
   *   assert (pr.Descrips ().IsList ());
   *   assert (pr.Descrips ().Count () == 0);
   *   assert (pr.Descrips ().Nth (0) == Slaw ("descrip"));
   * @endcode
   * while this will create a protein with descrips:
   * @code
   *   Protein pr (slaw_map_inline_cc ("a", "b", "c", "d", NULL));
   *   assert (pr.Descrips ().Count () == 0);
   *   Slaw ingests (pr.Ingests ());
   *   assert (ingests.IsMap ());
   *   assert (Slaw ("b") == ingests.Find ("a"));
   *   assert (Slaw ("d") == ingests.Find ("c"));
   *   assert (ingests.Count () == 2);
   * @endcode
   */
  explicit Protein (slaw p);

  /**
   * Constructs a Protein with the given Slaw, interpreting it as a
   * protein Slaw.
   */
  explicit Protein (Slaw slaw);

  /**
   * Constructs a Protein with the given @a descrips and @a ingests.
   */
  Protein (Slaw descrips, Slaw ingests);
  //@}

  /**
   * A Protein is null when its underlying C-protein (obtainable via
   * ProteinValue) is NULL. A null Protein is also empty.
   */
  bool IsNull () const;

  /**
   * A Protein is empty when both its descrips and ingests are null.
   */
  bool IsEmpty () const;

  /**
   * Constructor for a NULL Protein, distinct, mind you, from an
   * empty one (as constructed by Protein()).
   */
  static Protein Null ();

  /**
   * @name Equality
   * As per Slaw equality. That is, the index and position of the
   * Proteins is ignored to the effect of the comparison. Use
   */
  //@{
  bool operator== (const Protein &other) const;
  bool operator!= (const Protein &other) const;
  //@}

  /**
   * @name Protein structure accessors.
   */
  //@{
  /**
   * The list of Slawx that constitutes this protein's descrips.
   */
  Slaw Descrips () const;

  /**
   * A protein's ingests are a set of keyed Slaw. In other words, a
   * Slaw map, which is returned by this function.
   */
  Slaw Ingests () const;
  //@}

  /**
   * @name Transforming to other Slaw types.
   */
  //@{
  /**
   * View this Protein as a C-level object.
   */
  bprotein ProteinValue () const;

  /**
   * Explicit cast to a Slaw, possibly to make this protein part of
   * another Slaw.
   */
  Slaw ToSlaw () const;
  //@}

  /**
   * @name Descrips matching. These functions implement searches on
   * Protein descrips compliant with plasma/c semantics. Thus, no
   * coercion or reinterpretation of the descrips or the matching
   * needle is performed.
   */
  //@{
  /**
   * Descrips search. This function looks for @a needle in this
   * protein's descrips, using the plasma semantics. That means that
   * the search will only succeed if the descrips are a list (without
   * any coercion), in which case this functions returns:
   *     - if @a needle is a list, @c this->Descrips().IndexOfList(needle,false)
   *     - if @a needle is not a list, @c this->Descrips().IndexOf(needle)
   * If this protein's descrips are not a list, this function returns a
   * negative value.
   * The optional @a how argument works the same as in the C function
   * protein_search_ex().
   */
  int64 Search (Slaw needle, Protein_Search_Type how = SEARCH_GAP) const;

  /**
   * Works as Search(Slaw), only taking the needle as a bslaw.
   */
  int64 Search (bslaw needle, Protein_Search_Type how = SEARCH_GAP) const;

  /**
   * Convenience function checking if Search(needle) > -1
   */
  bool Matches (Slaw needle, Protein_Search_Type how = SEARCH_GAP) const;

  /**
   * Convenience function checking if Search(needle) > -1
   */
  bool Matches (bslaw needle, Protein_Search_Type how = SEARCH_GAP) const;
  //@}

  /**
   * @name Pool metadata
   */
  //@{
  /**
   * Proteins retrieved from pools come with an accompanying
   * timestamp, represented by this type.  This constant denotes an
   * undefined time.
   */
  static const pool_timestamp NO_TIME;

  /**
   * Proteins read from or written to pools come also with an
   * associated index. This constant denotes an undefined index.
   */
  static const int64 NO_INDEX;

  /**
   * Accessor to the pool timestamp of this protein.
   * The timestamp is the time at which the protein
   * was deposited into the pool.
   */
  pool_timestamp Timestamp () const;

  /**
   * The pool index at which this protein was retrieved or set.
   */
  int64 Index () const;

  /**
   * The Pool this protein comes from.
   */
  Str Origin () const;
  //@}

  /**
   * Debugging and testing aid.
   */
  void Spew (OStreamReference os) const;

  /**
   * Return a 64-bit hash code for this Protein.
   */
  unt64 Hash () const { return ToSlaw ().Hash (); }

  /**
   * Less than.
   */
  bool operator< (const Protein &other) const
  {
    return ToSlaw () < other.ToSlaw ();
  }

  /**
   * Greater than.
   */
  bool operator> (const Protein &other) const
  {
    return ToSlaw () > other.ToSlaw ();
  }

  /**
   * Less than or equal.
   */
  bool operator<= (const Protein &other) const
  {
    return ToSlaw () <= other.ToSlaw ();
  }

  /**
   * Greater than or equal.
   */
  bool operator>= (const Protein &other) const
  {
    return ToSlaw () >= other.ToSlaw ();
  }

 protected:
  friend class HoseGang;  // so it can use the following constructor...
  friend class Hose;      // ditto
  Protein (Slaw, pool_timestamp, int64, pool_hose);
  bool ComesFrom (pool_hose) const;

  Slaw protein_;
  pool_timestamp stamp_;
  int64 index_;
  pool_hose hose_;
};


// http://www.boost.org/doc/libs/1_47_0/doc/html/hash/custom.html
inline size_t hash_value (const Protein &s)
{
  return static_cast<size_t> (s.Hash ());
}

/**
 * A hash function wrapper that makes Protein easy to use in Boost or TR1
 * unordered maps.
 */
struct Protein_hash
{
  size_t operator() (const Protein &s) const
  {
    return static_cast<size_t> (s.Hash ());
  };
};
}
}  // namespace oblong::plasma

namespace std {

template <>
struct hash<oblong::plasma::Protein>
{
  typedef oblong::plasma::Protein argument_type;
  typedef std::size_t result_type;

  result_type operator() (const argument_type &p) const
  {
    return static_cast<size_t> (p.Hash ());
  }
};
}

#endif  // OBLONG_PLASMA_PROTEIN_H

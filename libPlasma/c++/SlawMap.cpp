
/* (c)  oblong industries */

#include "SlawMap.h"

#include "SlawCons.h"
#include "PlasmaStreams.h"

#include <libPlasma/c/protein.h>

#include <ostream>
#include <cassert>


namespace oblong {
namespace plasma {
namespace detail {

namespace {

void AddKV (SlawRefs &refs, SlawRef k, SlawRef v)
{
  if (!k.IsNull () && !v.IsNull ())
    {
      for (SRSize i = 0, s = refs.size (); i < s; i += 2)
        if (refs[i] == k)
          {
            refs[i + 1] = v;
            return;
          }
      refs.push_back (k);
      refs.push_back (v);
    }
}

SlawRefs UniqueKeys (const SlawRefs &cmps)
{
  // Quadratic algorithm, i know. But we're not expecting Maps with a
  // large number of entries; moreover, defining a cheap less<> for
  // slawx is not easy.

  const SRSize count (cmps.size ());
  SlawRefs result;

  if (0 != count)
    {
      const SRSize n = (count >> 1) << 1;
      result.reserve (n);
      for (SRSize i = 0; i < n; i += 2)
        AddKV (result, cmps[i], cmps[i + 1]);
    }

  return result;
}

bool LooksLikeAMap (const SlawRefs &refs)
{
  for (SRSize i = 0, s = refs.size (); i < s; ++i)
    if (!refs[i].IsNull () && !refs[i].IsCons ())
      return false;
  return true;
}

bool LooksLikeAMap (bslaw list)
{
  for (bslaw s = slaw_list_emit_first (list); s != NULL;
       s = slaw_list_emit_next (list, s))
    if (!slaw_is_cons (s))
      return false;
  return true;
}

SlawRefs ExtractPairs (SlawRef list)
{
  SlawRefs result;
  const int64 cnt (slaw_list_count (list));
  if (cnt > 0)
    result.reserve (2 * cnt);
  for (bslaw cons = slaw_list_emit_first (list); cons != NULL;
       cons = slaw_list_emit_next (list, cons))
    {
      result.push_back (SlawRef (slaw_cons_emit_car (cons), list));
      result.push_back (SlawRef (slaw_cons_emit_cdr (cons), list));
    }
  return result;
}

SlawRefs ExtractList (SlawRef list)
{
  SlawRefs result;
  const int64 cnt (slaw_list_count (list));
  if (cnt > 0)
    result.reserve (cnt);
  for (bslaw s = slaw_list_emit_first (list); s != NULL;
       s = slaw_list_emit_next (list, s))
    result.push_back (SlawRef (s, list));
  return result;
}

SlawRefs ExtractCons (SlawRef cons)
{
  SlawRefs result;
  result.push_back (SlawRef (slaw_cons_emit_car (cons), cons));
  result.push_back (SlawRef (slaw_cons_emit_cdr (cons), cons));
  return result;
}

SlawRefs ExtractIngests (SlawRef protein)
{
  return ExtractPairs (SlawRef (protein_ingests (protein), protein));
}

SlawRefs GetComponents (SlawRef ref)
{
  SlawRefs result;
  if (ref.IsProtein ())
    {
      result = ExtractIngests (ref);
    }
  if (ref.IsMap ())
    {
      result = ExtractPairs (ref);
    }
  else if (ref.IsList ())
    {
      result = UniqueKeys (LooksLikeAMap (ref) ? ExtractPairs (ref)
                                               : ExtractList (ref));
    }
  else if (ref.IsCons ())
    {
      result = ExtractCons (ref);
    }
  return result;
}

SlawRefs ExtractPairs (const SlawRefs &refs)
{
  SlawRefs result;
  for (SRSize i = 0, s = refs.size (); i < s; ++i)
    if (!refs[i].IsNull ())
      {
        result.push_back (SlawRef (slaw_cons_emit_car (refs[i]), refs[i]));
        result.push_back (SlawRef (slaw_cons_emit_cdr (refs[i]), refs[i]));
      }
  return result;
}

}  // namespace

/* ---------------------------------------------------------------------- */
// Construction

SlawMap::SlawMap (SlawRef ref) : cmps_ (GetComponents (ref))
{
  if (ref.IsMap ())
    pairs_ = SlawList (ref);
}

SlawMap::SlawMap (const SlawRefs &cs)
{
  if (LooksLikeAMap (cs))
    {
      SlawRefs tmp = UniqueKeys (ExtractPairs (cs));
      std::swap (tmp, cmps_.elements_);
    }
  else
    {
      SlawRefs tmp = UniqueKeys (cs);
      std::swap (tmp, cmps_.elements_);
    }
}

SlawMap::SlawMap (const Ref &base, SlawRef k, SlawRef v)
{
  SlawRefs kvs = UniqueKeys (base->KVList ());
  for (SlawIter i = kvs.begin (), e = kvs.end (); i != e; i += 2)
    {
      if (*i != k)
        {
          cmps_.Add (*i);
          cmps_.Add (*(i + 1));
        }
    }
  if (!k.IsNull () && !v.IsNull ())
    {
      cmps_.Add (k);
      cmps_.Add (v);
    }
}

SlawMap::SlawMap (const Ref &base, const Ref &other)
{
  SlawRefs kvs = other->KVList ();
  SlawRefs base_kvs = base->KVList ();
  kvs.insert (kvs.begin (), base_kvs.begin (), base_kvs.end ());
  kvs = UniqueKeys (kvs);
  for (SlawIter i = kvs.begin (), e = kvs.end (); i != e; ++i)
    cmps_.Add (*i);
}

/* ---------------------------------------------------------------------- */
// Type

bool SlawMap::IsMap () const
{
  return true;
}

bool SlawMap::IsNull () const
{
  return cmps_.IsNull ();
}

SlawRef SlawMap::AsSlaw () const
{
  slabu *sb = slabu_new ();
  // Make use of our "friend" status to avoid overhead of
  // calling SlawList's virtual functions.
  for (size_t i = 0, s = cmps_.elements_.size (); i < s; i += 2)
    {
      slabu_map_put (sb, cmps_.elements_[i], cmps_.elements_[i + 1]);
    }
  return SlawRef (slaw_map_f (sb));
}

bool SlawMap::IsEqual (const CompositeSlaw *other) const
{
  Sync ();
  return pairs_.Components () == other->Components ();
}

/* ---------------------------------------------------------------------- */
// List interface

int64 SlawMap::Count () const
{
  return cmps_.Count () / 2;
}

SlawRef SlawMap::Nth (int64 i) const
{
  Sync ();
  return pairs_.Nth (i);
}

SlawRefs SlawMap::Slice (int64 begin, int64 end) const
{
  Sync ();
  return pairs_.Slice (begin, end);
}

int64 SlawMap::IndexOf (bslaw s, unt64 start) const
{
  if (!slaw_is_cons (s))
    return -Count ();
  bslaw key = slaw_cons_emit_car (s);
  for (int64 i = 2 * start, n = cmps_.Count (); i < n; i += 2)
    {
      if (slawx_equal (cmps_.Nth (i), key))
        {
          if (slawx_equal (cmps_.Nth (i + 1), slaw_cons_emit_cdr (s)))
            return i / 2;
          else
            return -Count ();
        }
    }
  return -Count ();
}

/* ---------------------------------------------------------------------- */
// Map interface

SlawRef SlawMap::Find (bslaw key) const
{
  for (int64 i = 0, s = cmps_.Count (); i < s; i += 2)
    {
      if (slawx_equal (key, cmps_.Nth (i)))
        return cmps_.Nth (i + 1);
    }
  return SlawRef ();
}

unt64 SlawMap::KeyCount () const
{
  return Count ();
}

SlawRefs SlawMap::KVList () const
{
  return cmps_.Components ();
}

/* ---------------------------------------------------------------------- */
// Protein interface

SlawRefs SlawMap::IngestList () const
{
  return KVList ();
}

SlawRefs SlawMap::DescripList () const
{
  return SlawRefs ();
}

/* ---------------------------------------------------------------------- */
// Debugging

Str SlawMap::ToStr () const
{
  Sync ();
  return pairs_.ToStr ();
}

void SlawMap::Spew (OStreamReference os) const
{
  int64 len = cmps_.Count ();
  os.os << "#MAP(" << (len / 2) << ")<" << ::std::endl;
  for (int64 i = 0; i < len; i += 2)
    os.os << "KEY: " << cmps_.Nth (i) << ::std::endl
          << "VALUE: " << cmps_.Nth (i + 1) << ::std::endl;
  os.os << ">";
}

/* ---------------------------------------------------------------------- */
// Private

void SlawMap::Sync () const
{
  if (pairs_.Count () == 0)
    {
      SlawRefs lcs;
      const int64 N (cmps_.Count ());
      if (N > 0)
        lcs.reserve (N / 2);
      for (int64 i = 0; i < N; i += 2)
        lcs.push_back (SlawRef (slaw_cons (cmps_.Nth (i), cmps_.Nth (i + 1))));
      pairs_ = SlawList (lcs);
    }
}
}
}
}  // namespace oblong::plasma::detail

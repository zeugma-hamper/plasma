
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_DETAIL_SLAWLIST_H
#define OBLONG_PLASMA_DETAIL_SLAWLIST_H


#include "CompositeSlaw.h"


namespace oblong {
namespace plasma {
namespace detail {


class SlawList : public CompositeSlaw
{
 public:
  SlawList ();
  explicit SlawList (SlawRef list);
  explicit SlawList (const SlawRefs &elems);

  bool IsNull () const override;
  bool IsList () const override;
  SlawRef AsSlaw () const override;
  SlawRefs KVList () const override;
  unt64 KeyCount () const override;
  int64 Count () const override;
  SlawRef Nth (int64) const override;
  SlawRefs Slice (int64, int64) const override;
  SlawRef Find (bslaw key) const override;
  int64 IndexOf (bslaw s, unt64 start) const override;
  Str ToStr () const override;
  void Spew (OStreamReference os) const override;

  void Add (SlawRef elem);

 private:
  bool IsEqual (const CompositeSlaw *other) const override;
  SlawRefs DescripList () const override;
  SlawRefs IngestList () const override;

  SlawRefs elements_;

  friend class SlawMap;
};
}
}
}  // namespace oblong::plasma::detail


#endif  // OBLONG_PLASMA_DETAIL_SLAWLIST_H


/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_DETAIL_SLAWMAP_H
#define OBLONG_PLASMA_DETAIL_SLAWMAP_H


#include "SlawList.h"


namespace oblong {
namespace plasma {
namespace detail {


class SlawCons;


class SlawMap : public CompositeSlaw
{
 public:
  explicit SlawMap (SlawRef map);
  explicit SlawMap (const SlawRefs &cs);
  SlawMap (const Ref &base, const Ref &other);
  SlawMap (const Ref &base, SlawRef k, SlawRef v);

  bool IsNull () const override;
  bool IsMap () const override;

  SlawRef AsSlaw () const override;
  SlawRef Find (bslaw) const override;

  int64 Count () const override;
  SlawRef Nth (int64) const override;
  SlawRefs Slice (int64, int64) const override;
  int64 IndexOf (bslaw, unt64) const override;
  SlawRefs KVList () const override;
  unt64 KeyCount () const override;

  Str ToStr () const override;
  void Spew (OStreamReference os) const override;

 private:
  bool IsEqual (const CompositeSlaw *) const override;
  SlawRefs DescripList () const override;
  SlawRefs IngestList () const override;

  void Sync () const;

  mutable SlawList pairs_;
  SlawList cmps_;
};
}
}
}  // namespace oblong::plasma::detail


#endif  // OBLONG_PLASMA_DETAIL_SLAWMAP_H

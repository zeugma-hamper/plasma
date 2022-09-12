
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_DETAIL_ATOMICCOMPOSITE_H
#define OBLONG_PLASMA_DETAIL_ATOMICCOMPOSITE_H


#include "CompositeSlaw.h"


namespace oblong {
namespace plasma {
namespace detail {


class AtomicComposite : public CompositeSlaw
{
 public:
  explicit AtomicComposite (SlawRef s);

  bool IsNull () const override;
  bool IsAtomic () const override;

  SlawRef AsSlaw () const override;

  int64 Count () const override;
  SlawRef Nth (int64 idx) const override;
  int64 IndexOf (bslaw s, unt64 start) const override;
  SlawRefs Slice (int64 begin, int64 end) const override;
  SlawRefs KVList () const override;
  unt64 KeyCount () const override;

  Str ToStr () const override;

  SlawRef Find (bslaw key) const override;

  void Spew (OStreamReference os) const override;

 private:
  bool IsEqual (const CompositeSlaw *other) const override;
  SlawRefs DescripList () const override;
  SlawRefs IngestList () const override;

  SlawRef slaw_;
};
}
}
}  // namespace oblong::plasma::detail


#endif  // OBLONG_PLASMA_DETAIL_ATOMICCOMPOSITE_H


/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_DETAIL_SLAWCONS_H
#define OBLONG_PLASMA_DETAIL_SLAWCONS_H


#include "SlawList.h"
#include "CompositeSlaw.h"


namespace oblong {
namespace plasma {
namespace detail {


class SlawCons : public CompositeSlaw
{
 public:
  explicit SlawCons (SlawRef cons);
  SlawCons (SlawRef car, SlawRef cdr);

  bool IsNull () const override;
  bool IsCons () const override;
  SlawRef AsSlaw () const override;
  SlawRefs KVList () const override;
  unt64 KeyCount () const override;
  int64 Count () const override;
  SlawRef Nth (int64 idx) const override;
  SlawRefs Slice (int64, int64) const override;
  SlawRef Find (bslaw key) const override;
  int64 IndexOf (bslaw s, unt64 start) const override;
  Str ToStr () const override;
  void Spew (OStreamReference os) const override;

 private:
  bool IsEqual (const CompositeSlaw *other) const override;
  SlawRefs DescripList () const override;
  SlawRefs IngestList () const override;

  SlawRef Car () const;
  SlawRef Cdr () const;

  SlawList elements_;
};
}
}
}  // namespace oblong::plasma::detail


#endif  // OBLONG_PLASMA_DETAIL_SLAWCONS_H

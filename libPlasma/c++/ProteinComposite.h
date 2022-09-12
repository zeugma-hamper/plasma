
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_DETAIL_PROTEINCOMPOSITE_H
#define OBLONG_PLASMA_DETAIL_PROTEINCOMPOSITE_H


#include "CompositeSlaw.h"


namespace oblong {
namespace plasma {
namespace detail {


class ProteinComposite : public CompositeSlaw
{
 public:
  explicit ProteinComposite (SlawRef protein);
  ProteinComposite (Ref descrips, Ref ingests);

  bool IsNull () const override;
  bool IsProtein () const override;

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

  Ref Descrips () const override;
  Ref Ingests () const override;

 private:
  bool IsEqual (const CompositeSlaw *) const override;
  SlawRefs DescripList () const override;
  SlawRefs IngestList () const override;

  Ref descrips_;
  Ref ingests_;
};
}
}
}  // namespace oblong::plasma::detail


#endif  // OBLONG_PLASMA_DETAIL_PROTEINCOMPOSITE_H

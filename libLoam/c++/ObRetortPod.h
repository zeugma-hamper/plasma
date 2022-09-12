
/* (c)  oblong industries */

#ifndef OB_RETORT_GOITER_PROTUBERANCES
#define OB_RETORT_GOITER_PROTUBERANCES


#include <libLoam/c++/ObPseudopod.h>

#include <libLoam/c/ob-retorts.h>
#include <libLoam/c/ob-api.h>


namespace oblong {
namespace loam {

class OB_LOAMXX_API ObRetortPod : public ObPseudopod
{
  PATELLA_SUBCLASS (ObRetortPod, ObPseudopod);

 OB_PRIVATE:
  ob_retort c_ret;
  ObRef<ObRetortPod *, UnspecifiedMemMgmt, AnkleObject>  // oy...
    antecedent;

 public:
  ObRetortPod ();
  ObRetortPod (ob_retort obr);
  ObRetortPod (ob_retort obr, ObRetortPod *ant);

  ~ObRetortPod () override;
  void Delete () override { delete this; }

  ob_retort NumericRetort () const;
  void SetNumericRetort (ob_retort num_ret);

  ObRetortPod *Antecedent () const;
  template <class RPOD_SUBT>
  RPOD_SUBT *Antecedent () const
  {
    return dynamic_cast<RPOD_SUBT *> (~antecedent);
  }
  void SetAntecedent (ObRetortPod *ant);

  ObRetortPod *FirstRetortPodOfClass (const char *cls) const;
  ObRetortPod *NextRetortPodOfClass (const ObRetortPod *orp,
                                     const char *cls) const
  {
    if (!orp)
      return FirstRetortPodOfClass (cls);
    ObRetortPod *ant = orp->Antecedent ();
    return (ant ? ant->FirstRetortPodOfClass (cls) : NULL);
  }
  ObRetortPod *NextRetortPodOfClass (const char *cls) const
  {
    return NextRetortPodOfClass (Antecedent (), cls);
  }

  template <typename POD_TYPE>
  POD_TYPE *FirstRetortPodOfClass () const
  {
    for (const ObRetortPod *p = this; p; p = p->Antecedent ())
      {
        const POD_TYPE *maybe = dynamic_cast<const POD_TYPE *> (p);
        if (maybe)
          return const_cast<POD_TYPE *> (maybe);
      }
    return NULL;
  }
  template <typename POD_TYPE>
  POD_TYPE *NextRetortPodOfClass () const
  {
    for (ObRetortPod *p = Antecedent (); p; p = p->Antecedent ())
      {
        const POD_TYPE *maybe = dynamic_cast<const POD_TYPE *> (p);
        if (maybe)
          return const_cast<POD_TYPE *> (maybe);
      }
    return NULL;
  }
};

}
}  // requieming it oldschool for namespaces loam and oblong


#endif

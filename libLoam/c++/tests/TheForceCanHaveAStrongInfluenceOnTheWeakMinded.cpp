
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include "libLoam/c/ob-rand.h"
#include "libLoam/c++/ObMap.h"
#include "libLoam/c++/ObRef.h"
#include "libLoam/c++/ObTrove.h"
#include "libLoam/c++/Str.h"

using namespace oblong::loam;

class Achilles : public AnkleObject
{
 public:
  Achilles ();
  ~Achilles () override;
};

static ObTrove<Achilles *, WeakRef> heel;

Achilles::Achilles ()
{
  heel.Append (this);
}

Achilles::~Achilles ()
{
  heel.Remove (this);
  heel.CompactNulls ();
}

TEST (TheForceCanHaveAStrongInfluenceOnTheWeakMinded, Bug2053)
{
  ObTrove<Achilles *> strongTrove;
  Achilles *a = new Achilles;
  Achilles *b = new Achilles;
  strongTrove.Append (b);
  strongTrove.Append (a);
}


/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_CPP_TEST_CALLBACKS_H
#define OBLONG_PLASMA_CPP_TEST_CALLBACKS_H

#include <libPlasma/c++/Slaw.h>

using namespace oblong::plasma;

class Callbacks
{
 public:
  virtual Slaw SlawFromNthValue (int64) const = 0;
  virtual slaw CSlawFromNthValue (int64) const = 0;
  virtual int64 IndexOfNthValue (const Slaw &, int64) const = 0;

  virtual Slaw ConsFromValues (int64, int64) const = 0;
  virtual Slaw ConsWithNil (int64) const = 0;
  virtual Slaw FillList (int64, int64) const = 0;
  virtual Slaw MakeSlawList (int64, int64, int64) const = 0;

  virtual Slaw MapWithValues (const char *key) const = 0;
  virtual Slaw MapWithKeys (const char *value) const = 0;

  virtual bool CheckNthValue (const Slaw &, int64) const = 0;
  virtual bool CheckNthValue (bslaw s, int64) const = 0;
  virtual void CheckElements (const Slaw &) const = 0;

  virtual bool HasType (const Slaw &s) const = 0;
  virtual bool CanEmit (const Slaw &s) const = 0;

  virtual size_t Count () const = 0;

  virtual ~Callbacks () {}
};

// This class defines operations performed on Slawx derived from an
// array of values of type T, convertible to Slaw as per the
// corresponding slaw_traits instantiation. We collect them here
// (instead of calling their equivalents directly in tests) to
// speed-up compilation time, thanks to the explicit template
// instantiations defined in Callbacks.cpp.
template <typename T, size_t N>
class CallbacksImpl : public Callbacks
{
 public:
  // The argument contains the values to which the callbacks apply.
  explicit CallbacksImpl (T (&)[N]);

  size_t Count () const override;

  // Takes the nth T value and creates a Slaw from it.
  Slaw SlawFromNthValue (int64 n) const override;
  // Takes the nth T value and creates a plasma c slaw from it.
  slaw CSlawFromNthValue (int64) const override;
  // Looks up in s the nth T value.
  int64 IndexOfNthValue (const Slaw &s, int64 n) const override;

  // Creates a Slaw cons from the nth and mth T values.
  Slaw ConsFromValues (int64 n, int64 m) const override;
  // Creates a Slaw cons from the nth T value and a nil Slaw.
  Slaw ConsWithNil (int64) const override;
  // Creates a Slaw list containing len consecutive T values, starting
  // from start.
  Slaw FillList (int64 start, int64 len) const override;
  // Auxiliarly function creating a Slaw list with T values in the
  // given positions.
  Slaw MakeSlawList (int64 i, int64 j, int64 k) const override;

  // Special map constructors
  Slaw MapWithValues (const char *key) const override;
  Slaw MapWithKeys (const char *value) const override;

  // Checks whether s equals the T nth value, using Emit on s.
  bool CheckNthValue (const Slaw &s, int64) const override;
  // Checks whether s equals the T nth value, using Emit on s.
  bool CheckNthValue (bslaw s, int64) const override;
  // Checks whether the given Slaw list contains all T values, in order.
  void CheckElements (const Slaw &) const override;
  // Checks whether the given Slaw list contains n T values, in order.
  void CheckElements (const Slaw &, size_t n) const;

  // Check whether s complies to type T.
  bool HasType (const Slaw &s) const override;
  bool CanEmit (const Slaw &s) const override;

 private:
  T (&values)[N];
};

#endif  // OBLONG_PLASMA_CPP_TEST_CALLBACKS_H

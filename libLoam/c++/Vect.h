
/* (c)  oblong industries */

#ifndef VECT_BONANZA
#define VECT_BONANZA


#include <libLoam/c/ob-api.h>
#include <libLoam/c/ob-attrs.h>

#include <libLoam/c++/Preterite.h>
#include <libLoam/c++/LoamForward.h>
#include <libLoam/c++/ob-math-utils.h>

// On Windows, we need to define _USE_MATH_DEFINES in order to get M_PI
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>


namespace oblong {
namespace loam {

/**
 * the Vect, which represents a 3-vector, is one of the most exercised
 * objects in the g-speak platform: it enables the programmer to
 * operate in a conceptually coordinate-free way, which is a keen
 * advantage in the context of the system's emphasis on space,
 * geometry, and the relationships among graphical constructs with
 * real-world shape & orientation & position.
 */
class OB_LOAMXX_API Vect : public v3float64
{
 public:
  /**
   * the nekkid constructor sets \b x and \b y and \b z to zero.
   */
  Vect () { x = y = z = 0.0; }
  /**
   * explicitly use default copy constructor
   */
  Vect (const Vect &) = default;
  /**
   * explicitly use default move constructor
   */
  Vect (Vect &&) = default;
  /**
   * a version of copy constructor more friendly to Vect pointers
   */
  explicit Vect (const Vect *v)
  {
    if (!v)
      this->SetInvalid ();
    else
      {
        x = v->x;
        y = v->y;
        z = v->z;
      }
  }
  /**
   * the constructor where you let it all hang out -- give it what
   * for, with three explicit float64s.
   */
  Vect (float64 xx, float64 yy, float64 zz)
  {
    x = xx;
    y = yy;
    z = zz;
  }
  /**
   * this better be a length 3 array!
   */
  explicit Vect (const float64 *triple)
  {
    if (!triple)
      this->SetInvalid ();
    else
      {
        x = triple[0];
        y = triple[1];
        z = triple[2];
      }
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v3float32 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v3float64 &v) : v3float64 (v) {}
  /**
   * convert from libLoam vector
   */
  Vect (const v3int64 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v3int32 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v3int16 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v3int8 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v2float32 &v)
  {
    x = v.x;
    y = v.y;
    z = 0.0;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v2float64 &v)
  {
    x = v.x;
    y = v.y;
    z = 0.0;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v2int64 &v)
  {
    x = v.x;
    y = v.y;
    z = 0.0;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v2int32 &v)
  {
    x = v.x;
    y = v.y;
    z = 0.0;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v2int16 &v)
  {
    x = v.x;
    y = v.y;
    z = 0.0;
  }
  /**
   * convert from libLoam vector
   */
  Vect (const v2int8 &v)
  {
    x = v.x;
    y = v.y;
    z = 0.0;
  }

  void Delete () { delete this; }



  /**
   * accessors for the three scalar constituents, which are float64s.
   */
  //@{
  float64 X () const { return x; }
  float64 Y () const { return y; }
  float64 Z () const { return z; }
  //@}

  /**
   * construct an initially invalid vector. an invalid Vect has all
   * components set to NAN, which can be useful as a NULL value.
   */
  static Vect Invalid () { return Vect ().SetInvalid (); }

  /**
   * \return true if the vector is valid, false otherwise.  an invalid
   * Vect has all components set to NAN, which can be useful as a NULL value.
   */
  bool IsValid () const
  {
    return (!obIsNAN (x) && !obIsNAN (y) && !obIsNAN (z));
  }
  /**
   * the opposite: \return true when the vector's invalid; false
   * otherwise. an invalid Vect has all components set to NAN, which
   * can be useful as a NULL value.
   */
  bool IsInvalid () const { return !IsValid (); }

  /**
   * mark the vector as invalid. an invalid Vect has all components
   * set to NAN, which can be useful as a NULL value.
   */
  Vect &SetInvalid ()
  {
    x = y = z = OB_NAN;
    return *this;
  }

  /**
   * set/get-tors by index
   * returns {0=>x, 1=>y, 2=>z, else=>NAN}
   */
  //@{
  bool SetComponent (int ind, float64 val)
  {
    switch (ind)
      {
        case 0:
          x = val;
          break;
        case 1:
          y = val;
          break;
        case 2:
          z = val;
          break;
        default:
          return false;
      }
    return true;
  }
  float64 NthComponent (int ind) const
  {
    switch (ind)
      {
        case 0:
          return x;
        case 1:
          return y;
        case 2:
          return z;
        default:
          return OB_NAN;
      }
  }
  float64 GetComponent (int ind) const { return NthComponent (ind); }
  //@}

  /**
   * component-wise equality
   */
  bool Equals (const Vect &v) const
  {
    return (x == v.x && y == v.y && z == v.z);
  }

  /**
   * component-wise approximate equality
   */
  bool ApproxEquals (const Vect &v, float64 eps) const
  {
    return (fabs (x - v.x) < eps && fabs (y - v.y) < eps
            && fabs (z - v.z) < eps);
  }

  /**
   * overloperator version of component-wise equality
   */
  bool operator== (const Vect &v) const { return Equals (v); }

  /**
   * inequality that will never see justice
   */
  bool operator!= (const Vect &v) const { return !Equals (v); }

  /**
   * Returns a string representation of the vector.
   */
  Str AsStr () const;
  void SpewToStderr () const;

  /**
   * sets \b x & \b y & \b z to zero
   */
  Vect &Zero ()
  {
    x = y = z = 0.0;
    return *this;
  }
  /**
   * returns zero if any component isn't zero; otherise, if all zero,
   * one.
   */
  int IsZero () const { return (x == 0.0 && y == 0.0 && z == 0.0); }

  /**
   * loads the self-Vect with a random vector with unit length
   */
  Vect &UnitRandomize ();
  /**
   * returns a new Vect with random direction and unit length
   */
  static Vect UnitRandom () { return Vect ().UnitRandomize (); }

  /**
   * make the self-Vect be exactly like the other-Vect \a v.
   */
  Vect &Set (const Vect &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }
  /**
   * copy the pointer-other-Vect \a v into the self-Vect.
   */
  Vect &Set (const Vect *v)
  {
    if (v)
      return Set (*v);
    return *this;
  }
  /**
   * give the self-Vect what for: explicit componentwise settage.
   */
  Vect &Set (float64 xx, float64 yy, float64 zz)
  {
    x = xx;
    y = yy;
    z = zz;
    return *this;
  }
  /**
   * convert from libLoam vector
   */
  Vect &Set (const v3float32 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }
  /**
   * convert from libLoam vector
   */
  Vect &Set (const v3float64 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }
  /**
   * export to libLoam 64-bit vector
   */
  v3float64 ToV3 () const { return *this; }

  /**
   * export to libLoam 32-bit vector
   */
  v3float32 ToV3Float32 () const
  {
    return v3float32{(float32) x, (float32) y, (float32) z};
  }

  operator v3float32 () const { return ToV3Float32 (); }

  /**
   * a method that scales, in situ, the components of the self-Vect by \a s.
   */
  Vect &Scale (float64 s)
  {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }
  /**
   * a kind of perversion, really: scales each component of the
   * self-Vect by the corresponding component of other-Vect \a v (a
   * sort of anisotropic scale, thinking geometrically). The
   * perversion comes mostly because we're using the other Vect as
   * storage for three independent scaling components, not as anything
   * that'd traditionally be recognizable as a vector.
   */
  Vect &Scale (const Vect &v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }
  /**
   * Ah yes -- you prefer perhaps to pass a pointer and not a
   * reference?
   */
  Vect &Scale (const Vect *v) { return Scale (*v); }

  /**
   * Adds other-Vect \a v to the self-Vect (accumulating in situ into
   * self-Vect)
   */
  Vect &Add (const Vect &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  /**
   * do just that but with a pointerized other-Vect \a v.
   */
  Vect &Add (const Vect *v) { return Add (*v); }

  /**
   * Subtract the other-Vect \a v in situ from the self-Vect.
   */
  Vect &Sub (const Vect &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
  /**
   * likewise, but \a v is a pointer.
   */
  Vect &Sub (const Vect *v) { return Sub (*v); }

  /**
   * explicitly use default copy assignment
   */
  Vect &operator= (const Vect &) = default;

  /**
   * explicitly use default move assignment
   */
  Vect &operator= (Vect &&) = default;

  /**
   * unary negatizer -- returns a fresh-on-the-stack Vect that
   * is the additive inverse of lexically following Vect entity.
   */
  Vect operator- () const { return Vect (-x, -y, -z); }

  /**
   * binary additizer -- sum of the surrounding Vect entities is
   * placed as a fresh Vect on the stack
   */
  Vect operator+ (const Vect &v) const
  {
    return Vect (x + v.x, y + v.y, z + v.z);
  }
  /**
   * binary subtractifier, which puts a fresh Vect on the stack
   * representing the difference between the two surrounding Vect
   * entities.
   */
  Vect operator- (const Vect &v) const
  {
    return Vect (x - v.x, y - v.y, z - v.z);
  }
  /**
   * binary Vect multiplication, with the second "arg" being a scalar
   * (float) -- so this is really a scale operation; the result is a
   * fresh Vect on the stack.
   */
  Vect operator* (float64 f) const { return Vect (x * f, y * f, z * f); }
  /**
   * binary Vect multiplication, for when the scalar (float) comes
   * first and the Vect second. Of course we're cattleherded into
   * using the 'friend' formalism. Thanks heaps, Bjarne.
   */
  friend Vect operator* (float64 f, const Vect &v) { return v * f; }
  /**
   * binary divisionating: Vect divided by (float64) scalar; result
   * into fresh Vect on stack.
   */
  Vect operator/ (float64 f) const { return Vect (x / f, y / f, z / f); }

  /**
   * 'cumulating addition: second Vect \a v added to lhs-Vect, result
   * right back into lhs-Vect (so: <b>v1 += v2;</b> means, of course,
   * <b>v1 = v1 + v2;</b>
   */
  const Vect &operator+= (const Vect &v) { return Add (v); }
  /**
   * accumulating subtraction. as with the <b>+=</b> thingy.
   */
  const Vect &operator-= (const Vect &v) { return Sub (v); }
  /**
   * vector-by-scalar multiplication, i.e. <b>v *= f;</b>, back into
   * lhs-Vect
   */
  const Vect &operator*= (float64 f) { return Scale (f); }
  /**
   * vector-by-scalar division -- <b>v /= f;</b> -- with result going
   * back into the lhs-Vect.
   */
  const Vect &operator/= (float64 f)
  {
    f = ((f == 0) ? 1.0 : (1.0 / f));
    return Scale (f);
  }

  /**
   * produces the dot-product (or 'scalar product') between the
   * self-Vect and the other-Vect \a v.
   */
  float64 Dot (const Vect &v) const { return x * v.x + y * v.y + z * v.z; }
  /**
   * dot product between the self-Vect and the pointerly other-Vect
   * \a v.
   */
  float64 Dot (const Vect *v) const { return x * v->x + y * v->y + z * v->z; }
  /**
   * dot product of the self-Vect with its... self (and is thus the
   * square of the self-Vect's magnitude).
   */
  float64 AutoDot () const { return x * x + y * y + z * z; }

  /**
   * divine the projection of the self-vector onto the argument Vect \a a;
   * note that, in order to provide the behavior that is most expected,
   * \a a is not blindly assumed to be normalized; it is explicitly
   * normalized as part of the calculation. The result is equivalent to
   *   " a . Norm () * (this -> Dot (a . Norm ())) "
   * and is returned on the stack.
   */
  Vect ProjectOnto (const Vect &a) const
  {
    float64 s = a.x * a.x + a.y * a.y + a.z * a.z;
    if (s > 0.0)
      s = 1.0 / s;
    s *= (x * a.x + y * a.y + z * a.z);
    return Vect (s * a.x, s * a.y, s * a.z);
  }
  /**
   * in-place version of ProjectOnto(); equivalent to
   *   " Set (ProjectOnto (a)) "
   */
  Vect &ProjectSelfOnto (const Vect &a)
  {
    float64 s = a.x * a.x + a.y * a.y + a.z * a.z;
    if (s > 0.0)
      s = 1.0 / s;
    s *= (x * a.x + y * a.y + z * a.z);
    x = s * a.x;
    y = s * a.y;
    z = s * a.z;
    return *this;
  }

  /**
   * the length or magnitude of the self-Vect (computationally, the
   * square root of the dot product of the vector with itself).
   */
  float64 Mag () const { return sqrt (AutoDot ()); }
  /**
   * calculates & returns (as a hot-n-fresh Vect on the stack) the
   * normalized version of the self-Vect -- mathematically, that means
   * the vector that's in the same direction as the self-Vect but with
   * unit magnitude. Note that you can't normalize the zero vector: it
   * stays zero.
   */
  Vect Norm () const
  {
    Vect v = *this;
    return v.NormSelf ();
  }
  /**
   * normalizes the self-Vect in situ.
   */
  Vect &NormSelf ()
  {
    float64 m = Mag ();
    if (m)
      Scale (1.0 / m);
    return *this;
  }

  /**
   * the good ol' Euclidean distance between the self and the other
   * (\a v).
   */
  //@{
  float64 SquaredDistFrom (const Vect &v) const
  {
    float64 xx = x - v.x;
    float64 yy = y - v.y;
    float64 zz = z - v.z;
    return xx * xx + yy * yy + zz * zz;
  }
  float64 SquaredDistFrom (const Vect *v) const { return SquaredDistFrom (*v); }
  //@}

  /**
   * the good ol' Euclidean distance between the self and the other
   * (\a v).
   */
  //@{
  float64 DistFrom (const Vect &v) const { return sqrt (SquaredDistFrom (v)); }
  float64 DistFrom (const Vect *v) const { return DistFrom (*v); }
  //@}

  /**
   * returns as a float64 the angle, in radians, between the self-Vect
   * and the other-Vect \a v.
   */
  float64 AngleWith (const Vect &v) const
  {
    float64 a, b;
    if ((a = Mag ()) == 0.0)
      return 0.0;
    if ((b = v.Mag ()) == 0.0)
      return 0.0;
    b = Dot (v) / (a * b);
    return acos ((b < -1.0) ? -1.0 : ((b > 1.0) ? 1.0 : b));
  }
  /**
   * like the foregoing, except that other-Vect \a v is a pointer.
   */
  float64 AngleWith (const Vect *v) const { return AngleWith (*v); }
  /**
   * a more comprehensive prod at finding the angle between the
   * self-Vect and other-Vect \a v -- specifying \a planeNorm, which
   * is understood to be if not the exact normal to the plane in which
   * the two vectors lie then at the very least in the half-space
   * defined by that normal, allows disambiguation of positive and
   * negative angles (the other versions of AngleWith() essentially
   * return the absolute value of this angle).
   */
  float64 AngleWith (const Vect &v, const Vect &planeNorm) const
  {
    float64 c = Dot (v) / (Mag () * v.Mag ());
    float64 theta = acos ((c < -1.0) ? -1.0 : ((c > 1.0) ? 1.0 : c));
    return (((planeNorm.Cross (this)).Dot (v) > 0) ? theta : -theta);
  }

  /**
   * produces, as a fresh Vect on the stack, the cross product between
   * the self-Vect and the other-Vect \a v.
   */
  Vect Cross (const Vect &v) const
  {
    return Vect (y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
  }
  /**
   * cross product of self-Vect with other-Vect (a pointer, \a v, this
   * time) as a fresh Vect on the stack.
   */
  Vect Cross (const Vect *v) const { return Cross (*v); }

  /**
   * cross product of self-Vect with other-Vect \a v, back into
   * self-Vect
   */
  Vect &CrossSelf (const Vect &v)
  {
    float64 xx = y * v.z - z * v.y;
    float64 yy = z * v.x - x * v.z;
    float64 zz = x * v.y - y * v.x;
    return Set (xx, yy, zz);
  }
  /**
   * as above, but other-Vect's a pointer (\a v)
   */
  Vect &CrossSelf (const Vect *v) { return CrossSelf (*v); }

  /**
   * rotates the self-Vect about \a axis through \a angle (in radians)
   * with result going into fresh Vect on the stack
   */
  Vect Rotate (const Vect &axis, float64 angle) const;

  /**
   * self-Vect gets rotated by the self-Vect by \a angle around \a
   * axis (which is a pointer), and the result's a fresh stack-Vect
   */
  Vect Rotate (const Vect *axis, float64 angle) const
  {
    return Rotate (*axis, angle);
  }

  /**
   * the self-Vect gets rotated, in situ, by \a angle radians around
   * \a axis
   */
  Vect &RotateSelf (const Vect &axis, float64 angle)
  {
    return Set (Rotate (axis, angle));
  }
  /**
   * oh yes. almost exactly like the foregoing, but \a axis is a
   * pointer to a Vect
   */
  Vect &RotateSelf (const Vect *axis, float64 angle)
  {
    return RotateSelf (*axis, angle);
  }

  /**
   * the most complicated description of rotation you can get without
   * a prescription: rotate the self-Vect about point \a rotCent,
   * around \a axis, through \a angle radians. Result heads straight
   * onto the stack as a fresh Vect.
   */
  Vect RotateAbout (const Vect &rotCent, const Vect &axis, float64 angle) const
  {
    return ((Vect (this) - rotCent).RotateSelf (axis, angle)).Add (rotCent);
  }
  /**
   * waaaalp, reckon that'd be just like above but with pointers.
   */
  Vect RotateAbout (const Vect *rotCent, const Vect *axis, float64 angle) const
  {
    return RotateAbout (*rotCent, *axis, angle);
  }

  /**
   * as above, but the result goes back into the self-Vect.
   */
  Vect &RotateSelfAbout (const Vect &rotCent, const Vect &axis, float64 angle)
  {
    return Set (RotateAbout (rotCent, axis, angle));
  }
  /**
   * as several things above; pointers to the participating Vects, and
   * result goes back into self-Vect.
   */
  Vect &RotateSelfAbout (const Vect *rotCent, const Vect *axis, float64 angle)
  {
    return RotateSelfAbout (*rotCent, *axis, angle);
  }
};
}
}  // alas, poor namespaces loam and oblong; we knew them well


#endif

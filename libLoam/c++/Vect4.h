
/* (c)  oblong industries */

#ifndef VECT4_MESS
#define VECT4_MESS


#include <libLoam/c/ob-types.h>

#include <libLoam/c/ob-api.h>
#include <libLoam/c++/ob-math-utils.h>
#include <libLoam/c++/LoamForward.h>
#include <libLoam/c++/Preterite.h>

#include <math.h>


namespace oblong {
namespace loam {

/**
 * A 4-component mathematical vector, using 64-bit floating point numbers.
 */
class OB_LOAMXX_API Vect4 : public v4float64
{
 public:
  Vect4 () { x = y = z = w = 0.0; }
  Vect4 (const Vect4 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
  }
  Vect4 (const Vect4 *v)
  {
    x = v->x;
    y = v->y;
    z = v->z;
    w = v->w;
  }
  Vect4 (float64 xx, float64 yy, float64 zz, float64 ww)
  {
    x = xx;
    y = yy;
    z = zz;
    w = ww;
  }
  Vect4 (const v4float32 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
  }
  Vect4 (const v4float64 &v) : v4float64 (v) {}

  void Delete () { delete this; }


  /**
   * Returns the first component.
   */
  float64 X () const { return x; }
  /**
   * Returns the second component.
   */
  float64 Y () const { return y; }
  /**
   * Returns the third component.
   */
  float64 Z () const { return z; }
  /**
   * Returns the fourth component.
   */
  float64 W () const { return w; }

  /**
   * construct an initially invalid vector. an invalid Vect has all
   * components set to NAN, which can be useful as a NULL value.
   */
  static Vect4 Invalid () { return Vect4 ().SetInvalid (); }

  /**
   * \return true if the vector is valid, false otherwise. an invalid
   * Vect has all components set to NAN, which can be useful as a NULL value.
   */
  bool IsValid () const
  {
    return (!obIsNAN (x) && !obIsNAN (y) && !obIsNAN (z) && !obIsNAN (w));
  }
  /**
   * the opposite: \return true when the vector's invalid; false
   * otherwise. an invalid Vect has all components set to NAN, which
   * can be useful as a NULL value.
   */
  bool IsInvalid () const { return !IsValid (); }

  /**
   * mark the vector as invalid.an invalid Vect has all components set
   * to NAN, which can be useful as a NULL value.
   */
  Vect4 &SetInvalid ()
  {
    x = y = z = w = OB_NAN;
    return *this;
  }

  /**
   * component-wise equality
   */
  bool Equals (const Vect4 &v) const
  {
    return (x == v.x && y == v.y && z == v.z && w == v.w);
  }

  /**
   * component-wise approximate equality
   */
  bool ApproxEquals (const Vect4 &v, float64 eps) const
  {
    return (fabs (x - v.x) < eps && fabs (y - v.y) < eps && fabs (z - v.z) < eps
            && fabs (w - v.w) < eps);
  }

  /**
   * component-wise equality
   */
  bool operator== (const Vect4 &v) const { return (Equals (v)); }

  /**
   * component-wise inequality
   */
  bool operator!= (const Vect4 &v) const
  {
    return (x != v.x || y != v.y || z != v.z || w != v.w);
  }

  /**
   * Returns a string representation of the vector
   */
  Str AsStr () const;
  void SpewToStderr () const;

  /**
   * Sets all components to zero.
   */
  void Zero ()
  {
    x = y = z = w = 0.0;
    return;
  }

  /**
   * Returns true if all components equal zero.
   */
  bool IsZero () const
  {
    return (x == 0.0 && y == 0.0 && z == 0.0 && w == 0.0);
  }

  /**
   * Set this vector to the other vector and return reference to self.
   */
  Vect4 &Set (const Vect4 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
  }

  /**
   * Set this vector to the other vector and return reference to self.
   */
  Vect4 &Set (const Vect4 *v)
  {
    if (v)
      Set (*v);
    return *this;
  }

  /**
   * Set the components of this vector to \a xx, \a yy, \a zz, and
   * \a ww, respectively.  Returns a reference to this vector.
   */
  Vect4 &Set (float64 xx, float64 yy, float64 zz, float64 ww)
  {
    x = xx;
    y = yy;
    z = zz;
    w = ww;
    return *this;
  }

  /**
   * export to libLoam 64-bit vector
   */
  v4float64 ToV4 () const { return *this; }

  /**
   * export to libLoam 32-bit vector
   */
  v4float32 ToV4Float32 () const
  {
    return v4float32{(float32) x, (float32) y, (float32) z, (float32) w};
  }

  operator v4float32 () const { return ToV4Float32 (); }

  /**
   * Returns the distance between this vector and \a v, using the
   * Pythagorean Theorem.
   */
  //@{
  float64 DistFrom (const Vect4 &v) const
  {
    return sqrt ((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y)
                 + (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
  }
  float64 DistFrom (const Vect4 *v) const
  {
    return sqrt ((x - v->x) * (x - v->x) + (y - v->y) * (y - v->y)
                 + (z - v->z) * (z - v->z) + (w - v->w) * (w - v->w));
  }
  //@}

  /**
   * Multiply all components of this vector by \a s, and return a
   * reference to this vector.
   */
  Vect4 &Scale (float64 s)
  {
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
  }

  /**
   * Destructively adds \a v to the self-vector, and returns a
   * self-reference.
   */
  //@{
  Vect4 &Add (const Vect4 &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }
  Vect4 &Add (const Vect4 *v) { return Add (*v); }
  //@}

  /**
   * Destructively subtracts \a v from the self-vector, and returns a
   * self-reference.
   */
  //@{
  Vect4 &Sub (const Vect4 &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }
  Vect4 &Sub (Vect4 *v) { return Sub (*v); }
  //@}


  /**
   * Sets the self-vector to \a v.
   */
  Vect4 &operator= (const Vect4 &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
  }

  /**
   * Returns a new vector in which all four components of the
   * self-vector are negated.
   */
  Vect4 operator- () const { return Vect4 (-x, -y, -z, -w); }

  /**
   * Non-destructively adds \a v to the self-vector and returns the
   * result.
   */
  Vect4 operator+ (const Vect4 &v) const
  {
    return Vect4 (x + v.x, y + v.y, z + v.z, w + v.w);
  }

  /**
   * Non-destructively subtracts \a v from the self-vector and returns
   * the result.
   */
  Vect4 operator- (const Vect4 &v) const
  {
    return Vect4 (x - v.x, y - v.y, z - v.z, w - v.w);
  }

  /**
   * Non-destructively multiplies each component of the self-vector
   * with \a f and returns the result.
   */
  Vect4 operator* (float64 f) const
  {
    return Vect4 (x * f, y * f, z * f, w * f);
  }

  /**
   * Non-destructively multiplies each component of \a v with \a f and
   * returns the result.
   */
  friend Vect4 operator* (float64 f, const Vect4 &v) { return v * f; }

  /**
   * Non-destructively divides each component of the self-vector by
   * \a f and returns the result.
   */
  Vect4 operator/ (float64 f) const
  {
    return Vect4 (x / f, y / f, z / f, w / f);
  }

  /**
   * Destructively adds \a v to the self-vector, and returns a
   * self-reference.  (But why a const one?)
   */
  const Vect4 &operator+= (const Vect4 &v) { return Add (v); }

  /**
   * Destructively subtracts \a v from the self-vector, and returns a
   * self-reference.  (But why a const one?)
   */
  const Vect4 &operator-= (const Vect4 &v) { return Sub (v); }

  /**
   * Synonym for Scale().
   */
  const Vect4 &operator*= (float64 f) { return Scale (f); }

  /**
   * Scales this vector by the reciprocal of \a f, unless \a f is 0,
   * in which case the vector is unchanged.
   */
  const Vect4 &operator/= (float64 f)
  {
    f = (f == 0) ? 1.0 : (1.0 / f);
    return Scale (f);
  }

  /**
   * Returns dot product of self-vector with \a v.
   */
  //@{
  float64 Dot (const Vect4 &v) const
  {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }
  float64 Dot (const Vect4 *v) const
  {
    return x * v->x + y * v->y + z * v->z + w * v->w;
  }
  //@}

  /**
   * Returns dot product of vector with itself.
   */
  float64 AutoDot () const { return x * x + y * y + z * z + w * w; }

  /**
   * Returns magnitude of vector.
   */
  float64 Mag () const { return sqrt (AutoDot ()); }

  /**
   * Non-destructively normalizes this vector and returns the result.
   */
  Vect4 Norm () const
  {
    float64 m = Mag ();
    if (!m)
      return Vect4 (this);
    m = 1.0 / m;
    return *this * m;
  }

  /**
   * Destructively normalizes this vector and returns a
   * self-reference.
   */
  Vect4 &NormSelf () { return Set (Norm ()); }

  /**
   * Returns angle between self-vector and \a v.
   */
  //@{
  float64 AngleWith (const Vect4 &v) const
  {
    float64 a, b;
    if ((a = Mag ()) == 0.0)
      return 0.0;
    if ((b = v.Mag ()) == 0.0)
      return 0.0;
    b = Dot (v) / (a * b);
    return acos ((b < -1.0) ? -1.0 : ((b > 1.0) ? 1.0 : b));
  }
  float64 AngleWith (const Vect4 *v) const { return AngleWith (*v); }
  //@}
};
}
}  // in repose: namespaces loam and oblong


#endif

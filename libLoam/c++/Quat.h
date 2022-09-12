
/* (c)  oblong industries */

#ifndef QUAT_HAMILTONIZINGS
#define QUAT_HAMILTONIZINGS


#include <libLoam/c++/Vect.h>
#include <libLoam/c++/Matrix44.h>
#include <libLoam/c++/ob-math-utils.h>


namespace oblong {
namespace loam {


/**
 * Instances of the Quat class represent quaternions, mathematical
 * objects that are one notional extrapolation of complex
 * numbers; in the context of the g-speak platform, as in the
 * context of much of the CG world, the major application of
 * quaternions is as a well-heeled embodiment of three-space
 * rotations & orientations.
 *
 * A quaternion combines a scalar with three distinct
 * imaginary components. Where an ordinary complex number is
 * expressed as <b>c = a + ib</b> (with \b i being the square root of negative
 * one), a quaternion looks like <b>q = a + ib + jc + kd</b> -- and
 * each of \b i, \b j, and \b k works like 'familiar' \b i. That is:
 * <b>i*i = j*j = k*k = -1</b>.  Further, <b>i*j*k = -1</b>, because in turn
 * <b>i*j = k,  j*k = i</b>,  and <b>k*i = j</b>.  More freakily, \b i
 * and \b j and \b k
 * form an anticommutative mafia, so that <b>i*j = -j*i, j*k = -k*j</b>,
 * and <b>k*i = -i*k</b>. This heretical property gave Sir William Rowan
 * Hamilton, who 'discovered' quaternions in 1843, indigestion and
 * lavender hives for the rest of his life. Others were also upset.
 *
 * Let's switch to an alternate expression form
 * in which <b>q = (a, b, c, d)</b> means <b>q = a + ib + jc + kd</b>. There.
 * Addition and subtraction work in the 'obvious' ways:
 * <b>q1 + q2 = (a1, b1, c1, d1) + (a2, b2, c2, d2) = (a1+a2, b1+b2, c1+c2, d1+d2)</b>
 * and
 * <b>q1 - q2 = (a1, b1, c1, d1) - (a2, b2, c2, d2) = (a1-a2, b1-b2, c1-c2, d1-d2)</b>
 * But of course that ain't the half of it, because multiplication is
 * where stuff gets a little head-scratchy. There is a quaternion
 * dot-product, which works just like the vector one:
 * <b>q1.q2 = (a1*a2, b1*b2, c1*c2, d1*d2)</b>, so that's nice. Multiplication
 * of a quaternion by a scalar is smooth as silk too:
 * <b>s*q = (s*a, s*b, s*c, s*d)</b>.
 * It's when we do 'regular multiplication' -- i.e. the direct analogue
 * of scalar or complex multiplication -- that we really have to hunker down.
 * So:
 */
class OB_LOAMXX_API Quat
{
 public:
  /**
   * \cond INTERNAL
   */
  float64 a, i, j, k;
  /** \endcond */

  /**
   * argumentless Quat constructor -- set to identity
   * (<b>a = 1.0; i = j = k = 0.0</b>)
   */
  Quat () : a (1.0), i (0.0), j (0.0), k (0.0) {}
  /**
   * constructor that copies components from somebody else
   */
  Quat (const Quat &q) : a (q.a), i (q.i), j (q.j), k (q.k) {}
  /**
   * constructor that copies components from a pointer-style somebody else
   */
  Quat (const Quat *q) : a (q->a), i (q->i), j (q->j), k (q->k) {}
  /**
   * constructor that ingests explicit values for  the four components
   */
  Quat (float64 aa, float64 ii, float64 jj, float64 kk)
      : a (aa), i (ii), j (jj), k (kk)
  {
  }
  /**
   * convert from libLoam vector
   */
  Quat (const v4float32 &v) : a (v.x), i (v.y), j (v.z), k (v.w) {}
  /**
   * convert from libLoam vector
   */
  Quat (const v4float64 &v) : a (v.x), i (v.y), j (v.z), k (v.w) {}

  void Delete () { delete this; }


  inline operator v4float64 () const { return v4float64 ({a, i, j, k}); }

  /**
   * duh. coughs up the 'real' or scalar component of the quaternion
   */
  float64 A () const { return a; }
  /**
   * hm. what's your guess? that's right! it's the I component!
   */
  float64 I () const { return i; }
  /**
   * all right. enough with the sarcasm. yes, it's the J component.
   */
  float64 J () const { return j; }
  /**
   * mirabile dictu! it's the K component!
   */
  float64 K () const { return k; }

  /**
   * component-wise equality
   */
  bool Equals (const Quat &q) const
  {
    return (a == q.a && i == q.i && j == q.j && k == q.k);
  }
  /**
   * component-wise approximate equality
   */
  bool ApproxEquals (const Quat &q, float64 eps) const
  {
    return (fabs (a - q.a) < eps && fabs (i - q.i) < eps && fabs (j - q.j) < eps
            && fabs (k - q.k) < eps);
  }

  /**
   * test for equality of two rotations (does not check for factors of 2pi)
   */
  bool RotEquals (const Quat &q) const { return (Equals (q) || Equals (-q)); }
  /**
   * test for approximate equality of two rotations (does not check for
   * factors of 2pi)
   */
  bool RotApproxEquals (const Quat &q, float64 eps) const
  {
    return (ApproxEquals (q, eps) || ApproxEquals (-q, eps));
  }

  /**
   * shorthand for component-wise equality
   */
  bool operator== (const Quat &q) const { return (Equals (q)); }
  /**
   * wait... could it be? yes! component-wise inequality.
   */
  bool operator!= (const Quat &q) const
  {
    return (a != q.a || i != q.i || j != q.j || k != q.k);
  }

  /**
   * sets components each and all and every one to zero.
   */
  Quat &Zero ()
  {
    a = i = j = k = 0.0;
    return *this;
  }
  /**
   * tests if all and every component are/is zero, zilch, zip
   */
  bool IsZero () const
  {
    return (a == 0.0 && i == 0.0 && j == 0.0 && k == 0.0);
  }

  /**
   * from one to another: component-wise copy/set.
   */
  Quat &Set (const Quat &q)
  {
    a = q.a;
    i = q.i;
    j = q.j;
    k = q.k;
    return *this;
  }
  /**
   * as above, but with a pointer instead of a reference.
   * look, let's be honest: you didn't really need to be told that, did you?
   */
  Quat &Set (const Quat *q) { return Set (*q); }
  /**
   * an explicit and flagrant setting of each component
   */
  Quat &Set (float64 aa, float64 ii, float64 jj, float64 kk)
  {
    a = aa;
    i = ii;
    j = jj;
    k = kk;
    return *this;
  }

  /**
   * this is the 'Euclidean' distance between another quaternion
   * and the self-quaternion -- it's the magnitude of their difference
   */
  float64 DistFrom (const Quat &q) const
  {
    return sqrt ((a - q.a) * (a - q.a) + (i - q.i) * (i - q.i)
                 + (j - q.j) * (j - q.j) + (k - q.k) * (k - q.k));
  }
  /**
   * you tell me.
   */
  float64 DistFrom (const Quat *q) const
  {
    return sqrt ((a - q->a) * (a - q->a) + (i - q->i) * (i - q->i)
                 + (j - q->j) * (j - q->j) + (k - q->k) * (k - q->k));
  }

  /**
   * component-wise scale of the quaternion (by a scalar)
   */
  Quat &Scale (float64 s)
  {
    a *= s;
    i *= s;
    j *= s;
    k *= s;
    return *this;
  }

  /**
   * the 'conjugate' of the quaternion, as a new Quat (on the stack).
   * Works -- mathematically -- in a way analogous to complex conjugates:
   * the scalar or real part stays the same; the 'imaginary' parts (\a i,
   * \a j, and \a k) become their individual additive inverse.
   */
  Quat Conj () const { return Quat (a, -i, -j, -k); }
  /**
   * conjugates the self-Quat 'in situ'.
   */
  Quat &ConjSelf ()
  {
    i = -i;
    j = -j;
    k = -k;
    return *this;
  }

  /**
   * the multiplicative inverse of the quaternion, as a new one
   * on the stack. The math is like the math for complex numbahs:
   * divide the conjugate by the original's squared magnitude.
   * If the magnitude is zero, then outgoing equals incoming.
   */
  Quat Invert () const
  {
    float64 mm = AutoDot ();
    mm = ((mm == 0.0) ? 1.0 : 1.0 / mm);
    return Quat (mm * a, -mm * i, -mm * j, -mm * k);
  }
  /**
   * as above, but in-situ.
   */
  Quat &InvertSelf ()
  {
    float64 mm = AutoDot ();
    mm = ((mm == 0.0) ? 1.0 : 1.0 / mm);
    a *= mm;
    i *= -mm;
    j *= -mm;
    k *= -mm;
    return *this;
  }

  /**
   * adds \a q to the self-Quat (modifying it in-situ)
   */
  Quat &Add (const Quat &q)
  {
    a += q.a;
    i += q.i;
    j += q.j;
    k += q.k;
    return *this;
  }
  /**
   * ditto, but \a q is a pointer to a Quat.
   */
  Quat &Add (const Quat *q) { return Add (*q); }

  /**
   * subtracts \a q from the self-Quat (modifying it in-situ)
   */
  Quat &Sub (const Quat &q)
  {
    a -= q.a;
    i -= q.i;
    j -= q.j;
    k -= q.k;
    return *this;
  }
  /**
   * don't make me come back there.
   */
  Quat &Sub (const Quat *q) { return Sub (*q); }

  /**
   * multiplies the self-Quat by \a q (modifying the self in-situ)
   */
  Quat &Mul (const Quat &q)
  {
    return Set (a * q.a - i * q.i - j * q.j - k * q.k,
                a * q.i + i * q.a + j * q.k - k * q.j,
                a * q.j - i * q.k + j * q.a + k * q.i,
                a * q.k + i * q.j - j * q.i + k * q.a);
  }
  /**
   * I'm not kidding. I'll turn this compiler right around.
   */
  Quat &Mul (const Quat *q) { return Mul (*q); }

  /**
   * divides the self-Quat by \a q, with result going back into self.
   * (mod-in-sit). Mathematically, you multiply self-Quat by
   * <b>1 / q</b> -- which should make sense:
   * division-as-multiplication-by-inverse.
   */
  Quat &Div (const Quat &q)
  {
    InvertSelf ();
    return Mul (q);
  }
  /**
   * sorry. guess I drifted off there. What's going on?
   */
  Quat &Div (const Quat *q) { return Div (*q); }

  /**
   * assignment operator that -- not surprisingly -- does a copy of
   * each component of \a q into the self (self if modded in-situ)
   */
  Quat &operator= (const Quat &q)
  {
    a = q.a;
    i = q.i;
    j = q.j;
    k = q.k;
    return *this;
  }

  /**
   * a new Quat (the unary negative of the one after the operator)
   * goes on the stack.
   */
  Quat operator- () const { return Quat (-a, -i, -j, -k); }
  /**
   * adds two Quats; result goes on the stack as a new Quat.
   */
  Quat operator+ (const Quat &q) const
  {
    return Quat (a + q.a, i + q.i, j + q.j, k + q.k);
  }
  /**
   * difference of two Quats gets plopped into a new Quat and onto the stack.
   */
  Quat operator- (const Quat &q) const
  {
    return Quat (a - q.a, i - q.i, j - q.j, k - q.k);
  }
  /**
   * product of Quats, as new Quat, on the stack.
   */
  Quat operator* (const Quat &q) const
  {
    return Quat (a * q.a - i * q.i - j * q.j - k * q.k,
                 a * q.i + i * q.a + j * q.k - k * q.j,
                 a * q.j - i * q.k + j * q.a + k * q.i,
                 a * q.k + i * q.j - j * q.i + k * q.a);
  }
  /**
   * one Quat divided by another, into a new Quat, onto the stack
   */
  Quat operator/ (const Quat &q) const { return Quat (this).Div (q); }

  /**
   * this, by contrast, is the operator that lets you mutiply a
   * scalar and a quaternion (with result on stack): <b>f * q</b>
   */
  Quat operator* (float64 f) const { return Quat (a * f, i * f, j * f, k * f); }
  /**
   * and this is for when the quaternion comes first (and the
   * scalar second; as before, result on stack): <b>q * f</b>
   */
  friend Quat operator* (float64 f, const Quat &q) { return q * f; }
  /**
   * each component of the quaternion divided by a scalar,
   * left in a new Quat on the stack (conceptually, of course,
   * it's just <b>q * (1/f)</b>
   */
  Quat operator/ (float64 f) const { return Quat (a / f, i / f, j / f, k / f); }

  /**
   * this is 'accumulating addition', i.e. <b>q1 += q2</b> adds
   * quaternions q1 and q2 and leaves the result in q1.
   */
  const Quat &operator+= (const Quat &q) { return Add (q); }
  /**
   * same deal; subtraction instead.
   */
  const Quat &operator-= (const Quat &q) { return Sub (q); }
  /**
   * ditto, multiplication.
   */
  const Quat &operator*= (const Quat &q) { return Mul (q); }
  /**
   * ibid, division
   */
  const Quat &operator/= (const Quat &q) { return Div (q); }

  /**
   * multiplication by a scalar (\a f) with the result going back into self
   */
  const Quat &operator*= (float64 f) { return Scale (f); }
  /**
   * so too with division.
   */
  const Quat &operator/= (float64 f)
  {
    f = ((f == 0) ? 1.0 : (1.0 / f));
    return Scale (f);
  }

  /**
   * the quaternion dot product: mathfully, it's the sum of the
   * component-by-component product.
   */
  float64 Dot (const Quat &q) const
  {
    return (a * q.a + i * q.i + j * q.j + k * q.k);
  }
  /**
   * dot product, but with a pointerized other Quat (instead of reference)
   */
  float64 Dot (const Quat *q) const
  {
    return a * q->a + i * q->i + j * q->j + k * q->k;
  }
  /**
   * this is the quaternion dotted with itself -- the square of the
   * quaternion's magnitude ('modulus').
   */
  float64 AutoDot () const { return (a * a + i * i + j * j + k * k); }

  /**
   * the magnitude or modulus of the quaternion (which, conceptually
   * and implementationally both, goes like the square root of the
   * self-dot-product)
   */
  float64 Mag () const { return sqrt (AutoDot ()); }
  /**
   * places a new Quat on the stack expressing the 'normalized'
   * self-Quat; that is, the quaternion divided by its magnitude.
   */
  Quat Norm () const
  {
    float64 m = Mag ();
    if (!m)
      return Quat (this);
    m = 1.0 / m;
    return (*this * m);
  }
  /**
   * normalizes the quaternion in-situ.
   */
  Quat &NormSelf () { return Set (Norm ()); }


  /**
   * raises this quat to a power & places result on the stack
   * \code
   * q^t = ||q||^t * e^(t * v_hat * theta) =
   *     = ||q||^t * (cos(t * theta) + v_hat * sin(t * theta))
   * \endcode
   * "thresh" param gives a close-enough threshold for calling this a unit quat
   */
  Quat Pow (float64 t, float64 thresh = 0.9999) const
  {
    if (fabs (a) > thresh)
      return *this;
    float64 mag_t, theta;
    mag_t = pow (Mag (), t);
    theta = acos (a);
    Vect v_hat = Vect (i, j, k) / sin (theta);
    v_hat *= sin (t * theta);
    return mag_t * Quat (cos (t * theta), v_hat.x, v_hat.y, v_hat.z);
  }

  /**
   * performs exponentiation (e^self) and returns resulting quat on the stack
   */
  Quat Exp () const
  {
    Vect v (i, j, k);
    float64 vMag = v.Mag ();
    float64 m = exp (a);
    v = v.Norm () * sin (vMag) * m;
    return Quat (m * cos (vMag), v.x, v.y, v.z);
  }

  /**
   * returns natural log of self-Quat on the stack
   */
  Quat Ln () const
  {
    if (IsZero ())
      return *this;
    Vect v (i, j, k);
    float64 mag = Mag ();
    v = v.Norm () * acos (a / mag);
    return Quat (log (mag), v.x, v.y, v.z);
  }


  /**
   * finally, the good stuff: shove the quaternion representation
   * for an arbitrary three-space rotation (axis plus angle) into
   * the self-Quat.
   */
  Quat &LoadQRotFromAxisAngle (const Vect &ax, float64 ang)
  {
    Vect nax (ax.Norm ());
    float64 s = sin (ang *= 0.5);
    a = cos (ang);
    i = s * nax.x;
    j = s * nax.y;
    k = s * nax.z;
    return *this;
  }

  /**
   * the same, but goes into a Quat on the stack.
   */
  //@{
  static Quat QRotFromAxisAngle (const Vect &ax, float64 ang)
  {
    Vect nax (ax.Norm ());
    float64 s = sin (ang *= 0.5);
    return Quat (cos (ang), s * nax.x, s * nax.y, s * nax.z);
  }
  static Quat QRotFromAxisDegAngle (const Vect &ax, float64 dang)
  {
    Vect nax (ax.Norm ());
    float64 s = sin (dang *= 0.5 * (M_PI / 180.0));
    return Quat (cos (dang), s * nax.x, s * nax.y, s * nax.z);
  }
  //@}

  /**
   * more of same, but with a pointer to a Vect instead of a reference.
   */
  //@{
  static Quat QRotFromAxisAngle (const Vect *ax, float64 ang)
  {
    return QRotFromAxisAngle (*ax, ang);
  }
  static Quat QRotFromAxisDegAngle (const Vect *ax, float64 dang)
  {
    return QRotFromAxisAngle (*ax, dang * M_PI / 180.0);
  }
  //@}

  /**
   * transforms the given point (\a p) by the rotation that is
   * (let's hope) expressed by the self-Quat; result is a new
   * Vect on the stack.
   */
  Vect QuatRotVect (const Vect &p) const;

  /**
   * returns, on the stack, a Matrix44 representing the same
   * three-space rotation as the self-Quat.
   */
  Matrix44 DeriveRotationMatrix () const;

  /**
   * as with the foregoing, except that you give it the pre-existing
   * Matrix44 to shove stuff into.
   */
  Matrix44 &LoadDerivedRotationMatrix (Matrix44 &m) const;

  /**
   * loads a quaternion which represents an orientation (a rotation from
   * the object frame to the world frame) from the norm (z-hat) and over
   * (x-hat) of the object in world coordinates.
   */
  Quat &LoadQRotFromNormOver (const Vect &norm, const Vect &over);

  /**
   * same, but Quat placed on stack.
   */
  static Quat QRotFromNormOver (const Vect &norm, const Vect &over);

  /**
   * loads a quaternion which represents the same rotation as the input matrix
   * puts result in this self-quat
   * \note matrix must be special orthogonal (determinate = 1 and trace > 0)
   */
  Quat &LoadQRotFromRotationMatrix (const Matrix44 &m);

  /**
   * same, but Quat placed on stack
   */
  static Quat QRotFromRotationMatrix (const Matrix44 &m);

  /**
   * provides quaternion representation of rotation from one coordinate frame
   * to another. (both must be orthogonal, right-handed coordinate frames)
   */
  Quat &LoadQRotFromCoordTransform (const Vect &x1, const Vect &y1,
                                    const Vect &z1, const Vect &x2,
                                    const Vect &y2, const Vect &z2);

  /**
   * same, but Quat place on stack
   */
  static Quat QRotFromCoordTransform (const Vect &x1, const Vect &y1,
                                      const Vect &z1, const Vect &x2,
                                      const Vect &y2, const Vect &z2);

  /**
   * the angle part, in radians, of the rotation represented by the self-Quat.
   * returns value in [-pi,pi]
   */
  float64 ExtractAngle () const
  {
    if (a < -1.0 || a > 1.0)
      return 0.0;

    float64 angle = 2.0 * acos (a);
    if (angle > M_PI)
      return (angle - 2.0 * M_PI);
    return angle;
  }

  /**
   * similarly, the axis part of this self-Quat's implicit rotation, returned
   * as a Vect on the stack.
   */
  Vect ExtractAxis () const { return Vect (i, j, k).Norm (); }

  // XXX: should all this comment-outed-ness just be deleted?
  // Or is its presence here serving some higher purpose?

  // performs spherical linear interpolation between this and the given quat.
  // the benefit here is that we maintain constant angular velocity when
  // animating rotations.
  // Slerp (q0, q1, t) == (q0*sin((1-t)*theta) + q1*sin(t*theta)) / sin(theta),
  //                      theta = acos (q0 . q1)
  //   where t == [0,1]
  // Quat Slerp (const Quat &q, float64 t)  const
  //   { t = obMax (0.0, obMin (t, 1.0));
  //     float64 theta = acos (Dot (q));
  //     return (*this * sin((1.0 - t) * theta) + q * sin (t * theta))
  //                                                        / sin (theta);
  //   }

  // same as above, but stores the result in 'this'

  // Quat& SlerpSelf (const Quat &q, float64 t)
  // { return Set (Slerp (q, t)); }


  /**
   * rotates 'this' 'lambda' of the way toward 'to'. To go halfway,
   * set lambda to 0.5, e.g. Assumes that 'this' is normalized
   */
  Quat UnitInterp (float64 lambda, const Quat &to) const
  {
    Quat delta = Conj () * to;
    const float64 delta_angle = delta.ExtractAngle ();

    delta.LoadQRotFromAxisAngle (delta.ExtractAxis (), delta_angle * lambda);
    Quat result = (*this) * delta;

    // The quaternion should already be normalized, but let's
    // avoid roundoff error accumulation...
    return result.Norm ();
  }

  /**
   * same as UnitInterp(), but stores the result in 'this'
   */
  Quat &UnitInterpSelf (float64 lambda, const Quat &to)
  {
    return Set (UnitInterp (lambda, to));
  }

  /**
   * returns the angle between two quaternions (rotated the shorter way)
   */
  float64 AngleBetween (const Quat &other) const
  {
    Quat diff = Conj () * other;
    return diff.ExtractAngle ();
  }

  /**
   * Returns a string representation, like
   * "Quat(0.000000, 1.000000, 2.000000, 3.000000)"
   */
  Str AsStr () const;
};
}
}  // viking funeral for you, namespace loam and namespace oblong


#endif

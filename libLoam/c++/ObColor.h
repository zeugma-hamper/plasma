
/* (c)  oblong industries */

#ifndef OB_COLOR_SMUDGINGS
#define OB_COLOR_SMUDGINGS


#include <libLoam/c/ob-api.h>
#include <libLoam/c/ob-types.h>
#include "libLoam/c/ob-log.h"

#include <libLoam/c++/ob-math-utils.h>
#include "libLoam/c++/LoamForward.h"


namespace oblong {
namespace loam {


#define OB_COLOR_OPAQ 1.0


/**
 * ObColor is a simple class whose instances represent colors that can
 * be expressed in RGB-space (with an optional alpha channel).
 *
 * The underlying representation is as four floating-point values, one
 * for each of R, G, B, and A; and it is understood that the minimum
 * and maximum values are 0.0 and 1.0 respectively.  That's not to say
 * that you can't crowbar a larger or smaller value in there. How you
 * choose to interpret 'negative red' is a matter between you and your
 * philosophical advisor(s).
 *
 * A tasty smorgasbord of constructors and setters allows the
 * discerning coder to -- according to inspiration -- specify colors
 * as float32s, float64s, or int32s. These last scamps (ints) are
 * converted to floating point values in the range [0.0, 1.0], but
 * here's the thing about that: an implicit assumption takes the
 * 'natural' integer range of color values to be [0, 255]. So there's
 * a giant 8-bit boondoggle there that won't much help out our 10- or
 * 12-bit-per-channel friends -- if, that is, they insist on using
 * integer values. We'll all get along just fine if we can agree to
 * use the spacious precision afforded by the humble float32. Right?
 */
class OB_LOAMXX_API ObColor
{
 public:
  /**
   * just what they look like -- RGBA-space color values, range [0.0,1.0].
   */
  float32 r, g, b, a;

  /**
   * default constructor's default color: fully opaque white.
   */
  ObColor ()
  {
    r = g = b = 1.0;
    a = OB_COLOR_OPAQ;
  }

  ObColor (const ObColor &c)
  {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
  }
  ObColor (const ObColor *c)
  {
    r = c->r;
    g = c->g;
    b = c->b;
    a = c->a;
  }
  ObColor (float32 R_in, float32 G_in, float32 B_in)
  {
    r = R_in;
    g = G_in;
    b = B_in;
    a = OB_COLOR_OPAQ;
  }
  ObColor (float64 R_in, float64 G_in, float64 B_in)
  {
    r = R_in;
    g = G_in;
    b = B_in;
    a = OB_COLOR_OPAQ;
  }

  /**
   * a constructor that takes ints; remember, range is taken to be [0,255]
   */
  ObColor (int32 R_in, int32 G_in, int32 B_in)
  {
    r = (float32) R_in / 255.0;
    g = (float32) G_in / 255.0;
    b = (float32) B_in / 255.0;
    a = OB_COLOR_OPAQ;
  }

  /**
   * just like it looks: a constructor for specifiying color with alpha
   */
  //@{
  ObColor (float32 R_in, float32 G_in, float32 B_in, float32 A_in)
  {
    r = R_in;
    g = G_in;
    b = B_in;
    a = A_in;
  }
  ObColor (float64 R_in, float64 G_in, float64 B_in, float64 A_in)
  {
    r = R_in;
    g = G_in;
    b = B_in;
    a = A_in;
  }
  //@}

  /**
   * a constructor that takes ints; remember, range is taken to be [0,255]
   */
  ObColor (int32 R_in, int32 G_in, int32 B_in, int32 A_in)
  {
    r = (float32) R_in / 255.0;
    g = (float32) G_in / 255.0;
    b = (float32) B_in / 255.0;
    a = (float32) A_in / 255.0;
  }

  /**
   * this constructor, with a single argument, makes an opaque grey.
   */
  //@{
  explicit ObColor (float32 grey)
  {
    r = g = b = grey;
    a = OB_COLOR_OPAQ;
  }
  explicit ObColor (float64 grey)
  {
    r = g = b = grey;
    a = OB_COLOR_OPAQ;
  }
  //@}

  /**
   * two float32ing arguments to the constructor makes arbitrarily
   * transparent grey.
   */
  //@{
  ObColor (float32 grey, float32 A_in)
  {
    r = g = b = grey;
    a = A_in;
  }
  ObColor (float64 grey, float64 A_in)
  {
    r = g = b = grey;
    a = A_in;
  }
  //@}

  /**
   * convert from libLoam 3-vector (RGB, 0-255, opaque)
   */
  explicit ObColor (const v3unt8 &v)
  {
    r = (float32) v.x / 255.0;
    g = (float32) v.y / 255.0;
    b = (float32) v.z / 255.0;
    a = OB_COLOR_OPAQ;
  }
  /**
   * convert from libLoam 3-vector (RGB, 0.0-1.0, opaque)
   */
  explicit ObColor (const v3float32 &v)
  {
    r = v.x;
    g = v.y;
    b = v.z;
    a = OB_COLOR_OPAQ;
  }
  /**
   * convert from libLoam 3-vector (RGB, 0.0-1.0, opaque)
   */
  explicit ObColor (const v3float64 &v)
  {
    r = v.x;
    g = v.y;
    b = v.z;
    a = OB_COLOR_OPAQ;
  }
  /**
   * convert from libLoam 4-vector (RGBA, 0-255)
   */
  explicit ObColor (const v4unt8 &v)
  {
    r = (float32) v.x / 255.0;
    g = (float32) v.y / 255.0;
    b = (float32) v.z / 255.0;
    a = (float32) v.w / 255.0;
  }
  /**
   * convert from libLoam 4-vector (RGBA, 0.0-1.0)
   */
  explicit ObColor (const v4float32 &v)
  {
    r = v.x;
    g = v.y;
    b = v.z;
    a = v.w;
  }
  /**
   * convert from libLoam 4-vector (RGBA, 0.0-1.0)
   */
  explicit ObColor (const v4float64 &v)
  {
    r = v.x;
    g = v.y;
    b = v.z;
    a = v.w;
  }

  void Delete () { delete this; }

  /**
   * a perfect copy of your color -- every time!
   */
  ObColor *Dup () const { return new ObColor (*this); }

  /**
   * a convenience method (if pitch blackness is convenient).
   */
  ObColor &MakeBlack ()
  {
    r = g = b = 0.0;
    a = OB_COLOR_OPAQ;
    return *this;
  }

  /**
   * Sets this color to opaque white and returns it.
   */
  ObColor &MakeWhite ()
  {
    r = g = b = 1.0;
    a = OB_COLOR_OPAQ;
    return *this;
  }

  /**
   * Sets this color to opaque red and returns it.
   */
  ObColor &MakeRed ()
  {
    r = 1.0;
    g = b = 0.0;
    a = OB_COLOR_OPAQ;
    return *this;
  }

  /**
   * Sets this color to opaque green and returns it.
   */
  ObColor &MakeGreen ()
  {
    g = 1.0;
    r = b = 0.0;
    a = OB_COLOR_OPAQ;
    return *this;
  }

  /**
   * Sets this color to opaque blue and returns it.
   */
  ObColor &MakeBlue ()
  {
    b = 1.0;
    r = g = 0.0;
    a = OB_COLOR_OPAQ;
    return *this;
  }

  /**
   * construct an initially invalid color
   */
  static ObColor Invalid () { return ObColor ().SetInvalid (); }

  /**
   * \return true if this color is valid, false otherwise
   */
  bool IsValid () const
  {
    return (!obIsNAN (r) && !obIsNAN (g) && !obIsNAN (b) && !obIsNAN (a));
  }
  /**
   * the opposite: \return true when the color's invalid; false
   * otherwise
   */
  bool IsInvalid () const { return (!IsValid ()); }

  /**
   * mark this color vector as invalid
   */
  ObColor &SetInvalid ()
  {
    r = g = b = a = OB_NAN;
    return *this;
  }

  /**
   * this and its subsequent kin are just like the constructors
   * foregoing.
   */
  //@{
  ObColor &Set (const ObColor &c)
  {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    return *this;
  }
  ObColor &Set (float32 R_in, float32 G_in, float32 B_in)
  {
    r = R_in;
    g = G_in;
    b = B_in;
    a = OB_COLOR_OPAQ;
    return *this;
  }
  ObColor &Set (float64 R_in, float64 G_in, float64 B_in)
  {
    r = R_in;
    g = G_in;
    b = B_in;
    a = OB_COLOR_OPAQ;
    return *this;
  }
  ObColor &Set (int32 R_in, int32 G_in, int32 B_in)
  {
    r = (float32) R_in / 255.0;
    g = (float32) G_in / 255.0;
    b = (float32) B_in / 255.0;
    a = OB_COLOR_OPAQ;
    return *this;
  }
  ObColor &Set (float32 R_in, float32 G_in, float32 B_in, float32 A_in)
  {
    r = R_in;
    g = G_in;
    b = B_in;
    a = A_in;
    return *this;
  }
  ObColor &Set (float64 R_in, float64 G_in, float64 B_in, float64 A_in)
  {
    r = R_in;
    g = G_in;
    b = B_in;
    a = A_in;
    return *this;
  }
  ObColor &Set (int32 R_in, int32 G_in, int32 B_in, int32 A_in)
  {
    r = (float32) R_in / 255.0;
    g = (float32) G_in / 255.0;
    b = (float32) B_in / 255.0;
    a = (float32) A_in / 255.0;
    return *this;
  }
  ObColor &Set (float32 grey)
  {
    r = g = b = grey;
    a = OB_COLOR_OPAQ;
    return *this;
  }
  ObColor &Set (float64 grey)
  {
    r = g = b = grey;
    a = OB_COLOR_OPAQ;
    return *this;
  }
  ObColor &Set (float32 grey, float32 A_in)
  {
    r = g = b = grey;
    a = A_in;
    return *this;
  }
  ObColor &Set (float64 grey, float64 A_in)
  {
    r = g = b = grey;
    a = A_in;
    return *this;
  }
  //@}

  operator v4float64 () const { return ToV4 (); }
  /**
   * export to libLoam 64-bit vector
   */
  v4float64 ToV4 () const { return v4float64{r, g, b, a}; }

  /**
   * export to libLoam 32-bit vector
   */
  v4float32 ToV4Float32 () const
  {
    return v4float32{(float32) r, (float32) g, (float32) b, (float32) a};
  }

  operator v4float32 () const { return ToV4Float32 (); }
  /**
   * Converts from Hue-Saturation-Brightness to RGB, and
   * sets this instance to that opaque color.
   *
   * \a h, \a s, and \a b are all between 0.0 and 1.0.
   * In particular, this means the hue is specified as a
   * fraction of a circle, not as an angle in degrees or radians.
   */
  const ObColor &SetHSB (float64 h, float64 s, float64 b);

  /**
   * an explicit method for setting the alpha component alone
   */
  //@{
  ObColor &SetAlpha (float32 A_in)
  {
    a = A_in;
    return *this;
  }
  ObColor &SetAlpha (float64 A_in)
  {
    a = A_in;
    return *this;
  }
  //@}
  /**
   * so too: a way to scale the alpha component alone
   * (<b>alpha_new = s * alpha_old</b>)
   */
  //@{
  ObColor &ScaleAlpha (float32 s)
  {
    a *= s;
    return *this;
  }
  ObColor &ScaleAlpha (float64 s)
  {
    a *= s;
    return *this;
  }
  //@}


  /**
   * how we get at individual components: choose from \b r, \b g,
   * \b b, and \b a
   */
  //@{
  float32 R () const { return r; }
  float32 G () const { return g; }
  float32 B () const { return b; }
  float32 A () const { return a; }
  //@}

  /**
   * as elsewhere in the g-speak codebase, equality means 'identical
   * for the purposes of use', not 'same object in memory'.
   */
  bool Equals (const ObColor &c) const
  {
    return (r == c.r && g == c.g && b == c.b && a == c.a);
  }
  /**
   * same, expressed as an operator: component-wise equality
   */
  bool operator== (const ObColor &c) const { return (Equals (c)); }
  /**
   * component-wise inequality
   */
  bool operator!= (const ObColor &c) const { return (!Equals (c)); }

  /**
   * Are all four components equal to zero?
   */
  bool IsZero () const
  {
    return (r == 0.0 && g == 0.0 && b == 0.0 && a == 0.0);
  }

  ObColor &operator= (const ObColor &c) { return Set (c); }
  ObColor &operator= (float32 grey) { return Set (grey); }

  /**
   * add a constant to each component of a ObColor; return a new one
   */
  ObColor operator+ (float64 f) const
  {
    return ObColor (r + f, g + f, b + f, a + f);
  }
  /**
   * subtract a constant from each component of a ObColor; return a new one
   */
  ObColor operator- (float64 f) const
  {
    return ObColor (r - f, g - f, b - f, a - f);
  }
  /**
   * mutiply each component of a ObColor by a constant; return a new one
   */
  ObColor operator* (float64 f) const
  {
    return ObColor (r * f, g * f, b * f, a * f);
  }
  /**
   * Bjarne's "friend" is no friend of ours; nonetheless we must use
   * it to address the case in which it's scalar-times-color in that
   * order...
   */
  friend ObColor operator* (float64 f, const ObColor &c) { return c * f; }
  /**
   * divide each component of a ObColor by a constant; return a new
   * one
   */
  ObColor operator/ (float64 f) const
  {
    f = ((f == 0.0) ? 1.0 : 1.0 / f);
    return ObColor (r * f, g * f, b * f, a * f);
  }

  /**
   * componentwise addition of two ObColors; return a new one
   */
  ObColor operator+ (const ObColor &c) const
  {
    return ObColor (r + c.r, g + c.g, b + c.b, a + c.a);
  }
  /**
   * componentwise subtraction of two ObColors; return a new one
   */
  ObColor operator- (const ObColor &c) const
  {
    return ObColor (r - c.r, g - c.g, b - c.b, a - c.a);
  }
  /**
   * componentwise multiplication of two ObColors; return a new one
   */
  ObColor operator* (const ObColor &c) const
  {
    return ObColor (r * c.r, g * c.g, b * c.b, a * c.a);
  }
  /**
   * componentwise division of two ObColors; return a new one
   */
  ObColor operator/ (const ObColor &c) const
  {
    return ObColor (r / (c.r != 0.0 ? c.r : 1.0), g / (c.g != 0.0 ? c.g : 1.0),
                    b / (c.b != 0.0 ? c.b : 1.0), a / (c.a != 0.0 ? c.a : 1.0));
  }

  /**
   * componentwise addition of "this" ObColor to another; returns this
   * one
   */
  ObColor &operator+= (const ObColor &c)
  {
    r += c.r;
    g += c.g;
    b += c.b;
    a += c.a;
    return *this;
  }
  /**
   * same, but add a scalar to each component (and return this one)
   */
  ObColor &operator+= (float64 f)
  {
    r += f;
    g += f;
    b += f;
    a += f;
    return *this;
  }
  /**
   * componentwise subtraction of another ObColor from "this"; returns
   * this one
   */
  ObColor &operator-= (const ObColor &c)
  {
    r -= c.r;
    g -= c.g;
    b -= c.b;
    a -= c.a;
    return *this;
  }
  /**
   * same, but subtract a scalar from each component (and return this
   * one)
   */
  ObColor &operator-= (float64 f)
  {
    r -= f;
    g -= f;
    b -= f;
    a -= f;
    return *this;
  }
  /**
   * componentwise mutiplication of "this" ObColor by another; returns
   * this one
   */
  ObColor &operator*= (const ObColor &c)
  {
    r *= c.r;
    g *= c.g;
    b *= c.b;
    a *= c.a;
    return *this;
  }
  /**
   * same, but multiply each component by a scalar (and return this one)
   */
  ObColor &operator*= (float64 f)
  {
    r *= f;
    g *= f;
    b *= f;
    a *= f;
    return *this;
  }
  /**
   * componentwise division of "this" ObColor by another; returns this
   * one
   */
  ObColor &operator/= (const ObColor &c)
  {
    r /= (c.r != 0.0 ? c.r : 1.0);
    g /= (c.g != 0.0 ? c.g : 1.0);
    b /= (c.b != 0.0 ? c.b : 1.0);
    a /= (c.a != 0.0 ? c.a : 1.0);
    return *this;
  }
  /**
   * same, but divide each component by a scalar (and return this one)
   */
  ObColor &operator/= (float64 f)
  {
    f = (f == 0.0 ? 1.0 : 1.0 / f);
    r *= f;
    g *= f;
    b *= f;
    a *= f;
    return *this;
  }

  /** String representation of this ObColor. */
  Str AsStr () const;
};
}
}  // the end? well, yes, if you're namespace loam and then namespace oblong


#endif

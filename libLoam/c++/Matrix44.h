
/* (c)  oblong industries */

#ifndef MATRIX44_MORASS
#define MATRIX44_MORASS


#include <cstddef>

#include <libLoam/c/ob-api.h>
#include <libLoam/c/ob-types.h>

#include <libLoam/c++/LoamForward.h>


namespace oblong {
namespace loam {


/**
 * Matrix44 is a representation of a four-by-four matrix -- the sort
 * that's pretty ubiquitous & fundamental in all sorts of CG contexts:
 * scaling, rotation, shearing, translation, and all combinations
 * thereof are compactly expressible using a 4x4 matrix. This is
 * partly to say that although Matrix44 is perfectly useful for any
 * general application of a matrix of that size, most of its methods
 * are to do with CG-style affine (and almost-affine) transformations.
 *
 * Awright. Lissenup. These matrices are stored in 'row-major order'
 * -- which nomenclature is itself confusing and stupid -- and in such
 * a way as to make 'standard' multiplication post-multiplication.
 * which in turn doesn't really mean much, until we get to vectors:
 * when we transform, we consider the vectors to be 'horizontal' and
 * thus to receive the multiplicative action of the matrix 'from the
 * right'. Note that many words are 'in quotes'. So are certain
 * 'phrases'. Further to the point, it's clarifying to note that the
 * translational elements in an affine transform matrix are 'at the
 * bottom', i.e. at m[3][0-2].
 *
 * So, good? Well, yes, until we get to OpenGL. Lovely people that
 * they are, they've chosen to do column-major stuff. Now that weirdly
 * works with us here, because they also have (conceptually) vertical
 * vectors, which receive matrices from the left. Splendid.
 */
class OB_LOAMXX_API Matrix44
{
 public:
  /**
   * \cond INTERNAL
   * A more perfect union: lets us view the matrix as several different
   * types, without having to worry about aliasing problems caused by
   * casting.  See bug 2376 comment 4 for discussion/rationale.
   */
  union
  {
    // the goods, stored in row-major order
    float64 m[4][4];
    // in case we want a single-dimensional array
    float64 f_[16];
    // or an array of vectors
    v4float64 v_[4];
  };
  /** \endcond */

  /**
   * handy to keep around both the zero matrix...
   */
  static const Matrix44 zeroMat;
  /**
   * The identity matrix (ones on the diagonal, zeros elsewhere)
   */
  static const Matrix44 idMat;

  /**
   * the basic constructor; you end up with the indentity matrix in
   * your new Matrix44.
   */
  Matrix44 ();
  /**
   * or, if it's more your style, a copy constructor -- \a ma as
   * provided is copied into the new Matrix44
   */
  Matrix44 (const Matrix44 &ma);


  // clang-format off
  /**
   * a constructor with a lot of arguments -- sixteen, in fact,
   * which are installed -- again, in row-major format -- into the
   * new Matrix44
   */
  Matrix44 (float64 m00, float64 m01, float64 m02, float64 m03,
            float64 m10, float64 m11, float64 m12, float64 m13,
            float64 m20, float64 m21, float64 m22, float64 m23,
            float64 m30, float64 m31, float64 m32, float64 m33);
  // clang-format on


  void Delete () { delete this; }


  /**
   * an analogous method that loads sixteen values into the
   * self-Matrix44 (which of course already exists)
   */
  Matrix44 &Load (float64 m00, float64 m01, float64 m02, float64 m03,
                  float64 m10, float64 m11, float64 m12, float64 m13,
                  float64 m20, float64 m21, float64 m22, float64 m23,
                  float64 m30, float64 m31, float64 m32, float64 m33);


  /**
   * loads the identity matrix into the self-Matrix44, (naturally)
   * overwriting whatever benighted values were there before.
   */
  Matrix44 &LoadIdent ();
  /**
   * same deal, but it's zeros everywhere.
   */
  Matrix44 &LoadZero ();

  /**
   * converts the specified three-space rotation, expressed in
   * axis-angle format (n.b.: radians for your trigonometric
   * pleasure), into matrix form. Normalizes the rotation axis
   * as part of its obsequiousness.
   */
  Matrix44 &LoadRotation (const Vect &axis, float64 angle);

  /** the more trusting version, in which the axis is presumed normalized. */
  Matrix44 &LoadRotationNoNorm (const Vect &axis, float64 angle);

  /**
   * similar, but the rotation will be 'performed' not around the
   * origin but rather about \a cent.
   */
  Matrix44 &LoadRotationAbout (const Vect &axis, float64 angle,
                               const Vect &cent);

  /**
   * yeah. like that, but without first normalizing axis.
   */
  Matrix44 &LoadRotationAboutNoNorm (const Vect &axis, float64 angle,
                                     const Vect &cent);

  /**
   * converts a translational displacement, passed via the specified
   * Vect, into matrix form
   */
  Matrix44 &LoadTranslation (const Vect &v);
  /**
   * ditto, except you pass the spatial displacements via explicit &
   * separate x, y, and z values.
   */
  Matrix44 &LoadTranslation (float64 x, float64 y, float64 z);

  /**
   * loads an anisotropic scale into the self-Matrix44 -- \a xs along
   * the x-axis, \a ys along the y-axis, and ...  well, you surely
   * have the idea now.
   */
  Matrix44 &LoadScale (float64 xs, float64 ys, float64 zs);
  /**
   * loads an isotropic scale into the self-Matrix44: same in every
   * direction
   */
  Matrix44 &LoadScale (float64 s);

  // clang-format off
  /**
   * prepares the self-Matrix44 with an arbitrary shear (linear
   * algebra enthusiasts will note that these are all the
   * off-diagonal elements on the top-left 3x3 submatrix,
   * i.e. <b>x_new  =  x_old  +  xBYy * y_old  +  xBYz * z_old</b>).
   */
  Matrix44 &LoadShear (float64 xBYy, float64 xBYz,
                       float64 yBYx, float64 yBYz,
                       float64 zBYx, float64 zBYy);
  // clang-format on

  /**
   * copies all elements from \a ma into the self-Matrix44.
   */
  Matrix44 &CopyFrom (const Matrix44 &ma);

  /**
   * this is a big one: multiplies the self-Matrix44 'from the right',
   * emitting the result as a fresh Matrix44 on the stack.
   */
  Matrix44 Mult (const Matrix44 &ma) const;

  /**
   * a copy operator: \a ma is copied into the self-Matrix44
   */
  Matrix44 &operator= (const Matrix44 &ma);
  /**
   * an operator-synonym for Mult() -- self-Matrix44 gets
   * right-multiplied by \a ma and dumped as a fresh Matrix44 on the
   * stack
   */
  Matrix44 operator* (const Matrix44 &ma) const;
  /**
   * an 'accumulative' multiply -- \a ma right-multiplies
   * self-Matrix44 and the result replaces the original elements in
   * the self-Matrix44
   */
  Matrix44 &operator*= (const Matrix44 &ma);

  /**
   * equality
   */
  bool operator== (const Matrix44 &that) const;
  /**
   * inequality
   */
  bool operator!= (const Matrix44 &that) const { return !(that == *this); }

  /**
   * returns, as a fresh Matrix44 on the stack, the transpose of the
   * self-Matrix44 (i.e. elements flopped across the diagonal)
   */
  Matrix44 Transpose () const;

  /**
   * transposes the self-Matrix44's elements, as above, but in situ.
   */
  Matrix44 &TransposeInPlace ();

  /**
     loads the transpose of the argument-Matrix44 into the self-Matrix44
  */
  Matrix44 &LoadTransposeOf (const Matrix44 &load_from);


  /**
   * returns the inverse matrix, be aware of singular matrices and numerical
   * instabilities.
   *
   * \param singularity_detected set to true if inversion failed due to singular
   * matrix
   */
  Matrix44 Invert (bool *singularity_detected = NULL) const;

  /**
   * as above, but modifies self
   */
  Matrix44 &InvertInPlace (bool *singularity_detected = NULL);

  /**
   * multiplies the supplied Vect (\a v), which is understood to be a
   * row vector, from the right by the self-Matrix44 and returns the
   * result as a fresh Vect on the stack.
   *
   * Oh, heck. There's nothing for it -- this is the time and place
   * for a mini-exegesis on 3- vs. 4-vectors.  When we multiply a Vect
   * (which has three components, x, y, and z) by a Matrix44, we
   * pretend that the 3-vect is actually a 4-vect with the fourth
   * component set to one -- i.e. (x y z 1) -- which allows the bottom
   * row of the Matrix44 (that'd be m[3][*]) to affect x, y, and
   * z. Recall that it's this bottom matrix row that stores the
   * translation components of the transformation, so that without the
   * prosthetic 1 stapled onto the vector we wouldn't be able to 'do'
   * translation -- only scale, rotation, shear. Which is not nearly
   * as much fun. Cf. TransformVect4(), below.
   */
  Vect TransformVect (const Vect &v) const;
  /**
   * likewise, but the result ends back in the original Vect (\a v).
   */
  Vect &TransformVectInPlace (Vect &v) const;

  /**
   * systematically and mercilessly transforms the supplied list of
   * vectors, multiplying each by the self-Matrix44 and plopping the
   * results into a <b>new Vect[num]</b> list -- that's what gets
   * returned, and you're responsible for <b>delete []</b>ing it.
   */
  Vect *TransformVectList (const Vect *v, int64 num) const;
  /**
   * as above, but you pass in the output Vect list (\a outV)
   * explicitly.
   */
  Vect *TransformVectList2List (const Vect *inV, Vect *outV, int64 num) const;
  /**
   * and the third alternative: the results are put right back into
   * the original list.
   */
  Vect *TransformVectListInPlace (Vect *v, int64 num) const;

  /**
   * Just like above, but with 4-vectors instead of 3-vectors. That is
   * to say: we perform the authentic, virtuous right-multiplication
   * of the supplied Vect4 by the self-Matrix44, and instead of
   * discarding the fourth vector component (as we must in the
   * 3-vector case above), it gets to hang around. Fact is, Vect4 is
   * little used in the g-speak code base (so far).
   */
  //@{
  Vect4 TransformVect4 (const Vect4 &v) const;
  Vect4 &TransformVect4InPlace (Vect4 &v) const;
  Vect4 *TransformVect4List (const Vect4 *v, int64 num) const;
  Vect4 *TransformVect4List2List (const Vect4 *inV, Vect4 *outV,
                                  int64 num) const;
  Vect4 *TransformVect4ListInPlace (Vect4 *v, int64 num) const;
  //@}

  Str AsStr () const;
  void SpewToStderr () const;
};
}
}  // fare thee well, namespace loam and namespace oblong


#endif

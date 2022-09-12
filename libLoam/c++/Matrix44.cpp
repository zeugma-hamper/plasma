
/* (c)  oblong industries */

/*
 * Note on glh:
 *
 * Matrix44::Invert () is based on glh::matrix4::inverse ()
 * The remainder of glh has been stripped. The following appears
 * as part of the obligation attending glh's aforementioned (adapted)
 * use.
 */

/*
    glh - is a platform-indepenedent C++ OpenGL helper library


    Copyright (c) 2000 Cass Everitt
  Copyright (c) 2000 NVIDIA Corporation
    All rights reserved.

    Redistribution and use in source and binary forms, with or
  without modification, are permitted provided that the following
  conditions are met:

     * Redistributions of source code must retain the above
     copyright notice, this list of conditions and the following
     disclaimer.

     * Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials
     provided with the distribution.

     * The names of contributors to this software may not be used
     to endorse or promote products derived from this software
     without specific prior written permission.

       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
     REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
     BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
     CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
     LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
     ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
     POSSIBILITY OF SUCH DAMAGE.


    Cass Everitt - cass@r3.nu
*/


#include <libLoam/c++/Matrix44.h>
#include <libLoam/c++/Str.h>
#include <libLoam/c++/Vect.h>
#include <libLoam/c++/Vect4.h>
#include <libLoam/c++/LoamStreams.h>

#include <libLoam/c/ob-util.h>

#include <ostream>


using namespace oblong::loam;


const Matrix44 Matrix44::zeroMat (0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
const Matrix44 Matrix44::idMat;


Matrix44::Matrix44 ()
{
  LoadIdent ();
}

Matrix44::Matrix44 (const Matrix44 &ma)
{
  CopyFrom (ma);
}


// clang-format off
Matrix44::Matrix44 (float64 m00, float64 m01, float64 m02, float64 m03,
                    float64 m10, float64 m11, float64 m12, float64 m13,
                    float64 m20, float64 m21, float64 m22, float64 m23,
                    float64 m30, float64 m31, float64 m32, float64 m33)
{
  m[0][0] = m00;  m[0][1] = m01;  m[0][2] = m02;  m[0][3] = m03;
  m[1][0] = m10;  m[1][1] = m11;  m[1][2] = m12;  m[1][3] = m13;
  m[2][0] = m20;  m[2][1] = m21;  m[2][2] = m22;  m[2][3] = m23;
  m[3][0] = m30;  m[3][1] = m31;  m[3][2] = m32;  m[3][3] = m33;
}


Matrix44 &Matrix44::Load (float64 m00, float64 m01, float64 m02, float64 m03,
                          float64 m10, float64 m11, float64 m12, float64 m13,
                          float64 m20, float64 m21, float64 m22, float64 m23,
                          float64 m30, float64 m31, float64 m32, float64 m33)
{
  m[0][0] = m00;  m[0][1] = m01;  m[0][2] = m02;  m[0][3] = m03;
  m[1][0] = m10;  m[1][1] = m11;  m[1][2] = m12;  m[1][3] = m13;
  m[2][0] = m20;  m[2][1] = m21;  m[2][2] = m22;  m[2][3] = m23;
  m[3][0] = m30;  m[3][1] = m31;  m[3][2] = m32;  m[3][3] = m33;
  return *this;
}
// clang-format on

Matrix44 &Matrix44::LoadIdent ()
{
  m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0;
  m[1][0] = m[2][0] = m[3][0] = 0.0;
  m[0][1] = m[2][1] = m[3][1] = 0.0;
  m[0][2] = m[1][2] = m[3][2] = 0.0;
  m[0][3] = m[1][3] = m[2][3] = 0.0;
  return *this;
}

Matrix44 &Matrix44::LoadZero ()
{
  OB_CLEAR (m);
  return *this;
}


Matrix44 &Matrix44::LoadRotation (const Vect &axis, float64 angle)
{
  Vect ax = axis.Norm ();
  return LoadRotationNoNorm (ax, angle);
}

Matrix44 &Matrix44::LoadRotationNoNorm (const Vect &axis, float64 ang)
{
  float64 SS, CC, sq1, sq2, sq3, C1;

  SS = sin (ang);
  CC = cos (ang);
  sq1 = axis.x * axis.x;
  sq2 = axis.y * axis.y;
  sq3 = axis.z * axis.z;
  C1 = (1.0 - CC);

  m[0][0] = sq1 + (1.0 - sq1) * CC;
  m[0][1] = axis.x * axis.y * C1 + axis.z * SS;
  m[0][2] = axis.x * axis.z * C1 - axis.y * SS;
  m[0][3] = 0.0;

  m[1][0] = axis.x * axis.y * C1 - axis.z * SS;
  m[1][1] = sq2 + (1.0 - sq2) * CC;
  m[1][2] = axis.y * axis.z * C1 + axis.x * SS;
  m[1][3] = 0.0;

  m[2][0] = axis.x * axis.z * C1 + axis.y * SS;
  m[2][1] = axis.y * axis.z * C1 - axis.x * SS;
  m[2][2] = sq3 + (1.0 - sq3) * CC;
  m[2][3] = 0.0;

  m[3][0] = m[3][1] = m[3][2] = 0.0;
  m[3][3] = 1.0;

  return *this;
}


Matrix44 &Matrix44::LoadRotationAbout (const Vect &axis, float64 angle,
                                       const Vect &cent)
{
  Vect ax = axis.Norm ();
  return LoadRotationAboutNoNorm (ax, angle, cent);
}


Matrix44 &Matrix44::LoadRotationAboutNoNorm (const Vect &axis, float64 angle,
                                             const Vect &cent)
{
  Matrix44 mA, mB;
  mA.LoadTranslation (-cent);
  mB.LoadRotation (axis, angle);
  mA *= mB;
  mB.LoadTranslation (cent);
  CopyFrom (mA * mB);
  return *this;
}


Matrix44 &Matrix44::LoadTranslation (const Vect &v)
{
  LoadIdent ();
  m[3][0] = v.x;
  m[3][1] = v.y;
  m[3][2] = v.z;
  return *this;
}

Matrix44 &Matrix44::LoadTranslation (float64 x, float64 y, float64 z)
{
  LoadIdent ();
  m[3][0] = x;
  m[3][1] = y;
  m[3][2] = z;
  return *this;
}


Matrix44 &Matrix44::LoadScale (float64 xs, float64 ys, float64 zs)
{
  LoadIdent ();
  m[0][0] = xs;
  m[1][1] = ys;
  m[2][2] = zs;
  return *this;
}

Matrix44 &Matrix44::LoadScale (float64 s)
{
  return LoadScale (s, s, s);
}


// clang-format off
Matrix44 &Matrix44::LoadShear (float64 xBYy, float64 xBYz,
                               float64 yBYx, float64 yBYz,
                               float64 zBYx, float64 zBYy)
{
  LoadIdent ();
  m[1][0] = xBYy;  m[2][0] = xBYz;
  m[0][1] = yBYx;  m[2][1] = yBYz;
  m[0][2] = zBYx;  m[1][2] = zBYy;
  return *this;
}
// clang-format on


Matrix44 &Matrix44::CopyFrom (const Matrix44 &ma)
{
  const float64 *from = &ma.m[0][0];
  float64 *to = &m[0][0];
  for (int q = 0; q < 16; ++q)
    *to++ = *from++;
  return *this;
}


#define BLATZ(a, b)                                                            \
  outM.m[a][b] = m[a][0] * ma.m[0][b] + m[a][1] * ma.m[1][b]                   \
                 + m[a][2] * ma.m[2][b] + m[a][3] * ma.m[3][b]

// clang-format off
Matrix44 Matrix44::Mult (const Matrix44 &ma) const
{
  Matrix44 outM;
  BLATZ (0, 0);  BLATZ (0, 1);  BLATZ (0, 2);  BLATZ (0, 3);
  BLATZ (1, 0);  BLATZ (1, 1);  BLATZ (1, 2);  BLATZ (1, 3);
  BLATZ (2, 0);  BLATZ (2, 1);  BLATZ (2, 2);  BLATZ (2, 3);
  BLATZ (3, 0);  BLATZ (3, 1);  BLATZ (3, 2);  BLATZ (3, 3);
  return Matrix44 (outM);
}
// clang-format on


Matrix44 &Matrix44::operator= (const Matrix44 &ma)
{
  return CopyFrom (ma);
}


Matrix44 Matrix44::operator* (const Matrix44 &ma) const
{
  return Mult (ma);
}

Matrix44 &Matrix44::operator*= (const Matrix44 &ma)
{
  return CopyFrom (Mult (ma));
}


bool Matrix44::operator== (const Matrix44 &that) const
{
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (m[i][j] != that.m[i][j])
        return false;
  return true;
}


// clang-format off
Matrix44 Matrix44::Transpose ()  const
{
  return Matrix44 (m[0][0], m[1][0], m[2][0], m[3][0],
                   m[0][1], m[1][1], m[2][1], m[3][1],
                   m[0][2], m[1][2], m[2][2], m[3][2],
                   m[0][3], m[1][3], m[2][3], m[3][3]);
}
// clang-format on

Matrix44 &Matrix44::TransposeInPlace ()
{
  return CopyFrom (Transpose ());
}

Matrix44 &Matrix44::LoadTransposeOf (const Matrix44 &load_from)
{
  if (&load_from == this)
    return (*this = load_from.Transpose ());

  m[0][0] = load_from.m[0][0];
  m[0][1] = load_from.m[1][0];
  m[0][2] = load_from.m[2][0];
  m[0][3] = load_from.m[3][0];

  m[1][0] = load_from.m[0][1];
  m[1][1] = load_from.m[1][1];
  m[1][2] = load_from.m[2][1];
  m[1][3] = load_from.m[3][1];

  m[2][0] = load_from.m[0][2];
  m[2][1] = load_from.m[1][2];
  m[2][2] = load_from.m[2][2];
  m[2][3] = load_from.m[3][2];

  m[3][0] = load_from.m[0][3];
  m[3][1] = load_from.m[1][3];
  m[3][2] = load_from.m[2][3];
  m[3][3] = load_from.m[3][3];

  return *this;
}


Matrix44 Matrix44::Invert (bool *singularity_detected) const
{
  Matrix44 minv;

  float64 r1[8], r2[8], r3[8], r4[8];
  float64 *s[4], *tmprow;

  s[0] = &r1[0];
  s[1] = &r2[0];
  s[2] = &r3[0];
  s[3] = &r4[0];

  int i, j, p, jj;
  for (i = 0; i < 4; ++i)
    {
      for (j = 0; j < 4; ++j)
        {
          s[i][j] = m[i][j];
          if (i == j)
            s[i][j + 4] = 1.0;
          else
            s[i][j + 4] = 0.0;
        }
    }

  float64 scp[4];
  for (i = 0; i < 4; ++i)
    {
      scp[i] = fabs (s[i][0]);
      for (j = 1; j < 4; ++j)
        if (fabs (s[i][j]) > scp[i])
          scp[i] = fabs (s[i][j]);
      if (scp[i] == 0.0)
        {
          if (singularity_detected)
            (*singularity_detected) = true;
          return minv;  // singular matrix!
        }
    }

  int pivot_to;
  float64 scp_max;
  for (i = 0; i < 4; ++i)
    {
      // select pivot row
      pivot_to = i;
      scp_max = fabs (s[i][i] / scp[i]);

      // find out which row should be on top
      for (p = i + 1; p < 4; ++p)
        if (fabs (s[p][i] / scp[p]) > scp_max)
          {
            scp_max = fabs (s[p][i] / scp[p]);
            pivot_to = p;
          }

      // Pivot if necessary
      if (pivot_to != i)
        {
          tmprow = s[i];
          s[i] = s[pivot_to];
          s[pivot_to] = tmprow;
          float64 tmpscp;
          tmpscp = scp[i];
          scp[i] = scp[pivot_to];
          scp[pivot_to] = tmpscp;
        }

      float64 mji;
      // perform gaussian elimination
      for (j = i + 1; j < 4; ++j)
        {
          mji = s[j][i] / s[i][i];
          s[j][i] = 0.0;
          for (jj = i + 1; jj < 8; ++jj)
            s[j][jj] -= mji * s[i][jj];
        }
    }

  if (s[3][3] == 0.0)
    {
      if (singularity_detected)
        (*singularity_detected) = true;
      return minv;  // singular matrix!
    }

  //
  // Now we have an upper triangular matrix.
  //
  //  x x x x | y y y y
  //  0 x x x | y y y y
  //  0 0 x x | y y y y
  //  0 0 0 x | y y y y
  //
  //  we'll back substitute to get the inverse
  //
  //  1 0 0 0 | z z z z
  //  0 1 0 0 | z z z z
  //  0 0 1 0 | z z z z
  //  0 0 0 1 | z z z z
  //

  float64 mij;
  for (i = 3; i > 0; --i)
    {
      for (j = i - 1; j > -1; --j)
        {
          mij = s[j][i] / s[i][i];
          for (jj = j + 1; jj < 8; ++jj)
            s[j][jj] -= mij * s[i][jj];
        }
    }

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      minv.m[i][j] = s[i][j + 4] / s[i][i];

  if (singularity_detected)
    (*singularity_detected) = false;

  return minv;
}


Matrix44 &Matrix44::InvertInPlace (bool *singularity_detected)
{
  bool s_d = false;
  Matrix44 mat = Invert (&s_d);

  if (singularity_detected)
    *singularity_detected = s_d;

  if (!s_d)
    CopyFrom (mat);

  return *this;
}


Vect Matrix44::TransformVect (const Vect &v) const
{
  return Vect (v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
               v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
               v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]);
}


Vect &Matrix44::TransformVectInPlace (Vect &v) const
{
  float64 XX = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
  float64 YY = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
  float64 ZZ = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
  v.x = XX;
  v.y = YY;
  v.z = ZZ;
  return v;
}



Vect *Matrix44::TransformVectList (const Vect *v, int64 num) const
{
  float64 xx, yy, zz;
  Vect *outV;
  Vect *vv;

  if (num < 1 || !v)
    return NULL;

  outV = new Vect[num];
  vv = outV;

  for (; num > 0; --num)
    {
      xx = v->x * m[0][0] + v->y * m[1][0] + v->z * m[2][0] + m[3][0];
      yy = v->x * m[0][1] + v->y * m[1][1] + v->z * m[2][1] + m[3][1];
      zz = v->x * m[0][2] + v->y * m[1][2] + v->z * m[2][2] + m[3][2];
      vv++->Set (xx, yy, zz);
      v++;
    }
  return outV;
}



Vect *Matrix44::TransformVectList2List (const Vect *fromV, Vect *toV,
                                        int64 num) const
{
  float64 xx, yy, zz;
  const Vect *v;
  Vect *vv;

  if (num < 1 || !fromV || !toV)
    return NULL;

  v = fromV;
  vv = toV;

  for (; num > 0; --num)
    {
      xx = v->x * m[0][0] + v->y * m[1][0] + v->z * m[2][0] + m[3][0];
      yy = v->x * m[0][1] + v->y * m[1][1] + v->z * m[2][1] + m[3][1];
      zz = v->x * m[0][2] + v->y * m[1][2] + v->z * m[2][2] + m[3][2];
      vv++->Set (xx, yy, zz);
      v++;
    }
  return toV;
}


Vect *Matrix44::TransformVectListInPlace (Vect *v, int64 num) const
{
  float64 xx, yy, zz;
  Vect *inV = v;

  if (num < 1 || !v)
    return NULL;

  for (; num > 0; --num)
    {
      xx = v->x * m[0][0] + v->y * m[1][0] + v->z * m[2][0] + m[3][0];
      yy = v->x * m[0][1] + v->y * m[1][1] + v->z * m[2][1] + m[3][1];
      zz = v->x * m[0][2] + v->y * m[1][2] + v->z * m[2][2] + m[3][2];
      v++->Set (xx, yy, zz);
    }
  return inV;
}



/*
 *
  the 4 stuff
 *
 */



Vect4 Matrix44::TransformVect4 (const Vect4 &v) const
{
  return Vect4 (v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0],
                v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1],
                v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2],
                v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3]);
}


Vect4 &Matrix44::TransformVect4InPlace (Vect4 &v) const
{
  float64 XX = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
  float64 YY = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
  float64 ZZ = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
  float64 WW = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];
  v.x = XX;
  v.y = YY;
  v.z = ZZ;
  v.w = WW;
  return v;
}


Vect4 *Matrix44::TransformVect4List (const Vect4 *v, int64 num) const
{
  float64 xx, yy, zz, ww;
  Vect4 *outV;
  Vect4 *vv;

  if (num < 1 || !v)
    return NULL;

  outV = new Vect4[num];
  vv = outV;

  for (; num > 0; --num)
    {
      xx = v->x * m[0][0] + v->y * m[1][0] + v->z * m[2][0] + v->w * m[3][0];
      yy = v->x * m[0][1] + v->y * m[1][1] + v->z * m[2][1] + v->w * m[3][1];
      zz = v->x * m[0][2] + v->y * m[1][2] + v->z * m[2][2] + v->w * m[3][2];
      ww = v->x * m[0][3] + v->y * m[1][3] + v->z * m[2][3] + v->w * m[3][3];
      vv++->Set (xx, yy, zz, ww);
      v++;
    }
  return outV;
}



Vect4 *Matrix44::TransformVect4List2List (const Vect4 *fromV, Vect4 *toV,
                                          int64 num) const
{
  float64 xx, yy, zz, ww;
  const Vect4 *v;
  Vect4 *vv;

  if (num < 1 || !fromV || !toV)
    return NULL;

  v = fromV;
  vv = toV;

  for (; num > 0; --num)
    {
      xx = v->x * m[0][0] + v->y * m[1][0] + v->z * m[2][0] + v->w * m[3][0];
      yy = v->x * m[0][1] + v->y * m[1][1] + v->z * m[2][1] + v->w * m[3][1];
      zz = v->x * m[0][2] + v->y * m[1][2] + v->z * m[2][2] + v->w * m[3][2];
      ww = v->x * m[0][3] + v->y * m[1][3] + v->z * m[2][3] + v->w * m[3][3];
      vv++->Set (xx, yy, zz, ww);
      v++;
    }
  return toV;
}



Vect4 *Matrix44::TransformVect4ListInPlace (Vect4 *v, int64 num) const
{
  float64 xx, yy, zz, ww;
  Vect4 *inV = v;

  if (num < 1 || !v)
    return NULL;

  for (; num > 0; --num)
    {
      xx = v->x * m[0][0] + v->y * m[1][0] + v->z * m[2][0] + v->w * m[3][0];
      yy = v->x * m[0][1] + v->y * m[1][1] + v->z * m[2][1] + v->w * m[3][1];
      zz = v->x * m[0][2] + v->y * m[1][2] + v->z * m[2][2] + v->w * m[3][2];
      ww = v->x * m[0][3] + v->y * m[1][3] + v->z * m[2][3] + v->w * m[3][3];
      v++->Set (xx, yy, zz, ww);
    }
  return inV;
}



Str Matrix44::AsStr () const
{
  return Str ().Sprintf ("m44float64(%f, %f, %f, %f ; %f, %f, %f, %f ; "
                         "%f, %f, %f, %f ; %f, %f, %f, %f)",
                         m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1],
                         m[1][2], m[1][3], m[2][0], m[2][1], m[2][2], m[2][3],
                         m[3][0], m[3][1], m[3][2], m[3][3]);
}


void Matrix44::SpewToStderr () const
{
  fprintf (stderr, "%s", AsStr ().utf8 ());
}


namespace oblong {
namespace loam {

::std::ostream &operator<< (::std::ostream &os, const oblong::loam::Matrix44 &m)
{
  return os << m.AsStr ();
}
}
}  // end oblong::loam

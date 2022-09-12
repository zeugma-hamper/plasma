
/* (c)  oblong industries */

#include <libLoam/c++/Quat.h>
#include <libLoam/c++/Str.h>
#include <libLoam/c++/LoamStreams.h>
#include <ostream>


using namespace oblong::loam;


Vect Quat::QuatRotVect (const Vect &p) const
{
  Quat qInv = Invert ();  // could reasonably use the lower-cost
  // conjugate here instead of inverse, since we're more or less
  // assuming the quaternion is already unit-normal... but doing
  // it the virtuous (i.e. Inverse(), not Conj()) way insures that
  // things will work out even if it isn't normalized.
  Quat qVect (0.0, p.x, p.y, p.z);
  Quat qOut ((*this * qVect) * qInv);
  return Vect (qOut.i, qOut.j, qOut.k);
}


Matrix44 Quat::DeriveRotationMatrix () const
{
  float64 ii = i * i, jj = j * j, kk = k * k;
  float64 ai = a * i, aj = a * j, ak = a * k;
  float64 ij = i * j, ik = i * k, jk = j * k;
  return Matrix44 (1.0 - 2.0 * (jj + kk), 2.0 * (ij + ak), 2.0 * (ik - aj), 0.0,
                   2.0 * (ij - ak), 1.0 - 2.0 * (ii + kk), 2.0 * (jk + ai), 0.0,
                   2.0 * (ik + aj), 2.0 * (jk - ai), 1.0 - 2.0 * (ii + jj), 0.0,
                   0.0, 0.0, 0.0, 1.0);
}


Matrix44 &Quat::LoadDerivedRotationMatrix (Matrix44 &m) const
{
  float64 ii = i * i, jj = j * j, kk = k * k;
  float64 ai = a * i, aj = a * j, ak = a * k;
  float64 ij = i * j, ik = i * k, jk = j * k;
  return m.Load (1.0 - 2.0 * (jj + kk), 2.0 * (ij + ak), 2.0 * (ik - aj), 0.0,
                 2.0 * (ij - ak), 1.0 - 2.0 * (ii + kk), 2.0 * (jk + ai), 0.0,
                 2.0 * (ik + aj), 2.0 * (jk - ai), 1.0 - 2.0 * (ii + jj), 0.0,
                 0.0, 0.0, 0.0, 1.0);
}


Quat &Quat::LoadQRotFromNormOver (const Vect &norm, const Vect &over)
{
  Vect yhat = norm.Cross (over);
  Matrix44 rot (over.X (), over.Y (), over.Z (), 0.0, yhat.X (), yhat.Y (),
                yhat.Z (), 0.0, norm.X (), norm.Y (), norm.Z (), 0.0, 0.0, 0.0,
                0.0, 1.0);
  return LoadQRotFromRotationMatrix (rot);
}


Quat Quat::QRotFromNormOver (const Vect &norm, const Vect &over)
{
  Quat q;
  q.LoadQRotFromNormOver (norm, over);
  return q;
}


Quat &Quat::LoadQRotFromRotationMatrix (const Matrix44 &m)
{
  float64 trace = m.m[0][0] + m.m[1][1] + m.m[2][2];
  if (trace > -0.9)
    {
      a = (1 + trace);
      i = m.m[1][2] - m.m[2][1];
      j = m.m[2][0] - m.m[0][2];
      k = m.m[0][1] - m.m[1][0];
    }
  else if ((m.m[0][0] > m.m[1][1]) && (m.m[0][0] > m.m[2][2]))
    {
      i = 1.0 + m.m[0][0] - m.m[1][1] - m.m[2][2];
      a = m.m[1][2] - m.m[2][1];
      j = m.m[0][1] + m.m[1][0];
      k = m.m[2][0] + m.m[0][2];
    }
  else if (m.m[1][1] > m.m[2][2])
    {
      j = 1.0 + m.m[1][1] - m.m[0][0] - m.m[2][2];
      a = m.m[2][0] - m.m[0][2];
      i = m.m[0][1] + m.m[1][0];
      k = m.m[1][2] + m.m[2][1];
    }
  else
    {
      k = 1.0 + m.m[2][2] - m.m[0][0] - m.m[1][1];
      a = m.m[0][1] - m.m[1][0];
      i = m.m[2][0] + m.m[0][2];
      j = m.m[1][2] + m.m[2][1];
    }
  float64 scale = 1.0 / sqrt (a * a + i * i + j * j + k * k);

  // because of our transposed convention, our quats are "inverses"
  // of those given by the standard listing of this algorithm.
  if (a < 0.0)
    scale *= -1.0;

  a *= scale;
  i *= scale;
  j *= scale;
  k *= scale;

  return *this;
}


Quat Quat::QRotFromRotationMatrix (const Matrix44 &m)
{
  Quat q;
  q.LoadQRotFromRotationMatrix (m);
  return q;
}


Quat &Quat::LoadQRotFromCoordTransform (const Vect &x1, const Vect &y1,
                                        const Vect &z1, const Vect &x2,
                                        const Vect &y2, const Vect &z2)
{
  Vect xA = x1.Norm (), yA = y1.Norm (), zA = z1.Norm ();
  Vect xB = x2.Norm (), yB = y2.Norm (), zB = z2.Norm ();

  Matrix44 m;
  m.m[0][0] = xB.Dot (xA);
  m.m[0][1] = xB.Dot (yA);
  m.m[0][2] = xB.Dot (zA);

  m.m[1][0] = yB.Dot (xA);
  m.m[1][1] = yB.Dot (yA);
  m.m[1][2] = yB.Dot (zA);

  m.m[2][0] = zB.Dot (xA);
  m.m[2][1] = zB.Dot (yA);
  m.m[2][2] = zB.Dot (zA);

  m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0;

  return LoadQRotFromRotationMatrix (m);
}


Quat Quat::QRotFromCoordTransform (const Vect &x1, const Vect &y1,
                                   const Vect &z1, const Vect &x2,
                                   const Vect &y2, const Vect &z2)
{
  Quat q;
  q.LoadQRotFromCoordTransform (x1, y1, z1, x2, y2, z2);
  return q;
}


Str Quat::AsStr () const
{
  return Str ().Sprintf ("Quat(%f, %f, %f, %f)", a, i, j, k);
}


namespace oblong {
namespace loam {

::std::ostream &operator<< (::std::ostream &os, const oblong::loam::Quat &q)
{
  return os << q.AsStr ();
}
}
}  // end oblong::loam

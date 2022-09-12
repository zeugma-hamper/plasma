
/* (c)  oblong industries */

#include "GeomTransform.h"

using namespace oblong::loam;

Frustum GeomTransform::Mult (const Frustum &frustum, const Matrix44 &mat)
{
  return Frustum (
    std::array<loam::Vect, 8>{{mat.TransformVect (frustum.corners[0]),
                               mat.TransformVect (frustum.corners[1]),
                               mat.TransformVect (frustum.corners[2]),
                               mat.TransformVect (frustum.corners[3]),
                               mat.TransformVect (frustum.corners[4]),
                               mat.TransformVect (frustum.corners[5]),
                               mat.TransformVect (frustum.corners[6]),
                               mat.TransformVect (frustum.corners[7])}});
}
Rectangle GeomTransform::Mult (const Rectangle &rect, const Matrix44 &mat)
{
  return Rectangle (mat.TransformVect (rect.corner),
                    mat.TransformVect (rect.leg1),
                    mat.TransformVect (rect.leg2));
}
Line GeomTransform::Mult (const Line &line, const Matrix44 &mat)
{
  return Line (mat.TransformVect (line.Point ()),
               mat.TransformVect (line.Dir ()));
}

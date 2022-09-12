
/* (c)  oblong industries */

#include "Frustum.h"

using namespace oblong::loam;

Frustum::Frustum (const std::array<Vect, 8> corners_)
{
  corners = corners_;
  ComputePlanes ();
}

Vect Frustum::CornerAt (Vertex v) const
{
  return NthCorner (static_cast<int32> (v));
}

Vect Frustum::NthCorner (int i) const
{
  if (i < 0 || i > 7)
    OB_LOG_BUG ("invalid corner index");
  return corners[i];
}

std::array<Vect, 8> Frustum::Corners () const
{
  return corners;
}

std::array<Plane, 6> Frustum::Planes () const
{
  return planes;
}

void Frustum::ComputePlanes ()
{
  // near plane
  planes[0] =
    Plane (corners[1],
           (corners[0] - corners[1]).Cross (corners[3] - corners[1]).Norm ());
  // right plane
  planes[1] =
    Plane (corners[1],
           (corners[3] - corners[1]).Cross (corners[5] - corners[1]).Norm ());
  // bottom plane
  planes[2] =
    Plane (corners[1],
           (corners[5] - corners[1]).Cross (corners[0] - corners[1]).Norm ());
  // left plane
  planes[3] =
    Plane (corners[2],
           (corners[4] - corners[0]).Cross (corners[2] - corners[0]).Norm ());
  // top plane
  planes[4] =
    Plane (corners[2],
           (corners[6] - corners[2]).Cross (corners[3] - corners[2]).Norm ());
  // far plane
  planes[5] =
    Plane (corners[4],
           (corners[5] - corners[4]).Cross (corners[6] - corners[4]).Norm ());
}

Str Frustum::AsStr () const
{
  return "Frustum[c1=" + corners[0].AsStr () + ", c2=" + corners[1].AsStr ()
         + ", c3=" + corners[2].AsStr () + ", c4=" + corners[3].AsStr ()
         + ", c5=" + corners[4].AsStr () + ", c6=" + corners[5].AsStr ()
         + ", c7=" + corners[6].AsStr () + ", c8=" + corners[7].AsStr () + "]";
}

bool Frustum::IsValid () const
{
  return corners[0].IsValid () && corners[1].IsValid () && corners[2].IsValid ()
         && corners[3].IsValid () && corners[4].IsValid ()
         && corners[5].IsValid () && corners[6].IsValid ()
         && corners[7].IsValid ();
}

Frustum Frustum::Invalid ()
{
  return Frustum (std::array<Vect, 8>{
    {Vect::Invalid (), Vect::Invalid (), Vect::Invalid (), Vect::Invalid (),
     Vect::Invalid (), Vect::Invalid (), Vect::Invalid (), Vect::Invalid ()}});
}


/* (c)  oblong industries */

#pragma once

#include <array>
#include "libLoam/c++/ObTrove.h"
#include "libLoam/c++/Plane.h"
#include "libLoam/c++/Str.h"
#include "libLoam/c++/Vect.h"
#include "libLoam/c/ob-log.h"
#include <vector>

namespace oblong {
namespace loam {

/**
 * An axis-aligned bounding box, constructible from a set of points in space,
 * which represents their extent.
 */
class OB_LOAMXX_API AxisAlignedBox
{
 public:
  /**
   * You are advised to read but not alter these public fields.
   */
  //@{
  Vect maximum;
  Vect minimum;
  //@}

  AxisAlignedBox () = default;

  /**
   * Create a bounding box from a collection of points.  It is illegal to pass
   * an empty collection of points to any of AxisAlignedBox's constructors.
   */
  //@{
  explicit AxisAlignedBox (const std::vector<Vect> &points);
  explicit AxisAlignedBox (const ObTrove<Vect> &points);
  template <size_t N>
  explicit AxisAlignedBox (const std::array<Vect, N> &points);
  explicit AxisAlignedBox (std::initializer_list<Vect> il);
  //@}

  /**
   * Create a bounding box from a pair of iterators.  It is illegal for \a
   * begin and \a end to be equal.
   */
  template <typename T>
  explicit AxisAlignedBox (T begin, T end);

  /**
   * Here are our names for the corners of an AABB
   * Near means highest Z, far means lowest Z
   * Top means highest Y, bottom means lowest Y
   * Right means highest X, left means lowest X
   */
  enum struct Vertex : int32
  {
    NearTopRight = 0,
    NearTopLeft = 1,
    NearBottomRight = 2,
    NearBottomLeft = 3,
    FarTopRight = 4,
    FarTopLeft = 5,
    FarBottomRight = 6,
    FarBottomLeft = 7,
  };
  /*
   * Retrieve one of the 8 corners of the box.
   */
  Vect CornerAt (Vertex v) const;
  Vect NthCorner (int32 i) const;

  /*
   * Build and retrieve all eight corners of the box.
   */
  std::array<Vect, 8> Corners () const;

  /*
   * Build and retrieve set of bounding planes.
   * Each plane is oriented facing inward.
   */
  std::array<Plane, 6> Planes () const;

  Str AsStr () const;

  bool IsValid () const;

  static AxisAlignedBox Invalid ();

};  // AxisAlignedBox

inline AxisAlignedBox::AxisAlignedBox (const std::vector<Vect> &points)
    : AxisAlignedBox (std::begin (points), std::end (points))
{
}

inline AxisAlignedBox::AxisAlignedBox (const ObTrove<Vect> &points)
    : AxisAlignedBox (std::begin (points), std::end (points))
{
}

template <size_t N>
AxisAlignedBox::AxisAlignedBox (const std::array<Vect, N> &points)
    : AxisAlignedBox (std::begin (points), std::end (points))
{
}

inline AxisAlignedBox::AxisAlignedBox (std::initializer_list<Vect> il)
    : AxisAlignedBox (std::begin (il), std::end (il))
{
}

template <typename T>
AxisAlignedBox::AxisAlignedBox (T begin, T end)
{
  auto it = begin;
  static_assert (std::is_constructible<Vect, decltype (*it)>::value,
                 "*T must produce a value that Vect is constructible from");
  if (it == end)
    {
      OB_LOG_ERROR ("empty points in constructor");
      minimum = Vect::Invalid ();
      maximum = Vect::Invalid ();
      return;
    }
  minimum = *it;
  maximum = *it;
  ++it;
  while (it != end)
    {
      const Vect cur = *it;
      minimum = Vect (std::min (minimum.x, cur.x), std::min (minimum.y, cur.y),
                      std::min (minimum.z, cur.z));
      maximum = Vect (std::max (maximum.x, cur.x), std::max (maximum.y, cur.y),
                      std::max (maximum.z, cur.z));
      ++it;
    }
}

}  // namespace loam
}  // namespace oblong

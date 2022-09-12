
/* (c)  oblong industries */

#pragma once

#include <array>
#include "libLoam/c++/Matrix44.h"
#include "libLoam/c++/Plane.h"
#include "libLoam/c++/Str.h"
#include "libLoam/c++/Vect.h"
#include "libLoam/c/ob-log.h"

namespace oblong {
namespace loam {

/**
 * Frustum represents a truncated pyramidal cone, such as your camera view.
 * Frustum need not be symmetric.
 */
class OB_LOAMXX_API Frustum
{
 public:
  /**
    * You are advised to read but not alter these public fields.
    */
  std::array<Vect, 8> corners;
  // planes point inward
  std::array<Plane, 6> planes;

  /**
    Construct a frustum using its 8 corner points.
    The following illustrates the array-coordinates of corners to be passed in
    Corners 0-3 on the front plane, 4-7 on the far plane

   [6]                              [7]
     \             FAR              /
      o----------------------------o
      |\                          /|     up
      | \                        / |     A
      |  \                      /  |     |
      |   \  [2]          [3]  /   |     |
      |    \ /              \ /    |     |
      |     o----------------o     |     O----> over
      |     |                |     |
      |     |                |     |
      o-----+----------------+-----o
     / \    |                |    / \
  [4]   \   |                |   /  [5]
         \  |                |  /
          \ |                | /
           \|                |/
            o----------------o
           /      near        \
         [0]                  [1]

  */
  Frustum (const std::array<Vect, 8> corners_);

  enum struct Vertex : int32
  {
    NearBottomLeft = 0,
    NearBottomRight = 1,
    NearTopLeft = 2,
    NearTopRight = 3,
    FarBottomLeft = 4,
    FarBottomRight = 5,
    FarTopLeft = 6,
    FarTopRight = 7,
  };

  /**
   * Retrieve one of the 8 points of the frustum.
   */
  Vect CornerAt (Vertex v) const;
  Vect NthCorner (int32 i) const;

  /**
   * Retrieve all eight corners of the frustum.
   */
  std::array<Vect, 8> Corners () const;

  /**
   * Retrieve set of bounding planes.
   * Each plane is oriented facing inward.
   */
  std::array<Plane, 6> Planes () const;

  Str AsStr () const;

  bool IsValid () const;

  static Frustum Invalid ();

 private:
  void ComputePlanes ();

};  // frustum

}  // namespace loam
}  // namespace oblong

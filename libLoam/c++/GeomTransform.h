
/* (c)  oblong industries */

#ifndef GEOM_TRANSFORM_ERS_DIRECTED_BY_MICHAEL_BAY
#define GEOM_TRANSFORM_ERS_DIRECTED_BY_MICHAEL_BAY

#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-attrs.h"
#include "libLoam/c/ob-types.h"
#include "libLoam/c++/Matrix44.h"
#include "libLoam/c++/Frustum.h"
#include "libLoam/c++/Rectangle.h"
#include "libLoam/c++/Line.h"

namespace oblong {
namespace loam {


/**
 * GeomTransform is an extendable namespace for pure functions that transform
 * geometric types, typically by "multiplying" them by a 4x4 matrix.
 */
namespace GeomTransform {

OB_LOAMXX_API Frustum Mult (const Frustum &frustum, const Matrix44 &matrix);
OB_LOAMXX_API Rectangle Mult (const Rectangle &rect, const Matrix44 &matrix);
OB_LOAMXX_API Line Mult (const Line &line, const Matrix44 &matrix);

}  // namespace GeomTransform

}  // namespace loam
}  // namespace oblong

#endif

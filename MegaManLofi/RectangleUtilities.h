#pragma once

#include "Rectangle.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class RectangleUtilities
   {
   public:
      static bool RectanglesIntersectF( const Rectangle<float>& r1, float r1LeftOffset, float r1TopOffset,
                                        const Rectangle<float>& r2, float r2LeftOffset, float r2TopOffset );
      static bool CoordinateInRectangleF( const Coordinate<float>& c, float cLeftOffset, float cTopOffset,
                                          const Rectangle<float>& r, float rLeftOffset, float rTopOffset );
   };
}

#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   class RectangleUtilities
   {
   public:
      static bool RectanglesIntersectF( const Rectangle<float>& r1, float r1LeftOffset, float r1TopOffset,
                                        const Rectangle<float>& r2, float r2LeftOffset, float r2TopOffset );
   };
}

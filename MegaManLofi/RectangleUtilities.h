#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   class RectangleUtilities
   {
   public:
      static bool RectanglesIntersect( const Rectangle<float>& rect1, const Rectangle<float>& rect2 );

      static void UnclipHorizontal( Rectangle<float>& clippingRect, const Rectangle<float>& clippedRect );
      static void UnclipVertical( Rectangle<float>& clippingRect, const Rectangle<float>& clippedRect );
   };
}

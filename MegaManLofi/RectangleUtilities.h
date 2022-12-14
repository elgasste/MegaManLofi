#pragma once

#include "Rectangle.h"

namespace MegaManLofi
{
   class RectangleUtilities
   {
   public:
      static bool RectanglesIntersect( const Rectangle<long long>& rect1, const Rectangle<long long>& rect2 );

      static void UnclipHorizontal( Rectangle<long long>& clippingRect, const Rectangle<long long>& clippedRect );
      static void UnclipVertical( Rectangle<long long>& clippingRect, const Rectangle<long long>& clippedRect );
   };
}

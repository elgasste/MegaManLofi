#pragma once

namespace MegaManLofi
{
   struct Rectangle;

   class RectangleUtilities
   {
   public:
      static bool RectanglesIntersect( const Rectangle& rect1, const Rectangle& rect2 );

      static void UnclipHorizontal( Rectangle& clippingRect, const Rectangle& clippedRect );
      static void UnclipVertical( Rectangle& clippingRect, const Rectangle& clippedRect );
   };
}

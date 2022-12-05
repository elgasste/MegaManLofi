#include "RectangleUtilities.h"
#include "Rectangle.h"

using namespace std;
using namespace MegaManLofi;

bool RectangleUtilities::RectanglesIntersect( const Rectangle& rect1, const Rectangle& rect2 )
{
   auto rect1Right = rect1.Left + rect1.Width;
   auto rect2Right = rect2.Left + rect2.Width;
   auto rect1Bottom = rect1.Top + rect1.Height;
   auto rect2Bottom = rect2.Top + rect2.Height;

   bool leftInBounds = rect1.Left > rect2.Left && rect1Right < rect2Right;
   bool rightInBounds = rect1Right > rect2.Left && rect1Right < rect2Right;
   bool topInBounds = rect1.Top > rect2.Top && rect1.Top < rect2Bottom;
   bool bottomInBounds = rect1Bottom > rect1.Top && rect1Bottom < rect2Bottom;

   return ( leftInBounds && topInBounds ) || ( rightInBounds && topInBounds ) ||
          ( leftInBounds && bottomInBounds ) || ( rightInBounds && bottomInBounds );
}

void RectangleUtilities::UnclipHorizontal( Rectangle& clippingRect, const Rectangle& clippedRect )
{
   auto clippingRectMiddle = clippingRect.Left + ( clippingRect.Width / 2 );
   auto clippedRectMiddle = clippedRect.Left + ( clippedRect.Width / 2 );

   if ( clippingRectMiddle < clippedRectMiddle )
   {
      clippingRect.Left = clippedRect.Left - clippingRect.Width;
   }
   else
   {
      clippingRect.Left = clippedRect.Left + clippedRect.Width;
   }
}

void RectangleUtilities::UnclipVertical( Rectangle& clippingRect, const Rectangle& clippedRect )
{
   auto clippingRectMiddle = clippingRect.Top + ( clippingRect.Height / 2 );
   auto clippedRectMiddle = clippedRect.Top + ( clippedRect.Height / 2 );

   if ( clippingRectMiddle < clippedRectMiddle )
   {
      clippingRect.Top = clippedRect.Top - clippingRect.Height;
   }
   else
   {
      clippingRect.Top = clippedRect.Top + clippedRect.Height;
   }
}

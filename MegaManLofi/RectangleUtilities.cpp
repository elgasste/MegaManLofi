#include "RectangleUtilities.h"

using namespace std;
using namespace MegaManLofi;

bool RectangleUtilities::RectanglesIntersectF( const Rectangle<float>& r1, float r1LeftOffset, float r1TopOffset,
                                               const Rectangle<float>& r2, float r2LeftOffset, float r2TopOffset )
{
   auto r1Left = r1.Left + r1LeftOffset;
   auto r1Top = r1.Top + r1TopOffset;
   auto r1Right = r1Left + r1.Width;
   auto r1Bottom = r1Top + r1.Height;

   auto r2Left = r2.Left + r2LeftOffset;
   auto r2Top = r2.Top + r2TopOffset;
   auto r2Right = r2Left + r2.Width;
   auto r2Bottom = r2Top + r2.Height;

   bool leftInBounds = r1Left > r2Left && r1Right < r2Right;
   bool rightInBounds = r1Right > r2Left && r1Right < r2Right;
   bool topInBounds = r1Top > r2Top && r1Top < r2Bottom;
   bool bottomInBounds = r1Bottom > r2Top && r1Bottom < r2Bottom;

   return ( leftInBounds && topInBounds ) || ( rightInBounds && topInBounds ) ||
          ( leftInBounds && bottomInBounds ) || ( rightInBounds && bottomInBounds );
}

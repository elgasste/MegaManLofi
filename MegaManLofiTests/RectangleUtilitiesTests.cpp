#include "gtest/gtest.h"

#include <MegaManLofi/RectangleUtilities.h>
#include <MegaManLofi/Rectangle.h>

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class RectangleUtilitiesTests : public Test { };

TEST_F( RectangleUtilitiesTests, RectanglesIntersect_FarUpperLeft_ReturnsFalse )
{
   Rectangle<long long> r1 { 0, 0, 10, 10 };
   Rectangle<long long> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersect( r1, r2 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersect_FarUpperRight_ReturnsFalse )
{
   Rectangle<long long> r1 { 10, 0, 10, 10 };
   Rectangle<long long> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersect( r1, r2 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersect_FarLowerRight_ReturnsFalse )
{
   Rectangle<long long> r1 { 10, 10, 10, 10 };
   Rectangle<long long> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersect( r1, r2 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersect_FarLowerLeft_ReturnsFalse )
{
   Rectangle<long long> r1 { 0, 10, 10, 10 };
   Rectangle<long long> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersect( r1, r2 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersect_LowerRightIntersects_ReturnsTrue )
{
   Rectangle<long long> r1 { 1, 1, 10, 10 };
   Rectangle<long long> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersect( r1, r2 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersect_LowerLeftIntersects_ReturnsTrue )
{
   Rectangle<long long> r1 { 9, 1, 10, 10 };
   Rectangle<long long> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersect( r1, r2 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersect_UpperRightIntersects_ReturnsTrue )
{
   Rectangle<long long> r1 { 1, 9, 10, 10 };
   Rectangle<long long> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersect( r1, r2 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersect_UpperLeftIntersects_ReturnsTrue )
{
   Rectangle<long long> r1 { 9, 9, 10, 10 };
   Rectangle<long long> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersect( r1, r2 ) );
}

TEST_F( RectangleUtilitiesTests, UnclipHorizontal_OnTheLeftSide_ClampsToTheLeftSide )
{
   Rectangle<long long> clippingRect { 1, 1, 10, 10 };
   Rectangle<long long> clippedRect { 10, 10, 10, 10 };

   RectangleUtilities::UnclipHorizontal( clippingRect, clippedRect );

   EXPECT_EQ( clippingRect.Left, 0 );
}

TEST_F( RectangleUtilitiesTests, UnclipHorizontal_OnTheRightSide_ClampsToTheRighttSide )
{
   Rectangle<long long> clippingRect { 19, 1, 10, 10 };
   Rectangle<long long> clippedRect { 10, 10, 10, 10 };

   RectangleUtilities::UnclipHorizontal( clippingRect, clippedRect );

   EXPECT_EQ( clippingRect.Left, 20 );
}

TEST_F( RectangleUtilitiesTests, UnclipVertical_OnTheTopSide_ClampsToTheTopSide )
{
   Rectangle<long long> clippingRect { 0, 1, 10, 10 };
   Rectangle<long long> clippedRect { 10, 10, 10, 10 };

   RectangleUtilities::UnclipVertical( clippingRect, clippedRect );

   EXPECT_EQ( clippingRect.Top, 0 );
}

TEST_F( RectangleUtilitiesTests, UnclipVertical_OnTheBottomSide_ClampsToTheBottomSide )
{
   Rectangle<long long> clippingRect { 0, 19, 10, 10 };
   Rectangle<long long> clippedRect { 10, 10, 10, 10 };

   RectangleUtilities::UnclipVertical( clippingRect, clippedRect );

   EXPECT_EQ( clippingRect.Top, 20 );
}

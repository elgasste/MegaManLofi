#include "gtest/gtest.h"

#include <MegaManLofi/RectangleUtilities.h>
#include <MegaManLofi/Rectangle.h>

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class RectangleUtilitiesTests : public Test { };

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_FarUpperLeftNoOffsets_ReturnsFalse )
{
   Rectangle<float> r1 { 0, 0, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersectF( r1, 0, 0, r2, 0, 0 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_FarUpperRightNoOffsets_ReturnsFalse )
{
   Rectangle<float> r1 { 10, 0, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersectF( r1, 0, 0, r2, 0, 0 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_FarLowerRightNoOffsets_ReturnsFalse )
{
   Rectangle<float> r1 { 10, 10, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersectF( r1, 0, 0, r2, 0, 0 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_FarLowerLeftNoOffsets_ReturnsFalse )
{
   Rectangle<float> r1 { 0, 10, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersectF( r1, 0, 0, r2, 0, 0 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_LowerRightIntersectsNoOffsets_ReturnsTrue )
{
   Rectangle<float> r1 { 1, 1, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersectF( r1, 0, 0, r2, 0, 0 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_LowerLeftIntersectsNoOffsets_ReturnsTrue )
{
   Rectangle<float> r1 { 9, 1, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersectF( r1, 0, 0, r2, 0, 0 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_UpperRightIntersectsNoOffsets_ReturnsTrue )
{
   Rectangle<float> r1 { 1, 9, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersectF( r1, 0, 0, r2, 0, 0 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_UpperLeftIntersectsNoOffsets_ReturnsTrue )
{
   Rectangle<float> r1 { 9, 9, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersectF( r1, 0, 0, r2, 0, 0 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_FarUpperLeftWithOffsets_ReturnsFalse )
{
   Rectangle<float> r1 { 0, 0, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersectF( r1, 1000, 2000, r2, 1000, 2000 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_FarUpperRightWithOffsets_ReturnsFalse )
{
   Rectangle<float> r1 { 10, 0, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersectF( r1, 1000, 2000, r2, 1000, 2000 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_FarLowerRightWithOffsets_ReturnsFalse )
{
   Rectangle<float> r1 { 10, 10, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersectF( r1, 1000, 2000, r2, 1000, 2000 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_FarLowerLeftWithOffsets_ReturnsFalse )
{
   Rectangle<float> r1 { 0, 10, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_FALSE( RectangleUtilities::RectanglesIntersectF( r1, 1000, 2000, r2, 1000, 2000 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_LowerRightIntersectsWithOffsets_ReturnsTrue )
{
   Rectangle<float> r1 { 1, 1, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersectF( r1, 1000, 2000, r2, 1000, 2000 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_LowerLeftIntersectsWithOffsets_ReturnsTrue )
{
   Rectangle<float> r1 { 9, 1, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersectF( r1, 1000, 2000, r2, 1000, 2000 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_UpperRightIntersectsWithOffsets_ReturnsTrue )
{
   Rectangle<float> r1 { 1, 9, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersectF( r1, 1000, 2000, r2, 1000, 2000 ) );
}

TEST_F( RectangleUtilitiesTests, RectanglesIntersectF_UpperLeftIntersectsWithOffsets_ReturnsTrue )
{
   Rectangle<float> r1 { 9, 9, 10, 10 };
   Rectangle<float> r2 { 10, 10, 10, 10 };

   EXPECT_TRUE( RectangleUtilities::RectanglesIntersectF( r1, 1000, 2000, r2, 1000, 2000 ) );
}

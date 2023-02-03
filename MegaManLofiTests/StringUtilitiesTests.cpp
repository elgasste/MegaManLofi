#include "gtest/gtest.h"

#include <MegaManLofi/StringUtilities.h>

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class StringUtilitiesTests : public Test { };

TEST_F( StringUtilitiesTests, Explode_StringIsEmpty_ReturnsEmptyVector )
{
   auto tokens = StringUtilities::Explode( "", ':' );

   EXPECT_EQ( tokens.size(), 0 );
}

TEST_F( StringUtilitiesTests, Explode_DelimiterNotFound_ReturnsVectorWithEntireString )
{
   auto str = "This is a string with no delimiters";

   auto tokens = StringUtilities::Explode( str, ':' );

   EXPECT_EQ( tokens.size(), 1 );
   EXPECT_EQ( tokens[0], str );
}

TEST_F( StringUtilitiesTests, Explode_OnlyDelimitersFound_ReturnsVectorWithEmptyStrings )
{
   auto tokens = StringUtilities::Explode( "::", ':' );

   EXPECT_EQ( tokens.size(), 2 );
   EXPECT_EQ( tokens[0], "" );
   EXPECT_EQ( tokens[1], "" );
}

TEST_F( StringUtilitiesTests, Explode_DelimitersFound_ReturnsVectorWithSeparateStrings )
{
   auto tokens = StringUtilities::Explode( "  part 1 :part2  : part 3   ", ':' );

   EXPECT_EQ( tokens.size(), 3 );
   EXPECT_EQ( tokens[0], "  part 1 " );
   EXPECT_EQ( tokens[1], "part2  " );
   EXPECT_EQ( tokens[2], " part 3   " );
}

TEST_F( StringUtilitiesTests, ExplodeByWhitespace_StringIsEmpty_ReturnsEmptyVector )
{
   auto tokens = StringUtilities::ExplodeByWhitespace( "" );

   EXPECT_EQ( tokens.size(), 0 );
}

TEST_F( StringUtilitiesTests, ExplodeByWhitespace_StringIsOnlySpaces_ReturnsEmptyVector )
{
   auto tokens = StringUtilities::ExplodeByWhitespace( "   " );

   EXPECT_EQ( tokens.size(), 0 );
}

TEST_F( StringUtilitiesTests, ExplodeByWhitespace_StringIsOnlyTabs_ReturnsEmptyVector )
{
   auto tokens = StringUtilities::ExplodeByWhitespace( "\t\t\t" );

   EXPECT_EQ( tokens.size(), 0 );
}

TEST_F( StringUtilitiesTests, ExplodeByWhitespace_StringIsOnlySpacesAndTabs_ReturnsEmptyVector )
{
   auto tokens = StringUtilities::ExplodeByWhitespace( "  \t \t\t  \t" );

   EXPECT_EQ( tokens.size(), 0 );
}

TEST_F( StringUtilitiesTests, ExplodeByWhitespace_StringHasOneToken_ReturnsVectorWithToken )
{
   auto tokens = StringUtilities::ExplodeByWhitespace( "   \t \t\t \tBanana   \t" );

   EXPECT_EQ( tokens.size(), 1 );
   EXPECT_EQ( tokens[0], "Banana" );
}

TEST_F( StringUtilitiesTests, ExplodeByWhitespace_StringHasMultipleTokens_ReturnsVectorWithTokens )
{
   auto tokens = StringUtilities::ExplodeByWhitespace( "Apple\t  \t\t Banana Cherry\t      \tGrape   \t" );

   EXPECT_EQ( tokens.size(), 4 );
   EXPECT_EQ( tokens[0], "Apple" );
   EXPECT_EQ( tokens[1], "Banana" );
   EXPECT_EQ( tokens[2], "Cherry" );
   EXPECT_EQ( tokens[3], "Grape" );
}

TEST_F( StringUtilitiesTests, ToUpper_StringIsAllUpperCase_ReturnsAllUpperCaseString )
{
   auto result = StringUtilities::ToUpper( "BANANA" );

   EXPECT_EQ( result, "BANANA" );
}

TEST_F( StringUtilitiesTests, ToUpper_StringIsAllLowerCase_ReturnsAllUpperCaseString )
{
   auto result = StringUtilities::ToUpper( "banana" );

   EXPECT_EQ( result, "BANANA" );
}

TEST_F( StringUtilitiesTests, ToUpper_StringIsMixedUpperAndLowerCase_ReturnsAllUpperCaseString )
{
   auto result = StringUtilities::ToUpper( "bAnaNA" );

   EXPECT_EQ( result, "BANANA" );
}

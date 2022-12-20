#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/ConsoleSprite.h>

#include "mock_FrameRateProvider.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ConsoleSpriteTests : public Test { };

TEST_F( ConsoleSpriteTests, Reset_Always_ResetsToFirstImage )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0.75 ) );
   sprite->AddImage( { 0, 0 } );
   sprite->AddImage( { 1, 1 } );
   sprite->AddImage( { 2, 2 } );

   sprite->Tick( 1 ); // 1 second has passed, should be on the second image
   EXPECT_EQ( sprite->GetCurrentImage().Width, 1 );

   sprite->Reset(); // back to first image
   EXPECT_EQ( sprite->GetCurrentImage().Width, 0 );

   sprite->Tick( 1 ); // 1 second has passed, should be on the second image again
   EXPECT_EQ( sprite->GetCurrentImage().Width, 1 );
}

TEST_F( ConsoleSpriteTests, GetCurrentImage_ElapsedTimeMatchesTotalSpriteTime_ReturnsFirstImage )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0.25 ) );
   sprite->AddImage( { 0, 0 } );
   sprite->AddImage( { 1, 1 } );
   sprite->AddImage( { 2, 2 } );
   sprite->AddImage( { 3, 3 } );

   sprite->Tick( 1 ); // 1 second has passed, should be back on the first image

   EXPECT_EQ( sprite->GetCurrentImage().Width, 0 );
   EXPECT_EQ( sprite->GetCurrentImage().Height, 0 );
}

TEST_F( ConsoleSpriteTests, GetCurrentImage_ElapsedTimeLandsOnImageBoundary_ReturnsCorrectImage )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0.25 ) );
   sprite->AddImage( { 0, 0 } );
   sprite->AddImage( { 1, 1 } );
   sprite->AddImage( { 2, 2 } );
   sprite->AddImage( { 3, 3 } );

   sprite->Tick( 4 ); // 0.25 seconds have passed, should be on second image

   EXPECT_EQ( sprite->GetCurrentImage().Width, 1 );
   EXPECT_EQ( sprite->GetCurrentImage().Height, 1 );
}

TEST_F( ConsoleSpriteTests, GetWidth_Always_ReturnsWidthOfCurrentImage )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0.75 ) );
   sprite->AddImage( { 0, 1 } );
   sprite->AddImage( { 2, 3 } );
   sprite->AddImage( { 4, 5 } );
   sprite->AddImage( { 6, 7 } );

   sprite->Tick( 1 ); // 1 second has passed, should be on second image

   EXPECT_EQ( sprite->GetCurrentImage().Width, 2 );
}

TEST_F( ConsoleSpriteTests, GetHeight_Always_ReturnsHeightOfCurrentImage )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0.75 ) );
   sprite->AddImage( { 0, 1 } );
   sprite->AddImage( { 2, 3 } );
   sprite->AddImage( { 4, 5 } );
   sprite->AddImage( { 6, 7 } );

   sprite->Tick( 1 ); // 1 second has passed, should be on second image
   sprite->Tick( 1 ); // 2 seconds have passed, should be on third image

   EXPECT_EQ( sprite->GetCurrentImage().Height, 5 );
}

TEST_F( ConsoleSpriteTests, GetTotalTraversalSeconds_Always_ReturnsTotalTraversalSeconds )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 1 ) );
   sprite->AddImage( { 0, 1 } );
   sprite->AddImage( { 2, 3 } );
   sprite->AddImage( { 4, 5 } );

   EXPECT_EQ( sprite->GetTotalTraversalSeconds(), 3 );
}

TEST_F( ConsoleSpriteTests, Tick_ImageTraversalIsZeroSeconds_MovesDirectlyToTheNextImage )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0 ) );
   sprite->AddImage( { 0, 1 } );
   sprite->AddImage( { 2, 3 } );
   sprite->AddImage( { 4, 5 } );
   sprite->AddImage( { 6, 7 } );

   sprite->Tick( 1 ); // should be on second image
   sprite->Tick( 1 ); // should be on third image
   sprite->Tick( 1 ); // should be on fourth image

   EXPECT_EQ( sprite->GetCurrentImage().Width, 6 );
}

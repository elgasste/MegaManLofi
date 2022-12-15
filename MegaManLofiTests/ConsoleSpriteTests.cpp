#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/ConsoleSprite.h>

#include "mock_FrameRateProvider.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ConsoleSpriteTests : public Test
{
public:
   void SetUp() override
   {
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
};

TEST_F( ConsoleSpriteTests, GetCurrentImage_ElapsedTimeMatchesTotalSpriteTime_ReturnsFirstImage )
{
   ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 1 ) );

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( _frameRateProviderMock, 0.25 ) );
   sprite->AddImage( { 0, 0 } );
   sprite->AddImage( { 1, 1 } );
   sprite->AddImage( { 2, 2 } );
   sprite->AddImage( { 3, 3 } );

   sprite->Tick(); // 1 second has passed, should be back on the first image

   EXPECT_EQ( sprite->GetCurrentImage().Width, 0 );
   EXPECT_EQ( sprite->GetCurrentImage().Height, 0 );
}

TEST_F( ConsoleSpriteTests, GetCurrentImage_ElapsedTimeLandsOnImageBoundary_ReturnsCorrectImage )
{
   ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 4 ) );

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( _frameRateProviderMock, 0.25 ) );
   sprite->AddImage( { 0, 0 } );
   sprite->AddImage( { 1, 1 } );
   sprite->AddImage( { 2, 2 } );
   sprite->AddImage( { 3, 3 } );

   sprite->Tick(); // 0.25 seconds have passed, should be on second image

   EXPECT_EQ( sprite->GetCurrentImage().Width, 1 );
   EXPECT_EQ( sprite->GetCurrentImage().Height, 1 );
}

TEST_F( ConsoleSpriteTests, GetWidth_Always_ReturnsWidthOfCurrentImage )
{
   ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 1 ) );

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( _frameRateProviderMock, 0.75 ) );
   sprite->AddImage( { 0, 1 } );
   sprite->AddImage( { 2, 3 } );
   sprite->AddImage( { 4, 5 } );
   sprite->AddImage( { 6, 7 } );

   sprite->Tick(); // 1 second has passed, should be on second image

   EXPECT_EQ( sprite->GetCurrentImage().Width, 2 );
}

TEST_F( ConsoleSpriteTests, GetHeight_Always_ReturnsHeightOfCurrentImage )
{
   ON_CALL( *_frameRateProviderMock, GetFramesPerSecond() ).WillByDefault( Return( 1 ) );

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( _frameRateProviderMock, 0.75 ) );
   sprite->AddImage( { 0, 1 } );
   sprite->AddImage( { 2, 3 } );
   sprite->AddImage( { 4, 5 } );
   sprite->AddImage( { 6, 7 } );

   sprite->Tick(); // 1 second has passed, should be on second image
   sprite->Tick(); // 2 seconds have passed, should be on third image

   EXPECT_EQ( sprite->GetCurrentImage().Height, 5 );
}

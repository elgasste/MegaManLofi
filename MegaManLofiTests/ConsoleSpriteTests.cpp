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

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1 ) );

      _imageTraversalSeconds = 1;
   }

   void BuildSprite()
   {
      _sprite.reset( new ConsoleSprite( _frameRateProviderMock, _imageTraversalSeconds ) );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;

   double _imageTraversalSeconds;

   shared_ptr<ConsoleSprite> _sprite;
};

TEST_F( ConsoleSpriteTests, Reset_Always_ResetsToFirstImage )
{
   _imageTraversalSeconds = .75;
   BuildSprite();
   _sprite->AddImage( { 0, 0 } );
   _sprite->AddImage( { 1, 1 } );
   _sprite->AddImage( { 2, 2 } );

   _sprite->Tick(); // 1 second has passed, should be on the second image
   EXPECT_EQ( _sprite->GetCurrentImage().Width, 1 );

   _sprite->Reset(); // back to first image
   EXPECT_EQ( _sprite->GetCurrentImage().Width, 0 );

   _sprite->Tick(); // 1 second has passed, should be on the second image again
   EXPECT_EQ( _sprite->GetCurrentImage().Width, 1 );
}

TEST_F( ConsoleSpriteTests, GetCurrentImage_ElapsedTimeMatchesTotalSpriteTime_ReturnsFirstImage )
{
   _imageTraversalSeconds = .25;
   BuildSprite();
   _sprite->AddImage( { 0, 0 } );
   _sprite->AddImage( { 1, 1 } );
   _sprite->AddImage( { 2, 2 } );
   _sprite->AddImage( { 3, 3 } );

   _sprite->Tick(); // 1 second has passed, should be back on the first image

   EXPECT_EQ( _sprite->GetCurrentImage().Width, 0 );
   EXPECT_EQ( _sprite->GetCurrentImage().Height, 0 );
}

TEST_F( ConsoleSpriteTests, GetCurrentImage_ElapsedTimeLandsOnImageBoundary_ReturnsCorrectImage )
{
   ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( .25 ) );
   _imageTraversalSeconds = .25;
   BuildSprite();
   _sprite->AddImage( { 0, 0 } );
   _sprite->AddImage( { 1, 1 } );
   _sprite->AddImage( { 2, 2 } );
   _sprite->AddImage( { 3, 3 } );

   _sprite->Tick(); // 0.25 seconds have passed, should be on second image

   EXPECT_EQ( _sprite->GetCurrentImage().Width, 1 );
   EXPECT_EQ( _sprite->GetCurrentImage().Height, 1 );
}

TEST_F( ConsoleSpriteTests, GetWidth_Always_ReturnsWidthOfCurrentImage )
{
   _imageTraversalSeconds = .75;
   BuildSprite();
   _sprite->AddImage( { 0, 1 } );
   _sprite->AddImage( { 2, 3 } );
   _sprite->AddImage( { 4, 5 } );
   _sprite->AddImage( { 6, 7 } );

   _sprite->Tick(); // 1 second has passed, should be on second image

   EXPECT_EQ( _sprite->GetCurrentImage().Width, 2 );
}

TEST_F( ConsoleSpriteTests, GetHeight_Always_ReturnsHeightOfCurrentImage )
{
   _imageTraversalSeconds = .75;
   BuildSprite();
   _sprite->AddImage( { 0, 1 } );
   _sprite->AddImage( { 2, 3 } );
   _sprite->AddImage( { 4, 5 } );
   _sprite->AddImage( { 6, 7 } );

   _sprite->Tick(); // 1 second has passed, should be on second image
   _sprite->Tick(); // 2 seconds have passed, should be on third image

   EXPECT_EQ( _sprite->GetCurrentImage().Height, 5 );
}

TEST_F( ConsoleSpriteTests, GetTotalTraversalSeconds_Always_ReturnsTotalTraversalSeconds )
{
   BuildSprite();
   _sprite->AddImage( { 0, 1 } );
   _sprite->AddImage( { 2, 3 } );
   _sprite->AddImage( { 4, 5 } );

   EXPECT_EQ( _sprite->GetTotalTraversalSeconds(), 3 );
}

TEST_F( ConsoleSpriteTests, Tick_ImageTraversalIsZeroSeconds_MovesDirectlyToTheNextImage )
{
   _imageTraversalSeconds = 0;
   BuildSprite();
   _sprite->AddImage( { 0, 1 } );
   _sprite->AddImage( { 2, 3 } );
   _sprite->AddImage( { 4, 5 } );
   _sprite->AddImage( { 6, 7 } );

   _sprite->Tick(); // should be on second image
   _sprite->Tick(); // should be on third image
   _sprite->Tick(); // should be on fourth image

   EXPECT_EQ( _sprite->GetCurrentImage().Width, 6 );
}

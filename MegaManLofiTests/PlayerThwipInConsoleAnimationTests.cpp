#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/PlayerThwipInConsoleAnimation.h>
#include <MegaManLofi/ConsoleRenderDefs.h>
#include <MegaManLofi/ConsoleSpriteDefs.h>

#include "mock_ConsoleBuffer.h"
#include "mock_FrameRateProvider.h"
#include "mock_ConsoleSprite.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayerThwipInConsoleAnimationTests : public Test
{
public:
   void SetUp() override
   {
      _spriteDefs.reset( new ConsoleSpriteDefs );
      _renderDefs.reset( new ConsoleRenderDefs );
      _consoleBufferMock.reset( new NiceMock<mock_ConsoleBuffer> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _transitionSpriteMock.reset( new NiceMock<mock_ConsoleSprite> );
      _thwipSpriteMock.reset( new NiceMock<mock_ConsoleSprite> );

      _spriteDefs->PlayerThwipSprite = _thwipSpriteMock;
      _spriteDefs->PlayerThwipInTransitionSprite = _transitionSpriteMock;

      _renderDefs->ArenaCharWidth = 1;
      _renderDefs->ArenaCharHeight = 1;
      _renderDefs->PlayerThwipVelocity = 1;
      _renderDefs->SpriteDefs = _spriteDefs;

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1 ) );
      ON_CALL( *_transitionSpriteMock, GetTotalTraversalSeconds() ).WillByDefault( Return( 10 ) );
   }

   void BuildAnimation()
   {
      _animation.reset( new PlayerThwipInConsoleAnimation( _consoleBufferMock, _renderDefs, _frameRateProviderMock ) );
   }

protected:
   shared_ptr<mock_ConsoleBuffer> _consoleBufferMock;
   shared_ptr<ConsoleSpriteDefs> _spriteDefs;
   shared_ptr<ConsoleRenderDefs> _renderDefs;

   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;

   shared_ptr<mock_ConsoleSprite> _thwipSpriteMock;
   shared_ptr<mock_ConsoleSprite> _transitionSpriteMock;

   shared_ptr<PlayerThwipInConsoleAnimation> _animation;
};

TEST_F( PlayerThwipInConsoleAnimationTests, Constructor_Always_InitializesIsRunningToFalse )
{
   BuildAnimation();

   EXPECT_FALSE( _animation->IsRunning() );
}

TEST_F( PlayerThwipInConsoleAnimationTests, Start_StartPositionHasNoValue_ThrowsException )
{
   BuildAnimation();

   string message = "";
   try
   {
      _animation->Start( nullopt, Coordinate<short>( { 0, 0 } ) );
   }
   catch ( invalid_argument e )
   {
      message = e.what();
   }

   EXPECT_EQ( message, "Start position must have a value" );
}

TEST_F( PlayerThwipInConsoleAnimationTests, Start_EndPositionHasNoValue_ThrowsException )
{
   BuildAnimation();

   string message = "";
   try
   {
      _animation->Start( Coordinate<short>( { 0, 0 } ), nullopt );
   }
   catch ( invalid_argument e )
   {
      message = e.what();
   }

   EXPECT_EQ( message, "End position must have a value" );
}

TEST_F( PlayerThwipInConsoleAnimationTests, Start_Always_SetsIsRunningToTrue )
{
   BuildAnimation();

   _animation->Start( Coordinate<short>( { 0, 0 } ), Coordinate<short>( { 0, 0 } ) );

   EXPECT_TRUE( _animation->IsRunning() );
}

TEST_F( PlayerThwipInConsoleAnimationTests, Start_Always_ResetsSprites )
{
   BuildAnimation();

   EXPECT_CALL( *_transitionSpriteMock, Reset() );
   EXPECT_CALL( *_thwipSpriteMock, Reset() );

   _animation->Start( Coordinate<short>( { 0, 0 } ), Coordinate<short>( { 0, 0 } ) );
}

TEST_F( PlayerThwipInConsoleAnimationTests, Draw_PostThwipping_DrawsSpriteInEndPosition )
{
   BuildAnimation();
   _animation->Start( Coordinate<short>( { 0, 0 } ), Coordinate<short>( { 10, 10 } ) );

   for ( int i = 0; i < 10; i++ )
   {
      _animation->Tick(); // 10 seconds later, should be post-thwipping
   }

   EXPECT_CALL( *_consoleBufferMock, Draw( 0, 10, static_pointer_cast<IConsoleSprite>( _transitionSpriteMock ) ) );
   _animation->Draw();
}

TEST_F( PlayerThwipInConsoleAnimationTests, Draw_ThwippingDownward_DrawsSpriteInCorrectTopPosition )
{
   ON_CALL( *_transitionSpriteMock, GetTotalTraversalSeconds() ).WillByDefault( Return( 1 ) );
   BuildAnimation();
   _animation->Start( Coordinate<short>( { 0, 0 } ), Coordinate<short>( { 10, 10 } ) );

   EXPECT_CALL( *_consoleBufferMock, Draw( 0, 0, static_pointer_cast<IConsoleSprite>( _thwipSpriteMock ) ) );
   _animation->Draw();

   for( int i = 0; i < 3; i++ ) _animation->Tick(); // 3 seconds, should be 3 chars down

   EXPECT_CALL( *_consoleBufferMock, Draw( 0, 3, static_pointer_cast<IConsoleSprite>( _thwipSpriteMock ) ) );
   _animation->Draw();
}

TEST_F( PlayerThwipInConsoleAnimationTests, Draw_ThwippingUpward_DrawsSpriteInCorrectTopPosition )
{
   ON_CALL( *_transitionSpriteMock, GetTotalTraversalSeconds() ).WillByDefault( Return( 1 ) );
   BuildAnimation();
   _animation->Start( Coordinate<short>( { 10, 10 } ), Coordinate<short>( { 0, 0 } ) );

   EXPECT_CALL( *_consoleBufferMock, Draw( 10, 10, static_pointer_cast<IConsoleSprite>( _thwipSpriteMock ) ) );
   _animation->Draw();

   for( int i = 0; i < 3; i++ ) _animation->Tick(); // 3 seconds, should be 3 chars up

   EXPECT_CALL( *_consoleBufferMock, Draw( 10, 7, static_pointer_cast<IConsoleSprite>( _thwipSpriteMock ) ) );
   _animation->Draw();
}

TEST_F( PlayerThwipInConsoleAnimationTests, Tick_NotRunning_DoesNotTickAnySprites )
{
   BuildAnimation();

   EXPECT_CALL( *_thwipSpriteMock, Tick() ).Times( 0 );
   EXPECT_CALL( *_transitionSpriteMock, Tick() ).Times( 0 );

   _animation->Tick();
}

TEST_F( PlayerThwipInConsoleAnimationTests, Tick_ThwipAnimationHasFinished_StopsRunning )
{
   BuildAnimation();
   _animation->Start( Coordinate<short>( { 10, 10 } ), Coordinate<short>( { 8, 8 } ) );

   _animation->Tick(); // thwip sprite moves to 1 char up
   EXPECT_TRUE( _animation->IsRunning() );

   _animation->Tick(); // thwip sprite moves to 2 chars up, should switch to post-thwipping
   EXPECT_TRUE( _animation->IsRunning() );

   for ( int i = 0; i < 9; i++ )
   {
      _animation->Tick(); // after 9 seconds, should be on last frame
      EXPECT_TRUE( _animation->IsRunning() );
   }

   _animation->Tick(); // 10 seconds post-thwip, finished
   EXPECT_FALSE( _animation->IsRunning() );
}

#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/PlayerThwipOutConsoleAnimation.h>
#include <MegaManLofi/ConsoleRenderConfig.h>

#include "mock_ConsoleBuffer.h"
#include "mock_ConsoleSprite.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayerThwipOutConsoleAnimationTests : public Test
{
public:
   void SetUp() override
   {
      _renderConfig.reset( new ConsoleRenderConfig );
      _consoleBufferMock.reset( new NiceMock<mock_ConsoleBuffer> );
      _transitionSpriteMock.reset( new NiceMock<mock_ConsoleSprite> );
      _thwipSpriteMock.reset( new NiceMock<mock_ConsoleSprite> );

      _renderConfig->ArenaCharWidth = 1;
      _renderConfig->ArenaCharHeight = 1;
      _renderConfig->PlayerThwipVelocity = 1;

      _framesPerSecond = 1;

      ON_CALL( *_transitionSpriteMock, GetTotalTraversalSeconds() ).WillByDefault( Return( 10 ) );

      _renderConfig->PlayerThwipOutTransitionSprite = _transitionSpriteMock;
      _renderConfig->PlayerThwipSprite = _thwipSpriteMock;
   }

   void BuildAnimation()
   {
      _animation.reset( new PlayerThwipOutConsoleAnimation( _consoleBufferMock, _renderConfig ) );
   }

protected:
   shared_ptr<mock_ConsoleBuffer> _consoleBufferMock;
   shared_ptr<ConsoleRenderConfig> _renderConfig;

   shared_ptr<mock_ConsoleSprite> _transitionSpriteMock;
   shared_ptr<mock_ConsoleSprite> _thwipSpriteMock;

   int _framesPerSecond;

   shared_ptr<PlayerThwipOutConsoleAnimation> _animation;
};

TEST_F( PlayerThwipOutConsoleAnimationTests, Constructor_Always_InitializesIsRunningToFalse )
{
   BuildAnimation();

   EXPECT_FALSE( _animation->IsRunning() );
}

TEST_F( PlayerThwipOutConsoleAnimationTests, Start_Always_SetsIsRunningToTrue )
{
   BuildAnimation();

   _animation->Start( { 0, 0 }, { 0, 0 } );

   EXPECT_TRUE( _animation->IsRunning() );
}

TEST_F( PlayerThwipOutConsoleAnimationTests, Start_Always_ResetsSprites )
{
   BuildAnimation();

   EXPECT_CALL( *_transitionSpriteMock, Reset() );
   EXPECT_CALL( *_thwipSpriteMock, Reset() );

   _animation->Start( { 0, 0 }, { 0, 0 } );
}

TEST_F( PlayerThwipOutConsoleAnimationTests, Draw_PreThwipping_DrawsSpriteInStartPosition )
{
   BuildAnimation();
   _animation->Start( { 0, 0 }, { 10, 10 } );

   EXPECT_CALL( *_consoleBufferMock, Draw( 0, 0, static_pointer_cast<IConsoleSprite>( _transitionSpriteMock ) ) );
   _animation->Draw();

   EXPECT_CALL( *_transitionSpriteMock, Tick( _framesPerSecond ) );
   _animation->Tick( _framesPerSecond );

   EXPECT_CALL( *_consoleBufferMock, Draw( 0, 0, static_pointer_cast<IConsoleSprite>( _transitionSpriteMock ) ) );
   _animation->Draw();
}

TEST_F( PlayerThwipOutConsoleAnimationTests, Draw_ThwippingDownward_DrawsSpriteInCorrectTopPosition )
{
   ON_CALL( *_transitionSpriteMock, GetTotalTraversalSeconds() ).WillByDefault( Return( 1 ) );
   BuildAnimation();
   _animation->Start( { 0, 0 }, { 10, 10 } );

   _animation->Tick( _framesPerSecond ); // should switch from pre-thwipping to thwipping

   EXPECT_CALL( *_consoleBufferMock, Draw( 0, 0, static_pointer_cast<IConsoleSprite>( _thwipSpriteMock ) ) );
   _animation->Draw();

   for( int i = 0; i < 3; i++ ) _animation->Tick( _framesPerSecond ); // 3 seconds, should be 3 chars down

   EXPECT_CALL( *_consoleBufferMock, Draw( 0, 3, static_pointer_cast<IConsoleSprite>( _thwipSpriteMock ) ) );
   _animation->Draw();
}

TEST_F( PlayerThwipOutConsoleAnimationTests, Draw_ThwippingUpward_DrawsSpriteInCorrectTopPosition )
{
   ON_CALL( *_transitionSpriteMock, GetTotalTraversalSeconds() ).WillByDefault( Return( 1 ) );
   BuildAnimation();
   _animation->Start( { 10, 10 }, { 0, 0 } );

   _animation->Tick( _framesPerSecond ); // should switch from pre-thwipping to thwipping

   EXPECT_CALL( *_consoleBufferMock, Draw( 10, 10, static_pointer_cast<IConsoleSprite>( _thwipSpriteMock ) ) );
   _animation->Draw();

   for( int i = 0; i < 3; i++ ) _animation->Tick( _framesPerSecond ); // 3 seconds, should be 3 chars up

   EXPECT_CALL( *_consoleBufferMock, Draw( 10, 7, static_pointer_cast<IConsoleSprite>( _thwipSpriteMock ) ) );
   _animation->Draw();
}

TEST_F( PlayerThwipOutConsoleAnimationTests, Tick_NotRunning_DoesNotTickAnySprites )
{
   BuildAnimation();

   EXPECT_CALL( *_transitionSpriteMock, Tick( _ ) ).Times( 0 );
   EXPECT_CALL( *_thwipSpriteMock, Tick( _ ) ).Times( 0 );

   _animation->Tick( _framesPerSecond );
}

TEST_F( PlayerThwipOutConsoleAnimationTests, Tick_ThwipAnimationHasFinished_StopsRunning )
{
   ON_CALL( *_transitionSpriteMock, GetTotalTraversalSeconds() ).WillByDefault( Return( 1 ) );
   BuildAnimation();
   _animation->Start( { 10, 10 }, { 8, 8 } );

   _animation->Tick( _framesPerSecond ); // should switch from pre-thwipping to thwipping
   EXPECT_TRUE( _animation->IsRunning() );

   _animation->Tick( _framesPerSecond ); // thwip sprite moves to 1 char up
   EXPECT_TRUE( _animation->IsRunning() );

   _animation->Tick( _framesPerSecond ); // thwip sprite moves to 2 chars up, finished
   EXPECT_FALSE( _animation->IsRunning() );
}

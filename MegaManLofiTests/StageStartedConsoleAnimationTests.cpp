#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/StageStartedConsoleAnimation.h>
#include <MegaManLofi/ConsoleRenderConfig.h>

#include "mock_ConsoleBuffer.h"
#include "mock_FrameRateProvider.h"
#include "mock_ConsoleSprite.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class StageStartedConsoleAnimationTests : public Test
{
public:
   void SetUp() override
   {
      _consoleBufferMock.reset( new NiceMock<mock_ConsoleBuffer> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _renderConfig.reset( new ConsoleRenderConfig );
      _getReadySpriteMock.reset( new NiceMock<mock_ConsoleSprite> );

      _renderConfig->GetReadySprite = _getReadySpriteMock;

      ON_CALL( *_frameRateProviderMock, GetFrameScalar() ).WillByDefault( Return( 1 ) );
   }

   void BuildAnimation()
   {
      _animation.reset( new StageStartedConsoleAnimation( _consoleBufferMock, _frameRateProviderMock, _renderConfig ) );
   }

protected:
   shared_ptr<mock_ConsoleBuffer> _consoleBufferMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<ConsoleRenderConfig> _renderConfig;

   shared_ptr<mock_ConsoleSprite> _getReadySpriteMock;

   shared_ptr<StageStartedConsoleAnimation> _animation;
};

TEST_F( StageStartedConsoleAnimationTests, Constructor_Always_InitializesIsRunningToFalse )
{
   BuildAnimation();

   EXPECT_FALSE( _animation->IsRunning() );
}

TEST_F( StageStartedConsoleAnimationTests, Start_StartPositionHasNoValue_ThrowsException )
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

TEST_F( StageStartedConsoleAnimationTests, Start_Always_SetsIsRunningToTrue )
{
   BuildAnimation();

   _animation->Start( Coordinate<short>( { 0, 0 } ), nullopt );

   EXPECT_TRUE( _animation->IsRunning() );
}

TEST_F( StageStartedConsoleAnimationTests, Start_Always_ResetsGetReadySprite )
{
   BuildAnimation();

   EXPECT_CALL( *_getReadySpriteMock, Reset() );

   _animation->Start( Coordinate<short>( { 0, 0 } ), nullopt );
}

TEST_F( StageStartedConsoleAnimationTests, Draw_Always_DrawsStartPosition )
{
   BuildAnimation();
   _animation->Start( Coordinate<short>( { 1, 2 } ), nullopt );

   EXPECT_CALL( *_consoleBufferMock, Draw( 1, 2, static_pointer_cast<IConsoleSprite>( _getReadySpriteMock ) ) );
   _animation->Tick();

   _animation->Draw();
}

TEST_F( StageStartedConsoleAnimationTests, Tick_IsNotRunning_DoesNotTickGetReadySprite )
{
   BuildAnimation();

   EXPECT_CALL( *_getReadySpriteMock, Tick() ).Times( 0 );

   _animation->Tick();
}

TEST_F( StageStartedConsoleAnimationTests, Tick_IsRunning_TicksGetReadySprite )
{
   BuildAnimation();
   _animation->Start( Coordinate<short>( { 1, 2 } ), nullopt );

   EXPECT_CALL( *_getReadySpriteMock, Tick() );

   _animation->Tick();
}

TEST_F( StageStartedConsoleAnimationTests, Tick_FinishedRunning_SetsIsRunningToFalse )
{
   _renderConfig->GetReadyAnimationSeconds = 2;
   BuildAnimation();
   _animation->Start( Coordinate<short>( { 1, 2 } ), nullopt );

   _animation->Tick(); // 1 second has passed, still running
   EXPECT_TRUE( _animation->IsRunning() );

   _animation->Tick(); // 2 seconds have passed, should be finished
   EXPECT_FALSE( _animation->IsRunning() );
}

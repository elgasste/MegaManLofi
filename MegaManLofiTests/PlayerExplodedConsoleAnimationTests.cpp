#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/PlayerExplodedConsoleAnimation.h>
#include <MegaManLofi/ConsoleRenderConfig.h>

#include "mock_ConsoleBuffer.h"
#include "mock_FrameRateProvider.h"
#include "mock_ConsoleSprite.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayerExplodedConsoleAnimationTests : public Test
{
public:
   void SetUp() override
   {
      _consoleBufferMock.reset( new NiceMock<mock_ConsoleBuffer> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _renderConfig.reset( new ConsoleRenderConfig );

      ON_CALL( *_frameRateProviderMock, GetFrameScalar() ).WillByDefault( Return( 1 ) );
   }

   void BuildAnimation()
   {
      _animation.reset( new PlayerExplodedConsoleAnimation( _consoleBufferMock, _frameRateProviderMock, _renderConfig ) );
   }

protected:
   shared_ptr<mock_ConsoleBuffer> _consoleBufferMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<ConsoleRenderConfig> _renderConfig;

   shared_ptr<PlayerExplodedConsoleAnimation> _animation;
};

TEST_F( PlayerExplodedConsoleAnimationTests, Constructor_Always_InitializesIsRunningToFalse )
{
   BuildAnimation();

   EXPECT_FALSE( _animation->IsRunning() );
}

TEST_F( PlayerExplodedConsoleAnimationTests, Start_Always_SetsIsRunningToTrue )
{
   BuildAnimation();

   _animation->Start( { 0, 0 }, { 0,0 } );

   EXPECT_TRUE( _animation->IsRunning() );
}

#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/PlayerExplodedConsoleAnimation.h>
#include <MegaManLofi/ConsoleRenderDefs.h>
#include <MegaManLofi/ConsoleSpriteDefs.h>

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
      _spriteDefs.reset( new ConsoleSpriteDefs );
      _renderDefs.reset( new ConsoleRenderDefs );
      _particleSpriteMock.reset( new NiceMock<mock_ConsoleSprite> );

      _spriteDefs->PlayerExplosionParticleSprite = _particleSpriteMock;

      _renderDefs->PlayerExplosionParticleVelocity = 2;
      _renderDefs->ArenaCharWidth = 1;
      _renderDefs->ArenaCharHeight = 1;
      _renderDefs->SpriteDefs = _spriteDefs;

      ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1 ) );
   }

   void BuildAnimation()
   {
      _animation.reset( new PlayerExplodedConsoleAnimation( _consoleBufferMock, _frameRateProviderMock, _renderDefs ) );
   }

protected:
   shared_ptr<mock_ConsoleBuffer> _consoleBufferMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<ConsoleSpriteDefs> _spriteDefs;
   shared_ptr<ConsoleRenderDefs> _renderDefs;

   shared_ptr<mock_ConsoleSprite> _particleSpriteMock;

   shared_ptr<PlayerExplodedConsoleAnimation> _animation;
};

TEST_F( PlayerExplodedConsoleAnimationTests, Constructor_Always_InitializesIsRunningToFalse )
{
   BuildAnimation();

   EXPECT_FALSE( _animation->IsRunning() );
}

TEST_F( PlayerExplodedConsoleAnimationTests, Start_StartPositionHasNoValue_ThrowsException )
{
   BuildAnimation();

   string message = "";
   try
   {
      _animation->Start( nullopt, nullopt );
   }
   catch ( invalid_argument e )
   {
      message = e.what();
   }

   EXPECT_EQ( message, "Start position must have a value" );
}

TEST_F( PlayerExplodedConsoleAnimationTests, Start_Always_SetsIsRunningToTrue )
{
   BuildAnimation();

   _animation->Start( Coordinate<short>( { 0, 0 } ), nullopt );

   EXPECT_TRUE( _animation->IsRunning() );
}

TEST_F( PlayerExplodedConsoleAnimationTests, Start_Always_ResetsExplosionSprite )
{
   BuildAnimation();

   EXPECT_CALL( *_particleSpriteMock, Reset() );

   _animation->Start( Coordinate<short>( { 0, 0 } ), nullopt );

   EXPECT_TRUE( _animation->IsRunning() );
}

TEST_F( PlayerExplodedConsoleAnimationTests, Draw_Always_DrawsAllParticlesInCorrectPositions )
{
   ON_CALL( *_frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 3 ) );
   BuildAnimation();

   EXPECT_CALL( *_consoleBufferMock, Draw( 30, 30, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) ).Times( 16 );

   _animation->Start( Coordinate<short>( { 30, 30 } ), nullopt );
   _animation->Draw();

   // horizontal and vertical particles
   EXPECT_CALL( *_consoleBufferMock, Draw( 36, 30, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 33, 30, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 27, 30, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 24, 30, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 30, 36, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 30, 33, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 30, 27, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 30, 24, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );

   // diagonal particles
   EXPECT_CALL( *_consoleBufferMock, Draw( 26, 26, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 28, 28, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 26, 34, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 28, 32, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 34, 26, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 32, 28, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 34, 34, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );
   EXPECT_CALL( *_consoleBufferMock, Draw( 32, 32, static_pointer_cast<IConsoleSprite>( _particleSpriteMock ) ) );

   _animation->Tick();
   _animation->Draw();
}

TEST_F( PlayerExplodedConsoleAnimationTests, Tick_NotRunning_DoesNotTickParticleSprite )
{
   BuildAnimation();

   EXPECT_CALL( *_particleSpriteMock, Tick() ).Times( 0 );

   _animation->Tick();
}

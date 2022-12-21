#include "PlayerExplodedConsoleAnimation.h"
#include "IConsoleBuffer.h"
#include "IFrameRateProvider.h"
#include "ConsoleRenderConfig.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

PlayerExplodedConsoleAnimation::PlayerExplodedConsoleAnimation( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                                const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                                const shared_ptr<ConsoleRenderConfig> renderConfig ) :
   _consoleBuffer( consoleBuffer ),
   _frameRateProvider( frameRateProvider ),
   _renderConfig( renderConfig ),
   _isRunning( false ),
   _elapsedSeconds( 0 ),
   _explosionStartFrame( 0 ),
   _startPositionChars( { 0, 0 } )
{
}

void PlayerExplodedConsoleAnimation::Start( optional<Coordinate<short>> startPositionChars,
                                            optional<Coordinate<short>> endPositionChars )
{
   _isRunning = true;
   _elapsedSeconds = 0;
   _explosionStartFrame = _frameRateProvider->GetCurrentFrame();
   _startPositionChars = startPositionChars.value();

   _renderConfig->PlayerExplosionParticleSprite->Reset();
}

void PlayerExplodedConsoleAnimation::Draw()
{
   auto elapsedFrames = _frameRateProvider->GetCurrentFrame() - _explosionStartFrame;
   auto particleIncrement = ( _renderConfig->PlayerExplosionParticleVelocity * _frameRateProvider->GetFrameScalar() );
   auto particleDeltaXChars = (short)( ( particleIncrement * elapsedFrames ) / _renderConfig->ArenaCharWidth );
   auto particleDeltaYChars = (short)( ( particleIncrement * elapsedFrames ) / _renderConfig->ArenaCharHeight );

   auto particleSprite = _renderConfig->PlayerExplosionParticleSprite;

   // horizontal and vertical particles
   _consoleBuffer->Draw( _startPositionChars.Left + particleDeltaXChars, _startPositionChars.Top, particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left + ( particleDeltaXChars / 2 ), _startPositionChars.Top, particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left - particleDeltaXChars, _startPositionChars.Top, particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left - ( particleDeltaXChars / 2 ), _startPositionChars.Top, particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left, _startPositionChars.Top + particleDeltaYChars, particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left, _startPositionChars.Top + ( particleDeltaYChars / 2 ), particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left, _startPositionChars.Top - particleDeltaYChars, particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left, _startPositionChars.Top - ( particleDeltaYChars / 2 ), particleSprite );

   // diagonal particles
   _consoleBuffer->Draw( _startPositionChars.Left + (short)( particleDeltaXChars / 1.5 ), _startPositionChars.Top + (short)( particleDeltaYChars / 1.5 ), particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left + (short)( particleDeltaXChars / 3 ), _startPositionChars.Top + (short)( particleDeltaYChars / 3 ), particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left - (short)( particleDeltaXChars / 1.5 ), _startPositionChars.Top + (short)( particleDeltaYChars / 1.5 ), particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left - (short)( particleDeltaXChars / 3 ), _startPositionChars.Top + (short)( particleDeltaYChars / 3 ), particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left + (short)( particleDeltaXChars / 1.5 ), _startPositionChars.Top - (short)( particleDeltaYChars / 1.5 ), particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left + (short)( particleDeltaXChars / 3 ), _startPositionChars.Top - (short)( particleDeltaYChars / 3 ), particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left - (short)( particleDeltaXChars / 1.5 ), _startPositionChars.Top - (short)( particleDeltaYChars / 1.5 ), particleSprite );
   _consoleBuffer->Draw( _startPositionChars.Left - (short)( particleDeltaXChars / 3 ), _startPositionChars.Top - (short)( particleDeltaYChars / 3 ), particleSprite );
}

void PlayerExplodedConsoleAnimation::Tick()
{
   if ( !_isRunning )
   {
      return;
   }

   _elapsedSeconds += _frameRateProvider->GetFrameScalar();
   _renderConfig->PlayerExplosionParticleSprite->Tick();

   if ( _elapsedSeconds >= _renderConfig->PlayerExplosionAnimationSeconds )
   {
      _isRunning = false;
   }
}

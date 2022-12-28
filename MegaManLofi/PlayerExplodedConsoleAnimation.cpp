#include <stdexcept>

#include "PlayerExplodedConsoleAnimation.h"
#include "IConsoleBuffer.h"
#include "IFrameRateProvider.h"
#include "ConsoleRenderDefs.h"
#include "ConsoleSpriteDefs.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

PlayerExplodedConsoleAnimation::PlayerExplodedConsoleAnimation( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                                const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                                const shared_ptr<ConsoleRenderDefs> renderDefs ) :
   _consoleBuffer( consoleBuffer ),
   _frameRateProvider( frameRateProvider ),
   _renderDefs( renderDefs ),
   _isRunning( false ),
   _elapsedSeconds( 0 ),
   _totalParticleDeltaUnits( 0 ),
   _startPositionChars( { 0, 0 } )
{
}

void PlayerExplodedConsoleAnimation::Start( optional<Coordinate<short>> startPositionChars,
                                            optional<Coordinate<short>> endPositionChars )
{
   if ( !startPositionChars.has_value() )
   {
      throw invalid_argument( "Start position must have a value" );
   }

   _isRunning = true;
   _elapsedSeconds = 0;
   _totalParticleDeltaUnits = 0;
   _startPositionChars = startPositionChars.value();

   _renderDefs->SpriteDefs->PlayerExplosionParticleSprite->Reset();
}

void PlayerExplodedConsoleAnimation::Draw()
{
   auto particleDeltaXChars = (short)( _totalParticleDeltaUnits / _renderDefs->ArenaCharWidth );
   auto particleDeltaYChars = (short)( _totalParticleDeltaUnits / _renderDefs->ArenaCharHeight );

   auto particleSprite = _renderDefs->SpriteDefs->PlayerExplosionParticleSprite;

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

   auto frameSeconds = _frameRateProvider->GetFrameSeconds();
   _elapsedSeconds += frameSeconds;
   _totalParticleDeltaUnits += (long long)( _renderDefs->PlayerExplosionParticleVelocity * frameSeconds );

   _renderDefs->SpriteDefs->PlayerExplosionParticleSprite->Tick();

   if ( _elapsedSeconds >= _renderDefs->PlayerExplosionAnimationSeconds )
   {
      _isRunning = false;
   }
}

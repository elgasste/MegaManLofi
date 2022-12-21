#include "StageStartedConsoleAnimation.h"
#include "IConsoleBuffer.h"
#include "IFrameRateProvider.h"
#include "ConsoleRenderConfig.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

StageStartedConsoleAnimation::StageStartedConsoleAnimation( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                            const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                            const shared_ptr<ConsoleRenderConfig> renderConfig ) :
   _consoleBuffer( consoleBuffer ),
   _frameRateProvider( frameRateProvider ),
   _renderConfig( renderConfig ),
   _isRunning( false ),
   _elapsedSeconds( 0 ),
   _positionChars( { 0, 0 } )
{
}

void StageStartedConsoleAnimation::Start( optional<Coordinate<short>> startPositionChars,
                                          optional<Coordinate<short>> endPositionChars )
{
   _isRunning = true;
   _elapsedSeconds = 0;
   _positionChars = startPositionChars.value();

   _renderConfig->GetReadySprite->Reset();
}

void StageStartedConsoleAnimation::Draw()
{
   auto leftOffset = (short)( _renderConfig->GetReadySprite->GetWidth() / 2 );
   auto topOffset = (short)( _renderConfig->GetReadySprite->GetHeight() / 2 );
   _consoleBuffer->Draw( _positionChars.Left - leftOffset, _positionChars.Top - topOffset, _renderConfig->GetReadySprite );
}

void StageStartedConsoleAnimation::Tick()
{
   if ( _isRunning )
   {
      _elapsedSeconds += _frameRateProvider->GetFrameScalar();
      _renderConfig->GetReadySprite->Tick();

      if ( _elapsedSeconds >= _renderConfig->GetReadyAnimationSeconds )
      {
         _isRunning = false;
      }
   }
}

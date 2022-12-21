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

void StageStartedConsoleAnimation::Start( Coordinate<short> startPositionChars, Coordinate<short> endPositionChars )
{
   _isRunning = true;
   _elapsedSeconds = 0;
   _positionChars = startPositionChars;

   _renderConfig->GetReadySprite->Reset();
}

void StageStartedConsoleAnimation::Draw()
{
   _consoleBuffer->Draw( _positionChars.Left, _positionChars.Top, _renderConfig->GetReadySprite );
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

#include <stdexcept>

#include "StageStartedConsoleAnimation.h"
#include "ConsoleBuffer.h"
#include "IFrameRateProvider.h"
#include "ConsoleRenderDefs.h"
#include "ConsoleSpriteDefs.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

StageStartedConsoleAnimation::StageStartedConsoleAnimation( const shared_ptr<ConsoleBuffer> consoleBuffer,
                                                            const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                            const shared_ptr<ConsoleRenderDefs> renderDefs ) :
   _consoleBuffer( consoleBuffer ),
   _frameRateProvider( frameRateProvider ),
   _renderDefs( renderDefs ),
   _isRunning( false ),
   _elapsedSeconds( 0 ),
   _positionChars( { 0, 0 } )
{
}

void StageStartedConsoleAnimation::Start( optional<Coordinate<short>> startPositionChars,
                                          optional<Coordinate<short>> endPositionChars )
{
   if ( !startPositionChars.has_value() )
   {
      throw invalid_argument( "Start position must have a value" );
   }

   _isRunning = true;
   _elapsedSeconds = 0;
   _positionChars = startPositionChars.value();

   _renderDefs->SpriteDefs->StageGetReadySprite->Reset();
}

void StageStartedConsoleAnimation::Draw()
{
   auto getReadySprite = _renderDefs->SpriteDefs->StageGetReadySprite;
   auto leftOffset = (short)( getReadySprite->GetWidth() / 2 );
   auto topOffset = (short)( getReadySprite->GetHeight() / 2 );
   _consoleBuffer->Draw( _positionChars.Left - leftOffset, _positionChars.Top - topOffset, getReadySprite );
}

void StageStartedConsoleAnimation::Tick()
{
   if ( _isRunning )
   {
      _elapsedSeconds += _frameRateProvider->GetFrameSeconds();
      _renderDefs->SpriteDefs->StageGetReadySprite->Tick();

      if ( _elapsedSeconds >= _renderDefs->GetReadyAnimationSeconds )
      {
         _isRunning = false;
      }
   }
}

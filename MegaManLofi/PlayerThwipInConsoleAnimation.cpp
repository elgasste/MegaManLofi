#include <stdexcept>

#include "PlayerThwipInConsoleAnimation.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderDefs.h"
#include "ConsoleSpriteDefs.h"
#include "IFrameRateProvider.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

PlayerThwipInConsoleAnimation::PlayerThwipInConsoleAnimation( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                              const shared_ptr<ConsoleRenderDefs> renderDefs,
                                                              const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _consoleBuffer( consoleBuffer ),
   _renderDefs( renderDefs ),
   _frameRateProvider( frameRateProvider ),
   _isRunning( false ),
   _startPositionChars( {0, 0} ),
   _endPositionChars( {0, 0} ),
   _currentTopPositionUnits( 0 ),
   _endTopPositionUnits( 0 ),
   _postThwipping( false ),
   _elapsedSeconds( 0 )
{
}

void PlayerThwipInConsoleAnimation::Start( optional<Coordinate<short>> startPositionChars,
                                           optional<Coordinate<short>> endPositionChars )
{
   if ( !startPositionChars.has_value() )
   {
      throw invalid_argument( "Start position must have a value" );
   }
   else if ( !endPositionChars.has_value() )
   {
      throw invalid_argument( "End position must have a value" );
   }

   _isRunning = true;
   _startPositionChars = startPositionChars.value();
   _endPositionChars = endPositionChars.value();
   _currentTopPositionUnits = _startPositionChars.Top * _renderDefs->ArenaCharHeight;
   _endTopPositionUnits = _endPositionChars.Top * _renderDefs->ArenaCharHeight;
   _postThwipping = false;
   _elapsedSeconds = 0;

   _renderDefs->SpriteDefs->PlayerThwipInTransitionSprite->Reset();
   _renderDefs->SpriteDefs->PlayerThwipSprite->Reset();
}

void PlayerThwipInConsoleAnimation::Draw()
{
   auto transitionSprite = _renderDefs->SpriteDefs->PlayerThwipInTransitionSprite;

   if ( _postThwipping )
   {
      _consoleBuffer->Draw( _startPositionChars.Left, _endPositionChars.Top, transitionSprite );
   }
   else
   {
      auto thwipSprite = _renderDefs->SpriteDefs->PlayerThwipSprite;
      auto leftOffset = short( ( transitionSprite->GetWidth() - thwipSprite->GetWidth() ) / 2 );
      _consoleBuffer->Draw( _startPositionChars.Left + leftOffset, (short)( _currentTopPositionUnits / _renderDefs->ArenaCharHeight ), thwipSprite );
   }
}

void PlayerThwipInConsoleAnimation::Tick()
{
   if ( !_isRunning )
   {
      return;
   }

   _elapsedSeconds += _frameRateProvider->GetFrameSeconds();

   if ( _postThwipping )
   {
      auto transitionSprite = _renderDefs->SpriteDefs->PlayerThwipInTransitionSprite;
      transitionSprite->Tick();

      auto test = transitionSprite->GetTotalTraversalSeconds();
      if ( _elapsedSeconds >= transitionSprite->GetTotalTraversalSeconds() )
      {
         _isRunning = false;
      }
   }
   else
   {
      _renderDefs->SpriteDefs->PlayerThwipSprite->Tick();
      auto topDelta = _renderDefs->PlayerThwipVelocity * _frameRateProvider->GetFrameSeconds();

      if ( _endPositionChars.Top < _startPositionChars.Top )
      {
         _currentTopPositionUnits -= topDelta;
         if ( _currentTopPositionUnits <= _endTopPositionUnits )
         {
            _postThwipping = true;
            _elapsedSeconds = 0;
         }
      }
      else
      {
         _currentTopPositionUnits += topDelta;
         if ( _currentTopPositionUnits >= _endTopPositionUnits )
         {
            _postThwipping = true;
            _elapsedSeconds = 0;
         }
      }
   }
}

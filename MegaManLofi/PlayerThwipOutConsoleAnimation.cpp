#include <stdexcept>

#include "PlayerThwipOutConsoleAnimation.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderDefs.h"
#include "IFrameRateProvider.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

PlayerThwipOutConsoleAnimation::PlayerThwipOutConsoleAnimation( const shared_ptr<IConsoleBuffer> consoleBuffer,
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
   _preThwipping( false ),
   _postThwipping( false ),
   _elapsedSeconds( 0 )
{
}

void PlayerThwipOutConsoleAnimation::Start( optional<Coordinate<short>> startPositionChars,
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
   _preThwipping = true;
   _postThwipping = false;
   _elapsedSeconds = 0;

   _renderDefs->PlayerThwipOutTransitionSprite->Reset();
   _renderDefs->PlayerThwipSprite->Reset();
}

void PlayerThwipOutConsoleAnimation::Draw()
{
   if ( _preThwipping )
   {
      _consoleBuffer->Draw( _startPositionChars.Left, _startPositionChars.Top, _renderDefs->PlayerThwipOutTransitionSprite );
   }
   else if ( !_postThwipping )
   {
      auto leftOffset = short( ( _renderDefs->PlayerThwipOutTransitionSprite->GetWidth() - _renderDefs->PlayerThwipSprite->GetWidth() ) / 2 );
      _consoleBuffer->Draw( _startPositionChars.Left + leftOffset,
                            (short)( _currentTopPositionUnits / _renderDefs->ArenaCharHeight ),
                            _renderDefs->PlayerThwipSprite );
   }
}

void PlayerThwipOutConsoleAnimation::Tick()
{
   if ( !_isRunning )
   {
      return;
   }

   _elapsedSeconds += _frameRateProvider->GetFrameSeconds();

   if ( _preThwipping )
   {
      _renderDefs->PlayerThwipOutTransitionSprite->Tick();

      if ( _elapsedSeconds >= _renderDefs->PlayerThwipOutTransitionSprite->GetTotalTraversalSeconds() )
      {
         _preThwipping = false;
         _elapsedSeconds = 0;
      }
   }
   else if ( _postThwipping )
   {
      if ( _elapsedSeconds >= _renderDefs->PlayerPostThwipDelaySeconds )
      {
         _isRunning = false;
      }
   }
   else
   {
      _renderDefs->PlayerThwipSprite->Tick();
      auto topDelta = (long long)( _renderDefs->PlayerThwipVelocity * _frameRateProvider->GetFrameSeconds() );

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

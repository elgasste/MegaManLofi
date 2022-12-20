#include "PlayerThwipOutConsoleAnimation.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

// MUFFINS: test this whole class
PlayerThwipOutConsoleAnimation::PlayerThwipOutConsoleAnimation( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                                const shared_ptr<ConsoleRenderConfig> renderConfig ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
   _isRunning( false ),
   _startPositionChars( {0, 0} ),
   _endPositionChars( {0, 0} ),
   _currentTopPositionUnits( 0 ),
   _endTopPositionUnits( 0 ),
   _preThwipping( false ),
   _elapsedSeconds( 0 )
{
}

void PlayerThwipOutConsoleAnimation::Start( Coordinate<short> startPositionChars, Coordinate<short> endPositionChars )
{
   _isRunning = true;
   _startPositionChars = startPositionChars;
   _endPositionChars = endPositionChars;
   _currentTopPositionUnits = startPositionChars.Top * _renderConfig->ArenaCharHeight;
   _endTopPositionUnits = endPositionChars.Top * _renderConfig->ArenaCharHeight;
   _preThwipping = true;
   _elapsedSeconds = 0;

   _renderConfig->PlayerThwipOutTransitionSprite->Reset();
   _renderConfig->PlayerThwipSprite->Reset();
}

void PlayerThwipOutConsoleAnimation::Draw()
{
   if ( _preThwipping )
   {
      _consoleBuffer->Draw( _startPositionChars.Left, _startPositionChars.Top, _renderConfig->PlayerThwipOutTransitionSprite );
   }
   else
   {
      _consoleBuffer->Draw( _startPositionChars.Left,
                            (short)( _currentTopPositionUnits / _renderConfig->ArenaCharHeight ),
                            _renderConfig->PlayerThwipSprite );
   }
}

void PlayerThwipOutConsoleAnimation::Tick( int framesPerSecond )
{
   if ( !_isRunning )
   {
      return;
   }

   auto frameRateScalar = 1 / (double)framesPerSecond;
   _elapsedSeconds += frameRateScalar;

   if ( _preThwipping )
   {
      _renderConfig->PlayerThwipOutTransitionSprite->Tick( framesPerSecond );

      if ( _elapsedSeconds >= _renderConfig->PlayerThwipOutTransitionSprite->GetTotalTraversalSeconds() )
      {
         _preThwipping = false;
         _elapsedSeconds = 0;
      }
   }
   else
   {
      _renderConfig->PlayerThwipSprite->Tick( framesPerSecond );
      auto topDelta = (long long)( _renderConfig->PlayerThwipVelocity * frameRateScalar );

      if ( _endPositionChars.Top < _startPositionChars.Top )
      {
         _currentTopPositionUnits -= topDelta;
         if ( _currentTopPositionUnits <= _endTopPositionUnits )
         {
            _isRunning = false;
         }
      }
      else
      {
         _currentTopPositionUnits += topDelta;
         if ( _currentTopPositionUnits >= _endTopPositionUnits )
         {
            _isRunning = false;
         }
      }
   }
}

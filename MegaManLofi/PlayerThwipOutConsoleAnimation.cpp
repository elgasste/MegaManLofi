#include "PlayerThwipOutConsoleAnimation.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"

using namespace std;
using namespace MegaManLofi;

PlayerThwipOutConsoleAnimation::PlayerThwipOutConsoleAnimation( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                                const shared_ptr<ConsoleRenderConfig> renderConfig ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
   _isRunning( false ),
   _startCoordinate( {0, 0} ),
   _endCoordinate( {0, 0} ),
   _preThwipping( false ),
   _elapsedSeconds( 0 )
{
}

void PlayerThwipOutConsoleAnimation::Start( Coordinate<short> startCoordinate, Coordinate<short> endCoordinate )
{
   _startCoordinate = startCoordinate;
   _endCoordinate = endCoordinate;
   _preThwipping = true;
   _elapsedSeconds = 0;

   // MUFFINS: reset the thwip and transition sprites
}

void PlayerThwipOutConsoleAnimation::Draw()
{
   if ( _preThwipping )
   {
      _consoleBuffer->Draw( _startCoordinate.Left, _startCoordinate.Top, _renderConfig->PlayerThwipOutTransitionSprite );
   }
   else
   {
      // MUFFINS: draw the twhip sprite wherever it needs to be based on velocity and _elapsedSeconds
   }
}

void PlayerThwipOutConsoleAnimation::Tick( int framesPerSecond )
{
   _elapsedSeconds += 1 / (double)framesPerSecond;

   if ( _preThwipping )
   {
      // MUFFINS: tick the sprite and check if it's finished
      // if it is finished, reset _elapsedSeconds and set _preThwipping to false
   }
   else
   {
      // MUFFINS: just tick the sprite
   }
}

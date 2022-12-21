#include "PlayerThwipInConsoleAnimation.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"
#include "IFrameRateProvider.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

PlayerThwipInConsoleAnimation::PlayerThwipInConsoleAnimation( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                              const shared_ptr<ConsoleRenderConfig> renderConfig,
                                                              const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
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
   _isRunning = true;
   _startPositionChars = startPositionChars.value();
   _endPositionChars = endPositionChars.value();
   _currentTopPositionUnits = _startPositionChars.Top * _renderConfig->ArenaCharHeight;
   _endTopPositionUnits = _endPositionChars.Top * _renderConfig->ArenaCharHeight;
   _postThwipping = false;
   _elapsedSeconds = 0;

   _renderConfig->PlayerThwipInTransitionSprite->Reset();
   _renderConfig->PlayerThwipSprite->Reset();
}

void PlayerThwipInConsoleAnimation::Draw()
{
   if ( _postThwipping )
   {
      _consoleBuffer->Draw( _startPositionChars.Left, _endPositionChars.Top, _renderConfig->PlayerThwipInTransitionSprite );
   }
   else
   {
      auto leftOffset = short( ( _renderConfig->PlayerThwipInTransitionSprite->GetWidth() - _renderConfig->PlayerThwipSprite->GetWidth() ) / 2 );
      _consoleBuffer->Draw( _startPositionChars.Left + leftOffset,
                            (short)( _currentTopPositionUnits / _renderConfig->ArenaCharHeight ),
                            _renderConfig->PlayerThwipSprite );
   }
}

void PlayerThwipInConsoleAnimation::Tick()
{
   if ( !_isRunning )
   {
      return;
   }

   _elapsedSeconds += _frameRateProvider->GetFrameScalar();

   if ( _postThwipping )
   {
      _renderConfig->PlayerThwipInTransitionSprite->Tick();

      auto test = _renderConfig->PlayerThwipInTransitionSprite->GetTotalTraversalSeconds();
      if ( _elapsedSeconds >= _renderConfig->PlayerThwipInTransitionSprite->GetTotalTraversalSeconds() )
      {
         _isRunning = false;
      }
   }
   else
   {
      _renderConfig->PlayerThwipSprite->Tick();
      auto topDelta = (long long)( _renderConfig->PlayerThwipVelocity * _frameRateProvider->GetFrameScalar() );

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

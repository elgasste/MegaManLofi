#include "PlayingStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"
#include "IPlayerInfoProvider.h"
#include "IArenaInfoProvider.h"
#include "IGameEventAggregator.h"
#include "IFrameRateProvider.h"
#include "ConsoleColor.h"
#include "Direction.h"
#include "ConsoleSprite.h"
#include "ConsolePixel.h"
#include "GameEvent.h"

using namespace std;
using namespace MegaManLofi;

PlayingStateConsoleRenderer::PlayingStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                          const shared_ptr<ConsoleRenderConfig> renderConfig,
                                                          const shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                                          const shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                                          const shared_ptr<IGameEventAggregator> eventAggregator,
                                                          const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
   _playerInfoProvider( playerInfoProvider ),
   _arenaInfoProvider( arenaInfoProvider ),
   _eventAggregator( eventAggregator ),
   _frameRateProvider( frameRateProvider ),
   _arenaCoordConverterX( renderConfig->ArenaCharWidth / (double)arenaInfoProvider->GetWidth() ),
   _arenaCoordConverterY( renderConfig->ArenaCharHeight / (double)arenaInfoProvider->GetHeight() ),
   _isAnimatingGameStart( false ),
   _gameStartBlinkElapsedSeconds( 0 )
{
   eventAggregator->RegisterEventHandler( GameEvent::GameStarted, std::bind( &PlayingStateConsoleRenderer::HandleGameStartedEvent, this ) );
}

void PlayingStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultBackgroundColor( ConsoleColor::Black );
   _consoleBuffer->SetDefaultForegroundColor( ConsoleColor::White );

   if ( _isAnimatingGameStart )
   {
      DrawGameStartAnimation();
   }
   else
   {
      _consoleBuffer->Draw( 2, 1, "Use the direction buttons to move around, or press the select button to quit." );

      DrawArenaFence();
      DrawArenaSprites();
      DrawPlayer();
   }
}

bool PlayingStateConsoleRenderer::HasFocus() const
{
   return _isAnimatingGameStart;
}

void PlayingStateConsoleRenderer::HandleGameStartedEvent()
{
   _isAnimatingGameStart = true;
   _gameStartBlinkElapsedSeconds = 0;
}

void PlayingStateConsoleRenderer::DrawGameStartAnimation()
{
   _gameStartBlinkElapsedSeconds += 1 / (double)_frameRateProvider->GetFramesPerSecond();

   if ( (int)( _gameStartBlinkElapsedSeconds / _renderConfig->GameStartSingleBlinkSeconds ) % 2 == 0 )
   {
      _consoleBuffer->Draw( ( _renderConfig->ArenaCharWidth / 2 ) - 5, _renderConfig->ArenaCharHeight / 2, "GET READY!" );
   }

   if ( _gameStartBlinkElapsedSeconds >= ( _renderConfig->GameStartSingleBlinkSeconds * _renderConfig->GameStartBlinkCount ) )
   {
      _isAnimatingGameStart = false;
   }
}

void PlayingStateConsoleRenderer::DrawArenaFence()
{
   // corners
   _consoleBuffer->Draw( _renderConfig->ArenaFenceX, _renderConfig->ArenaFenceY, ',');
   _consoleBuffer->Draw( _renderConfig->ArenaFenceX + _renderConfig->ArenaCharWidth + 1, _renderConfig->ArenaFenceY, '.' );
   _consoleBuffer->Draw( _renderConfig->ArenaFenceX, _renderConfig->ArenaFenceY + _renderConfig->ArenaCharHeight + 1, '\'' );
   _consoleBuffer->Draw( _renderConfig->ArenaFenceX + _renderConfig->ArenaCharWidth + 1, _renderConfig->ArenaFenceY + _renderConfig->ArenaCharHeight + 1, '\'' );

   // top and bottom fences
   for ( int left = _renderConfig->ArenaFenceX; left < _renderConfig->ArenaCharWidth + 1; left++ )
   {
      _consoleBuffer->Draw( left + 1, _renderConfig->ArenaFenceY, '-' );
      _consoleBuffer->Draw( left + 1, _renderConfig->ArenaFenceY + _renderConfig->ArenaCharHeight + 1, '-' );
   }

   // side fences
   for ( int top = _renderConfig->ArenaFenceY + 1; top < _renderConfig->ArenaFenceY + _renderConfig->ArenaCharHeight + 1; top++ )
   {
      _consoleBuffer->Draw( _renderConfig->ArenaFenceX, top, '|' );
      _consoleBuffer->Draw( _renderConfig->ArenaFenceX + _renderConfig->ArenaCharWidth + 1, top, '|' );
   }
}

void PlayingStateConsoleRenderer::DrawArenaSprites()
{
   for ( int i = 0; i < _renderConfig->ArenaCharHeight; i++ )
   {
      for ( int j = 0; j < _renderConfig->ArenaCharWidth; j++ )
      {
         auto arenaIndex = ( i * _renderConfig->ArenaCharWidth ) + j;
         auto spriteIterator = _renderConfig->ArenaSpriteMap.find( arenaIndex );

         if ( spriteIterator != _renderConfig->ArenaSpriteMap.end() )
         {
            auto x = _renderConfig->ArenaFenceX + j + 1;
            auto y = _renderConfig->ArenaFenceY + i + 1;
            _consoleBuffer->Draw( x, y, _renderConfig->ArenaSprites[ spriteIterator->second ] );
         }
      }
   }
}

void PlayingStateConsoleRenderer::DrawPlayer()
{
   auto convertedPlayerX = (short)( _arenaInfoProvider->GetPlayerPositionX() * _arenaCoordConverterX );
   auto convertedPlayerY = (short)( _arenaInfoProvider->GetPlayerPositionY() * _arenaCoordConverterY );

   auto playerX = convertedPlayerX + _renderConfig->ArenaFenceX + 1;
   auto playerY = convertedPlayerY + _renderConfig->ArenaFenceY + 1;

   auto direction = _playerInfoProvider->GetDirection();
   auto sprite = _playerInfoProvider->IsMoving() ? _renderConfig->PlayerMovingSpriteMap[direction] : _renderConfig->PlayerStaticSpriteMap[direction];

   _consoleBuffer->Draw( playerX, playerY, sprite );
}

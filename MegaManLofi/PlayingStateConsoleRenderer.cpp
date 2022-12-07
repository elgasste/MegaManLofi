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
   _viewportWidth( _renderConfig->ArenaViewportWidthChar * _renderConfig->ArenaCharWidth ),
   _viewportHeight( _renderConfig->ArenaViewportHeightChar * _renderConfig->ArenaCharHeight ),
   _viewportOffsetX( 0 ),
   _viewportOffsetY( 0 ),
   _isAnimatingGameStart( false ),
   _gameStartBlinkElapsedSeconds( 0 )
{
   eventAggregator->RegisterEventHandler( GameEvent::GameStarted, std::bind( &PlayingStateConsoleRenderer::HandleGameStartedEvent, this ) );
}

void PlayingStateConsoleRenderer::Render()
{
   // TODO: move these into a config somewhere
   _consoleBuffer->SetDefaultBackgroundColor( ConsoleColor::Black );
   _consoleBuffer->SetDefaultForegroundColor( ConsoleColor::White );

   if ( _isAnimatingGameStart )
   {
      DrawGameStartAnimation();
   }
   else
   {
      CalculateViewportOffsets();
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

void PlayingStateConsoleRenderer::CalculateViewportOffsets()
{
   _viewportOffsetX = _arenaInfoProvider->GetPlayerPositionX() - ( _viewportWidth / 2 );
   _viewportOffsetY = _arenaInfoProvider->GetPlayerPositionY() - ( _viewportHeight / 2 );
}

void PlayingStateConsoleRenderer::DrawGameStartAnimation()
{
   _gameStartBlinkElapsedSeconds += 1 / (double)_frameRateProvider->GetFramesPerSecond();

   if ( (int)( _gameStartBlinkElapsedSeconds / _renderConfig->GameStartSingleBlinkSeconds ) % 2 == 0 )
   {
      _consoleBuffer->Draw( ( _renderConfig->ArenaViewportWidthChar / 2 ) - 5, _renderConfig->ArenaViewportHeightChar / 2, "GET READY!" );
   }

   if ( _gameStartBlinkElapsedSeconds >= ( _renderConfig->GameStartSingleBlinkSeconds * _renderConfig->GameStartBlinkCount ) )
   {
      _isAnimatingGameStart = false;
   }
}

void PlayingStateConsoleRenderer::DrawArenaSprites()
{
   auto spriteOffsetX = (int)max( _viewportOffsetX / _renderConfig->ArenaCharWidth, 0ll );
   auto spriteOffsetY = (int)max( _viewportOffsetY / _renderConfig->ArenaCharHeight, 0ll );
   auto arenaWidthChar = (int)( _arenaInfoProvider->GetWidth() / _renderConfig->ArenaCharWidth );
   auto arenaHeightChar = (int)( _arenaInfoProvider->GetHeight() / _renderConfig->ArenaCharHeight );

   if ( ( spriteOffsetX + _renderConfig->ArenaViewportWidthChar ) > arenaWidthChar )
   {
      spriteOffsetX = arenaWidthChar - _renderConfig->ArenaViewportWidthChar;
   }
   if ( ( spriteOffsetY + _renderConfig->ArenaViewportHeightChar ) > arenaHeightChar )
   {
      spriteOffsetY = arenaHeightChar - _renderConfig->ArenaViewportHeightChar;
   }
   
   // TODO: this will probably crash if the arena is smaller than the viewport
   for ( int y = 0; y < _renderConfig->ArenaViewportHeightChar; y++ )
   {
      for ( int x = 0; x < _renderConfig->ArenaViewportWidthChar; x++ )
      {
         auto spriteIndex = ( ( y + spriteOffsetY ) * arenaWidthChar ) + ( x + spriteOffsetX );
         auto spriteIterator = _renderConfig->ArenaSpriteMap.find( spriteIndex );

         if ( spriteIterator != _renderConfig->ArenaSpriteMap.end() )
         {
            auto viewportX = _renderConfig->ArenaViewportX + x;
            auto viewportY = _renderConfig->ArenaViewportY + y;
            _consoleBuffer->Draw( viewportX, viewportY, _renderConfig->ArenaSprites[ spriteIterator->second ] );
         }
      }
   }
}

void PlayingStateConsoleRenderer::DrawPlayer()
{
   auto playerDrawX = _arenaInfoProvider->GetPlayerPositionX() - _viewportOffsetX;
   auto playerDrawY = _arenaInfoProvider->GetPlayerPositionY() - _viewportOffsetY;

   if ( _viewportOffsetX < 0 )
   {
      playerDrawX += _viewportOffsetX;
   }
   else if ( _viewportOffsetX + _viewportWidth > _arenaInfoProvider->GetWidth() )
   {
      playerDrawX += ( _viewportOffsetX + _viewportWidth ) - _arenaInfoProvider->GetWidth();
   }

   if ( _viewportOffsetY < 0 )
   {
      playerDrawY += _viewportOffsetY;
   }
   else if ( _viewportOffsetY + _viewportHeight > _arenaInfoProvider->GetHeight() )
   {
      playerDrawY += ( _viewportOffsetY + _viewportHeight ) - _arenaInfoProvider->GetHeight();
   }

   auto convertedPlayerDrawX = (short)( playerDrawX / _renderConfig->ArenaCharWidth );
   auto convertedPlayerDrawY = (short)( playerDrawY / _renderConfig->ArenaCharHeight );

   auto direction = _playerInfoProvider->GetDirection();
   auto sprite = _playerInfoProvider->IsMoving() ? _renderConfig->PlayerMovingSpriteMap[direction] : _renderConfig->PlayerStaticSpriteMap[direction];

   _consoleBuffer->Draw( convertedPlayerDrawX, convertedPlayerDrawY, sprite );
}

#include "PlayingStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"
#include "IGameInfoProvider.h"
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
                                                          const shared_ptr<IGameInfoProvider> gameInfoProvider,
                                                          const shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                                          const shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                                          const shared_ptr<IGameEventAggregator> eventAggregator,
                                                          const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
   _gameInfoProvider( gameInfoProvider ),
   _playerInfoProvider( playerInfoProvider ),
   _arenaInfoProvider( arenaInfoProvider ),
   _eventAggregator( eventAggregator ),
   _frameRateProvider( frameRateProvider ),
   _viewportWidth( _renderConfig->ArenaViewportWidthChar * _renderConfig->ArenaCharWidth ),
   _viewportHeight( _renderConfig->ArenaViewportHeightChar * _renderConfig->ArenaCharHeight ),
   _viewportOffsetX( 0 ),
   _viewportOffsetY( 0 ),
   _isAnimatingGameStart( false ),
   _isAnimatingPitfall( false ),
   _isAnimatingPlayerExplosion( false ),
   _gameStartElapsedSeconds( 0 ),
   _pitfallElapsedSeconds( 0 ),
   _playerExplosionElapsedSeconds( 0 ),
   _playerExplosionStartFrame( 0 )
{
   eventAggregator->RegisterEventHandler( GameEvent::GameStarted, std::bind( &PlayingStateConsoleRenderer::HandleGameStartedEvent, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::Pitfall, std::bind( &PlayingStateConsoleRenderer::HandlePitfallEvent, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::TileDeath, std::bind( &PlayingStateConsoleRenderer::HandleTileDeathEvent, this ) );
}

void PlayingStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultForegroundColor( _renderConfig->ArenaForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderConfig->ArenaBackgroundColor );

   CalculateViewportOffsets();
   DrawArenaSprites();

   if ( _isAnimatingGameStart )
   {
      DrawGameStartAnimation();
   }
   else if ( _isAnimatingPitfall )
   {
      DrawPitfallAnimation();
   }
   else if ( _isAnimatingPlayerExplosion )
   {
      DrawPlayerExplosionAnimation();
   }
   else if ( _gameInfoProvider->IsPaused() )
   {
      DrawPauseOverlay();
   }
   else
   {
      DrawPlayer();
   }
}

bool PlayingStateConsoleRenderer::HasFocus() const
{
   return _isAnimatingGameStart || _isAnimatingPitfall || _isAnimatingPlayerExplosion;
}

void PlayingStateConsoleRenderer::HandleGameStartedEvent()
{
   _isAnimatingGameStart = true;
   _gameStartElapsedSeconds = 0;
}

void PlayingStateConsoleRenderer::HandlePitfallEvent()
{
   _isAnimatingPitfall = true;
   _pitfallElapsedSeconds = 0;
}

void PlayingStateConsoleRenderer::HandleTileDeathEvent()
{
   _isAnimatingPlayerExplosion = true;
   _playerExplosionElapsedSeconds = 0;
   _playerExplosionStartFrame = _frameRateProvider->GetCurrentFrame();
}

void PlayingStateConsoleRenderer::CalculateViewportOffsets()
{
   _viewportOffsetX = _arenaInfoProvider->GetPlayerPositionX() - ( _viewportWidth / 2 );
   _viewportOffsetY = _arenaInfoProvider->GetPlayerPositionY() - ( _viewportHeight / 2 );
}

void PlayingStateConsoleRenderer::DrawGameStartAnimation()
{
   _gameStartElapsedSeconds += ( 1 / (double)_frameRateProvider->GetFramesPerSecond() );

   if ( (int)( _gameStartElapsedSeconds / _renderConfig->GameStartSingleBlinkSeconds ) % 2 == 0 )
   {
      auto left = ( _renderConfig->ArenaViewportWidthChar / 2 ) - ( _renderConfig->GetReadySprite.Width / 2 ) + _renderConfig->ArenaViewportX;
      auto top = ( _renderConfig->ArenaViewportHeightChar / 2 ) - ( _renderConfig->GetReadySprite.Height / 2 ) + _renderConfig->ArenaViewportY;

      _consoleBuffer->Draw( left, top, _renderConfig->GetReadySprite );
   }

   if ( _gameStartElapsedSeconds >= ( _renderConfig->GameStartSingleBlinkSeconds * _renderConfig->GameStartBlinkCount ) )
   {
      _isAnimatingGameStart = false;
   }
}

void PlayingStateConsoleRenderer::DrawPitfallAnimation()
{
   _pitfallElapsedSeconds += ( 1 / (double)_frameRateProvider->GetFramesPerSecond() );

   if ( _pitfallElapsedSeconds >= _renderConfig->PitfallAnimationSeconds )
   {
      _isAnimatingPitfall = false;
   }
}

void PlayingStateConsoleRenderer::DrawPlayerExplosionAnimation()
{
   auto frameRateScalar = ( 1 / (double)_frameRateProvider->GetFramesPerSecond() );
   _playerExplosionElapsedSeconds += frameRateScalar;

   const auto& particleSprite = (int)( _playerExplosionElapsedSeconds / _renderConfig->PlayerExplosionSpriteSwapSeconds ) % 2 == 0 ?
      _renderConfig->PlayerExplosionParticleSprite1 : _renderConfig->PlayerExplosionParticleSprite2;

   const auto& hitBox = _playerInfoProvider->GetHitBox();
   auto particleStartX = GetPlayerViewportX() + (short)( hitBox.Width / 2 / _renderConfig->ArenaCharWidth ) + _renderConfig->ArenaViewportX;
   auto particleStartY = GetPlayerViewportY() + (short)( hitBox.Height / 2 / _renderConfig->ArenaCharHeight ) + _renderConfig->ArenaViewportY;
   
   auto elapsedFrames = _frameRateProvider->GetCurrentFrame() - _playerExplosionStartFrame;
   auto particleIncrement = ( _renderConfig->PlayerExplosionParticleVelocity * frameRateScalar );
   auto particleDeltaX = (short)( ( particleIncrement * elapsedFrames ) / _renderConfig->ArenaCharWidth );
   auto particleDeltaY = (short)( ( particleIncrement * elapsedFrames ) / _renderConfig->ArenaCharHeight );

   // horizontal and vertical particles
   _consoleBuffer->Draw( particleStartX + particleDeltaX, particleStartY, particleSprite );
   _consoleBuffer->Draw( particleStartX - particleDeltaX, particleStartY, particleSprite );
   _consoleBuffer->Draw( particleStartX, particleStartY + particleDeltaY, particleSprite );
   _consoleBuffer->Draw( particleStartX, particleStartY - particleDeltaY, particleSprite );

   // diagonal particles
   _consoleBuffer->Draw( particleStartX + (short)( particleDeltaX / 1.5 ), particleStartY + (short)( particleDeltaY / 1.5 ), particleSprite );
   _consoleBuffer->Draw( particleStartX - (short)( particleDeltaX / 1.5 ), particleStartY + (short)( particleDeltaY / 1.5 ), particleSprite );
   _consoleBuffer->Draw( particleStartX + (short)( particleDeltaX / 1.5 ), particleStartY - (short)( particleDeltaY / 1.5 ), particleSprite );
   _consoleBuffer->Draw( particleStartX - (short)( particleDeltaX / 1.5 ), particleStartY - (short)( particleDeltaY / 1.5 ), particleSprite );

   if ( _playerExplosionElapsedSeconds >= _renderConfig->PlayerExplosionAnimationSeconds )
   {
      _isAnimatingPlayerExplosion = false;
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
         auto spriteId = _renderConfig->ArenaSprites[spriteIndex];

         if ( spriteId != -1 )
         {
            auto viewportX = x + _renderConfig->ArenaViewportX;
            auto viewportY = y + _renderConfig->ArenaViewportY;
            _consoleBuffer->Draw( viewportX, viewportY, _renderConfig->ArenaSpriteMap[ spriteId ] );
         }
      }
   }
}

void PlayingStateConsoleRenderer::DrawPlayer()
{
   auto playerDrawX = GetPlayerViewportX() + _renderConfig->ArenaViewportX;
   auto playerDrawY = GetPlayerViewportY() + _renderConfig->ArenaViewportY;

   auto direction = _playerInfoProvider->GetDirection();
   auto sprite = _playerInfoProvider->IsMoving() ? _renderConfig->PlayerMovingSpriteMap[direction] : _renderConfig->PlayerStaticSpriteMap[direction];

   _consoleBuffer->Draw( playerDrawX, playerDrawY, sprite );
}

void PlayingStateConsoleRenderer::DrawPauseOverlay()
{
   auto left = ( _renderConfig->ArenaViewportWidthChar / 2 ) - ( _renderConfig->PauseOverlaySprite.Width / 2 );
   auto top = ( _renderConfig->ArenaViewportHeightChar / 2 ) - ( _renderConfig->PauseOverlaySprite.Height / 2 );

   _consoleBuffer->Draw( left, top, _renderConfig->PauseOverlaySprite );
}

short PlayingStateConsoleRenderer::GetPlayerViewportX() const
{
   auto playerDrawX = _arenaInfoProvider->GetPlayerPositionX() - _viewportOffsetX;

   if ( _viewportOffsetX < 0 )
   {
      playerDrawX += _viewportOffsetX;
   }
   else if ( _viewportOffsetX + _viewportWidth > _arenaInfoProvider->GetWidth() )
   {
      playerDrawX += ( _viewportOffsetX + _viewportWidth ) - _arenaInfoProvider->GetWidth();
   }

   return (short)( playerDrawX / _renderConfig->ArenaCharWidth );
}

short PlayingStateConsoleRenderer::GetPlayerViewportY() const
{
   auto playerDrawY = _arenaInfoProvider->GetPlayerPositionY() - _viewportOffsetY;

   if ( _viewportOffsetY < 0 )
   {
      playerDrawY += _viewportOffsetY;
   }
   else if ( _viewportOffsetY + _viewportHeight > _arenaInfoProvider->GetHeight() )
   {
      playerDrawY += ( _viewportOffsetY + _viewportHeight ) - _arenaInfoProvider->GetHeight();
   }

   return (short)( playerDrawY / _renderConfig->ArenaCharHeight );
}

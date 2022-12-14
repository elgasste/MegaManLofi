#include <format>

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
   _viewportQuadUnits( { 0, 0, 0, 0 } ),
   _viewportRectChars( { 0, 0, 0, 0 } ),
   _viewportOffsetChars( { 0, 0 } ),
   _playerViewportChars( { 0, 0 } ),
   _isAnimatingStageStart( false ),
   _isAnimatingPlayerThwipIn( false ),
   _isAnimatingPitfall( false ),
   _isAnimatingPlayerExplosion( false ),
   _stageStartAnimationElapsedSeconds( 0 ),
   _pitfallAnimationElapsedSeconds( 0 ),
   _playerExplosionAnimationElapsedSeconds( 0 ),
   _playerThwipBottom( 0 ),
   _playerExplosionStartFrame( 0 )
{
   eventAggregator->RegisterEventHandler( GameEvent::StageStarted, std::bind( &PlayingStateConsoleRenderer::HandleStageStartedEvent, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::Pitfall, std::bind( &PlayingStateConsoleRenderer::HandlePitfallEvent, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::TileDeath, std::bind( &PlayingStateConsoleRenderer::HandleTileDeathEvent, this ) );
}

void PlayingStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultForegroundColor( _renderConfig->ArenaForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderConfig->ArenaBackgroundColor );

   UpdateCaches();
   DrawArenaSprites();

   if ( _isAnimatingStageStart )
   {
      DrawGameStartAnimation();
   }
   else if ( _isAnimatingPlayerThwipIn )
   {
      DrawPlayerThwipInAnimation();
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
      DrawStatusBar();
   }
}

bool PlayingStateConsoleRenderer::HasFocus() const
{
   return _isAnimatingStageStart || _isAnimatingPlayerThwipIn || _isAnimatingPitfall || _isAnimatingPlayerExplosion;
}

void PlayingStateConsoleRenderer::HandleStageStartedEvent()
{
   _isAnimatingStageStart = true;
   _stageStartAnimationElapsedSeconds = 0;
}

void PlayingStateConsoleRenderer::HandlePitfallEvent()
{
   _isAnimatingPitfall = true;
   _pitfallAnimationElapsedSeconds = 0;
}

void PlayingStateConsoleRenderer::HandleTileDeathEvent()
{
   _isAnimatingPlayerExplosion = true;
   _playerExplosionAnimationElapsedSeconds = 0;
   _playerExplosionStartFrame = _frameRateProvider->GetCurrentFrame();
}

void PlayingStateConsoleRenderer::UpdateCaches()
{
   auto viewportWidthUnits = _renderConfig->ArenaViewportWidthChars * _renderConfig->ArenaCharWidth;
   auto viewportHeightUnits = _renderConfig->ArenaViewportHeightChars * _renderConfig->ArenaCharHeight;

   _viewportQuadUnits.Left = max( _arenaInfoProvider->GetPlayerPositionX() - ( viewportWidthUnits / 2 ), 0ll );
   _viewportQuadUnits.Top = max( _arenaInfoProvider->GetPlayerPositionY() - ( viewportHeightUnits / 2 ), 0ll );
   _viewportQuadUnits.Right = _viewportQuadUnits.Left + viewportWidthUnits;
   _viewportQuadUnits.Bottom = _viewportQuadUnits.Top + viewportHeightUnits;

   auto arenaWidth = _arenaInfoProvider->GetWidth();
   if ( _viewportQuadUnits.Right > arenaWidth )
   {
      _viewportQuadUnits.Right = arenaWidth;
      _viewportQuadUnits.Left = max( 0ll, _viewportQuadUnits.Right - viewportWidthUnits );
   }

   auto arenaHeight = _arenaInfoProvider->GetHeight();
   if ( _viewportQuadUnits.Bottom > arenaHeight )
   {
      _viewportQuadUnits.Bottom = arenaHeight;
      _viewportQuadUnits.Top = max( 0ll, _viewportQuadUnits.Bottom - viewportHeightUnits );
   }

   _viewportRectChars.Left = (short)( _viewportQuadUnits.Left / _renderConfig->ArenaCharWidth );
   _viewportRectChars.Top = (short)( _viewportQuadUnits.Top / _renderConfig->ArenaCharHeight );
   _viewportRectChars.Width = (short)( viewportWidthUnits / _renderConfig->ArenaCharWidth );
   _viewportRectChars.Height = (short)( viewportHeightUnits / _renderConfig->ArenaCharHeight );

   // this accounts for the case where the arena is smaller than the viewport
   _viewportOffsetChars.Left = _renderConfig->ArenaViewportLeftChars + ( ( _renderConfig->ArenaViewportWidthChars - _viewportRectChars.Width ) / 2 );
   _viewportOffsetChars.Top = _renderConfig->ArenaViewportTopChars + ( ( _renderConfig->ArenaViewportHeightChars - _viewportRectChars.Height ) / 2 );

   _playerViewportChars.Left = (short)( ( _arenaInfoProvider->GetPlayerPositionX() - _viewportQuadUnits.Left ) / _renderConfig->ArenaCharWidth );
   _playerViewportChars.Top = (short)( ( _arenaInfoProvider->GetPlayerPositionY() - _viewportQuadUnits.Top ) / _renderConfig->ArenaCharHeight );
}

void PlayingStateConsoleRenderer::DrawGameStartAnimation()
{
   _stageStartAnimationElapsedSeconds += ( 1 / (double)_frameRateProvider->GetFramesPerSecond() );

   if ( (int)( _stageStartAnimationElapsedSeconds / _renderConfig->GameStartSingleBlinkSeconds ) % 2 == 0 )
   {
      auto left = ( _viewportRectChars.Width / 2 ) - ( _renderConfig->GetReadySprite.Width / 2 ) + _viewportOffsetChars.Left;
      auto top = ( _viewportRectChars.Height / 2 ) - ( _renderConfig->GetReadySprite.Height / 2 ) + _viewportOffsetChars.Top;

      _consoleBuffer->Draw( left, top, _renderConfig->GetReadySprite );
   }

   if ( _stageStartAnimationElapsedSeconds >= ( _renderConfig->GameStartSingleBlinkSeconds * _renderConfig->GameStartBlinkCount ) )
   {
      _isAnimatingStageStart = false;
      _isAnimatingPlayerThwipIn = true;
      _playerThwipBottom = _viewportQuadUnits.Top;
   }
}

void PlayingStateConsoleRenderer::DrawPlayerThwipInAnimation()
{
   auto thwipDeltaUnits = ( _renderConfig->PlayerThwipVelocity / _frameRateProvider->GetFramesPerSecond() );
   _playerThwipBottom += thwipDeltaUnits;

   auto playerSprite = _renderConfig->PlayerStaticSpriteMap[_playerInfoProvider->GetDirection()];
   auto thwipSpriteLeftOffsetChars = (short)( ( playerSprite.Width - _renderConfig->PlayerThwipSprite.Width ) / 2 );

   const auto& hitBox = _playerInfoProvider->GetHitBox();
   if ( _playerThwipBottom >= ( _arenaInfoProvider->GetPlayerPositionY() + hitBox.Height ) )
   {
      _isAnimatingPlayerThwipIn = false;
      return;
   }

   auto playerThwipBottomViewportChars = (short)( ( _playerThwipBottom - _viewportQuadUnits.Top ) / _renderConfig->ArenaCharHeight );
   _consoleBuffer->Draw( _playerViewportChars.Left + thwipSpriteLeftOffsetChars + _viewportOffsetChars.Left,
                         ( playerThwipBottomViewportChars - _renderConfig->PlayerThwipSprite.Height ) + _viewportOffsetChars.Top,
                         _renderConfig->PlayerThwipSprite );
}

void PlayingStateConsoleRenderer::DrawPitfallAnimation()
{
   _pitfallAnimationElapsedSeconds += ( 1 / (double)_frameRateProvider->GetFramesPerSecond() );

   if ( _pitfallAnimationElapsedSeconds >= _renderConfig->PitfallAnimationSeconds )
   {
      _isAnimatingPitfall = false;
   }
}

void PlayingStateConsoleRenderer::DrawPlayerExplosionAnimation()
{
   auto frameRateScalar = ( 1 / (double)_frameRateProvider->GetFramesPerSecond() );
   _playerExplosionAnimationElapsedSeconds += frameRateScalar;

   const auto& particleSprite = (int)( _playerExplosionAnimationElapsedSeconds / _renderConfig->PlayerExplosionSpriteSwapSeconds ) % 2 == 0 ?
      _renderConfig->PlayerExplosionParticleSprite1 : _renderConfig->PlayerExplosionParticleSprite2;

   const auto& hitBox = _playerInfoProvider->GetHitBox();
   auto particleStartLeftChars = _playerViewportChars.Left + (short)( hitBox.Width / 2 / _renderConfig->ArenaCharWidth ) + _viewportOffsetChars.Left;
   auto particleStartTopChars = _playerViewportChars.Top + (short)( hitBox.Height / 2 / _renderConfig->ArenaCharHeight ) + _viewportOffsetChars.Top;
   
   auto elapsedFrames = _frameRateProvider->GetCurrentFrame() - _playerExplosionStartFrame;
   auto particleIncrement = ( _renderConfig->PlayerExplosionParticleVelocity * frameRateScalar );
   auto particleDeltaXChars = (short)( ( particleIncrement * elapsedFrames ) / _renderConfig->ArenaCharWidth );
   auto particleDeltaYChars = (short)( ( particleIncrement * elapsedFrames ) / _renderConfig->ArenaCharHeight );

   // horizontal and vertical particles
   _consoleBuffer->Draw( particleStartLeftChars + particleDeltaXChars, particleStartTopChars, particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars + ( particleDeltaXChars / 2 ), particleStartTopChars, particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars - particleDeltaXChars, particleStartTopChars, particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars - ( particleDeltaXChars / 2 ), particleStartTopChars, particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars, particleStartTopChars + particleDeltaYChars, particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars, particleStartTopChars + ( particleDeltaYChars / 2 ), particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars, particleStartTopChars - particleDeltaYChars, particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars, particleStartTopChars - ( particleDeltaYChars / 2 ), particleSprite );

   // diagonal particles
   _consoleBuffer->Draw( particleStartLeftChars + (short)( particleDeltaXChars / 1.5 ), particleStartTopChars + (short)( particleDeltaYChars / 1.5 ), particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars + (short)( particleDeltaXChars / 3 ), particleStartTopChars + (short)( particleDeltaYChars / 3 ), particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars - (short)( particleDeltaXChars / 1.5 ), particleStartTopChars + (short)( particleDeltaYChars / 1.5 ), particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars - (short)( particleDeltaXChars / 3 ), particleStartTopChars + (short)( particleDeltaYChars / 3 ), particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars + (short)( particleDeltaXChars / 1.5 ), particleStartTopChars - (short)( particleDeltaYChars / 1.5 ), particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars + (short)( particleDeltaXChars / 3 ), particleStartTopChars - (short)( particleDeltaYChars / 3 ), particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars - (short)( particleDeltaXChars / 1.5 ), particleStartTopChars - (short)( particleDeltaYChars / 1.5 ), particleSprite );
   _consoleBuffer->Draw( particleStartLeftChars - (short)( particleDeltaXChars / 3 ), particleStartTopChars - (short)( particleDeltaYChars / 3 ), particleSprite );

   if ( _playerExplosionAnimationElapsedSeconds >= _renderConfig->PlayerExplosionAnimationSeconds )
   {
      _isAnimatingPlayerExplosion = false;
   }
}

void PlayingStateConsoleRenderer::DrawArenaSprites()
{
   auto arenaWidthChars = (short)( _arenaInfoProvider->GetWidth() / _renderConfig->ArenaCharWidth );

   for ( short y = 0; y < _viewportRectChars.Height; y++ )
   {
      for ( short x = 0; x < _viewportRectChars.Width; x++ )
      {
         auto spriteIndex = ( ( _viewportRectChars.Top + y ) * arenaWidthChars ) + ( _viewportRectChars.Left + x );
         auto spriteId = _renderConfig->ArenaSprites[spriteIndex];

         if ( spriteId != -1 )
         {
            _consoleBuffer->Draw( x + _viewportOffsetChars.Left, y + _viewportOffsetChars.Top, _renderConfig->ArenaSpriteMap[ spriteId ] );
         }
      }
   }
}

void PlayingStateConsoleRenderer::DrawPlayer()
{
   auto direction = _playerInfoProvider->GetDirection();
   auto sprite = _playerInfoProvider->IsMoving() ? _renderConfig->PlayerMovingSpriteMap[direction] : _renderConfig->PlayerStaticSpriteMap[direction];

   _consoleBuffer->Draw( _playerViewportChars.Left + _viewportOffsetChars.Left, _playerViewportChars.Top + _viewportOffsetChars.Top, sprite );
}

void PlayingStateConsoleRenderer::DrawStatusBar()
{
   _consoleBuffer->Draw( _renderConfig->ArenaStatusBarLeftChars, _renderConfig->ArenaStatusBarTopChars, format( "Lives: {}", _playerInfoProvider->GetLivesRemaining() ) );
}

void PlayingStateConsoleRenderer::DrawPauseOverlay()
{
   auto left = ( _viewportRectChars.Width / 2 ) - ( _renderConfig->PauseOverlaySprite.Width / 2 );
   auto top = ( _viewportRectChars.Height / 2 ) - ( _renderConfig->PauseOverlaySprite.Height / 2 );

   _consoleBuffer->Draw( left + _viewportOffsetChars.Left, top + _viewportOffsetChars.Top, _renderConfig->PauseOverlaySprite );
}

#include <format>

#include "PlayingStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"
#include "IGameInfoProvider.h"
#include "IPlayerInfoProvider.h"
#include "IArenaInfoProvider.h"
#include "IGameEventAggregator.h"
#include "IFrameRateProvider.h"
#include "IConsoleAnimationProvider.h"
#include "IConsoleAnimation.h"
#include "Direction.h"
#include "GameEvent.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

PlayingStateConsoleRenderer::PlayingStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                          const shared_ptr<ConsoleRenderConfig> renderConfig,
                                                          const shared_ptr<IGameInfoProvider> gameInfoProvider,
                                                          const shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                                          const shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                                          const shared_ptr<IGameEventAggregator> eventAggregator,
                                                          const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                          const shared_ptr<IConsoleAnimationProvider> animationProvider ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
   _gameInfoProvider( gameInfoProvider ),
   _playerInfoProvider( playerInfoProvider ),
   _arenaInfoProvider( arenaInfoProvider ),
   _eventAggregator( eventAggregator ),
   _frameRateProvider( frameRateProvider ),
   _animationProvider( animationProvider ),
   _viewportQuadUnits( { 0, 0, 0, 0 } ),
   _viewportRectChars( { 0, 0, 0, 0 } ),
   _viewportOffsetChars( { 0, 0 } ),
   _playerViewportChars( { 0, 0 } ),
   _isAnimatingPitfall( false ),
   _pitfallAnimationElapsedSeconds( 0 )
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

   if ( _animationProvider->GetAnimation( ConsoleAnimationType::StageStarted )->IsRunning() )
   {
      DrawStageStartAnimation();
   }
   else if ( _animationProvider->GetAnimation( ConsoleAnimationType::PlayerThwipIn )->IsRunning() )
   {
      DrawPlayerThwipInAnimation();
   }
   else if ( _isAnimatingPitfall )
   {
      DrawPitfallAnimation();
   }
   else if ( _animationProvider->GetAnimation( ConsoleAnimationType::PlayerExploded )->IsRunning() )
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
   return _animationProvider->GetAnimation( ConsoleAnimationType::StageStarted )->IsRunning() ||
          _animationProvider->GetAnimation( ConsoleAnimationType::PlayerThwipIn )->IsRunning() ||
          _animationProvider->GetAnimation( ConsoleAnimationType::PlayerExploded)->IsRunning() ||
          _isAnimatingPitfall;
}

void PlayingStateConsoleRenderer::HandleStageStartedEvent()
{
   UpdateCaches();
   Coordinate<short> position = { _viewportOffsetChars.Left + _viewportRectChars.Width / 2,
                                  _viewportOffsetChars.Top + _viewportRectChars.Height / 2 };
   _animationProvider->GetAnimation( ConsoleAnimationType::StageStarted )->Start( position, nullopt );
}

void PlayingStateConsoleRenderer::HandlePitfallEvent()
{
   _isAnimatingPitfall = true;
   _pitfallAnimationElapsedSeconds = 0;
}

void PlayingStateConsoleRenderer::HandleTileDeathEvent()
{
   const auto& hitBox = _playerInfoProvider->GetHitBox();
   auto particleStartLeftChars = _playerViewportChars.Left + (short)( hitBox.Width / 2 / _renderConfig->ArenaCharWidth ) + _viewportOffsetChars.Left;
   auto particleStartTopChars = _playerViewportChars.Top + (short)( hitBox.Height / 2 / _renderConfig->ArenaCharHeight ) + _viewportOffsetChars.Top;
   Coordinate<short> startPosition = { (short)particleStartLeftChars, (short)particleStartTopChars };

   _animationProvider->GetAnimation( ConsoleAnimationType::PlayerExploded )->Start( startPosition, nullopt );
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

void PlayingStateConsoleRenderer::DrawStageStartAnimation()
{
   const auto& animation = _animationProvider->GetAnimation( ConsoleAnimationType::StageStarted );

   animation->Draw();
   animation->Tick();

   if ( !animation->IsRunning() )
   {
      Coordinate<short> thwipStartPosition =
      {
         _viewportOffsetChars.Left + _playerViewportChars.Left,
         _viewportOffsetChars.Top - _renderConfig->PlayerThwipInTransitionSprite->GetHeight()
      };
      Coordinate<short> thwipEndPosition =
      {
         thwipStartPosition.Left,
         _viewportOffsetChars.Top + _playerViewportChars.Top
      };

      _animationProvider->GetAnimation( ConsoleAnimationType::PlayerThwipIn )->Start( thwipStartPosition, thwipEndPosition );
   }
}

void PlayingStateConsoleRenderer::DrawPlayerThwipInAnimation()
{
   _animationProvider->GetAnimation( ConsoleAnimationType::PlayerThwipIn )->Draw();
   _animationProvider->GetAnimation( ConsoleAnimationType::PlayerThwipIn )->Tick();
}

void PlayingStateConsoleRenderer::DrawPitfallAnimation()
{
   _pitfallAnimationElapsedSeconds += _frameRateProvider->GetFrameScalar();

   if ( _pitfallAnimationElapsedSeconds >= _renderConfig->PitfallAnimationSeconds )
   {
      _isAnimatingPitfall = false;
   }
}

void PlayingStateConsoleRenderer::DrawPlayerExplosionAnimation()
{
   _animationProvider->GetAnimation( ConsoleAnimationType::PlayerExploded )->Draw();
   _animationProvider->GetAnimation( ConsoleAnimationType::PlayerExploded )->Tick();
}

void PlayingStateConsoleRenderer::DrawArenaSprites()
{
   auto arenaWidthChars = (short)( _arenaInfoProvider->GetWidth() / _renderConfig->ArenaCharWidth );

   for ( short y = 0; y < _viewportRectChars.Height; y++ )
   {
      for ( short x = 0; x < _viewportRectChars.Width; x++ )
      {
         auto tileIndex = ( ( _viewportRectChars.Top + y ) * arenaWidthChars ) + ( _viewportRectChars.Left + x );
         auto imageId = _renderConfig->ArenaTiles[tileIndex];

         if ( imageId != -1 )
         {
            _consoleBuffer->Draw( x + _viewportOffsetChars.Left, y + _viewportOffsetChars.Top, _renderConfig->ArenaImageMap[ imageId ] );
         }
      }
   }
}

void PlayingStateConsoleRenderer::DrawPlayer()
{
   auto sprite = GetPlayerSprite();
   _consoleBuffer->Draw( _playerViewportChars.Left + _viewportOffsetChars.Left, _playerViewportChars.Top + _viewportOffsetChars.Top, sprite );
   sprite->Tick();
}

void PlayingStateConsoleRenderer::DrawStatusBar()
{
   _consoleBuffer->Draw( _renderConfig->ArenaStatusBarLeftChars, _renderConfig->ArenaStatusBarTopChars, format( "Lives: {}", _playerInfoProvider->GetLivesRemaining() ) );
}

void PlayingStateConsoleRenderer::DrawPauseOverlay()
{
   auto left = ( _viewportRectChars.Width / 2 ) - ( _renderConfig->PauseOverlayImage.Width / 2 );
   auto top = ( _viewportRectChars.Height / 2 ) - ( _renderConfig->PauseOverlayImage.Height / 2 );

   _consoleBuffer->Draw( left + _viewportOffsetChars.Left, top + _viewportOffsetChars.Top, _renderConfig->PauseOverlayImage );
}

const shared_ptr<IConsoleSprite> PlayingStateConsoleRenderer::GetPlayerSprite() const
{
   auto direction = _playerInfoProvider->GetDirection();

   if ( _playerInfoProvider->IsStanding() )
   {
      return _playerInfoProvider->IsMoving() ? _renderConfig->PlayerWalkingSpriteMap[direction] : _renderConfig->PlayerStandingSpriteMap[direction];
   }
   else
   {
      return _renderConfig->PlayerFallingSpriteMap[direction];
   }
}

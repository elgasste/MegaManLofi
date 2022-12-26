#include <format>

#include "PlayingStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderDefs.h"
#include "IGameInfoProvider.h"
#include "IPlayerInfoProvider.h"
#include "IArenaInfoProvider.h"
#include "IGameEventAggregator.h"
#include "IFrameRateProvider.h"
#include "IConsoleAnimationProvider.h"
#include "IConsoleAnimation.h"
#include "IReadOnlyPlayer.h"
#include "IReadOnlyEntity.h"
#include "IReadOnlyArena.h"
#include "Direction.h"
#include "GameEvent.h"
#include "IConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

PlayingStateConsoleRenderer::PlayingStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                          const shared_ptr<ConsoleRenderDefs> renderDefs,
                                                          const shared_ptr<IGameInfoProvider> gameInfoProvider,
                                                          const shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                                          const shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                                          const shared_ptr<IGameEventAggregator> eventAggregator,
                                                          const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                          const shared_ptr<IConsoleAnimationProvider> animationProvider ) :
   _consoleBuffer( consoleBuffer ),
   _renderDefs( renderDefs ),
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
   _consoleBuffer->SetDefaultForegroundColor( _renderDefs->ArenaForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderDefs->ArenaBackgroundColor );

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
   const auto& hitBox = _playerInfoProvider->GetPlayerEntity()->GetHitBox();
   auto particleStartLeftChars = _playerViewportChars.Left + (short)( hitBox.Width / 2 / _renderDefs->ArenaCharWidth ) + _viewportOffsetChars.Left;
   auto particleStartTopChars = _playerViewportChars.Top + (short)( hitBox.Height / 2 / _renderDefs->ArenaCharHeight ) + _viewportOffsetChars.Top;
   Coordinate<short> startPosition = { (short)particleStartLeftChars, (short)particleStartTopChars };

   _animationProvider->GetAnimation( ConsoleAnimationType::PlayerExploded )->Start( startPosition, nullopt );
}

void PlayingStateConsoleRenderer::UpdateCaches()
{
   _arena = _arenaInfoProvider->GetArena();

   auto viewportWidthUnits = _renderDefs->ArenaViewportWidthChars * _renderDefs->ArenaCharWidth;
   auto viewportHeightUnits = _renderDefs->ArenaViewportHeightChars * _renderDefs->ArenaCharHeight;
   auto playerPosition = _playerInfoProvider->GetPlayerEntity()->GetArenaPosition();

   _viewportQuadUnits.Left = max( playerPosition.Left - ( viewportWidthUnits / 2 ), 0ll );
   _viewportQuadUnits.Top = max( playerPosition.Top - ( viewportHeightUnits / 2 ), 0ll );
   _viewportQuadUnits.Right = _viewportQuadUnits.Left + viewportWidthUnits;
   _viewportQuadUnits.Bottom = _viewportQuadUnits.Top + viewportHeightUnits;

   auto arenaWidth = _arena->GetWidth();
   if ( _viewportQuadUnits.Right > arenaWidth )
   {
      _viewportQuadUnits.Right = arenaWidth;
      _viewportQuadUnits.Left = max( 0ll, _viewportQuadUnits.Right - viewportWidthUnits );
   }

   auto arenaHeight = _arena->GetHeight();
   if ( _viewportQuadUnits.Bottom > arenaHeight )
   {
      _viewportQuadUnits.Bottom = arenaHeight;
      _viewportQuadUnits.Top = max( 0ll, _viewportQuadUnits.Bottom - viewportHeightUnits );
   }

   _viewportRectChars.Left = (short)( _viewportQuadUnits.Left / _renderDefs->ArenaCharWidth );
   _viewportRectChars.Top = (short)( _viewportQuadUnits.Top / _renderDefs->ArenaCharHeight );
   _viewportRectChars.Width = (short)( viewportWidthUnits / _renderDefs->ArenaCharWidth );
   _viewportRectChars.Height = (short)( viewportHeightUnits / _renderDefs->ArenaCharHeight );

   // this accounts for the case where the arena is smaller than the viewport
   _viewportOffsetChars.Left = _renderDefs->ArenaViewportLeftChars + ( ( _renderDefs->ArenaViewportWidthChars - _viewportRectChars.Width ) / 2 );
   _viewportOffsetChars.Top = _renderDefs->ArenaViewportTopChars + ( ( _renderDefs->ArenaViewportHeightChars - _viewportRectChars.Height ) / 2 );

   _playerViewportChars.Left = (short)( ( playerPosition.Left - _viewportQuadUnits.Left ) / _renderDefs->ArenaCharWidth );
   _playerViewportChars.Top = (short)( ( playerPosition.Top - _viewportQuadUnits.Top ) / _renderDefs->ArenaCharHeight );
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
         _viewportOffsetChars.Top - _renderDefs->PlayerThwipInTransitionSprite->GetHeight()
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
   _pitfallAnimationElapsedSeconds += _frameRateProvider->GetFrameSeconds();

   if ( _pitfallAnimationElapsedSeconds >= _renderDefs->PitfallAnimationSeconds )
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
   auto arenaWidthChars = (short)( _arena->GetWidth() / _renderDefs->ArenaCharWidth );

   for ( short y = 0; y < _viewportRectChars.Height; y++ )
   {
      for ( short x = 0; x < _viewportRectChars.Width; x++ )
      {
         auto tileIndex = ( ( _viewportRectChars.Top + y ) * arenaWidthChars ) + ( _viewportRectChars.Left + x );
         auto imageId = _renderDefs->ArenaTiles[tileIndex];

         if ( imageId != -1 )
         {
            _consoleBuffer->Draw( x + _viewportOffsetChars.Left, y + _viewportOffsetChars.Top, _renderDefs->ArenaImageMap[ imageId ] );
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
   auto player = _playerInfoProvider->GetPlayer();
   _consoleBuffer->Draw( _renderDefs->ArenaStatusBarLeftChars, _renderDefs->ArenaStatusBarTopChars, format( "Lives: {}", player->GetLivesRemaining() ) );
}

void PlayingStateConsoleRenderer::DrawPauseOverlay()
{
   auto left = ( _viewportRectChars.Width / 2 ) - ( _renderDefs->PauseOverlayImage.Width / 2 );
   auto top = ( _viewportRectChars.Height / 2 ) - ( _renderDefs->PauseOverlayImage.Height / 2 );

   _consoleBuffer->Draw( left + _viewportOffsetChars.Left, top + _viewportOffsetChars.Top, _renderDefs->PauseOverlayImage );
}

const shared_ptr<IConsoleSprite> PlayingStateConsoleRenderer::GetPlayerSprite() const
{
   auto player = _playerInfoProvider->GetPlayer();
   auto playerEntity = _playerInfoProvider->GetPlayerEntity();
   auto direction = playerEntity->GetDirection();

   if ( player->IsStanding() )
   {
      return playerEntity->IsMoving() ? _renderDefs->PlayerWalkingSpriteMap[direction] : _renderDefs->PlayerStandingSpriteMap[direction];
   }
   else
   {
      return _renderDefs->PlayerFallingSpriteMap[direction];
   }
}

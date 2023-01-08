#include <format>

#include "PlayingStateConsoleRenderer.h"
#include "ConsoleBuffer.h"
#include "ConsoleRenderDefs.h"
#include "ConsoleSpriteDefs.h"
#include "IGameInfoProvider.h"
#include "IPlayerInfoProvider.h"
#include "IArenaInfoProvider.h"
#include "GameEventAggregator.h"
#include "IFrameRateProvider.h"
#include "IConsoleAnimationProvider.h"
#include "EntityConsoleSpriteRepository.h"
#include "IConsoleAnimation.h"
#include "ReadOnlyPlayer.h"
#include "ReadOnlyEntity.h"
#include "ReadOnlyArena.h"
#include "Direction.h"
#include "ConsoleSprite.h"
#include "EntityConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

PlayingStateConsoleRenderer::PlayingStateConsoleRenderer( const shared_ptr<ConsoleBuffer> consoleBuffer,
                                                          const shared_ptr<ConsoleRenderDefs> renderDefs,
                                                          const shared_ptr<IGameInfoProvider> gameInfoProvider,
                                                          const shared_ptr<IPlayerInfoProvider> playerInfoProvider,
                                                          const shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                                          const shared_ptr<GameEventAggregator> eventAggregator,
                                                          const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                          const shared_ptr<IConsoleAnimationProvider> animationProvider,
                                                          const shared_ptr<EntityConsoleSpriteRepository> spriteRepository ) :
   _consoleBuffer( consoleBuffer ),
   _renderDefs( renderDefs ),
   _gameInfoProvider( gameInfoProvider ),
   _playerInfoProvider( playerInfoProvider ),
   _arenaInfoProvider( arenaInfoProvider ),
   _eventAggregator( eventAggregator ),
   _frameRateProvider( frameRateProvider ),
   _animationProvider( animationProvider ),
   _spriteRepository( spriteRepository ),
   _viewportQuadUnits( { 0, 0, 0, 0 } ),
   _viewportRectChars( { 0, 0, 0, 0 } ),
   _viewportOffsetChars( { 0, 0 } ),
   _playerViewportChars( { 0, 0 } ),
   _isAnimatingPitfall( false ),
   _pitfallAnimationElapsedSeconds( 0 )
{
   eventAggregator->RegisterEventHandler( GameEvent::StageStarted, std::bind( &PlayingStateConsoleRenderer::HandleStageStartedEvent, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::Pitfall, std::bind( &PlayingStateConsoleRenderer::HandlePitfallEvent, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::TileDeath, std::bind( &PlayingStateConsoleRenderer::HandleCollisionDeathEvent, this ) );
   eventAggregator->RegisterEventHandler( GameEvent::CollisionDeath, std::bind( &PlayingStateConsoleRenderer::HandleCollisionDeathEvent, this ) );
}

void PlayingStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultForegroundColor( _renderDefs->ArenaForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderDefs->ArenaBackgroundColor );

   UpdateCaches();
   DrawStatusBar();
   DrawArenaTiles();

   if ( _animationProvider->GetAnimation( ConsoleAnimationType::StageStarted )->IsRunning() )
   {
      DrawStageStartAnimation();
   }
   else if ( _animationProvider->GetAnimation( ConsoleAnimationType::PlayerThwipIn )->IsRunning() )
   {
      DrawPlayerThwipInAnimation();
   }
   else if ( _gameInfoProvider->IsPaused() )
   {
      DrawPauseOverlay();
   }
   else
   {
      DrawNonPlayerEntities();

      if ( _isAnimatingPitfall )
      {
         DrawPitfallAnimation();
      }
      else if ( _animationProvider->GetAnimation( ConsoleAnimationType::PlayerExploded )->IsRunning() )
      {
         DrawPlayerExplosionAnimation();
      }
      else
      {
         DrawPlayer();
      }
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
   Coordinate<short> position = { _viewportOffsetChars.Left + ( _viewportRectChars.Width / 2 ),
                                  _viewportOffsetChars.Top + ( _viewportRectChars.Height / 2 ) };
   _animationProvider->GetAnimation( ConsoleAnimationType::StageStarted )->Start( position, nullopt );
}

void PlayingStateConsoleRenderer::HandlePitfallEvent()
{
   _isAnimatingPitfall = true;
   _pitfallAnimationElapsedSeconds = 0;
}

void PlayingStateConsoleRenderer::HandleCollisionDeathEvent()
{
   const auto& hitBox = _playerInfoProvider->GetPlayerEntity()->GetHitBox();
   auto particleStartLeftChars = _playerViewportChars.Left + (short)( hitBox.Width / 2 / _renderDefs->ArenaCharWidth ) + _viewportOffsetChars.Left;
   auto particleStartTopChars = _playerViewportChars.Top + (short)( hitBox.Height / 2 / _renderDefs->ArenaCharHeight ) + _viewportOffsetChars.Top;
   Coordinate<short> startPosition = { (short)particleStartLeftChars, (short)particleStartTopChars };

   _animationProvider->GetAnimation( ConsoleAnimationType::PlayerExploded )->Start( startPosition, nullopt );
}

void PlayingStateConsoleRenderer::UpdateCaches()
{
   _arena = _arenaInfoProvider->GetActiveArena();

   auto viewportWidthUnits = _renderDefs->ArenaViewportWidthChars * _renderDefs->ArenaCharWidth;
   auto viewportHeightUnits = _renderDefs->ArenaViewportHeightChars * _renderDefs->ArenaCharHeight;
   auto playerPosition = _playerInfoProvider->GetPlayerEntity()->GetArenaPosition();

   _viewportQuadUnits.Left = max( playerPosition.Left - ( viewportWidthUnits / 2 ), 0.0f );
   _viewportQuadUnits.Top = max( playerPosition.Top - ( viewportHeightUnits / 2 ), 0.0f );
   _viewportQuadUnits.Right = _viewportQuadUnits.Left + viewportWidthUnits;
   _viewportQuadUnits.Bottom = _viewportQuadUnits.Top + viewportHeightUnits;

   auto arenaWidth = _arena->GetWidth();
   if ( _viewportQuadUnits.Right > arenaWidth )
   {
      _viewportQuadUnits.Right = arenaWidth;
      _viewportQuadUnits.Left = max( 0.0f, _viewportQuadUnits.Right - viewportWidthUnits );
   }

   auto arenaHeight = _arena->GetHeight();
   if ( _viewportQuadUnits.Bottom > arenaHeight )
   {
      _viewportQuadUnits.Bottom = arenaHeight;
      _viewportQuadUnits.Top = max( 0.0f, _viewportQuadUnits.Bottom - viewportHeightUnits );
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
         _viewportOffsetChars.Top - _renderDefs->SpriteDefs->PlayerThwipInTransitionSprite->GetHeight()
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

void PlayingStateConsoleRenderer::DrawArenaTiles()
{
   auto arenaWidthChars = (short)( _arena->GetWidth() / _renderDefs->ArenaCharWidth );
   auto arenaId = _arenaInfoProvider->GetActiveArena()->GetArenaId();

   for ( short y = 0; y < _viewportRectChars.Height; y++ )
   {
      for ( short x = 0; x < _viewportRectChars.Width; x++ )
      {
         auto tileIndex = ( ( _viewportRectChars.Top + y ) * arenaWidthChars ) + ( _viewportRectChars.Left + x );
         auto imageId = _renderDefs->ArenaTileImageIdMap[arenaId][tileIndex];

         if ( imageId != -1 )
         {
            _consoleBuffer->Draw( x + _viewportOffsetChars.Left, y + _viewportOffsetChars.Top, _renderDefs->ArenaImageMap[ imageId ] );
         }
      }
   }
}

void PlayingStateConsoleRenderer::DrawPlayer()
{
   auto player = _playerInfoProvider->GetPlayerEntity();
   DrawEntity( player );
}

void PlayingStateConsoleRenderer::DrawNonPlayerEntities()
{
   for ( int i = 0; i < _arena->GetEntityCount(); i++ )
   {
      auto entity = _arena->GetEntity( i );
      if ( entity != _playerInfoProvider->GetPlayerEntity() )
      {
         DrawEntity( entity );
      }
   }
}

void PlayingStateConsoleRenderer::DrawEntity( const shared_ptr<ReadOnlyEntity> entity )
{
   auto sprite = _spriteRepository->GetSprite( entity->GetUniqueId() );
   auto left = (short)( ( entity->GetArenaPositionLeft() - _viewportQuadUnits.Left ) / _renderDefs->ArenaCharWidth ) + _viewportOffsetChars.Left;
   auto top = (short)( ( entity->GetArenaPositionTop() - _viewportQuadUnits.Top ) / _renderDefs->ArenaCharHeight ) + _viewportOffsetChars.Top;

   _consoleBuffer->Draw( left, top, sprite );
   sprite->Tick();
}

void PlayingStateConsoleRenderer::DrawStatusBar()
{
   auto player = _playerInfoProvider->GetPlayer();
   auto playerEntity = _playerInfoProvider->GetPlayerEntity();
   auto left = _renderDefs->ArenaStatusBarLeftChars + 2;
   auto top = _renderDefs->ArenaStatusBarTopChars;

   _consoleBuffer->Draw( left, top, format( "Lives:  {}", player->GetLivesRemaining() ) );

   top++;
   _consoleBuffer->Draw( left, top, "Health: " );
   left += 8;

   int totalHealthBars = 30;
   int healthyBars = totalHealthBars * ( playerEntity->GetHealth() / (float)playerEntity->GetMaxHealth() );
   int unHealthyBars = totalHealthBars - healthyBars;
   
   for ( int i = 0; i < healthyBars; i++ )
   {
      _consoleBuffer->Draw( left, top, '|', ConsoleColor::White );
      left++;
   }
   for ( int i = 0; i < unHealthyBars; i++ )
   {
      _consoleBuffer->Draw( left, top, '|', ConsoleColor::DarkGrey );
      left++;
   }
}

void PlayingStateConsoleRenderer::DrawPauseOverlay()
{
   auto left = ( _viewportRectChars.Width / 2 ) - ( _renderDefs->PauseOverlayImage.Width / 2 );
   auto top = ( _viewportRectChars.Height / 2 ) - ( _renderDefs->PauseOverlayImage.Height / 2 );

   _consoleBuffer->Draw( left + _viewportOffsetChars.Left, top + _viewportOffsetChars.Top, _renderDefs->PauseOverlayImage );
}

#include "ArenaPhysics.h"
#include "IFrameRateProvider.h"
#include "IFrameActionRegistry.h"
#include "IGameEventAggregator.h"
#include "ArenaDefs.h"
#include "IPlayer.h"
#include "IArena.h"
#include "FrameAction.h"
#include "GameEvent.h"
#include "BasicEntity.h"

using namespace std;
using namespace MegaManLofi;

ArenaPhysics::ArenaPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                            const shared_ptr<IFrameActionRegistry> frameActionRegistry,
                            const shared_ptr<IGameEventAggregator> eventAggregator,
                            const shared_ptr<ArenaDefs> arenaDefs ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _eventAggregator( eventAggregator ),
   _arenaDefs( arenaDefs ),
   _arena( nullptr )
{
}

void ArenaPhysics::AssignTo( const shared_ptr<IArena> arena )
{
   _entityOccupyingTileIndicesMap.clear();
   _arena = arena;

   for ( int i = 0; i < _arena->GetEntityCount(); i++ )
   {
      UpdateEntityOccupyingTileIndices( _arena->GetMutableEntity( i ) );
   }
}

void ArenaPhysics::Tick()
{
   MoveEntities();
   UpdateActiveRegion();

   if ( !DetectTileDeath() )
   {
      DetectPlayerStanding();
   }
}

void ArenaPhysics::UpdateEntityOccupyingTileIndices( const shared_ptr<IEntity> entity )
{
   const auto& hitBox = entity->GetHitBox();
   auto position = entity->GetArenaPosition();

   _entityOccupyingTileIndicesMap[entity] =
   {
      ( position.Left + hitBox.Left ) / _arena->GetTileWidth(),
      ( position.Top + hitBox.Top ) / _arena->GetTileHeight(),
      (long long)( ( position.Left + hitBox.Left + hitBox.Width ) / _arena->GetTileWidth() ),
      (long long)( ( position.Top + hitBox.Top + hitBox.Height ) / _arena->GetTileHeight() )
   };

   // when the entity is positioned exactly at the edge of the right or bottom tile,
   // these indices will be incorrect, and need to be decremented.
   if ( ( ( position.Left + hitBox.Left + hitBox.Width ) % _arena->GetTileWidth() ) == 0 )
   {
      _entityOccupyingTileIndicesMap[entity].Right--;
   }
   if ( ( ( position.Top + hitBox.Top + hitBox.Height ) % _arena->GetTileHeight() ) == 0 )
   {
      _entityOccupyingTileIndicesMap[entity].Bottom--;
   }
}

void ArenaPhysics::MoveEntities()
{
   for ( int i = 0; i < _arena->GetEntityCount(); i++ )
   {
      auto entity = _arena->GetMutableEntity( i );
      MoveEntity( entity );
      UpdateEntityOccupyingTileIndices( entity );
   }
}

void ArenaPhysics::MoveEntity( const shared_ptr<IEntity> entity )
{
   auto currentPositionLeft = entity->GetArenaPositionLeft();
   auto currentPositionTop = entity->GetArenaPositionTop();
   auto newPositionLeft = (long long)( currentPositionLeft + ( entity->GetVelocityX() * _frameRateProvider->GetFrameSeconds() ) );
   auto newPositionTop = (long long)( currentPositionTop + ( entity->GetVelocityY() * _frameRateProvider->GetFrameSeconds() ) );
   DetectEntityTileCollisionX( entity, newPositionLeft );
   DetectEntityTileCollisionY( entity, newPositionTop );

   entity->SetArenaPosition( { newPositionLeft, newPositionTop } );

   auto player = _arena->GetMutablePlayer();
   if ( entity == player )
   {
      if ( currentPositionLeft != newPositionLeft )
      {
         _frameActionRegistry->FlagAction( FrameAction::PlayerMovedHorizontal );
      }
      if ( currentPositionTop != newPositionTop )
      {
         _frameActionRegistry->FlagAction( FrameAction::PlayerMovedVertical );
      }
   }
}

void ArenaPhysics::DetectEntityTileCollisionX( const std::shared_ptr<IEntity> entity, long long& newPositionLeft )
{
   const auto& hitBox = entity->GetHitBox();
   auto currentPositionLeft = entity->GetArenaPositionLeft();
   const auto& occupyingTileIndices = _entityOccupyingTileIndicesMap[entity];

   // cycle from the top tile we're currently occupying to the bottom tile we're currently occupying
   for ( long long y = occupyingTileIndices.Top; y <= occupyingTileIndices.Bottom; y++ )
   {
      if ( newPositionLeft < currentPositionLeft ) // moving left
      {
         auto leftOccupyingTileLeftEdge = occupyingTileIndices.Left * _arena->GetTileWidth();

         if ( newPositionLeft < 0 )
         {
            // we've collided with the left edge of the arena
            newPositionLeft = 0;
            entity->StopX();
            break;
         }
         else if ( newPositionLeft < leftOccupyingTileLeftEdge )
         {
            // check if we're trying to enter a new tile to the left
            const auto& nextLeftTile = _arena->GetTile( ( y * _arena->GetHorizontalTiles() ) + ( occupyingTileIndices.Left - 1 ) );
            if ( !nextLeftTile.LeftPassable )
            {
               newPositionLeft = leftOccupyingTileLeftEdge;
               entity->StopX();
            }
         }
      }
      else // moving right
      {
         auto rightOccupyingTileRightEdge = ( occupyingTileIndices.Right + 1 ) * _arena->GetTileWidth();
         auto newPositionRight = newPositionLeft + hitBox.Width;
         auto arenaWidth = _arena->GetWidth();

         if ( newPositionRight > arenaWidth )
         {
            // we've collided with the right edge of the arena
            newPositionLeft = arenaWidth - hitBox.Width;
            entity->StopX();
            break;
         }
         else if ( newPositionRight > rightOccupyingTileRightEdge )
         {
            // check if we're trying to enter a new tile to the right
            const auto& nextRightTile = _arena->GetTile( ( y * _arena->GetHorizontalTiles() ) + ( occupyingTileIndices.Right + 1 ) );
            if ( !nextRightTile.RightPassable )
            {
               newPositionLeft = rightOccupyingTileRightEdge - hitBox.Width;
               entity->StopX();
            }
         }
      }
   }
}

void ArenaPhysics::DetectEntityTileCollisionY( const shared_ptr<IEntity> entity, long long& newPositionTop )
{
   const auto& hitBox = entity->GetHitBox();
   auto currentPositionTop = entity->GetArenaPositionTop();
   const auto& occupyingTileIndices = _entityOccupyingTileIndicesMap[entity];

   // cycle from the left tile we're currently occupying to the right tile we're currently occupying
   for ( long long x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
   {
      if ( newPositionTop < currentPositionTop ) // moving up
      {
         auto topOccupyingTileTopEdge = occupyingTileIndices.Top * _arena->GetTileHeight();

         if ( newPositionTop < 0 )
         {
            // we've collided with the top edge of the arena
            newPositionTop = 0;
            entity->StopY();
            break;
         }
         else if ( newPositionTop < topOccupyingTileTopEdge )
         {
            // check if we're trying to enter a new tile upward
            const auto& nextTileUp = _arena->GetTile( ( ( occupyingTileIndices.Top - 1 ) * _arena->GetHorizontalTiles() ) + x );
            if ( !nextTileUp.UpPassable )
            {
               newPositionTop = topOccupyingTileTopEdge;
               entity->StopY();
            }
         }
      }
      else // moving down
      {
         auto bottomOccupyingTileBottomEdge = ( occupyingTileIndices.Bottom + 1 ) * _arena->GetTileHeight();
         auto newPositionBottom = newPositionTop + hitBox.Height;
         auto arenaHeight = _arena->GetHeight();

         if ( ( newPositionBottom ) > arenaHeight )
         {
            // we've collided with the bottom edge of the arena
            newPositionTop = arenaHeight - hitBox.Height;
            if ( entity == _arena->GetMutablePlayer() )
            {
               _eventAggregator->RaiseEvent( GameEvent::Pitfall );
            }
            break;
         }
         else if ( newPositionBottom > bottomOccupyingTileBottomEdge )
         {
            // check if we're trying to enter a new tile downward
            const auto& nextTileDown = _arena->GetTile( ( ( occupyingTileIndices.Bottom + 1 ) * _arena->GetHorizontalTiles() ) + x );
            if ( !nextTileDown.DownPassable )
            {
               newPositionTop = bottomOccupyingTileBottomEdge - hitBox.Height;
               entity->StopY();
            }
         }
      }
   }
}

void ArenaPhysics::UpdateActiveRegion()
{
   const auto& playerPosition = _arena->GetMutablePlayer()->GetArenaPosition();
   auto regionLeft = playerPosition.Left - ( _arenaDefs->ActiveRegionWidth / 2 );
   auto regionTop = playerPosition.Top - ( _arenaDefs->ActiveRegionHeight / 2 );

   _arena->SetActiveRegion( { regionLeft, regionTop, regionLeft + _arenaDefs->ActiveRegionWidth, regionTop + _arenaDefs->ActiveRegionHeight } );
}

bool ArenaPhysics::DetectTileDeath() const
{
   auto player = _arena->GetMutablePlayer();
   const auto& occupyingTileIndices = _entityOccupyingTileIndicesMap.at( player );

   for ( long long x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
   {
      for ( long long y = occupyingTileIndices.Top; y <= occupyingTileIndices.Bottom; y++ )
      {
         auto tile = _arena->GetTile( ( y * _arena->GetHorizontalTiles() ) + x );

         if ( tile.CausesDeath )
         {
            _eventAggregator->RaiseEvent( GameEvent::TileDeath );
            return true;
         }
      }
   }

   return false;
}

void ArenaPhysics::DetectPlayerStanding()
{
   auto player = _arena->GetMutablePlayer();
   const auto& hitBox = player->GetHitBox();
   auto positionTop = player->GetArenaPositionTop();
   auto hitBoxBottom = positionTop + hitBox.Top + hitBox.Height;

   player->SetIsStanding( false );

   if ( hitBoxBottom >= _arena->GetHeight() )
   {
      // TODO: what's this? I don't think it's possible to get into this state,
      // but wouldn't it mean a pitfall?
      player->SetIsStanding( true );
      return;
   }
   else if ( ( hitBoxBottom % _arena->GetTileHeight() ) != 0 )
   {
      return;
   }

   const auto& occupyingTileIndices = _entityOccupyingTileIndicesMap[player];
   for ( long long x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
   {
      const auto& nextTileDown = _arena->GetTile( ( ( occupyingTileIndices.Bottom + 1 ) * _arena->GetHorizontalTiles() ) + x );

      if ( !nextTileDown.DownPassable )
      {
         player->SetIsStanding( true );
         return;
      }
   }
}

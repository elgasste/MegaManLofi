#include "ArenaPhysics.h"
#include "IFrameRateProvider.h"
#include "IGameEventAggregator.h"
#include "ArenaDefs.h"
#include "IPlayer.h"
#include "IArena.h"
#include "FrameAction.h"
#include "BasicEntity.h"

using namespace std;
using namespace MegaManLofi;

ArenaPhysics::ArenaPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                            const shared_ptr<IGameEventAggregator> eventAggregator,
                            const shared_ptr<ArenaDefs> arenaDefs ) :
   _frameRateProvider( frameRateProvider ),
   _eventAggregator( eventAggregator ),
   _arenaDefs( arenaDefs ),
   _arena( nullptr )
{
}

void ArenaPhysics::AssignTo( const shared_ptr<IArena> arena )
{
   _entityTileIndicesCache.clear();
   _arena = arena;

   for ( int i = 0; i < _arena->GetEntityCount(); i++ )
   {
      UpdateEntityTileIndicesCache( _arena->GetMutableEntity( i ) );
   }
}

void ArenaPhysics::Tick()
{
   MoveEntities();
   UpdateActiveRegion();
   DetectTileDeath();
}

void ArenaPhysics::UpdateEntityTileIndicesCache( const shared_ptr<IEntity> entity )
{
   const auto& hitBox = entity->GetHitBox();
   auto position = entity->GetArenaPosition();

   _entityTileIndicesCache[entity] =
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
      _entityTileIndicesCache[entity].Right--;
   }
   if ( ( ( position.Top + hitBox.Top + hitBox.Height ) % _arena->GetTileHeight() ) == 0 )
   {
      _entityTileIndicesCache[entity].Bottom--;
   }
}

void ArenaPhysics::MoveEntities()
{
   for ( int i = 0; i < _arena->GetEntityCount(); i++ )
   {
      auto entity = _arena->GetMutableEntity( i );
      MoveEntity( entity );
      UpdateEntityTileIndicesCache( entity );
      DetectEntityMovementType( entity );
   }
}

void ArenaPhysics::MoveEntity( const shared_ptr<IEntity> entity )
{
   auto newPositionLeft = (long long)( entity->GetArenaPositionLeft() + ( entity->GetVelocityX() * _frameRateProvider->GetFrameSeconds() ) );
   auto newPositionTop = (long long)( entity->GetArenaPositionTop() + ( entity->GetVelocityY() * _frameRateProvider->GetFrameSeconds() ) );
   DetectEntityTileCollisionX( entity, newPositionLeft );
   DetectEntityTileCollisionY( entity, newPositionTop );

   entity->SetArenaPosition( { newPositionLeft, newPositionTop } );
}

void ArenaPhysics::DetectEntityTileCollisionX( const std::shared_ptr<IEntity> entity, long long& newPositionLeft )
{
   const auto& hitBox = entity->GetHitBox();
   auto currentPositionLeft = entity->GetArenaPositionLeft();
   const auto& occupyingTileIndices = _entityTileIndicesCache[entity];

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
            HandleEntityEnvironmentCollision( entity );
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
               HandleEntityEnvironmentCollision( entity );
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
            HandleEntityEnvironmentCollision( entity );
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
               HandleEntityEnvironmentCollision( entity );
            }
         }
      }
   }
}

void ArenaPhysics::DetectEntityTileCollisionY( const shared_ptr<IEntity> entity, long long& newPositionTop )
{
   const auto& hitBox = entity->GetHitBox();
   auto currentPositionTop = entity->GetArenaPositionTop();
   const auto& occupyingTileIndices = _entityTileIndicesCache[entity];

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
            HandleEntityEnvironmentCollision( entity );
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
               HandleEntityEnvironmentCollision( entity );
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
            HandleEntityEnvironmentCollision( entity );
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
               HandleEntityEnvironmentCollision( entity );
            }
         }
      }
   }
}

void ArenaPhysics::HandleEntityEnvironmentCollision( const shared_ptr<IEntity> entity )
{
   if ( entity->GetEntityType() == EntityType::Projectile )
   {
      _entityTileIndicesCache.erase( entity );
      _arena->RemoveEntity( entity );
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
   const auto& occupyingTileIndices = _entityTileIndicesCache.at( player );

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

void ArenaPhysics::DetectEntityMovementType( const shared_ptr<IEntity> entity ) const
{
   const auto& hitBox = entity->GetHitBox();
   auto positionTop = entity->GetArenaPositionTop();
   auto hitBoxBottom = positionTop + hitBox.Top + hitBox.Height;

   bool isOnGround = false;
   const auto& occupyingTileIndices = _entityTileIndicesCache.at( entity );

   for ( long long x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
   {
      auto nextTileDownIndex = occupyingTileIndices.Bottom + 1;

      if ( nextTileDownIndex >= _arena->GetVerticalTiles() )
      {
         break;
      }

      const auto& nextTileDown = _arena->GetTile( ( nextTileDownIndex * _arena->GetHorizontalTiles() ) + x );

      if ( !nextTileDown.DownPassable )
      {
         isOnGround = true;
         break;
      }
   }

   if ( isOnGround )
   {
      auto type = ( entity->GetVelocityX() == 0 ) ? MovementType::Standing : MovementType::Walking;
      entity->SetMovementType( type );
   }
   else
   {
      entity->SetMovementType( MovementType::Airborne );
   }
}

#include "ArenaPhysics.h"
#include "IFrameRateProvider.h"
#include "GameEventAggregator.h"
#include "ArenaDefs.h"
#include "WorldDefs.h"
#include "Arena.h"
#include "Entity.h"
#include "FrameAction.h"

using namespace std;
using namespace MegaManLofi;

ArenaPhysics::ArenaPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                            const shared_ptr<GameEventAggregator> eventAggregator,
                            const shared_ptr<WorldDefs> worldDefs ) :
   _frameRateProvider( frameRateProvider ),
   _eventAggregator( eventAggregator ),
   _worldDefs( worldDefs ),
   _arena( nullptr )
{
}

void ArenaPhysics::AssignTo( const shared_ptr<Arena> arena )
{
   _entityTileIndicesCache.clear();
   _arena = arena;
   UpdateEntityTileIndicesCaches();
}

void ArenaPhysics::Reset()
{
   _entityTileIndicesCache.clear();

   if ( _arena )
   {
      UpdateEntityTileIndicesCaches();
   }
}

void ArenaPhysics::Tick()
{
   MoveEntities();
   UpdateActiveRegion();
   DetectTileDeath();
}

void ArenaPhysics::UpdateEntityTileIndicesCaches()
{
   for ( int i = 0; i < _arena->GetEntityCount(); i++ )
   {
      UpdateEntityTileIndicesCache( _arena->GetEntity( i ) );
   }
}

void ArenaPhysics::UpdateEntityTileIndicesCache( const shared_ptr<ReadOnlyEntity> entity )
{
   const auto& hitBox = entity->GetHitBox();
   auto position = entity->GetArenaPosition();

   _entityTileIndicesCache[entity] =
   {
      (int)( ( position.Left + hitBox.Left ) / _arena->GetTileWidth() ),
      (int)( ( position.Top + hitBox.Top ) / _arena->GetTileHeight() ),
      (int)( ( position.Left + hitBox.Left + hitBox.Width ) / _arena->GetTileWidth() ),
      (int)( ( position.Top + hitBox.Top + hitBox.Height ) / _arena->GetTileHeight() )
   };

   // when the entity is positioned exactly at the edge of the right or bottom tile,
   // these indices will be incorrect, and need to be decremented.
   if ( fmod( position.Left + hitBox.Left + hitBox.Width, _arena->GetTileWidth() ) == 0 )
   {
      _entityTileIndicesCache[entity].Right--;
   }
   if ( fmod( position.Top + hitBox.Top + hitBox.Height, _arena->GetTileHeight() ) == 0 )
   {
      _entityTileIndicesCache[entity].Bottom--;
   }
}

void ArenaPhysics::MoveEntities()
{
   bool crossedPortal = false;

   for ( int i = 0; i < _arena->GetEntityCount(); i++ )
   {
      auto entity = static_pointer_cast<Entity>( _arena->GetEntity( i ) );
      MoveEntity( entity, crossedPortal );

      if ( crossedPortal )
      {
         break;
      }

      UpdateEntityTileIndicesCache( entity );
      DetectEntityMovementType( entity );
   }
}

void ArenaPhysics::MoveEntity( const shared_ptr<Entity> entity, bool& crossedPortal )
{
   auto newPositionLeft = entity->GetArenaPositionLeft() + ( entity->GetVelocityX() * _frameRateProvider->GetFrameSeconds() );
   auto newPositionTop = entity->GetArenaPositionTop() + ( entity->GetVelocityY() * _frameRateProvider->GetFrameSeconds() );

   DetectEntityTileCollisionX( entity, newPositionLeft, crossedPortal );

   if ( !crossedPortal )
   {
      DetectEntityTileCollisionY( entity, newPositionTop, crossedPortal );
   }

   if ( !crossedPortal )
   {
      entity->SetArenaPosition( { newPositionLeft, newPositionTop } );
   }
}

void ArenaPhysics::DetectEntityTileCollisionX( const shared_ptr<Entity> entity, float& newPositionLeft, bool& crossedPortal )
{
   const auto& hitBox = entity->GetHitBox();
   auto currentPositionLeft = entity->GetArenaPositionLeft();
   const auto& occupyingTileIndices = _entityTileIndicesCache[entity];

   // cycle from the top tile we're currently occupying to the bottom tile we're currently occupying
   for ( auto y = occupyingTileIndices.Top; y <= occupyingTileIndices.Bottom; y++ )
   {
      if ( newPositionLeft < currentPositionLeft ) // moving left
      {
         auto leftOccupyingTileLeftEdge = occupyingTileIndices.Left * _arena->GetTileWidth();

         if ( newPositionLeft < 0 )
         {
            // we've collided with the left edge of the arena
            newPositionLeft = 0;
            if ( DetectPlayerCrossedPortal( Direction::Left, entity ) )
            {
               crossedPortal = true;
               return;
            }
            else
            {
               entity->StopX();
               HandleEntityEnvironmentCollision( entity );
               break;
            }
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
            if ( DetectPlayerCrossedPortal( Direction::Right, entity ) )
            {
               crossedPortal = true;
               return;
            }
            else
            {
               entity->StopX();
               HandleEntityEnvironmentCollision( entity );
               break;
            }
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

void ArenaPhysics::DetectEntityTileCollisionY( const shared_ptr<Entity> entity, float& newPositionTop, bool& crossedPortal )
{
   const auto& hitBox = entity->GetHitBox();
   auto currentPositionTop = entity->GetArenaPositionTop();
   const auto& occupyingTileIndices = _entityTileIndicesCache[entity];

   // cycle from the left tile we're currently occupying to the right tile we're currently occupying
   for ( auto x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
   {
      if ( newPositionTop < currentPositionTop ) // moving up
      {
         auto topOccupyingTileTopEdge = occupyingTileIndices.Top * _arena->GetTileHeight();

         if ( newPositionTop < 0 )
         {
            // we've collided with the top edge of the arena
            newPositionTop = 0;
            if ( DetectPlayerCrossedPortal( Direction::Up, entity ) )
            {
               crossedPortal = true;
               return;
            }
            else
            {
               entity->StopY();
               HandleEntityEnvironmentCollision( entity );
               break;
            }
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
            if ( DetectPlayerCrossedPortal( Direction::Down, entity ) )
            {
               crossedPortal = true;
               return;
            }
            else
            {
               if ( entity == _arena->GetPlayerEntity() )
               {
                  _eventAggregator->RaiseEvent( GameEvent::Pitfall );
               }
               HandleEntityEnvironmentCollision( entity );
               break;
            }
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

bool ArenaPhysics::DetectPlayerCrossedPortal( Direction direction, const shared_ptr<Entity> entity ) const
{
   if ( entity != _arena->GetPlayerEntity() )
   {
      return false;
   }

   // TODO:
   // - search the stage's portal map for portals that match the direction and "from" ID for this arena
   // - if found, check if the player is in range
   // - if they are, call arena->CrossPortal( portal ) or whatever
   // - return true

   return false;
}

void ArenaPhysics::HandleEntityEnvironmentCollision( const shared_ptr<Entity> entity )
{
   if ( entity->GetEntityType() == EntityType::Projectile )
   {
      _entityTileIndicesCache.erase( entity );
      _arena->RemoveEntity( entity );
   }
}

void ArenaPhysics::UpdateActiveRegion()
{
   const auto& playerPosition = _arena->GetPlayerEntity()->GetArenaPosition();
   auto regionLeft = playerPosition.Left - ( _worldDefs->ActiveRegionWidth / 2 );
   auto regionTop = playerPosition.Top - ( _worldDefs->ActiveRegionHeight / 2 );

   _arena->SetActiveRegion( { regionLeft, regionTop, regionLeft + _worldDefs->ActiveRegionWidth, regionTop + _worldDefs->ActiveRegionHeight } );
}

bool ArenaPhysics::DetectTileDeath() const
{
   auto player = _arena->GetPlayerEntity();
   const auto& occupyingTileIndices = _entityTileIndicesCache.at( player );

   for ( auto x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
   {
      for ( auto y = occupyingTileIndices.Top; y <= occupyingTileIndices.Bottom; y++ )
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

void ArenaPhysics::DetectEntityMovementType( const shared_ptr<Entity> entity ) const
{
   const auto& hitBox = entity->GetHitBox();
   auto positionTop = entity->GetArenaPositionTop();
   auto hitBoxBottom = positionTop + hitBox.Top + hitBox.Height;

   bool isOnGround = false;
   const auto& occupyingTileIndices = _entityTileIndicesCache.at( entity );

   for ( auto x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
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

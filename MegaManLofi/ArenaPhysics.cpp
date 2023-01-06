#include "ArenaPhysics.h"
#include "IFrameRateProvider.h"
#include "GameEventAggregator.h"
#include "ArenaDefs.h"
#include "WorldDefs.h"
#include "Stage.h"
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
   _stage( nullptr )
{
}

void ArenaPhysics::AssignTo( const shared_ptr<Stage> stage )
{
   Reset();
   _stage = stage;
   
}

void ArenaPhysics::Reset()
{
   _entityTileIndicesCache.clear();
}

void ArenaPhysics::Tick()
{
   UpdateEntityTileIndicesCaches();
   MoveEntities();
   UpdateActiveRegion();
   DetectTileDeath();
}

void ArenaPhysics::UpdateEntityTileIndicesCaches()
{
   auto arena = _stage->GetActiveArena();

   for ( int i = 0; i < arena->GetEntityCount(); i++ )
   {
      if ( arena->GetEntityCount() > 1 )
      {
         bool testing = true;
      }
      UpdateEntityTileIndicesCache( arena->GetEntity( i ) );
   }
}

void ArenaPhysics::UpdateEntityTileIndicesCache( const shared_ptr<ReadOnlyEntity> entity )
{
   const auto& hitBox = entity->GetHitBox();
   auto position = entity->GetArenaPosition();

   _entityTileIndicesCache[entity] =
   {
      (int)( ( position.Left + hitBox.Left ) / _worldDefs->TileWidth ),
      (int)( ( position.Top + hitBox.Top ) / _worldDefs->TileHeight ),
      (int)( ( position.Left + hitBox.Left + hitBox.Width ) / _worldDefs->TileWidth ),
      (int)( ( position.Top + hitBox.Top + hitBox.Height ) / _worldDefs->TileHeight )
   };

   // when the entity is positioned exactly at the edge of the right or bottom tile,
   // these indices will be incorrect, and need to be decremented.
   if ( fmod( position.Left + hitBox.Left + hitBox.Width, _worldDefs->TileWidth ) == 0 )
   {
      _entityTileIndicesCache[entity].Right--;
   }
   if ( fmod( position.Top + hitBox.Top + hitBox.Height, _worldDefs->TileHeight ) == 0 )
   {
      _entityTileIndicesCache[entity].Bottom--;
   }
}

void ArenaPhysics::MoveEntities()
{
   auto arena = _stage->GetMutableActiveArena();
   bool crossedPortal = false;

   for ( int i = 0; i < arena->GetEntityCount(); i++ )
   {
      auto entity = arena->GetMutableEntity( i );
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
   auto arena = _stage->GetActiveArena();

   // cycle from the top tile we're currently occupying to the bottom tile we're currently occupying
   for ( auto y = occupyingTileIndices.Top; y <= occupyingTileIndices.Bottom; y++ )
   {
      if ( newPositionLeft < currentPositionLeft ) // moving left
      {
         auto leftOccupyingTileLeftEdge = occupyingTileIndices.Left * _worldDefs->TileWidth;

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
            const auto& nextLeftTile = arena->GetTile( ( y * arena->GetHorizontalTiles() ) + ( occupyingTileIndices.Left - 1 ) );
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
         auto rightOccupyingTileRightEdge = ( occupyingTileIndices.Right + 1 ) * _worldDefs->TileWidth;
         auto newPositionRight = newPositionLeft + hitBox.Width;
         auto arenaWidth = arena->GetWidth();

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
            const auto& nextRightTile = arena->GetTile( ( y * arena->GetHorizontalTiles() ) + ( occupyingTileIndices.Right + 1 ) );
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
   auto arena = _stage->GetActiveArena();

   // cycle from the left tile we're currently occupying to the right tile we're currently occupying
   for ( auto x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
   {
      if ( newPositionTop < currentPositionTop ) // moving up
      {
         auto topOccupyingTileTopEdge = occupyingTileIndices.Top * _worldDefs->TileHeight;

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
            const auto& nextTileUp = arena->GetTile( ( ( occupyingTileIndices.Top - 1 ) * arena->GetHorizontalTiles() ) + x );
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
         auto bottomOccupyingTileBottomEdge = ( occupyingTileIndices.Bottom + 1 ) * _worldDefs->TileHeight;
         auto newPositionBottom = newPositionTop + hitBox.Height;
         auto arenaHeight = arena->GetHeight();

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
               if ( entity == arena->GetPlayerEntity() )
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
            const auto& nextTileDown = arena->GetTile( ( ( occupyingTileIndices.Bottom + 1 ) * arena->GetHorizontalTiles() ) + x );
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

bool ArenaPhysics::DetectPlayerCrossedPortal( Direction direction, const shared_ptr<Entity> entity )
{
   auto arena = _stage->GetActiveArena();
   if ( entity != arena->GetPlayerEntity() )
   {
      return false;
   }

   auto portal = _stage->GetArenaPortal( direction, arena->GetArenaId() );
   if ( !portal )
   {
      return false;
   }

   if ( direction == Direction::Left || direction == Direction::Right )
   {
      auto topPosition = entity->GetArenaPositionTop();
      auto bottomPosition = topPosition + entity->GetHitBox().Height;
      auto portalTop = portal->FromTileOffset * _worldDefs->TileHeight;
      auto portalBottom = portalTop + ( portal->TileRange * _worldDefs->TileHeight );
      if ( ( topPosition >= portalTop && topPosition <= portalBottom ) || ( bottomPosition >= portalTop && bottomPosition <= portalBottom ) )
      {
         _stage->SetActiveArena( portal->ToArenaId );
         auto newLeftPosition = direction == Direction::Right ? 0 : _stage->GetActiveArena()->GetWidth() - entity->GetHitBox().Width;
         auto newTopPosition = ( portal->ToTileOffset * _worldDefs->TileHeight) + ( topPosition - portalTop );
         entity->SetArenaPosition( { newLeftPosition, newTopPosition } );
         UpdateEntityTileIndicesCaches();
         UpdateActiveRegion();
         return true;
      }
   }
   else
   {
      auto leftPosition = entity->GetArenaPositionLeft();
      auto rightPosition = leftPosition + entity->GetHitBox().Width;
      auto portalLeft = portal->FromTileOffset * _worldDefs->TileWidth;
      auto portalRight = portalLeft + ( portal->TileRange * _worldDefs->TileWidth );
      if ( ( leftPosition >= portalLeft && leftPosition <= portalRight ) || ( rightPosition >= portalLeft && rightPosition <= portalRight ) )
      {
         _stage->SetActiveArena( portal->ToArenaId );
         auto newLeftPosition = ( portal->ToTileOffset * _worldDefs->TileWidth) + ( leftPosition - portalLeft );
         auto newTopPosition = direction == Direction::Down ? 0 : _stage->GetActiveArena()->GetHeight() - entity->GetHitBox().Height;
         entity->SetArenaPosition( { newLeftPosition, newTopPosition } );
         UpdateEntityTileIndicesCaches();
         UpdateActiveRegion();
         return true;
      }
   }

   return false;
}

void ArenaPhysics::HandleEntityEnvironmentCollision( const shared_ptr<Entity> entity )
{
   if ( entity->GetEntityType() == EntityType::Projectile )
   {
      _entityTileIndicesCache.erase( entity );
      _stage->GetMutableActiveArena()->RemoveEntity( entity );
   }
}

void ArenaPhysics::UpdateActiveRegion()
{
   auto arena = _stage->GetMutableActiveArena();
   const auto& playerPosition = arena->GetPlayerEntity()->GetArenaPosition();
   auto regionLeft = max( 0.0f, playerPosition.Left - ( _worldDefs->ActiveRegionWidth / 2 ) );
   auto regionTop = max( 0.0f, playerPosition.Top - ( _worldDefs->ActiveRegionHeight / 2 ) );
   auto regionRight = regionLeft + _worldDefs->ActiveRegionWidth;
   auto regionBottom = regionTop + _worldDefs->ActiveRegionHeight;

   if ( regionRight > arena->GetWidth() )
   {
      regionLeft = max( 0.0f, regionLeft - ( regionRight - arena->GetWidth() ) );
      regionRight = arena->GetWidth();
   }
   if ( regionBottom > arena->GetHeight() )
   {
      regionTop = max( 0.0f, regionTop - ( regionBottom - arena->GetHeight() ) );
      regionBottom = arena->GetHeight();
   }

   arena->SetActiveRegion( { regionLeft, regionTop, regionRight - regionLeft, regionBottom - regionTop } );
}

bool ArenaPhysics::DetectTileDeath() const
{
   auto arena = _stage->GetActiveArena();
   auto player = arena->GetPlayerEntity();
   const auto& occupyingTileIndices = _entityTileIndicesCache.at( player );

   for ( auto x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
   {
      for ( auto y = occupyingTileIndices.Top; y <= occupyingTileIndices.Bottom; y++ )
      {
         auto tile = arena->GetTile( ( y * arena->GetHorizontalTiles() ) + x );

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
   auto arena = _stage->GetActiveArena();

   bool isOnGround = false;
   const auto& occupyingTileIndices = _entityTileIndicesCache.at( entity );

   for ( auto x = occupyingTileIndices.Left; x <= occupyingTileIndices.Right; x++ )
   {
      auto nextTileDownIndex = occupyingTileIndices.Bottom + 1;

      if ( nextTileDownIndex >= arena->GetVerticalTiles() )
      {
         break;
      }

      const auto& nextTileDown = arena->GetTile( ( nextTileDownIndex * arena->GetHorizontalTiles() ) + x );

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

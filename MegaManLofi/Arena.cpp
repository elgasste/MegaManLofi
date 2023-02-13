#include <algorithm>

#include "Arena.h"
#include "ArenaDefs.h"
#include "WorldDefs.h"
#include "EntityDefs.h"
#include "GameEventAggregator.h"
#include "IFrameRateProvider.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "RectangleUtilities.h"

using namespace std;
using namespace MegaManLofi;

Arena::Arena( const shared_ptr<ArenaDefs> arenaDefs,
              const shared_ptr<WorldDefs> worldDefs,
              const shared_ptr<EntityDefs> entityDefs,
              const shared_ptr<GameEventAggregator> eventAggregator,
              const shared_ptr<IFrameRateProvider> frameRateProvider,
              const shared_ptr<EntityFactory> entityFactory ) :
   _arenaDefs( arenaDefs ),
   _entityDefs( entityDefs ),
   _eventAggregator( eventAggregator ),
   _frameRateProvider( frameRateProvider ),
   _entityFactory( entityFactory )
{
   _arenaId = arenaDefs->ArenaId;
   _tiles = arenaDefs->Tiles;
   _width = worldDefs->TileWidth * arenaDefs->HorizontalTiles;
   _height = worldDefs->TileHeight * arenaDefs->VerticalTiles;
   _horizontalTiles = arenaDefs->HorizontalTiles;
   _verticalTiles = arenaDefs->VerticalTiles;

   Reload();
}

void Arena::Reload()
{
   _entities.clear();
   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitiesCleared );
   _playerEntity = nullptr;

   _spawnPoints.clear();
   for ( const auto& spawnPoint : _arenaDefs->SpawnPoints )
   {
      _spawnPoints.push_back( shared_ptr<SpawnPoint>( new SpawnPoint( spawnPoint ) ) );
   }
}

void Arena::Reset()
{
   _entities.clear();
   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitiesCleared );
   _playerEntity = nullptr;

   for ( auto& spawnPoint : _spawnPoints )
   {
      spawnPoint->IsActive = false;
   }
}

void Arena::SetPlayerEntity( const shared_ptr<Entity> playerEntity )
{
   _playerEntity = playerEntity;
   _playerEntity->SetArenaPosition( _arenaDefs->PlayerStartPosition );
   AddEntity( playerEntity );
}

void Arena::AddEntity( const std::shared_ptr<Entity> entity )
{
   if ( find( _entities.begin(), _entities.end(), entity ) == _entities.end() )
   {
      _entities.push_back( entity );
      _eventAggregator->RaiseEvent( GameEvent::ArenaEntitySpawned );
   }
}

void Arena::RemoveEntity( const std::shared_ptr<Entity> entity )
{
   for ( auto it = _entities.begin(); it != _entities.end(); it++ )
   {
      if ( *it == entity )
      {
         _entities.erase( it );
         _eventAggregator->RaiseEvent( GameEvent::ArenaEntityDeSpawned );
         break;
      }
   }
}

void Arena::CheckSpawnPoints()
{
   for ( auto& spawnPoint : _spawnPoints )
   {
      auto spawnPosition = spawnPoint->ArenaPosition;

      if ( RectangleUtilities::CoordinateInRectangleF( spawnPosition, 0, 0, _activeRegion, 0, 0 ) )
      {
         if ( spawnPoint->IsActive )
         {
            if ( spawnPoint->ReSpawnsAtInterval )
            {
               spawnPoint->IntervalElapsedSeconds += _frameRateProvider->GetFrameSeconds();

               if ( spawnPoint->IntervalElapsedSeconds > spawnPoint->ReSpawnIntervalSeconds )
               {
                  spawnPoint->IntervalElapsedSeconds = 0;
                  auto entity = _entityFactory->CreateEntity( spawnPoint->EntityMetaId, spawnPoint->ArenaPosition, spawnPoint->Direction );
                  if ( spawnPoint->IsBoundToUniqueId )
                  {
                     spawnPoint->UniqueIdBinding = entity->GetUniqueId();
                  }
                  AddEntity( entity );
               }
            }
         }
         else
         {
            spawnPoint->IsActive = true;
            spawnPoint->IntervalElapsedSeconds = 0;

            if ( spawnPoint->IsDecommissioned )
            {
               continue;
            }

            bool shouldSpawn = true;
            if ( spawnPoint->IsBoundToUniqueId )
            {
               for ( auto entity : _entities )
               {
                  if ( entity->GetUniqueId() == spawnPoint->UniqueIdBinding )
                  {
                     shouldSpawn = false;
                     break;
                  }
               }
            }

            if ( shouldSpawn )
            {
               auto entity = _entityFactory->CreateEntity( spawnPoint->EntityMetaId, spawnPoint->ArenaPosition, spawnPoint->Direction );
               if ( spawnPoint->IsBoundToUniqueId )
               {
                  spawnPoint->UniqueIdBinding = entity->GetUniqueId();
               }
               AddEntity( entity );
            }
         }
      }
      else
      {
         spawnPoint->IsActive = false;
      }
   }
}

void Arena::DeSpawnInactiveEntities()
{
   vector<shared_ptr<Entity>> entitiesToDeSpawn;

   for ( auto entity : _entities )
   {
      bool isOutsideDespawnRegion = !RectangleUtilities::RectanglesIntersectF( entity->GetHitBox(),
                                                                               entity->GetArenaPositionLeft(),
                                                                               entity->GetArenaPositionTop(),
                                                                               _deSpawnRegion,
                                                                               0,
                                                                               0 );

      if ( entity != _playerEntity && ( isOutsideDespawnRegion || entity->GetHealth() == 0 ) )
      {
         entitiesToDeSpawn.push_back( entity );
      }
   }

   for ( auto entity : entitiesToDeSpawn )
   {
      RemoveEntity( entity );

      if ( entity->GetHealth() == 0 )
      {
         for ( auto spawnPoint : _spawnPoints )
         {
            if ( spawnPoint->UniqueIdBinding == entity->GetUniqueId() && !spawnPoint->AllowUniqueReSpawn )
            {
               spawnPoint->IsDecommissioned = true;
            }
         }
      }
   }
}

void Arena::DetectEntityCollisions()
{
   for ( int i = 0; i < (int)_entities.size(); i++ )
   {
      auto entity1 = _entities[i];

      for ( int j = i + 1; j < (int)_entities.size(); j++ )
      {
         auto entity2 = _entities[j];

         if ( RectangleUtilities::RectanglesIntersectF( entity1->GetHitBox(), entity1->GetArenaPositionLeft(), entity1->GetArenaPositionTop(),
                                                        entity2->GetHitBox(), entity2->GetArenaPositionLeft(), entity2->GetArenaPositionTop() ) )
         {
            auto otherType = entity2->GetEntityType();

            switch ( entity1->GetEntityType() )
            {
               case EntityType::Player:
                  if ( otherType == EntityType::Item )
                  {
                     PlayerPickUpItem( entity1, entity2 );
                  }
                  else if ( otherType == EntityType::Enemy || otherType == EntityType::Projectile )
                  {
                     EntityTakeHealthPayload( entity1, entity2 );
                  }
                  break;
               case EntityType::Item:
                  if ( otherType == EntityType::Player )
                  {
                     PlayerPickUpItem( entity2, entity1 );
                  }
                  break;
               case EntityType::Projectile:
                  if ( otherType == EntityType::Player || otherType == EntityType::Enemy )
                  {
                     EntityTakeHealthPayload( entity2, entity1 );
                  }
                  break;
               case EntityType::Enemy:
                  if ( otherType == EntityType::Player )
                  {
                     EntityTakeHealthPayload( entity2, entity1 );
                  }
                  else if ( otherType == EntityType::Projectile )
                  {
                     EntityTakeHealthPayload( entity1, entity2 );
                  }
                  break;
            }
         }
      }
   }
}

void Arena::PlayerPickUpItem( const shared_ptr<Entity> player, const shared_ptr<Entity> item )
{
   const auto& payload = _entityDefs->CollisionPayloadMap[item->GetEntityMetaId()];

   if ( player->TakeCollisionPayload( payload ) )
   {
      for ( auto& spawnPoint : _spawnPoints )
      {
         if ( spawnPoint->IsBoundToUniqueId && spawnPoint->UniqueIdBinding == item->GetUniqueId() && !spawnPoint->AllowUniqueReSpawn )
         {
            spawnPoint->IsDecommissioned = true;
         }
      }

      RemoveEntity( item );
   }
}

void Arena::EntityTakeHealthPayload( const shared_ptr<Entity> taker, const shared_ptr<Entity> giver )
{
   if ( giver->GetEntityType() == EntityType::Projectile )
   {
      auto isFriendly = _entityDefs->ProjectileInfoMap[giver->GetEntityMetaId()].IsFriendly;

      if ( taker->GetEntityType() == EntityType::Player ) // friendly projectiles only hurt enemies
      {
         if ( isFriendly )
         {
            return;
         }
      }
      else // un-friendly projectiles only hurt the player
      {
         if ( !isFriendly )
         {
            return;
         }
      }
   }

   const auto& payload = _entityDefs->CollisionPayloadMap[giver->GetEntityMetaId()];
   taker->TakeCollisionPayload( payload );

   if ( giver->GetEntityType() == EntityType::Projectile )
   {
      RemoveEntity( giver );
   }
}

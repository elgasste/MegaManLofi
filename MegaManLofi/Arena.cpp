#include <algorithm>

#include "Arena.h"
#include "ArenaDefs.h"
#include "WorldDefs.h"
#include "GameEventAggregator.h"
#include "IFrameRateProvider.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "RectangleUtilities.h"

using namespace std;
using namespace MegaManLofi;

Arena::Arena( const shared_ptr<ArenaDefs> arenaDefs,
              const shared_ptr<WorldDefs> worldDefs,
              const shared_ptr<GameEventAggregator> eventAggregator,
              const shared_ptr<IFrameRateProvider> frameRateProvider,
              const shared_ptr<EntityFactory> entityFactory ) :
   _arenaDefs( arenaDefs ),
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
   for ( auto spawnPoint : _arenaDefs->SpawnPoints )
   {
      _spawnPoints.push_back( shared_ptr<SpawnPoint>( new SpawnPoint( spawnPoint ) ) );
   }
}

void Arena::Reset()
{
   _entities.clear();
   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitiesCleared );
   _playerEntity = nullptr;

   for ( auto spawnPoint : _spawnPoints )
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
   for ( auto spawnPoint : _spawnPoints )
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
                  auto entity = _entityFactory->CreateEntity( spawnPoint->EntityMetaId, spawnPoint->Direction );
                  entity->SetArenaPosition( spawnPoint->ArenaPosition );
                  AddEntity( entity );
               }
            }
         }
         else
         {
            spawnPoint->IsActive = true;
            spawnPoint->IntervalElapsedSeconds = 0;

            if ( !spawnPoint->IsDecommissioned )
            {
               auto entity = _entityFactory->CreateEntity( spawnPoint->EntityMetaId, spawnPoint->Direction );
               entity->SetArenaPosition( spawnPoint->ArenaPosition );
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
      if ( !RectangleUtilities::RectanglesIntersectF( entity->GetHitBox(), entity->GetArenaPositionLeft(), entity->GetArenaPositionTop(), _deSpawnRegion, 0, 0 ) &&
           entity != _playerEntity )
      {
         entitiesToDeSpawn.push_back( entity );
      }
   }

   for ( auto entity : entitiesToDeSpawn )
   {
      RemoveEntity( entity );
   }
}

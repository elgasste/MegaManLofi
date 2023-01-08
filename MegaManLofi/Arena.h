#pragma once

#include "ReadOnlyArena.h"

namespace MegaManLofi
{
   class ArenaDefs;
   class WorldDefs;
   class EntityDefs;
   class GameEventAggregator;
   class IFrameRateProvider;
   class EntityFactory;

   class Arena : public ReadOnlyArena
   {
   public:
      Arena() { }
      Arena( const std::shared_ptr<ArenaDefs> arenaDefs,
             const std::shared_ptr<WorldDefs> worldDefs,
             const std::shared_ptr<EntityDefs> entityDefs,
             const std::shared_ptr<GameEventAggregator> eventAggregator,
             const std::shared_ptr<IFrameRateProvider> frameRateProvider,
             const std::shared_ptr<EntityFactory> entityFactory );

      virtual void Reload();
      virtual void Reset();

      virtual const std::shared_ptr<Entity> GetMutableEntity( int index ) const { return _entities[index]; }
      virtual int GetSpawnPointCount() const { return (int)_spawnPoints.size(); }
      virtual const std::shared_ptr<SpawnPoint> GetSpawnPoint( int index ) const { return _spawnPoints[index]; }
      virtual void SetArenaId( int id ) { _arenaId = id; }
      virtual void SetPlayerEntity( const std::shared_ptr<Entity> playerEntity );
      virtual void SetActiveRegion( Rectangle<float> region ) { _activeRegion = region; }
      virtual void SetDeSpawnRegion( Rectangle<float> region ) { _deSpawnRegion = region; }
      virtual void AddEntity( const std::shared_ptr<Entity> entity );
      virtual void RemoveEntity( const std::shared_ptr<Entity> entity );
      virtual void CheckSpawnPoints();
      virtual void DeSpawnInactiveEntities();
      virtual void DetectEntityCollisions();

   private:
      void PlayerPickUpItem( const std::shared_ptr<Entity> player, const std::shared_ptr<Entity> item );
      void EntityTakeHealthPayload( const std::shared_ptr<Entity> taker, const std::shared_ptr<Entity> giver );

   private:
      const std::shared_ptr<ArenaDefs> _arenaDefs;
      const std::shared_ptr<EntityDefs> _entityDefs;
      const std::shared_ptr<GameEventAggregator> _eventAggregator;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<EntityFactory> _entityFactory;
   };
}

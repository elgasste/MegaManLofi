#pragma once

#include "ReadOnlyArena.h"

namespace MegaManLofi
{
   class ArenaDefs;
   class WorldDefs;
   class GameEventAggregator;
   class IFrameRateProvider;
   class EntityFactory;

   class Arena : public ReadOnlyArena
   {
   public:
      Arena() { }
      Arena( const std::shared_ptr<ArenaDefs> arenaDefs,
             const std::shared_ptr<WorldDefs> worldDefs,
             const std::shared_ptr<GameEventAggregator> eventAggregator,
             const std::shared_ptr<IFrameRateProvider> frameRateProvider,
             const std::shared_ptr<EntityFactory> entityFactory );

      virtual void Reset();
      virtual void Clear();

      virtual const std::shared_ptr<Entity> GetMutableEntity( int index ) const { return _entities[index]; }
      virtual void SetArenaId( int id ) { _arenaId = id; }
      virtual void SetPlayerEntity( const std::shared_ptr<Entity> playerEntity );
      virtual void SetActiveRegion( Rectangle<float> region ) { _activeRegion = region; }
      virtual void SetDeSpawnRegion( Rectangle<float> region ) { _deSpawnRegion = region; }
      virtual void AddEntity( const std::shared_ptr<Entity> entity );
      virtual void RemoveEntity( const std::shared_ptr<Entity> entity );
      virtual void CheckSpawnPoints();
      virtual void DeSpawnInactiveEntities();

   private:
      const std::shared_ptr<ArenaDefs> _arenaDefs;
      const std::shared_ptr<GameEventAggregator> _eventAggregator;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<EntityFactory> _entityFactory;
   };
}

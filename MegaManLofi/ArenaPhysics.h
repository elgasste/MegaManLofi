#pragma once

#include <memory>
#include <map>

#include "Coordinate.h"
#include "Quad.h"
#include "Direction.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class GameEventAggregator;
   class WorldDefs;
   class Stage;
   class Entity;
   class ReadOnlyEntity;

   class ArenaPhysics
   {
   public:
      ArenaPhysics() { }
      ArenaPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                    const std::shared_ptr<GameEventAggregator> eventAggregator,
                    const std::shared_ptr<WorldDefs> worldDefs );

      virtual void AssignTo( const std::shared_ptr<Stage> stage );
      virtual void Reset();
      virtual void Tick();

   private:
      void UpdateEntityTileIndicesCaches();
      void UpdateEntityTileIndicesCache( const std::shared_ptr<ReadOnlyEntity> entity );
      void MoveEntities();
      void MoveEntity( const std::shared_ptr<Entity> entity, bool& crossedPortal );
      void DetectEntityTileCollisionX( const std::shared_ptr<Entity> entity, float& newPositionLeft, bool& crossedPortal );
      void DetectEntityTileCollisionY( const std::shared_ptr<Entity> entity, float& newPositionTop, bool& crossedPortal );
      bool DetectPlayerCrossedPortal( Direction direction, const std::shared_ptr<Entity> entity );
      void HandleEntityEnvironmentCollision( const std::shared_ptr<Entity> entity );
      void DetectEntityMovementType( const std::shared_ptr<Entity> entity ) const;

      void UpdateRegions();
      bool DetectTileDeath() const;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<GameEventAggregator> _eventAggregator;
      const std::shared_ptr<WorldDefs> _worldDefs;

      std::shared_ptr<Stage> _stage;

      std::map<std::shared_ptr<ReadOnlyEntity>, Quad<int>> _entityTileIndicesCache;
   };
}

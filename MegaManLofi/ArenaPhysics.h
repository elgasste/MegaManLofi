#pragma once

#include <memory>
#include <map>

#include "Coordinate.h"
#include "Quad.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class GameEventAggregator;
   class WorldDefs;
   class Arena;
   class Entity;
   class ReadOnlyEntity;

   class ArenaPhysics
   {
   public:
      ArenaPhysics() { }
      ArenaPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                    const std::shared_ptr<GameEventAggregator> eventAggregator,
                    const std::shared_ptr<WorldDefs> worldDefs );

      virtual void AssignTo( const std::shared_ptr<Arena> arena );
      virtual void Reset();
      virtual void Tick();

   private:
      void UpdateEntityTileIndicesCaches();
      void UpdateEntityTileIndicesCache( const std::shared_ptr<ReadOnlyEntity> entity );
      void MoveEntities();
      void MoveEntity( const std::shared_ptr<Entity> entity );
      void DetectEntityTileCollisionX( const std::shared_ptr<Entity> entity, float& newPositionLeft );
      void DetectEntityTileCollisionY( const std::shared_ptr<Entity> entity, float& newPositionTop );
      void HandleEntityEnvironmentCollision( const std::shared_ptr<Entity> entity );
      void DetectEntityMovementType( const std::shared_ptr<Entity> entity ) const;

      void UpdateActiveRegion();
      bool DetectTileDeath() const;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<GameEventAggregator> _eventAggregator;
      const std::shared_ptr<WorldDefs> _worldDefs;

      std::shared_ptr<Arena> _arena;

      std::map<std::shared_ptr<ReadOnlyEntity>, Quad<int>> _entityTileIndicesCache;
   };
}

#pragma once

#include <map>

#include "IArenaPhysics.h"
#include "Coordinate.h"
#include "Quad.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IGameEventAggregator;
   class ArenaDefs;
   class Entity;
   class ReadOnlyEntity;

   class ArenaPhysics : public IArenaPhysics
   {
   public:
      ArenaPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                    const std::shared_ptr<IGameEventAggregator> eventAggregator,
                    const std::shared_ptr<ArenaDefs> arenaDefs );

      void AssignTo( const std::shared_ptr<Arena> arena ) override;
      void Tick() override;

   private:
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
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<ArenaDefs> _arenaDefs;

      std::shared_ptr<Arena> _arena;

      std::map<std::shared_ptr<ReadOnlyEntity>, Quad<int>> _entityTileIndicesCache;
   };
}

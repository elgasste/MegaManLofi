#pragma once

#include <map>

#include "IArenaPhysics.h"
#include "Coordinate.h"
#include "Quad.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IFrameActionRegistry;
   class IGameEventAggregator;
   class ArenaDefs;
   class IEntity;

   class ArenaPhysics : public IArenaPhysics
   {
   public:
      ArenaPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                    const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
                    const std::shared_ptr<IGameEventAggregator> eventAggregator,
                    const std::shared_ptr<ArenaDefs> arenaDefs );

      void AssignTo( const std::shared_ptr<IArena> arena ) override;
      void Tick() override;

   private:
      void UpdateEntityOccupyingTileIndices( const std::shared_ptr<IEntity> entity );
      void MoveEntities();
      void MoveEntity( const std::shared_ptr<IEntity> entity );
      void DetectEntityTileCollisionX( const std::shared_ptr<IEntity> entity, long long& newPositionLeft );
      void DetectEntityTileCollisionY( const std::shared_ptr<IEntity> entity, long long& newPositionTop );
      void HandleEntityEnvironmentCollision( const std::shared_ptr<IEntity> entity );

      void UpdateActiveRegion();
      bool DetectTileDeath() const;
      void DetectPlayerStanding();

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<ArenaDefs> _arenaDefs;

      std::shared_ptr<IArena> _arena;

      std::map<std::shared_ptr<IEntity>, Quad<long long>> _entityOccupyingTileIndicesMap;
      Quad<long long> _playerOccupyingTileIndices;
   };
}

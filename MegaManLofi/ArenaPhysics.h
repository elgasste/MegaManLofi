#pragma once

#include "IArenaPhysics.h"
#include "Quad.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IFrameActionRegistry;
   class IGameEventAggregator;
   class ArenaDefs;

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
      void MovePlayer();
      void UpdatePlayerOccupyingTileIndices();
      void MovePlayerX();
      void MovePlayerY();
      void DetectPlayerTileCollisionX( long long& newPositionLeft );
      void DetectPlayerTileCollisionY( long long& newPositionTop );
      bool DetectTileDeath() const;
      void UpdateActiveRegion();
      void DetectPlayerStanding();

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;
      const std::shared_ptr<ArenaDefs> _arenaDefs;

      std::shared_ptr<IArena> _arena;

      Quad<long long> _playerOccupyingTileIndices;
   };
}

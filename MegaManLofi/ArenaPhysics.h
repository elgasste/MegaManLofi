#pragma once

#include "IArenaPhysics.h"
#include "TileIndices.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IFrameActionRegistry;

   class ArenaPhysics : public IArenaPhysics
   {
   public:
      ArenaPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                    const std::shared_ptr<IFrameActionRegistry> frameActionRegistry );

      void AssignTo( const std::shared_ptr<IArena> arena,
                     const std::shared_ptr<IPlayer> player ) override;
      void Tick() override;

   private:
      void MovePlayer();
      void UpdatePlayerOccupyingTileIndices();
      void MovePlayerX();
      void MovePlayerY();
      void DetectPlayerTileCollisionX( long long& newPositionX );
      void DetectPlayerTileCollisionY( long long& newPositionY );
      void DetectPlayerStanding();

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      std::shared_ptr<IArena> _arena;
      std::shared_ptr<IPlayer> _player;

      TileIndices _playerOccupyingTileIndices;
   };
}

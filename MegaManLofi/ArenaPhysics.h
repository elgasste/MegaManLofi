#pragma once

#include <memory>

#include "IArenaPhysics.h"
#include "TileIndices.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class IFrameActionRegistry;
   class IArena;
   class IPlayer;

   class ArenaPhysics : public IArenaPhysics
   {
   public:
      ArenaPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                    const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
                    const std::shared_ptr<IArena> arena,
                    const std::shared_ptr<IPlayer> player );

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
      const std::shared_ptr<IArena> _arena;
      const std::shared_ptr<IPlayer> _player;

      TileIndices _playerOccupyingTileIndices;
   };
}

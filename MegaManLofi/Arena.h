#pragma once

#include <memory>
#include <vector>

#include "IArena.h"
#include "ArenaTile.h"
#include "TileIndices.h"

namespace MegaManLofi
{
   class ArenaConfig;
   class IPlayer;
   class IFrameActionRegistry;
   class IFrameRateProvider;

   class Arena : public IArena
   {
   public:
      Arena( const std::shared_ptr<ArenaConfig> config,
             const std::shared_ptr<IPlayer> player,
             const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
             const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      double GetWidth() const override { return _width; }
      double GetHeight() const override { return _height; }

      double GetPlayerPositionX() const override { return _playerPositionX; }
      double GetPlayerPositionY() const override { return _playerPositionY; }

      void MovePlayer() override;

   private:
      void MovePlayerX();
      void MovePlayerY();

      void UpdatePlayerOccupyingTileIndices();
      void DetectPlayerTileCollisionX( double& newPositionX );
      void DetectPlayerTileCollisionY( double& newPositionY );

   private:
      const std::shared_ptr<ArenaConfig> _config;
      const std::shared_ptr<IPlayer> _player;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;

      std::vector<ArenaTile> _tiles;

      double _width;
      double _height;

      double _playerPositionX;
      double _playerPositionY;

      TileIndices _playerOccupyingTileIndices;
   };
}

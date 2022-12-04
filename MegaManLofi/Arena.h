#pragma once

#include <memory>
#include <vector>

#include "IArena.h"
#include "ArenaTile.h"

namespace MegaManLofi
{
   class ArenaConfig;

   class Arena : public IArena
   {
   public:
      Arena( const std::shared_ptr<ArenaConfig> config );

      double GetWidth() const override { return _width; }
      double GetHeight() const override { return _height; }

      double GetPlayerPositionX() const override { return _playerPositionX; }
      double GetPlayerPositionY() const override { return _playerPositionY; }

      void SetPlayerPositionX( double positionX ) override { _playerPositionX = positionX; }
      void SetPlayerPositionY( double positionY ) override { _playerPositionY = positionY; }

   private:
      std::vector<ArenaTile> _tiles;

      double _width;
      double _height;

      double _playerPositionX;
      double _playerPositionY;
   };
}

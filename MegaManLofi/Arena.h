#pragma once

#include <memory>
#include <vector>

#include "IArena.h"

namespace MegaManLofi
{
   class ArenaConfig;

   class Arena : public IArena
   {
   public:
      Arena( const std::shared_ptr<ArenaConfig> config );

      void Reset();

      long long GetWidth() const override { return _width; }
      long long GetHeight() const override { return _height; }

      long long GetPlayerPositionX() const override { return _playerPositionX; }
      long long GetPlayerPositionY() const override { return _playerPositionY; }

      void SetPlayerPositionX( long long positionX ) override { _playerPositionX = positionX; }
      void SetPlayerPositionY( long long positionY ) override { _playerPositionY = positionY; }

      long long GetTileWidth() const override { return _tileWidth; }
      long long GetTileHeight() const override { return _tileHeight; }

      int GetHorizontalTiles() const override { return _horizontalTiles; }
      int GetVerticalTiles() const override { return _verticalTiles; }

      const ArenaTile& GetTile( long long index ) const override { return _tiles[index]; }

   private:
      const std::shared_ptr<ArenaConfig> _config;

      std::vector<ArenaTile> _tiles;

      long long _width;
      long long _height;

      long long _playerPositionX;
      long long _playerPositionY;

      long long _tileWidth;
      long long _tileHeight;

      int _horizontalTiles;
      int _verticalTiles;
   };
}

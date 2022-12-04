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

      double GetWidth() const override { return _width; }
      double GetHeight() const override { return _height; }

      double GetPlayerPositionX() const override { return _playerPositionX; }
      double GetPlayerPositionY() const override { return _playerPositionY; }

      void SetPlayerPositionX( double positionX ) override { _playerPositionX = positionX; }
      void SetPlayerPositionY( double positionY ) override { _playerPositionY = positionY; }

      double GetTileWidth() const override { return _tileWidth; }
      double GetTileHeight() const override { return _tileHeight; }

      int GetHorizontalTiles() const override { return _horizontalTiles; }
      int GetVerticalTiles() const override { return _verticalTiles; }

      const ArenaTile& GetTile( long long index ) const override { return _tiles[index]; }

   private:
      std::vector<ArenaTile> _tiles;

      double _width;
      double _height;

      double _playerPositionX;
      double _playerPositionY;

      double _tileWidth;
      double _tileHeight;

      int _horizontalTiles;
      int _verticalTiles;
   };
}

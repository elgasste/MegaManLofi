#pragma once

#include "ArenaTile.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IArenaInfoProvider
   {
   public:
      virtual double GetWidth() const = 0;
      virtual double GetHeight() const = 0;

      virtual double GetPlayerPositionX() const = 0;
      virtual double GetPlayerPositionY() const = 0;

      virtual double GetTileWidth() const = 0;
      virtual double GetTileHeight() const = 0;

      virtual int GetHorizontalTiles() const = 0;
      virtual int GetVerticalTiles() const = 0;

      virtual const ArenaTile& GetTile( long long index ) const = 0;
   };
}

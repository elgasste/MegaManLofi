#pragma once

#include "ArenaTile.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IArenaInfoProvider
   {
   public:
      virtual long long GetWidth() const = 0;
      virtual long long GetHeight() const = 0;

      virtual long long GetTileWidth() const = 0;
      virtual long long GetTileHeight() const = 0;

      virtual int GetHorizontalTiles() const = 0;
      virtual int GetVerticalTiles() const = 0;

      virtual const ArenaTile& GetTile( long long index ) const = 0;
   };
}

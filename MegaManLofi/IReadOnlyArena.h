#pragma once

#include <memory>

#include "Quad.h"
#include "ArenaTile.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IReadOnlyArena
   {
   public:
      virtual long long GetWidth() const = 0;
      virtual long long GetHeight() const = 0;

      virtual long long GetTileWidth() const = 0;
      virtual long long GetTileHeight() const = 0;

      virtual int GetHorizontalTiles() const = 0;
      virtual int GetVerticalTiles() const = 0;

      virtual const Quad<long long>& GetActiveRegion() const = 0;

      virtual const ArenaTile& GetTile( long long index ) const = 0;
   };
}